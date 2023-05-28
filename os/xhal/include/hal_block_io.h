/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file        hal_block_io.h
 * @brief       Generated Block I/O header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_BLOCK_IO
 * @brief       Block devices interface.
 * @{
 */

#ifndef HAL_BLOCK_IO_H
#define HAL_BLOCK_IO_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @interface   block_io_i
 * @extends     base_interface_i.
 *
 * @brief       Base block device interface.
 * @details     This header defines an abstract interface useful to access
 *              generic I/O block devices in a standardized way.
 *
 * @name        Interface @p block_io_i structures
 * @{
 */

/**
 * @brief       Type of a block device interface.
 */
typedef struct block_io block_io_i;

/**
 * @brief       Interface @p block_io_i virtual methods table.
 */
struct block_io_vmt {
  /* Memory offset between this interface structure and begin of
     the implementing class structure.*/
  size_t instance_offset;
  /* From base_interface_i.*/
  /* From block_io_i.*/
  bool (*is_inserted)(void *ip);
};

/**
 * @brief       Structure representing a block device interface.
 */
struct block_io {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct block_io_vmt *vmt;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of block_io_i
 * @{
 */
/**
 * @memberof    block_io_i
 * @public
 *
 * @brief       Removable media detection.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @return                      The media state.
 * @retval false                If media not inserted.
 * @retval true                 If media is inserted.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkIsInserted(void *ip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->is_inserted(ip);
}
/** @} */

#endif /* HAL_BLOCK_IO_H */

/** @} */
