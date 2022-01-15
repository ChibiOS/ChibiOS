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
#define ET_EXEC                 2U

/* Relevant section types.*/
#define SHT_PROGBITS            1U
#define SHT_SYMTAB              2U
#define SHT_NOBITS              8U
#define SHT_REL                 9U

/* Relevant section flags.*/
#define SHF_WRITE               (1U << 0)
#define SHF_ALLOC               (1U << 1)
#define SHF_EXECINSTR           (1U << 2)
#define SHF_INFO_LINK           (1U << 6)

/* Special section indices.*/
#define SHN_UNDEF               0U

/* Supported relocation types.*/
#define R_ARM_ABS32             2U
#define R_ARM_THM_PC22          10U
#define R_ARM_THM_JUMP24        30U
#define R_ARM_PREL31            42U
#define R_ARM_THM_MOVW_ABS_NC   47U
#define R_ARM_THM_MOVT_ABS      48U

#define ELF32_R_SYM(v)          ((v) >> 8)
#define ELF32_R_TYPE(v)         ((v) & 0xFFU)

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
//  uint32_t                  entry;
  elf_secnum_t              sections_num;
  vfs_offset_t              sections_off;
  bool                      rel_movw_found;
  uint32_t                  rel_movw_symbol;
  uint32_t                  rel_movw_address;
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

static uint32_t get_const16(uint32_t address) {
  uint32_t ins  = ((uint32_t)(((uint16_t *)address)[0]) << 16) |
                  ((uint32_t)(((uint16_t *)address)[1]) << 0);

  return ((ins & 0x000000FFU) >> 0) |
         ((ins & 0x00007000U) >> 4) |
         ((ins & 0x000F0000U) >> 4) |
         ((ins & 0x04000000U) >> 14);
}

static void set_const16(uint32_t address, uint32_t val16) {
  uint32_t ins  = ((uint32_t)(((uint16_t *)address)[0]) << 16) |
                  ((uint32_t)(((uint16_t *)address)[1]) << 0);

  ins &= ~0x040F70FFU;
  ins |= ((val16 & 0x00000800U) << 14) |
         ((val16 & 0x0000F000U) << 4) |
         ((val16 & 0x00000700U) << 4) |
         ((val16 & 0x000000FFU) << 0);

  ((uint16_t *)address)[0] = (uint16_t)(ins >> 16);
  ((uint16_t *)address)[1] = (uint16_t)(ins >> 0);
}

static msg_t reloc_entry(elf_load_context_t *ctxp,
                         elf_section_info_t *esip,
                         elf32_rel_t *rp) {
  uint32_t relocation_address, offset;

  /* Relocation point address.*/
  relocation_address = (uint32_t)ctxp->map->base + rp->r_offset;
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
  case R_ARM_THM_MOVW_ABS_NC:
    /* Checking for consecutive "movw" relocations without a "movt", we
       consider this an error.*/
    if (ctxp->rel_movw_found) {
      return CH_RET_ENOEXEC;
    }

    /* Storing information about the "movw" instruction to be processed later
       when the associated "movt" instruction is found.*/
    ctxp->rel_movw_found   = true;
    ctxp->rel_movw_symbol  = ELF32_R_SYM(rp->r_info);
    ctxp->rel_movw_address = relocation_address;
    break;
  case R_ARM_THM_MOVT_ABS:
    /* Checking if we found a "movw" instruction before this "movt".*/
    if (!ctxp->rel_movw_found) {
      return CH_RET_ENOEXEC;
    }

    /* Checking if both instructions referred to the same symbol.*/
    if (ctxp->rel_movw_symbol != ELF32_R_SYM(rp->r_info)) {
      return CH_RET_ENOEXEC;
    }

    /* Relocating both the "movw" and the "movt" instructions.*/
    offset  = (get_const16(relocation_address) << 16) |
              (get_const16(ctxp->rel_movw_address) << 0);
    offset += (uint32_t)ctxp->map->base;
    set_const16(relocation_address, offset >> 16);
    set_const16(ctxp->rel_movw_address, offset & 0xFFFFU);

    ctxp->rel_movw_found = false;
    break;
  case R_ARM_THM_PC22:
  case R_ARM_THM_JUMP24:
  case R_ARM_PREL31:
    /* To be ignored.*/
    break;
  default:
    /* Anything unexpected is handled as an error.*/
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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

msg_t sbElfLoad(vfs_file_node_c *fnp, const memory_area_t *map) {
  msg_t ret;
  elf_load_context_t ctx;
  elf_section_info_t *esip;

  /* Large structures not used at same time, the compiler could optimize it
     but it is still a problem when running the code without optimizations for
     debug.*/
  union {
    elf32_header_t h;
    elf32_section_header_t sh;
  } u;

  /* Load context initialization.*/
  {
    static const uint8_t elf32_header[16] = {
      0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    /* Context fully cleared.*/
    memset((void *)&ctx, 0, sizeof (elf_load_context_t));

    /* Initializing the fixed part of the context.*/
    ctx.fnp  = fnp;
    ctx.map  = map;
    ctx.next = &ctx.allocated[0];

    /* Reading the main ELF header.*/
    ret = vfsSetFilePosition(ctx.fnp, (vfs_offset_t)0, VFS_SEEK_SET);
    CH_RETURN_ON_ERROR(ret);
    ret = vfsReadFile(ctx.fnp, (void *)&u.h, sizeof (elf32_header_t));
    CH_RETURN_ON_ERROR(ret);

    /* Checking for the expected header.*/
    if (memcmp(u.h.e_ident, elf32_header, 16) != 0) {
      return CH_RET_ENOEXEC;
    }

    /* Accepting executable files only.*/
    if (u.h.e_type != ET_EXEC) {
      return CH_RET_ENOEXEC;
    }

    /* TODO more consistency checks.*/

    /* Storing info required later.*/
//    ctx.entry        = u.h.e_entry;
    ctx.sections_num = (unsigned)u.h.e_shnum;
    ctx.sections_off = (vfs_offset_t)u.h.e_shoff;
  }

  /* Loading phase, scanning section headers.*/
  {
    elf_secnum_t i;

    for (i = 0U; i < ctx.sections_num; i++) {

      /* Reading the header.*/
      ret = vfsSetFilePosition(ctx.fnp,
                               ctx.sections_off + ((vfs_offset_t)i *
                                                   (vfs_offset_t)sizeof (elf32_section_header_t)),
                               VFS_SEEK_SET);
      CH_RETURN_ON_ERROR(ret);
      ret = vfsReadFile(ctx.fnp, (void *)&u.sh, sizeof (elf32_section_header_t));
      CH_RETURN_ON_ERROR(ret);

      /* Empty sections are not processed.*/
      if (u.sh.sh_size == 0U) {
        continue;
      }

      /* Deciding what to do with the section depending on type.*/
      switch (u.sh.sh_type) {
      case SHT_PROGBITS:
        /* Allocatable section type, needs to be loaded.*/
        if ((u.sh.sh_flags & SHF_ALLOC) != 0U) {

          /* Allocating and loading, could fail.*/
          ret = allocate_load_section(&ctx, i, &u.sh);
          CH_RETURN_ON_ERROR(ret);
        }
        break;

      case SHT_NOBITS:
        /* Uninitialized data section, we can have more than one, just checking
           address ranges.*/
        if ((u.sh.sh_flags & SHF_ALLOC) != 0U) {
          ret = allocate_section(&ctx, i, &u.sh);
          CH_RETURN_ON_ERROR(ret);
        }
        break;

      case SHT_REL:
        if ((u.sh.sh_flags & SHF_INFO_LINK) != 0U) {

          esip = find_allocated_section(&ctx, (elf_secnum_t)u.sh.sh_info);
          if (esip == NULL) {
            /* Ignoring other relocation sections.*/
            break;
          }

          /* Multiple relocation sections associated to the same section.*/
          if (esip->rel_size != 0U) {
            return CH_RET_ENOEXEC;
          }

          esip->rel_size = u.sh.sh_size;
          esip->rel_off  = (vfs_offset_t)u.sh.sh_offset;
        }
        break;

      default:
        /* Ignoring other section types.*/
        break;
      }
    }
  }

  /* Relocating all sections with an associated relocation table.*/
  for (esip = &ctx.allocated[0]; esip < ctx.next; esip++) {
    if (esip->rel_off != (vfs_offset_t)0) {
      ret = reloc_section(&ctx, esip);
      CH_RETURN_ON_ERROR(ret);
    }
  }

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
