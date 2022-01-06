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
#define SHT_REL             9U

/* Relevant section flags.*/
#define SHF_WRITE           (1U << 0)
#define SHF_ALLOC           (1U << 1)
#define SHF_EXECINSTR       (1U << 2)
#define SHF_INFO_LINK       (1U << 6)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   Type of an ELF loader context.
 */
typedef struct elf_load_context {
  vfs_file_node_c           *fnp;
  memory_area_t             *map;
  uint8_t                   *next;

  uint32_t                  entry;
  uint16_t                  shnum;
  vfs_offset_t              shoff;
  vfs_offset_t              stringsoff;
  uint64_t                  loaded;
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
  uint16_t                  sh_name;
  uint16_t                  sh_type;
  uint16_t                  sh_flags;
  uint32_t                  sh_addr;
  uint32_t                  sh_offset;
  uint16_t                  sh_size;
  uint16_t                  sh_link;
  uint16_t                  sh_info;
  uint16_t                  sh_addralign;
  uint16_t                  sh_entsize;
} elf32_section_header_t;

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
                                 unsigned index) {
  msg_t ret;

  ret = vfsSetFilePosition(ctxp->fnp,
                           ctxp->shoff + ((vfs_offset_t)index *
                                          (vfs_offset_t)sizeof (elf32_section_header_t)),
                           VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  return vfsReadFile(ctxp->fnp, (void *)shp, sizeof (elf32_section_header_t));
}

static msg_t load_section(elf_load_context_t *ctxp,
                          elf32_section_header_t *shp) {

  (void)ctxp;
  (void)shp;

  return CH_RET_SUCCESS;
}

static msg_t reloc_section(elf_load_context_t *ctxp,
                           elf32_section_header_t *shp) {

  (void)ctxp;
  (void)shp;

  return CH_RET_SUCCESS;
}

static msg_t init_elf_context(elf_load_context_t *ctxp,
                              elf32_section_header_t *shp,
                              vfs_file_node_c *fnp,
                              memory_area_t *map) {
  static uint8_t elf32_header[16] = {
    0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  elf32_header_t h;
  msg_t ret;

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
  ctxp->entry       = h.e_entry;
  ctxp->shnum       = h.e_shnum;
  ctxp->shoff       = (vfs_offset_t)h.e_shoff;

  /* Reading the header of the section containing the section names.*/
  ret = read_section_header(ctxp, shp, (unsigned)h.e_shstrndx);
  ctxp->stringsoff  = (vfs_offset_t)shp->sh_offset;

  return CH_RET_SUCCESS;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

msg_t sbElfLoad(vfs_file_node_c *fnp, memory_area_t *map) {
  elf_load_context_t ctx;
  elf32_section_header_t sh;
  unsigned i;
  msg_t ret;

  do {
    ret = init_elf_context(&ctx, &sh, fnp, map);
    CH_BREAK_ON_ERROR(ret);

    /* Iterating through sections.*/
    for (i = 0U; i < (unsigned)ctx.shnum; i++) {
//      char name[8];

      ret = read_section_header(&ctx, &sh, i);
      CH_BREAK_ON_ERROR(ret);

#if 0
      ret = read_section_name(&ctx, &sh, name, sizeof name);
      CH_BREAK_ON_ERROR(ret);

      /* Handling the various sections.*/
      if (strcmp(name, ".text") == (size_t)0) {
        ret = load_section(&ctx, &sh, i);
      }
      else if (strcmp(name, ".rodata") == (size_t)0) {
        ret = load_section(&ctx, &sh, i);
      }
      else if (strcmp(name, ".data") == (size_t)0) {
        ret = load_section(&ctx, &sh, i);
      }
      else if (strcmp(name, ".bss") == (size_t)0) {

      }
#endif

      /* Sections to be loaded.*/
      if ((sh.sh_type == SHT_PROGBITS) && ((sh.sh_flags & SHF_ALLOC) != 0U)) {

        /* Loading sections with index greater than 63 is not supported.*/
        if (i >= 64U) {
          return CH_RET_ENOMEM;
        }

        ret = load_section(&ctx, &sh);
        CH_RETURN_ON_ERROR(ret);

        ctx.loaded |= (1ULL << i);
      }

      /* Sections to be relocated, must refer to a loaded section.*/
      if ((sh.sh_type == SHT_REL) &&
          ((sh.sh_flags & SHF_INFO_LINK) != 0U) &&
          ((ctx.loaded & (1ULL << sh.sh_info)) != 0ULL)) {

        ret = reloc_section(&ctx, &sh);
        CH_RETURN_ON_ERROR(ret);

      }
    }

  } while (false);

  return ret;
}

msg_t sbElfLoadFile(vfs_driver_c *drvp, const char *path, memory_area_t *map) {
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
