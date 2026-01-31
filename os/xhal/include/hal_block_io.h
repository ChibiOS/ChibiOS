/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @brief       Type of structure representing a block device information.
 */
typedef struct hal_blk_info hal_blk_info_t;

/**
 * @brief       Block device information structure.
 */
struct hal_blk_info {
  /**
   * @brief       Block size in bytes, usually 512.
   */
  uint32_t                  blk_size;
  /**
   * @brief       Total number of blocks.
   */
  uint32_t                  blk_num;
};

/**
 * @interface   block_io_i
 * @extends     base_interface_i
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
  bool (*is_protected)(void *ip);
  bool (*connect)(void *ip);
  bool (*disconnect)(void *ip);
  bool (*read)(void *ip, uint32_t startblk, uint8_t *buffer, uint32_t n);
  bool (*write)(void *ip, uint32_t startblk, const uint8_t *buffer, uint32_t n);
  bool (*sync)(void *ip);
  bool (*getinfo)(void *ip, hal_blk_info_t *bdip);
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

/**
 * @brief       Returns the media write protection status.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @return                      The media state.
 * @retval false                If media is writable.
 * @retval true                 If media is not writable.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkIsWriteProtected(void *ip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->is_protected(ip);
}

/**
 * @brief       Connection to the block device.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkConnect(void *ip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->connect(ip);
}

/**
 * @brief       Disconnection from the block device.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkDisconnect(void *ip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->disconnect(ip);
}

/**
 * @brief       Reads one or more blocks.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @param         startblk      Initial block to read.
 * @param         buffer        Pointer to the read buffer.
 * @param         n             Number of blocks to read.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkRead(void *ip, uint32_t startblk, uint8_t *buffer,
                           uint32_t n) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->read(ip, startblk, buffer, n);
}

/**
 * @brief       Writes one or more blocks.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @param         startblk      Initial block to write.
 * @param         buffer        Pointer to the write buffer.
 * @param         n             Number of blocks to write.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkWrite(void *ip, uint32_t startblk, const uint8_t *buffer,
                            uint32_t n) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->write(ip, startblk, buffer, n);
}

/**
 * @brief       Synchronization with asynchronous write operations.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkSync(void *ip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->sync(ip);
}

/**
 * @brief       Writes one or more blocks.
 *
 * @param[in,out] ip            Pointer to a @p block_io_i instance.
 * @param         bdip          Device information buffer pointer.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool blkGetInfo(void *ip, hal_blk_info_t *bdip) {
  block_io_i *self = (block_io_i *)ip;

  return self->vmt->getinfo(ip, bdip);
}
/** @} */

#endif /* HAL_BLOCK_IO_H */

/** @} */
