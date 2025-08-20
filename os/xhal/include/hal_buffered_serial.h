/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file        hal_buffered_serial.h
 * @brief       Generated Buffered Serial Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_BUFFERED_SERIAL
 * @brief       Common ancestor of drivers based on circular I/O buffers.
 * @details     This class implements a channel interface and links it to two
 *              circular I/O queues.
 * @{
 */

#ifndef HAL_BUFFERED_SERIAL_H
#define HAL_BUFFERED_SERIAL_H

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
 * @class       hal_buffered_serial_c
 * @extends     hal_base_driver_c
 * @implements  asynchronous_channel_i
 *
 * @brief       Ancestor class of serial buffered drivers.
 *
 * @name        Class @p hal_buffered_serial_c structures
 * @{
 */

/**
 * @brief       Type of a HAL buffered serial driver class.
 */
typedef struct hal_buffered_serial hal_buffered_serial_c;

/**
 * @brief       Class @p hal_buffered_serial_c virtual methods table.
 */
struct hal_buffered_serial_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_buffered_serial_c.*/
};

/**
 * @brief       Structure representing a HAL buffered serial driver class.
 */
struct hal_buffered_serial {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_buffered_serial_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Implemented interface @p asynchronous_channel_i.
   */
  asynchronous_channel_i    chn;
  /**
   * @brief       Input queue.
   */
  input_queue_t             iqueue;
  /**
   * @brief       Output queue.
   */
  output_queue_t            oqueue;
  /**
   * @brief       I/O condition event source.
   */
  event_source_t            event;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_buffered_serial_c.*/
  void *__bs_objinit_impl(void *ip, const void *vmt, uint8_t *ib,
                          size_t ibsize, qnotify_t inotify, void *iarg,
                          uint8_t *ob, size_t obsize, qnotify_t onotify,
                          void *oarg);
  void __bs_dispose_impl(void *ip);
  void bsIncomingDataI(void *ip, uint8_t b);
  msg_t bsRequestDataI(void *ip);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Inline methods of hal_buffered_serial_c
 * @{
 */
/**
 * @brief       Adds status flags to the flags mask.
 * @details     This function is usually called from the I/O ISRs in order to
 *              notify I/O conditions such as data events, errors, signal
 *              changes etc.
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_serial_c instance.
 * @param[in]     flags         Event flags to be added.
 */
CC_FORCE_INLINE
static inline void bsAddFlagsI(void *ip, eventflags_t flags) {
  hal_buffered_serial_c *self = (hal_buffered_serial_c *)ip;

  osalEventBroadcastFlagsI(&self->event, flags);
}
/** @} */

#endif /* HAL_BUFFERED_SERIAL_H */

/** @} */
