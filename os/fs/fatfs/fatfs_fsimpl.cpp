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
 * @file    fs_fatfs_impl.cpp
 * @brief   FatFS file system wrapper.
 *
 * @addtogroup fs_fatfs_wrapper
 * @{
 */

#include "ch.hpp"
#include "fs.hpp"
#include "fatfs_fsimpl.hpp"
#include "hal.h"

#define MSG_TERMINATE                   (msg_t)0

#define ERR_OK                          (msg_t)0
#define ERR_TERMINATING                 (msg_t)1
#define ERR_UNKNOWN_MSG                 (msg_t)2

using namespace chibios_rt;
using namespace chibios_fs;

/**
 * @brief   FatFS wrapper-related classes and interfaces.
 */
namespace chibios_fatfs {

  typedef struct {
    uint32_t            msg_code;
    union {
      struct {

      };
    } op;
  } wmsg_t;

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSFileWrapper                                        *
   *------------------------------------------------------------------------*/
  FatFSFileWrapper::FatFSFileWrapper(void) : fs(NULL) {

  }

  FatFSFileWrapper::FatFSFileWrapper(FatFSWrapper *fsref) : fs(fsref) {

  }

  size_t FatFSFileWrapper::write(const uint8_t *bp, size_t n) {

    return 0;
  }

  size_t FatFSFileWrapper::read(uint8_t *bp, size_t n) {

    return 0;
  }

  msg_t FatFSFileWrapper::put(uint8_t b) {

    return 0;
  }

  msg_t FatFSFileWrapper::get(void) {

    return 0;
  }

  uint32_t FatFSFileWrapper::getAndClearLastError(void) {

    return 0;
  }

  fileoffset_t FatFSFileWrapper::getSize(void) {

    return 0;
  }

  fileoffset_t FatFSFileWrapper::getPosition(void) {

    return 0;
  }

  uint32_t FatFSFileWrapper::setPosition(fileoffset_t offset) {

    return 0;
  }

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSFilesPool                                          *
   *------------------------------------------------------------------------*/
  FatFSFilesPool::FatFSFilesPool(void) : ObjectsPool<FatFSFileWrapper,
                                                     FATFS_MAX_FILES>() {

  }

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSServerThread                                       *
   *------------------------------------------------------------------------*/
  FatFSServerThread::FatFSServerThread(void) :
      BaseStaticThread<FATFS_THREAD_STACK_SIZE>() {
  }

  msg_t FatFSServerThread::main() {
    msg_t sts;

    setName("fatfs");

    /* Synchronous messages processing loop.*/
    while (true) {
      ThreadReference tr = waitMessage();
      msg_t msg = tr.getMessage();
      switch (msg) {
      case MSG_TERMINATE:
        /* The server object is being destroyed, terminating.*/
        tr.releaseMessage(ERR_TERMINATING);
        return 0;
      default:
        sts = ERR_UNKNOWN_MSG;
      }
      tr.releaseMessage(sts);
    }
  }

  void FatFSServerThread::stop(void) {

    sendMessage(MSG_TERMINATE);
    wait();
  }

  /*------------------------------------------------------------------------*
   * chibios_fatfs::FatFSWrapper                                            *
   *------------------------------------------------------------------------*/
  FatFSWrapper::FatFSWrapper(void) {

  }

  void FatFSWrapper::mount(void) {

    server.start(FATFS_THREAD_PRIORITY);
  }

  void FatFSWrapper::unmount(void) {

    server.stop();
  }

  uint32_t FatFSWrapper::getAndClearLastError(void) {

    return 0;
  }

  void FatFSWrapper::synchronize(void) {

  }

  void FatFSWrapper::remove(const char *fname) {

    (void)fname;
  }

  BaseFileStreamInterface *FatFSWrapper::open(const char *fname) {

    (void)fname;
    return NULL;
  }

  BaseFileStreamInterface *FatFSWrapper::openForRead(const char *fname) {

    (void)fname;
    return NULL;
  }

  BaseFileStreamInterface *FatFSWrapper::openForWrite(const char *fname) {

    (void)fname;
    return NULL;
  }

  BaseFileStreamInterface *FatFSWrapper::create(const char *fname) {

    (void)fname;
    return NULL;
  }

  void FatFSWrapper::close(BaseFileStreamInterface *file) {

    (void)file;
  }
}

/** @} */
