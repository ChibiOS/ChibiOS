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
 * @file    fs_fatfs_impl.hpp
 * @brief   FatFS file system wrapper header.
 *
 * @addtogroup cpp_library
 * @{
 */

#include "ch.hpp"
#include "fs.hpp"

#ifndef _FS_FATFS_IMPL_HPP_
#define _FS_FATFS_IMPL_HPP_

/**
 * @brief   Stack size for the internal server thread.
 */
#if !defined(FATFS_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define FATFS_THREAD_STACK_SIZE         1024
#endif

/**
 * @brief   Priority for the internal server thread.
 */
#if !defined(FATFS_THREAD_PRIORITY) || defined(__DOXYGEN__)
#define FATFS_THREAD_PRIORITY           NORMALPRIO
#endif

/**
 * @brief   Maximum number of open files.
 */
#if !defined(FATFS_MAX_FILES) || defined(__DOXYGEN__)
#define FATFS_MAX_FILES                 16
#endif

using namespace chibios_rt;
using namespace chibios_fs;

/**
 * @brief   FatFS wrapper-related classes and interfaces.
 */
namespace chibios_fatfs {

  class FatFSWrapper;

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSFileWrapper                                        *
   *------------------------------------------------------------------------*/
  class FatFSFileWrapper : public BaseFileStreamInterface {
    friend class FatFSWrapper;

  protected:
    FatFSWrapper *fs;

  public:
    FatFSFileWrapper(void);
    FatFSFileWrapper(FatFSWrapper *fsref);

    virtual size_t write(const uint8_t *bp, size_t n);
    virtual size_t read(uint8_t *bp, size_t n);
    virtual msg_t put(uint8_t b);
    virtual msg_t get(void);
    virtual uint32_t getAndClearLastError(void);
    virtual fileoffset_t getSize(void);
    virtual fileoffset_t getPosition(void);
    virtual uint32_t setPosition(fileoffset_t offset);
  };

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSFilesPool                                          *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class of memory pool of @p FatFSFileWrapper objects.
   */
  class FatFSFilesPool : public ObjectsPool<FatFSFileWrapper,
                                            FATFS_MAX_FILES> {
  public:
    FatFSFilesPool(void);
  };

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSServerThread                                       *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class of the internal server thread.
   */
  class FatFSServerThread : public BaseStaticThread<FATFS_THREAD_STACK_SIZE> {
  private:
    FatFSFilesPool files;
  protected:
    virtual msg_t main(void);
  public:
    FatFSServerThread(void);
    virtual void stop(void);
  };

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSWrapper                                            *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class of the FatFS wrapper.
   */
  class FatFSWrapper : public chibios_fs::BaseFileSystemInterface {
    friend class FatFSFileWrapper;

  protected:
    FatFSServerThread server;

  public:
    FatFSWrapper(void);
    virtual uint32_t getAndClearLastError(void);
    virtual void synchronize(void);
    virtual void remove(const char *fname);
    virtual BaseFileStreamInterface *open(const char *fname);
    virtual BaseFileStreamInterface *openForRead(const char *fname);
    virtual BaseFileStreamInterface *openForWrite(const char *fname);
    virtual BaseFileStreamInterface *create(const char *fname);
    virtual void close(BaseFileStreamInterface *file);

    /**
     * @brief   Mounts the file system.
     */
    void mount(void);

    /**
     * @brief   Unmounts the file system.
     */
    void unmount(void);
  };
}

#endif /* _FS_FATFS_IMPL_HPP_ */

/** @} */
