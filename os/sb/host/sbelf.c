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

/* Relevant ELF file types.*/
#define ET_EXEC             2U

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
//typedef unsigned elf_symnum_t;

/**
 * @brief   Type of a loadable section info structure.
 */
typedef struct {
  elf_secnum_t              section;
  memory_area_t             area;
  size_t                    rel_size;
  vfs_offset_t              rel_off;
} elf_section_info_t;

/**
 * @brief   Type of an ELF loader context.
 */
typedef struct elf_load_context {
  vfs_file_node_c           *fnp;
  const memory_area_t       *map;

  uint32_t                  entry;
  elf_secnum_t              sections_num;
  vfs_offset_t              sections_off;
  elf_section_info_t        *next;
  elf_section_info_t        allocated[SB_CFG_ELF_MAX_ALLOCATED];
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

static msg_t area_is_intersecting(elf_load_context_t *ctxp,
                                 const memory_area_t *map) {
  elf_section_info_t *esip;

  /* Scanning allocated sections.*/
  for (esip = &ctxp->allocated[0]; esip < ctxp->next; esip++) {
    if (chMemIsAreaIntersectingX(&esip->area, map)) {
      return true;
    }
  }

  return false;
}

static msg_t allocate_section(elf_load_context_t *ctxp,
                              elf_secnum_t section,
                              const elf32_section_header_t *shp) {
  elf_section_info_t *esip;

  /* Checking if there is space in the sections table.*/
  if (ctxp->next >= &ctxp->allocated[SB_CFG_ELF_MAX_ALLOCATED]) {
    return CH_RET_ENOMEM;
  }

  /* Adding an entry for this section.*/
  esip = ctxp->next;
  esip->section   = section;
  esip->area.base = ctxp->map->base + (size_t)shp->sh_addr;
  esip->area.size = (size_t)shp->sh_size;

  /* Checking if the section can fit into the destination memory area.*/
  if (!chMemIsAreaWithinX(ctxp->map, &esip->area)) {
    return CH_RET_ENOMEM;
  }

  /* Checking if this section is overlapping some other allocated section.*/
  if (area_is_intersecting(ctxp, &esip->area)) {
    return CH_RET_ENOEXEC;
  }

  ctxp->next++;

  return CH_RET_SUCCESS;
}

static msg_t allocate_load_section(elf_load_context_t *ctxp,
                                   elf_secnum_t section,
                                   const elf32_section_header_t *shp) {
  elf_section_info_t *esip;
  msg_t ret;

  /* Checking if there is space in the sections table.*/
  if (ctxp->next >= &ctxp->allocated[SB_CFG_ELF_MAX_ALLOCATED]) {
    return CH_RET_ENOMEM;
  }

  /* Adding an entry for this section.*/
  esip = ctxp->next;
  esip->section   = section;
  esip->area.base = ctxp->map->base + (size_t)shp->sh_addr;
  esip->area.size = (size_t)shp->sh_size;

  /* Checking if the section can fit into the destination memory area.*/
  if (!chMemIsAreaWithinX(ctxp->map, &esip->area)) {
    return CH_RET_ENOMEM;
  }

  /* Checking if this section is overlapping some other allocated section.*/
  if (area_is_intersecting(ctxp, &esip->area)) {
    return CH_RET_ENOEXEC;
  }

  /* Loading section data.*/
  ret = vfsSetFilePosition(ctxp->fnp, (vfs_offset_t)shp->sh_offset, VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)esip->area.base, esip->area.size);
  CH_RETURN_ON_ERROR(ret);

  ctxp->next++;

  return ret;
}

static elf_section_info_t *find_allocated_section(elf_load_context_t *ctxp,
                                                  elf_secnum_t section) {
  elf_section_info_t *esip;

  /* Scanning allocated sections.*/
  for (esip = &ctxp->allocated[0]; esip < ctxp->next; esip++) {
    if (esip->section == section) {
      return esip;
    }
  }

  return NULL;
}

static msg_t reloc_entry(elf_load_context_t *ctxp,
                         elf_section_info_t *esip,
                         elf32_rel_t *rp) {
  uint32_t relocation_address;

  /* Relocation point address.*/
  relocation_address = (uint32_t)esip->area.base + rp->r_offset;
  if (!chMemIsSpaceWithinX(&esip->area,
                           (const void *)relocation_address,
                           sizeof (uint32_t))) {
    return CH_RET_ENOMEM;
  }

  /* Handling the various relocation point types.*/
  switch (ELF32_R_TYPE(rp->r_info)) {
  case R_ARM_ABS32:
    *((uint32_t *)relocation_address) += (uint32_t)ctxp->map->base;
    break;
  case R_ARM_THM_PC22:
  case R_ARM_THM_JUMP24:
    /* TODO ????*/
    break;
  default:
    return CH_RET_ENOEXEC;
  }

  return CH_RET_SUCCESS;
}

static msg_t reloc_section(elf_load_context_t *ctxp,
                           elf_section_info_t *esip) {
  elf32_rel_t *rbuf;
  size_t size, done_size, remaining_size;
  msg_t ret;

  rbuf = (elf32_rel_t *)(void *)vfs_buffer_take();

  /* Reading the relocation section data.*/
  remaining_size = esip->rel_size;
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
                             esip->rel_off + (vfs_offset_t)done_size,
                             VFS_SEEK_SET);
    CH_BREAK_ON_ERROR(ret);
    ret = vfsReadFile(ctxp->fnp, (void *)rbuf, size);
    CH_BREAK_ON_ERROR(ret);

    /* Number of relocation entries in the buffer.*/
    n = (unsigned)ret / (unsigned)sizeof (elf32_rel_t);
    for (i = 0U; i < n; i++) {
      ret = reloc_entry(ctxp, esip, &rbuf[i]);
      CH_BREAK_ON_ERROR(ret);
    }
    CH_BREAK_ON_ERROR(ret);

    remaining_size -= size;
    done_size      += size;
  }

  vfs_buffer_release((char *)rbuf);

  return ret;
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
  ctxp->next = &ctxp->allocated[0];

  /* Reading the main ELF header.*/
  ret = vfsSetFilePosition(ctxp->fnp, (vfs_offset_t)0, VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)&h, sizeof (elf32_header_t));
  CH_RETURN_ON_ERROR(ret);

  /* Checking for the expected header.*/
  if (memcmp(h.e_ident, elf32_header, 16) != 0) {
    return CH_RET_ENOEXEC;
  }

  /* Accepting executable files only.*/
  if (h.e_type != ET_EXEC) {
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
    elf_secnum_t i;
    elf_section_info_t *esip;

    ret = init_elf_context(&ctx, fnp, map);
    CH_BREAK_ON_ERROR(ret);

    /* Loading phase, scanning section headers.*/
    for (i = 0U; i < ctx.sections_num; i++) {
      elf32_section_header_t sh;

      /* Reading the header.*/
      ret = read_section_header(&ctx, &sh, i);
      CH_BREAK_ON_ERROR(ret);

      /* Empty sections are not processed.*/
      if (sh.sh_size == 0U) {
        continue;
      }

      /* Deciding what to do with the section depending on type.*/
      switch (sh.sh_type) {
      case SHT_PROGBITS:
        /* Allocatable section type, needs to be loaded.*/
        if ((sh.sh_flags & SHF_ALLOC) != 0U) {

          /* Allocating and loading, could fail.*/
          ret = allocate_load_section(&ctx, i, &sh);
          CH_RETURN_ON_ERROR(ret);
        }
        break;

      case SHT_NOBITS:
        /* Uninitialized data section, we can have more than one, just checking
           address ranges.*/
        if ((sh.sh_flags & SHF_ALLOC) != 0U) {
          ret = allocate_section(&ctx, i, &sh);
          CH_RETURN_ON_ERROR(ret);
        }
        break;

      case SHT_REL:
        if ((sh.sh_flags & SHF_INFO_LINK) != 0U) {

          esip = find_allocated_section(&ctx, (elf_secnum_t)sh.sh_info);
          if (esip == NULL) {
            /* Ignoring other relocation sections.*/
            break;
          }

          /* Multiple relocation sections associated to the same section.*/
          if (esip->rel_size != 0U) {
            return CH_RET_ENOEXEC;
          }

          esip->rel_size = sh.sh_size;
          esip->rel_off  = (vfs_offset_t)sh.sh_offset;
        }
        break;

      default:
        /* Ignoring other section types.*/
        break;
      }
    }

    /* Relocating all sections with an associated relocation table.*/
    for (esip = &ctx.allocated[0]; esip < ctx.next; esip++) {
      if (esip->rel_off != (vfs_offset_t)0) {
        ret = reloc_section(&ctx, esip);
        CH_RETURN_ON_ERROR(ret);
      }
    }
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
