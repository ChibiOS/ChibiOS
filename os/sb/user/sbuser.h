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
 * @file    sb/user/sbapi.h
 * @brief   ARM SandBox user API macros and structures.
 *
 * @addtogroup ARM_SANDBOX_USER_API
 * @{
 */

#ifndef SBUSER_H
#define SBUSER_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#include "errcodes.h"
#include "dirent.h"
#include "sbsysc.h"

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
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a parameters structure passed to the sandbox.
 */
typedef struct sb_parameters {
  /**
   * @brief   Number of arguments.
   */
  int                       argc;
  /**
   * @brief   Vector of arguments.
   */
  char                      **argv;
  /**
   * @brief   Pointer to environment variables array.
   */
  char                      **envp;
  /**
   * @brief   Address of the physical sandbox end.
   */
  uint8_t                   *sb_end;
  /**
   * @brief   Address of heap end.
   */
  uint8_t                   *heap_end;
} sb_parameters_t;

/**
 * @brief   Type of system time counter.
 */
typedef uint32_t systime_t;

/**
 * @brief   Type of system time interval.
 */
typedef uint32_t sysinterval_t;

/**
 * @brief   Type of a wide time conversion variable.
 */
typedef uint64_t time_conv_t;

/**
 * @brief   Type of time in microseconds.
 */
typedef uint32_t time_usecs_t;

/**
 * @brief   Type of time in milliseconds.
 */
typedef uint32_t time_msecs_t;

/**
 * @brief   Type of time in seconds.
 */
typedef uint32_t time_secs_t;

/**
 * @brief   Type of a message.
 */
typedef int32_t msg_t;

/**
 * @brief   Type of an event mask.
 */
typedef uint32_t eventmask_t;

/**
 * @brief   Type of event flags.
 */
typedef uint32_t eventflags_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name   Messages-related macros
 * @{
 */
#define MSG_OK              (msg_t)0
#define MSG_TIMEOUT         (msg_t)-1
#define MSG_RESET           (msg_t)-2
/** @} */

/**
 * @name   Events-related macros
 * @{
 */
#define ALL_EVENTS          ((eventmask_t)-1)
#define EVENT_MASK(eid)     ((eventmask_t)1 << (eventmask_t)(eid))
/** @} */

/**
 * @name   Time and intervals related macros
 * @{
 */
#define TIME_IMMEDIATE      ((sysinterval_t)0)
#define TIME_INFINITE       ((sysinterval_t)-1)
#define TIME_MAX_INTERVAL   ((sysinterval_t)-2)
#define TIME_MAX_SYSTIME    ((systime_t)-1)
/** @} */

/**
 * @name   SVC instruction wrappers.
 * @{
 */
#define __syscall0(x)                                                       \
  asm volatile ("svc " #x : : : "memory")

#define __syscall0r(x)                                                      \
  register uint32_t r0 asm ("r0");                                          \
  asm volatile ("svc " #x : "=r" (r0) : : "memory")

#define __syscall0rr(x)                                                     \
  register uint32_t r0 asm ("r0");                                          \
  register uint32_t r1 asm ("r1");                                          \
  asm volatile ("svc " #x : "=r" (r0), "=r" (r1) : : "memory")

#define __syscall1r(x, p1)                                                  \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0) : "memory")

#define __syscall1rr(x, p1)                                                 \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1");                                          \
  asm volatile ("svc " #x : "=r" (r0), "=r" (r1) :                          \
                             "r" (r0) :                                     \
                             "memory")

#define __syscall2r(x, p1, p2)                                              \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  asm volatile ("svc " #x : "=r" (r0) :                                     \
                            "r" (r0), "r" (r1) :                            \
                            "memory")

#define __syscall2rr(x, p1, p2)                                             \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  asm volatile ("svc " #x : "=r" (r0), "=r" (r1) :                          \
                            "r" (r0), "r" (r1) :                            \
                            "memory")

#define __syscall3r(x, p1, p2, p3)                                          \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0), "r" (r1),                 \
                                        "r" (r2) :                          \
                                        "memory")

#define __syscall3rr(x, p1, p2, p3)                                         \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  asm volatile ("svc " #x : "=r" (r0), "=r" (r1) :                          \
                            "r" (r0), "r" (r1), "r" (r2) :                  \
                            "memory")

#define __syscall4r(x, p1, p2, p3, p4)                                      \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  register uint32_t r3 asm ("r3") = (uint32_t)(p4);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0), "r" (r1),                 \
                                        "r" (r2), "r" (r3) :                \
                                        "memory")

#define __syscall4rr(x, p1, p2, p3, p4)                                     \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  register uint32_t r3 asm ("r3") = (uint32_t)(p4);                         \
  asm volatile ("svc " #x : "=r" (r0), "=r" (r1) :                          \
                            "r" (r0), "r" (r1), "r" (r2), "r" (r3) :        \
                            "memory")
/** @} */

/**
 * @brief   VRQ return pseudo-instruction.
 *
 * @api
 */
#define __sb_vrq_return()   __syscall0(255)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern sb_parameters_t __sb_parameters;

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
 * @brief   Posix-style file status.
 *
 * @param[in] pathname  file to be examined
 * @param[in] statbuf   pointer to a @p stat structure
 * @return              Operation result.
 */
static inline int sbStat(const char *pathname, struct stat *statbuf) {

  __syscall3r(128, SB_POSIX_STAT, pathname, statbuf);
  return (int)r0;
}

/**
 * @brief   Posix-style file open.
 *
 * @param[in] pathname  file to be opened
 * @param[in] flags     open mode
 * @return              The file descriptor or an error.
 */
static inline int sbOpen(const char *pathname, int flags) {

  __syscall3r(128, SB_POSIX_OPEN, pathname, flags);
  return (int)r0;
}

/**
 * @brief   Posix-style file close.
 *
 * @param[in] fd        file descriptor
 * @return              Operation result.
 */
static inline int sbClose(int fd) {

  __syscall2r(128, SB_POSIX_CLOSE, fd);
  return (int)r0;
}

/**
 * @brief   Posix-style file descriptor duplication.
 *
 * @param[in] fd        file descriptor
 * @return              Operation result.
 */
static inline int sbDup(int fd) {

  __syscall2r(128, SB_POSIX_DUP, fd);
  return (int)r0;
}

/**
 * @brief   Posix-style file descriptor assignment.
 *
 * @param[in] oldfd     old file descriptor
 * @param[in] newfd     new file descriptor
 * @return              Operation result.
 */
static inline int sbDup2(int oldfd, int newfd) {

  __syscall3r(128, SB_POSIX_DUP, oldfd, newfd);
  return (int)r0;
}

/**
 * @brief   Posix-style file status.
 *
 * @param[in] fd        file descriptor
 * @param[in] statbuf   pointer to a @p stat structure
 * @return              Operation result.
 */
static inline int sbFstat(int fd, struct stat *statbuf) {

  __syscall3r(128, SB_POSIX_FSTAT, fd, statbuf);
  return (off_t)r0;
}

/**
 * @brief   Posix-style file read.
 *
 * @param[in] fd        file descriptor
 * @param[out] buf      buffer pointer
 * @param[in] count     number of bytes
 * @return              The number of bytes really transferred or an error.
 */
static inline ssize_t sbRead(int fd, void *buf, size_t count) {

  __syscall4r(128, SB_POSIX_READ, fd, buf, count);
  return (ssize_t)r0;
}

/**
 * @brief   Posix-style file write.
 *
 * @param[in] fd        file descriptor
 * @param[in] buf       buffer pointer
 * @param[in] count     number of bytes
 * @return              The number of bytes really transferred or an error.
 */
static inline ssize_t sbWrite(int fd, const void *buf, size_t count) {

  __syscall4r(128, SB_POSIX_WRITE, fd, buf, count);
  return (ssize_t)r0;
}

/**
 * @brief   Posix-style file seek.
 *
 * @param[in] fd        file descriptor
 * @param[in] offset    file offset
 * @param[in] whence    operation mode
 * @return              Operation result.
 */
static inline off_t sbSeek(int fd, off_t offset, int whence) {

  __syscall4r(128, SB_POSIX_LSEEK, fd, offset, whence);
  return (off_t)r0;
}

/**
 * @brief   Posix-style directory read.
 * @param[in] fd        file descriptor
 * @param[in] buf       buffer pointer
 * @param[in] count     number of bytes
 * @return              The number of bytes really transferred or an error.
 */
static inline ssize_t sbGetdents(int fd, void *buf, size_t count) {

  __syscall4r(128, SB_POSIX_GETDENTS, fd, buf, count);
  return (ssize_t)r0;
}

/**
 * @brief   Posix-style change current directory.
 *
 * @param[in] path      new current path
 * @return              Operation result.
 */
static inline int sbChdir(const char *path) {

  __syscall2r(128, SB_POSIX_CHDIR, path);
  return (int)r0;
}

/**
 * @brief   Posix-style get current directory.
 *
 * @param[in] buf       path buffer
 * @param[in] size      path buffer size
 * @return              Operation result.
 */
static inline int sbGetcwd(char *buf, size_t size) {

  __syscall3r(128, SB_POSIX_GETCWD, buf, size);
  return (int)r0;
}

/**
 * @brief   Posix-style unlink file.
 *
 * @param[in] path      file to be unlinked
 * @return              Operation result.
 */
static inline int sbUnlink(const char *path) {

  __syscall2r(128, SB_POSIX_UNLINK, path);
  return (int)r0;
}

/**
 * @brief   Posix-style rename file or directory.
 *
 * @param[in] oldpath   old path to the file or directory
 * @param[in] newpath   new path to the file or directory
 * @return              Operation result.
 */
static inline int sbRename(const char *oldpath, const char *newpath) {

  __syscall3r(128, SB_POSIX_RENAME, oldpath, newpath);
  return (int)r0;
}

/**
 * @brief   Posix-style create directory.
 *
 * @param[in] path      directory to be created
 * @param[in] mode      directory creation mode
 * @return              Operation result.
 */
static inline int sbMkdir(const char *path, mode_t mode) {

  __syscall3r(128, SB_POSIX_MKDIR, path, mode);
  return (int)r0;
}

/**
 * @brief   Posix-style remove directory.
 *
 * @param[in] path      directory to be removed
 * @return              Operation result.
 */
static inline int sbRmdir(const char *path) {

  __syscall2r(128, SB_POSIX_RMDIR, path);
  return (int)r0;
}

/**
 * @brief   Returns the system time.
 *
 * @return              The current system time.
 */
static inline systime_t sbGetSystemTime(void) {

  __syscall0r(1);
  return (systime_t)r0;
}

/**
 * @brief   Returns the system time frequency.
 *
 * @return              The system time frequency.
 */
static inline uint32_t sbGetFrequency(void) {

  __syscall0r(2);
  return (uint32_t)r0;
}

/**
 * @brief   Terminates the sandbox.
 *
 * @param[in] msg       The exit message.
 *
 * @api
 */
static inline void sbExit(msg_t msg) {

  __syscall1r(129, msg);
}

/**
 * @brief   Suspends the invoking thread for the specified interval.
 *
 * @param[in] interval  the delay in system ticks
 *
 * @api
 */
static inline void sbSleep(sysinterval_t interval) {

  __syscall1r(130, interval);
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 * @note    The system time is assumed to be between @p start and @p next
 *          else the call is assumed to have been called outside the
 *          allowed time interval, in this case no sleep is performed.
 *
 * @param[in] prev      absolute system time of the previous deadline
 * @param[in] next      absolute system time of the next deadline
 * @return              the @p next parameter
 *
 * @api
 */
static inline void sbSleepUntil(systime_t prev, systime_t next) {

  __syscall2r(131, prev, next);
}

/**
 * @brief   Waits for a message.
 *
 * @return              The received message.
 */
static inline msg_t sbMsgWait(void) {

  __syscall0r(132);
  return (uint32_t)r0;
}

/**
 * @brief   Replies to a message.
 *
 * @param[in] msg       the reply message
 *
 * @api
 */
static inline uint32_t sbMsgReply(msg_t msg) {

  __syscall1r(133, msg);
  return (uint32_t)r0;
}

/**
 * @brief   Waits for exactly one of the specified events.
 * @details The function waits for one event among those specified in
 *          @p events to become pending then the event is cleared and returned.
 * @note    One and only one event is served in the function, the one with the
 *          lowest event id. The function is meant to be invoked into a loop
 *          in order to serve all the pending events.<br>
 *          This means that Event Listeners with a lower event identifier have
 *          an higher priority.
 *
 * @param[in] events    events that the function should wait
 *                      for, @p ALL_EVENTS enables all the events
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mask of the lowest event id served and cleared.
 * @retval 0            if the operation has timed out.
 *
 * @api
 */
static inline eventmask_t sbEventWaitOneTimeout(eventmask_t events,
                                                sysinterval_t timeout) {

  __syscall2r(134, events, timeout);
  return (uint32_t)r0;
}

/**
 * @brief   Waits for any of the specified events.
 * @details The function waits for any event among those specified in
 *          @p events to become pending then the events are cleared and
 *          returned.
 *
 * @param[in] events    events that the function should wait
 *                      for, @p ALL_EVENTS enables all the events
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mask of the served and cleared events.
 * @retval 0            if the operation has timed out.
 *
 * @api
 */
static inline eventmask_t sbEventWaitAnyTimeout(eventmask_t events,
                                                sysinterval_t timeout) {

  __syscall2r(135, events, timeout);
  return (uint32_t)r0;
}

/**
 * @brief   Waits for all the specified events.
 * @details The function waits for all the events specified in @p events to
 *          become pending then the events are cleared and returned.
 *
 * @param[in] events    events that the function should wait
 *                      for, @p ALL_EVENTS requires all the events
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mask of the served and cleared events.
 * @retval 0            if the operation has timed out.
 *
 * @api
 */
static inline eventmask_t sbEventWaitAllTimeout(eventmask_t events,
                                                sysinterval_t timeout) {

  __syscall2r(136, events, timeout);
  return (uint32_t)r0;
}

/**
 * @brief   Signals all the Event Listeners registered on the specified Event
 *          Source.
 *
 * @param[in] flags     the flags set to be added to the listener flags mask
 *
 * @api
 */
static inline uint32_t sbEventBroadcastFlags(eventflags_t flags) {

  __syscall1r(137, flags);
  return (uint32_t)r0;
}

/**
 * @brief   Loads an elf file within the sandbox into the specified buffer.
 * @details The file is loaded and relocated starting from the buffer
 *          address.
 *
 * @param[in] fname     file to be loaded
 * @param[in] buf       load buffer
 * @param[in] size      size of the load buffer
 *
 * @api
 */
static inline int sbLoadElf(const char *fname, uint8_t *buf, size_t size) {

  __syscall3r(138, fname, buf, size);
  return (uint32_t)r0;

}

/**
 * @brief   Seconds to time interval.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] secs      number of seconds
 * @return              The number of ticks.
 *
 * @special
 */
static inline sysinterval_t sbTimeS2I(time_secs_t secs) {
  time_conv_t ticks;
  uint32_t f = sbGetFrequency();

  ticks = (time_conv_t)secs * f;

/*  sbDbgAssert(ticks <= (time_conv_t)TIME_MAX_INTERVAL,
              "conversion overflow");*/

  return (sysinterval_t)ticks;
}

/**
 * @brief   Milliseconds to time interval.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @special
 */
static inline sysinterval_t sbTimeMS2I(time_msecs_t msec) {
  time_conv_t ticks;
  uint32_t f = sbGetFrequency();

  ticks = (((time_conv_t)msec * f) + (time_conv_t)999) / (time_conv_t)1000;

/*  chDbgAssert(ticks <= (time_conv_t)TIME_MAX_INTERVAL,
              "conversion overflow");*/

  return (sysinterval_t)ticks;
}

/**
 * @brief   Microseconds to time interval.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @special
 */
static inline sysinterval_t sbTimeUS2I(time_usecs_t usec) {
  time_conv_t ticks;
  uint32_t f = sbGetFrequency();

  ticks = (((time_conv_t)usec * f) + (time_conv_t)999999) / (time_conv_t)1000000;

/*  chDbgAssert(ticks <= (time_conv_t)TIME_MAX_INTERVAL,
              "conversion overflow");*/

  return (sysinterval_t)ticks;
}

/**
 * @brief   Time interval to seconds.
 * @details Converts from system interval to seconds.
 * @note    The result is rounded up to the next second boundary.
 *
 * @param[in] interval  interval in ticks
 * @return              The number of seconds.
 *
 * @special
 */
static inline time_secs_t sbTimeI2S(sysinterval_t interval) {
  time_conv_t secs;
  uint32_t f = sbGetFrequency();

  secs = ((time_conv_t)interval + f - (time_conv_t)1) / f;

/*  sbDbgAssert(secs < (time_conv_t)((time_secs_t)-1),
              "conversion overflow");*/

  return (time_secs_t)secs;
}

/**
 * @brief   Time interval to milliseconds.
 * @details Converts from system interval to milliseconds.
 * @note    The result is rounded up to the next millisecond boundary.
 *
 * @param[in] interval  interval in ticks
 * @return              The number of milliseconds.
 *
 * @special
 */
static inline time_msecs_t sbTimeI2MS(sysinterval_t interval) {
  time_conv_t msecs;
  uint32_t f = sbGetFrequency();

  msecs = (((time_conv_t)interval * (time_conv_t)1000) + f - (time_conv_t)1) / f;

/*  sbDbgAssert(msecs < (time_conv_t)((time_msecs_t)-1),
              "conversion overflow");*/

  return (time_msecs_t)msecs;
}

/**
 * @brief   Time interval to microseconds.
 * @details Converts from system interval to microseconds.
 * @note    The result is rounded up to the next microsecond boundary.
 *
 * @param[in] interval  interval in ticks
 * @return              The number of microseconds.
 *
 * @special
 */
static inline time_usecs_t sbTimeI2US(sysinterval_t interval) {
  time_conv_t usecs;
  uint32_t f = sbGetFrequency();

  usecs = (((time_conv_t)interval * (time_conv_t)1000000) + f - (time_conv_t)1) / f;

/*  sbDbgAssert(usecs <= (time_conv_t)((time_usecs_t)-1),
              "conversion overflow");*/

  return (time_usecs_t)usecs;
}

/**
 * @brief   Adds an interval to a system time returning a system time.
 *
 * @param[in] systime   base system time
 * @param[in] interval  interval to be added
 * @return              The new system time.
 *
 * @xclass
 */
static inline systime_t sbTimeAddX(systime_t systime, sysinterval_t interval) {

  return systime + (systime_t)interval;
}

/**
 * @brief   Subtracts two system times returning an interval.
 *
 * @param[in] start     first system time
 * @param[in] end       second system time
 * @return              The interval representing the time difference.
 *
 * @xclass
 */
static inline sysinterval_t sbTimeDiffX(systime_t start, systime_t end) {

  return (sysinterval_t)((systime_t)(end - start));
}

/**
 * @brief   Checks if the specified time is within the specified time range.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @return              The time range check result.
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
static inline bool sbTimeIsInRangeX(systime_t time, systime_t start, systime_t end) {

  return (bool)((systime_t)((systime_t)time - (systime_t)start) <
                (systime_t)((systime_t)end - (systime_t)start));
}

/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 *
 * @param[in] secs      time in seconds
 *
 * @api
 */
static inline void sbSleepSeconds(time_secs_t secs) {

  sbSleep(sbTimeS2I(secs));
}

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 *
 * @param[in] msecs     time in milliseconds
 *
 * @api
 */
static inline void sbSleepMilliseconds(time_msecs_t msecs) {

  sbSleep(sbTimeMS2I(msecs));
}

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 *
 * @param[in] usecs     time in microseconds
 *
 * @api
 */
static inline void sbSleepMicroseconds(time_usecs_t usecs) {

  sbSleep(sbTimeUS2I(usecs));
}

/**
 * @brief   Sets an alarm.
 * @note    On alarm a VRQ is triggered, the VRQ number is hard-coded in the
 *          sandbox configuration, default is zero.
 *
 * @param[in] interval  the interval in system ticks
 * @param[in] reload    specifies a periodic alarm
 *
 * @api
 */
static inline void sbSetAlarm(sysinterval_t interval, bool reload) {

  __syscall2r(253, (uint32_t)interval, (uint32_t)reload);
}

/**
 * @brief   Resets an alarm.
 *
 * @api
 */
static inline void sbResetAlarm(void) {

  __syscall0(254);
}

/**
 * @brief   VRQ @p wait pseudo-instruction.
 *
 * @api
 */
static inline void __sb_vrq_wait(void) {

  __syscall0(255);
}

/**
 * @brief   VRQ @p gcsts pseudo-instruction.
 *
 * @param[in] n         VRQ number
 * @return              The associated VRQ flags.
 *
 * @api
 */
static inline uint32_t __sb_vrq_gcsts(uint32_t nvrq) {

  __syscall1r(120, nvrq);
  return r0;
}

/**
 * @brief   VRQ @p setwt pseudo-instruction.
 *
 * @param[in] m         VRQs mask
 * @return              The mask of pending interrupts.
 *
 * @api
 */
static inline uint32_t __sb_vrq_setwt(uint32_t m) {

  __syscall1r(120, m);
  return r0;
}

/**
 * @brief   VRQ @p clrwt pseudo-instruction.
 *
 * @param[in] m         VRQs mask
 * @return              The mask of pending interrupts.
 *
 * @api
 */
static inline uint32_t __sb_vrq_clrwt(uint32_t m) {

  __syscall1r(121, m);
  return r0;
}

/**
 * @brief   VRQ @p seten pseudo-instruction.
 *
 * @param[in] m         VRQs mask
 * @return              The mask of enabled interrupts.
 *
 * @api
 */
static inline uint32_t __sb_vrq_seten(uint32_t m) {

  __syscall1r(122, m);
  return r0;
}

/**
 * @brief   VRQ @p clren pseudo-instruction.
 *
 * @param[in] m         VRQs mask
 * @return              The mask of enabled interrupts.
 *
 * @api
 */
static inline uint32_t __sb_vrq_clren(uint32_t m) {

  __syscall1r(123, m);
  return r0;
}

/**
 * @brief   VRQ @p disable pseudo-instruction.
 *
 * @api
 */
static inline void __sb_vrq_disable(void) {

  __syscall0(124);
}

/**
 * @brief   VRQ @p enable pseudo-instruction.
 *
 * @api
 */
static inline void __sb_vrq_enable(void) {

  __syscall0(125);
}

/**
 * @brief   VRQ @p getisr pseudo-instruction.
 *
 * @return              The interrupts status register value.
 *
 * @api
 */
static inline uint32_t __sb_vrq_getisr(void) {

  __syscall0r(126);
  return r0;
}

#endif /* SBUSER_H */

/** @} */
