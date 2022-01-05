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

static msg_t init_elf_context(elf_load_context_t *ctxp,
                              vfs_file_node_c *fnp,
                              memory_area_t *map) {
  static uint8_t elf32_header[16] = {
    0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  elf32_header_t h;
  elf32_section_header_t sh;
  msg_t ret;

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

  /* Reading the header of the section containing the section names.*/
  ret = vfsSetFilePosition(ctxp->fnp,
                           (vfs_offset_t)(h.e_shoff +
                                         (h.e_shstrndx * sizeof (elf32_section_header_t))),
                           VFS_SEEK_SET);
  CH_RETURN_ON_ERROR(ret);
  ret = vfsReadFile(ctxp->fnp, (void *)&sh, sizeof (elf32_section_header_t));
  CH_RETURN_ON_ERROR(ret);

  /* Storing info required later.*/
  ctxp->entry       = h.e_entry;
  ctxp->shnum       = h.e_shnum;
  ctxp->shoff       = (vfs_offset_t)h.e_shoff;
  ctxp->stringsoff  = (vfs_offset_t)sh.sh_offset;

  return CH_RET_SUCCESS;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

msg_t sbElfLoad(vfs_driver_c *drvp, const char *path, memory_area_t *map) {
  vfs_file_node_c *fnp;
  msg_t ret;
  elf_load_context_t ctx;

  ret = vfsDrvOpenFile(drvp, path, VO_RDONLY, &fnp);
  CH_RETURN_ON_ERROR(ret);

  do {
    ret = init_elf_context(&ctx, fnp, map);
    CH_BREAK_ON_ERROR(ret);

  } while (false);

  vfsClose((vfs_node_c *)fnp);

  return CH_RET_SUCCESS;
}

#endif /* SB_CFG_ENABLE_VFS == TRUE */

/** @} */
