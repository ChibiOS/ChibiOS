/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/host/sbelf.c
 * @brief   ARM SandBox ELF loader code.
 *
 * @addtogroup ARM_SANDBOX_ELF
 * @{
 */

#include <string.h>

#include "ch.h"
#include "sb.h"

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/* Relevant section types.*/
#define SHT_PROGBITS        1U
#define SHT_SYMTAB          2U
#define SHT_NOBITS          8U
#define SHT_REL             9U

/* Relevant section flags.*/
#define SHF_WRITE           (1U << 0)
#define SHF_ALLOC           (1U << 1)
#define SHF_EXECINSTR       (1U << 2)
#define SHF_INFO_LINK       (1U << 6)

/* Special section indices.*/
#define SHN_UNDEF           0U

/* Supported relocation types.*/
#define R_ARM_ABS32         2U
#define R_ARM_THM_PC22      10U
#define R_ARM_THM_JUMP24    30U

#define ELF32_R_SYM(v)      ((v) >> 8)
#define ELF32_R_TYPE(v)     ((v) & 0xFFU)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   Type of a section number.
 */
typedef unsigned elf_secnum_t;

/**
 * @brief   Type of a symbol number.
 */
typedef unsigned elf_symnum_t;

/**
 * @brief   Type of a loadable section info structure.
 */
typedef struct {
  elf_secnum_t              section;
  uint32_t                  address;
  size_t                    bits_size;
  vfs_offset_t              bits_off;
  size_t                    rel_size;
  vfs_offset_t              rel_off;
} elf_loadable_info_t;

/**
 * @brief   Type of an ELF loader context.
 */
typedef struct elf_load_context {
  vfs_file_node_c           *fnp;
  const memory_area_t       *map;
  uint8_t                   *next;

  uint32_t                  entry;
  elf_secnum_t              sections_num;
  vfs_offset_t              sections_off;
  elf_symnum_t              symbols_num;
  vfs_offset_t              symbols_off;
  elf_loadable_info_t       loadable_code;
  elf_loadable_info_t       loadable_const;
  elf_loadable_info_t       loadable_data;
#if 0
//  vfs_offset_t              strings_off;
#endif
} elf_load_context_t;

/**
 * @brief   Type of an ELF32 header.
 */
typedef struct {
  unsigned char             e_ident[16];
  uint16_t                  e_type;
  uint16_t                  e_machine;
  uint32_t                  e_version;
  uint32_t                  e_entry;
  uint32_t                  e_phoff;
  uint32_t                  e_shoff;
  uint32_t                  e_flags;
  uint16_t                  e_ehsize;
  uint16_t                  e_phentsize;
  uint16_t                  e_phnum;
  uint16_t                  e_shentsize;
  uint16_t                  e_shnum;
  uint16_t                  e_shstrndx;
} elf32_header_t;

typedef struct {
  uint32_t                  sh_name;
  uint32_t                  sh_type;
  uint32_t                  sh_flags;
  uint32_t                  sh_addr;
  uint32_t                  sh_offset;
  uint32_t                  sh_size;
  uint32_t                  sh_link;
  uint32_t                  sh_info;
  uint32_t                  sh_addralign;
  uint32_t                  sh_entsize;
} elf32_section_header_t;

typedef struct {
  uint32_t                  r_offset;
  uint32_t                  r_info;
} elf32_rel_t;

typedef struct {
  uint32_t                  st_name;
  uint32_t                  st_value;
  uint32_t                  st_size;
  uint8_t                   st_info;
  uint8_t                   st_other;
  uint16_t                  st_shndx;
} elf32_symbol_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if 0
static msg_t read_section_name(elf_load_context_t *ctxp,
                               elf32_section_header_t *shp,
                               char *buf, size_t buflen) {
  msg_t ret;

  if (shp->sh_name == 0U) {
    *buf = '\0';
    return CH_RET_SUCCESS;
  }

  ret = vfsSetFilePosition(ctxp->fnp,
                           ctxp->stringsoff + (vfs_offset_t)shp->sh_name,
                           VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)buf, buflen);
  CH_RETURN_ON_ERROR(ret);

  return CH_RET_SUCCESS;
}
#endif

static msg_t read_section_header(elf_load_context_t *ctxp,
                                 elf32_section_header_t *shp,
                                 elf_secnum_t index) {
  msg_t ret;

  ret = vfsSetFilePosition(ctxp->fnp,
                           ctxp->sections_off + ((vfs_offset_t)index *
                                                 (vfs_offset_t)sizeof (elf32_section_header_t)),
                           VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  return vfsReadFile(ctxp->fnp, (void *)shp, sizeof (elf32_section_header_t));
}

static msg_t allocate_section(elf_load_context_t *ctxp,
                              elf32_section_header_t *shp) {
  uint8_t *load_address;

  /* Checking if the section can fit into the destination memory area.*/
  load_address = ctxp->map->base + shp->sh_addr;
  if (!chMemIsAreaWithinX(ctxp->map,
                          (const void *)load_address,
                          (size_t)shp->sh_size)) {
    return CH_RET_ENOMEM;
  }

  return CH_RET_SUCCESS;
}

#if 0
static msg_t reloc_section(elf_load_context_t *ctxp,
                           elf32_section_header_t *shp) {
  elf32_relocation_t *rbuf;
  msg_t ret;

  rbuf = (elf32_relocation_t *)(void *)vfs_buffer_take();

  do {
    size_t size, remaining_size = (size_t)shp->sh_size;

    ret = vfsSetFilePosition(ctxp->fnp, (vfs_offset_t)shp->sh_offset, VFS_SEEK_SET);
    CH_BREAK_ON_ERROR(ret);

    /* Reading the relocation section data.*/
    while (remaining_size > 0U) {
      unsigned i, n;

      /* Reading relocation data using buffers in order to not make continuous
         calls to the FS which could be unbuffered.*/
      if (remaining_size > VFS_BUFFERS_SIZE) {
        size = VFS_BUFFERS_SIZE;
      }
      else {
        size = remaining_size;
      }

      ret = vfsReadFile(ctxp->fnp, (void *)rbuf, size);
      if (CH_RET_IS_ERROR(ret)) {
        goto yes_it_is_a_goto;
      }

      /* Number of relocation entries in the buffer.*/
      n = (unsigned)ret / (unsigned)sizeof (elf32_relocation_t);
      for (i = 0U; i < n; i++) {
        ret = reloc_entry(ctxp, &rbuf[i]);
        if (CH_RET_IS_ERROR(ret)) {
          goto yes_it_is_a_goto;
        }
      }

      remaining_size -= size;
    }

    ret = CH_RET_SUCCESS;
  } while (false);

yes_it_is_a_goto:

  vfs_buffer_release((char *)rbuf);

  return ret;
}
#endif

static elf_loadable_info_t *find_loaded_section(elf_load_context_t *ctxp,
                                                elf_secnum_t sn) {

  if (sn == ctxp->loadable_code.section) {
    /* Executable code section.*/
    return &ctxp->loadable_code;
  }
  if (sn == ctxp->loadable_data.section) {
    /* Writable data section.*/
    return &ctxp->loadable_data;
  }
  if (sn == ctxp->loadable_const.section) {
    /* Constants data section.*/
    return &ctxp->loadable_const;
  }

  return NULL;
}

static msg_t reloc_entry(elf_load_context_t *ctxp,
                         elf_loadable_info_t *lip,
                         elf32_rel_t *rp) {
  vfs_offset_t symoff;
  elf_symnum_t symnum;
  uint32_t relocation_address;
  elf_loadable_info_t *symbol_lip;
  elf32_symbol_t symbol;
  msg_t ret;

  /* Checking for a symbol number overflow.*/
  symnum = (elf_symnum_t)ELF32_R_SYM(rp->r_info);
  if (symnum > ctxp->symbols_num) {
    return CH_RET_ENOEXEC;
  }

  /* Moving file pointer to the symbol then reading it.*/
  symoff = ctxp->symbols_off + ((vfs_offset_t)symnum *
                                (vfs_offset_t)sizeof (elf32_symbol_t));
  ret = vfsSetFilePosition(ctxp->fnp, symoff, VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)&symbol, sizeof (elf32_symbol_t));
  CH_RETURN_ON_ERROR(ret);

  /* Undefined symbols are not handled, this is a loader not a linker.*/
  if (symbol.st_shndx == SHN_UNDEF) {
    return CH_RET_ENOEXEC;
  }

  /* Symbols must be associated to an already loaded section.*/
  symbol_lip = find_loaded_section(ctxp, (elf_symnum_t)symbol.st_shndx);
  if (symbol_lip == NULL) {
    return CH_RET_ENOEXEC;
  }

  /* Relocation point address.*/
  relocation_address = (uint32_t)ctxp->map->base +
                       lip->address +
                       rp->r_offset;
  if (!chMemIsAreaWithinX(ctxp->map,
                          (const void *)relocation_address,
                          sizeof (uint32_t))) {
    return CH_RET_ENOMEM;
  }

  /* Handling the various relocation point types.*/
  switch (ELF32_R_TYPE(rp->r_info)) {
  case R_ARM_ABS32:
    *((uint32_t *)relocation_address) += (uint32_t)ctxp->map->base +
                                         symbol_lip->address +
                                         symbol.st_value;
    break;
  case R_ARM_THM_PC22:
  case R_ARM_THM_JUMP24:
    /* TODO */
  default:
    return CH_RET_ENOEXEC;
  }

  return CH_RET_SUCCESS;
}

static msg_t reloc_section(elf_load_context_t *ctxp,
                           elf_loadable_info_t *lip) {
  elf32_rel_t *rbuf;
  size_t size, done_size, remaining_size;
  msg_t ret;

  rbuf = (elf32_rel_t *)(void *)vfs_buffer_take();

  /* Reading the relocation section data.*/
  remaining_size = lip->rel_size;
  done_size = 0U;
  while (remaining_size > 0U) {
    unsigned i, n;

    /* Reading relocation data using buffers in order to not make continuous
       calls to the FS which could be unbuffered.*/
    if (remaining_size > VFS_BUFFERS_SIZE) {
      size = VFS_BUFFERS_SIZE;
    }
    else {
      size = remaining_size;
    }

    /* Reading a buffer-worth of relocation data.*/
    ret = vfsSetFilePosition(ctxp->fnp,
                             lip->rel_off + (vfs_offset_t)done_size,
                             VFS_SEEK_SET);
    CH_BREAK_ON_ERROR(ret);
    ret = vfsReadFile(ctxp->fnp, (void *)rbuf, size);
    CH_BREAK_ON_ERROR(ret);

    /* Number of relocation entries in the buffer.*/
    n = (unsigned)ret / (unsigned)sizeof (elf32_rel_t);
    for (i = 0U; i < n; i++) {
      ret = reloc_entry(ctxp, lip, &rbuf[i]);
      CH_BREAK_ON_ERROR(ret);
    }
    CH_BREAK_ON_ERROR(ret);

    remaining_size -= size;
    done_size      += size;
  }

  vfs_buffer_release((char *)rbuf);

  return ret;
}

static msg_t load_relocate_section(elf_load_context_t *ctxp,
                                   elf_loadable_info_t *lip) {
  uint8_t *load_address;
  msg_t ret;

  /* Checking if the section can fit into the destination memory area.*/
  load_address = ctxp->map->base + lip->address;
  if (!chMemIsAreaWithinX(ctxp->map,
                          (const void *)load_address,
                          lip->bits_size)) {
    return CH_RET_ENOMEM;
  }

  /* Loading the section data into the final memory area.*/
  if (lip->bits_size > 0U) {
    ret = vfsSetFilePosition(ctxp->fnp, lip->bits_off, VFS_SEEK_SET);
    CH_RETURN_ON_ERROR(ret);
    ret = vfsReadFile(ctxp->fnp, (void *)load_address, lip->bits_size);
    CH_RETURN_ON_ERROR(ret);
  }

  if (lip->rel_size > 0U) {
    ret = reloc_section(ctxp, lip);
    CH_RETURN_ON_ERROR(ret);
  }

  return CH_RET_SUCCESS;
}

static msg_t init_elf_context(elf_load_context_t *ctxp,
                              vfs_file_node_c *fnp,
                              const memory_area_t *map) {
  static uint8_t elf32_header[16] = {
    0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  elf32_header_t h;
  msg_t ret;

  /* Context fully cleared.*/
  memset((void *)ctxp, 0, sizeof (elf_load_context_t));

  /* Initializing the fixed part of the context.*/
  ctxp->fnp  = fnp;
  ctxp->map  = map;
  ctxp->next = map->base;

  /* Reading the main ELF header.*/
  ret = vfsSetFilePosition(ctxp->fnp, (vfs_offset_t)0, VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)&h, sizeof (elf32_header_t));
  CH_RETURN_ON_ERROR(ret);

  /* Checking for the expected header.*/
  if (memcmp(h.e_ident, elf32_header, 16) != 0) {
    return CH_RET_ENOEXEC;
  }

  /* TODO more consistency checks.*/

  /* Storing info required later.*/
  ctxp->entry        = h.e_entry;
  ctxp->sections_num = (unsigned)h.e_shnum;
  ctxp->sections_off = (vfs_offset_t)h.e_shoff;

  return CH_RET_SUCCESS;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

msg_t sbElfLoad(vfs_file_node_c *fnp, const memory_area_t *map) {
  msg_t ret;

  do {
    elf_load_context_t ctx;
    elf_loadable_info_t *lip;
    elf_secnum_t i;

    ret = init_elf_context(&ctx, fnp, map);
    CH_BREAK_ON_ERROR(ret);

    /* Iterating through sections, loading.*/
    for (i = 0U; i < ctx.sections_num; i++) {
      elf32_section_header_t sh;

      /* Reading the header.*/
      ret = read_section_header(&ctx, &sh, i);
      CH_BREAK_ON_ERROR(ret);

      /* Discovery phase, scanning section headers and gathering data.*/
      switch (sh.sh_type) {
      case SHT_PROGBITS:
        /* Loadable section type, we allow for one executable, one data and
           one constants sections.*/
        if ((sh.sh_flags & SHF_ALLOC) != 0U) {

          if ((sh.sh_flags & SHF_EXECINSTR) != 0U) {
            /* Executable code section.*/
            lip = &ctx.loadable_code;
          }
          else if ((sh.sh_flags & SHF_WRITE) != 0U) {
            /* Writable data section.*/
            lip = &ctx.loadable_data;
          }
          else {
            /* Constants data section.*/
            lip = &ctx.loadable_const;
          }

          /* Multiple sections of same kind.*/
          if (lip->section != 0U) {
            return CH_RET_ENOEXEC;
          }

          lip->section   = i;
          lip->address   = sh.sh_addr;
          lip->bits_size = (size_t)sh.sh_size;
          lip->bits_off  = (vfs_offset_t)sh.sh_offset;

        }
        break;

      case SHT_NOBITS:
        /* Uninitialized data section, we can have more than one, just checking
           address ranges.*/
        if ((sh.sh_flags & SHF_ALLOC) != 0U) {
          ret = allocate_section(&ctx, &sh);
          CH_RETURN_ON_ERROR(ret);
        }
        break;

      case SHT_REL:
        if ((sh.sh_flags & SHF_INFO_LINK) != 0U) {

          lip = find_loaded_section(&ctx, (elf_secnum_t)sh.sh_info);
          if (lip == NULL) {
            /* Ignoring other relocation sections.*/
            break;
          }

          /* Multiple relocation sections associated to the same section.*/
          if (lip->rel_size != 0U) {
            return CH_RET_ENOEXEC;
          }

          lip->rel_size = sh.sh_size;
          lip->rel_off  = (vfs_offset_t)sh.sh_offset;
        }
        break;

      case SHT_SYMTAB:
        /* Symbols section, this is required for relocation.*/

        if (ctx.symbols_num != 0U) {
          /* Multiple symbol sections.*/
          return CH_RET_ENOEXEC;
        }

        ctx.symbols_num = (elf_symnum_t)sh.sh_size /
                          (elf_symnum_t)sizeof (elf32_symbol_t);
        ctx.symbols_off = (vfs_offset_t)sh.sh_offset;
        break;

      default:
        /* Ignoring other section types.*/
        break;
      }
    }

    /* Loading phase.*/
    ret = load_relocate_section(&ctx, &ctx.loadable_code);
    CH_RETURN_ON_ERROR(ret);
    ret = load_relocate_section(&ctx, &ctx.loadable_data);
    CH_RETURN_ON_ERROR(ret);
    ret = load_relocate_section(&ctx, &ctx.loadable_const);
    CH_RETURN_ON_ERROR(ret);
  } while (false);

  return ret;
}

msg_t sbElfLoadFile(vfs_driver_c *drvp,
                    const char *path,
                    const memory_area_t *map) {
  vfs_file_node_c *fnp;
  msg_t ret;

  ret = vfsDrvOpenFile(drvp, path, VO_RDONLY, &fnp);
  CH_RETURN_ON_ERROR(ret);

  do {
    ret = sbElfLoad(fnp, map);
    CH_BREAK_ON_ERROR(ret);

  } while (false);

  vfsClose((vfs_node_c *)fnp);

  return ret;
}

#endif /* SB_CFG_ENABLE_VFS == TRUE */

/** @} */
