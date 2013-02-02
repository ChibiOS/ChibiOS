/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    fs.hpp
 * @brief   File System interfaces header.
 *
 * @addtogroup fs_interface
 * @{
 */

#ifndef _FS_HPP_
#define _FS_HPP_

#include "ch.hpp"

/**
 * @name    Error codes
 * @{
 */
/**
 * @brief   No error return code.
 */
#define FILE_OK         0

/**
 * @brief   Error code from the file stream methods.
 */
#define FILE_ERROR      0xFFFFFFFFUL
/** @} */

/**
 * @brief   ChibiOS FS-related interfaces.
 */
namespace chibios_fs {

  /**
   * @brief   File offset type.
   */
  typedef uint32_t fileoffset_t;

  /*------------------------------------------------------------------------*
   * chibios_fs::BaseFileStreamInterface                                    *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Interface of an abstract file object.
   */
  class BaseFileStreamInterface : public chibios_rt::BaseSequentialStreamInterface {
  public:
    /**
     * @brief   Returns an implementation dependent error code.
     *
     * @return              An implementation-dependent error code.
     *
     * @api
     */
    virtual uint32_t getAndClearLastError(void) = 0;

    /**
     * @brief   Returns the current file size.
     *
     * @return              The file size.
     * @retval FILE_ERROR   if the operation failed.
     *
     * @api
     */
    virtual fileoffset_t getSize(void) = 0;

    /**
     * @brief   Returns the current file pointer position.
     *
     * @return              The current position inside the file.
     * @retval FILE_ERROR   if the operation failed.
     *
     * @api
     */
    virtual fileoffset_t getPosition(void) = 0;

    /**
     * @brief   sets the current file pointer position.
     *
     * @param[in] offset    new absolute position
     * @return              The operation status.
     * @retval FILE_OK      if no error.
     * @retval FILE_ERROR   if the operation failed.
     *
     * @api
     */
    virtual uint32_t setPosition(fileoffset_t offset) = 0;
  };

  /*------------------------------------------------------------------------*
   * chibios_fs::BaseFileSystemInterface                                    *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Interface of an abstract file system object.
   * @note    The interface only exposes common features, the implementing
   *          classes can offer an extended interface.
   */
  class BaseFileSystemInterface {
  public:
    /**
     * @brief   Returns an implementation dependent error code.
     *
     * @return              An implementation-dependent error code.
     *
     * @api
     */
    virtual uint32_t getAndClearLastError(void) = 0;

    /**
     * @brief   Synchronizes caches with media device.
     */
    virtual void synchronize(void) = 0;

    /**
     * @brief   Removes a file.
     *
     * @param[in] fname     file name
     *
     * @api
     */
    virtual void remove(const char *fname) = 0;

    /**
     * @brief   Opens a file for read and write.
     *
     * @param[in] fname     file name
     * @return              An interface of a file object.
     * @retval NULL         if the operation failed.
     *
     * @api
     */
    virtual BaseFileStreamInterface *open(const char *fname) = 0;

    /**
     * @brief   Opens a file for read only.
     *
     * @param[in] fname     file name
     * @return              An interface of a file object.
     * @retval NULL         if the operation failed.
     *
     * @api
     */
    virtual BaseFileStreamInterface *openForRead(const char *fname) = 0;

    /**
     * @brief   Opens a file for write only.
     *
     * @param[in] fname     file name
     * @return              An interface of a file object.
     * @retval NULL         if the operation failed.
     *
     * @api
     */
    virtual BaseFileStreamInterface *openForWrite(const char *fname) = 0;

    /**
     * @brief   Creates a file and opens it for write only.
     * @details If a file with the same name already exists then it is
     *          overwritten.
     *
     * @param[in] fname     file name
     * @return              An interface of a file object.
     * @retval NULL         if the operation failed.
     *
     * @api
     */
    virtual BaseFileStreamInterface *create(const char *fname) = 0;

    /**
     * @brief   Closes a file.
     *
     * @api
     */
    virtual void close(BaseFileStreamInterface *file) = 0;
  };
}
#endif /* _FS_HPP_ */

/** @} */
