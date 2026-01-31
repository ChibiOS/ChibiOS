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
 * @file        hal_channels.h
 * @brief       Generated I/O Channels header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_CHANNELS
 * @brief       Sequential data streams interface.
 * @{
 */

#ifndef HAL_CHANNELS_H
#define HAL_CHANNELS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Default control operation codes
 * @{
 */
/**
 * @brief       Invalid operation code.
 */
#define CHN_CTL_INVALID                     0

/**
 * @brief       Does nothing.
 */
#define CHN_CTL_NOP                         1

/**
 * @brief       Wait for TX completion.
 */
#define CHN_CTL_TX_WAIT                     2
/** @} */

/**
 * @name    Channel event flags
 * @{
 */
#define CHN_FL_NONE                         0
#define CHN_FL_PARITY_ERR_POS               0
#define CHN_FL_PARITY_ERR                   (1U << CHN_FL_PARITY_ERR_POS)
#define CHN_FL_FRAMING_ERR_POS              1
#define CHN_FL_FRAMING_ERR                  (1U << CHN_FL_FRAMING_ERR_POS)
#define CHN_FL_NOISE_ERR_POS                2
#define CHN_FL_NOISE_ERR                    (1U << CHN_FL_NOISE_ERR_POS)
#define CHN_FL_OVERRUN_ERR_POS              3
#define CHN_FL_OVERRUN_ERR                  (1U << CHN_FL_OVERRUN_ERR_POS)
#define CHN_FL_BUFFER_FULL_ERR_POS          4
#define CHN_FL_BUFFER_FULL_ERR              (1U << CHN_FL_BUFFER_FULL_ERR_POS)
#define CHN_FL_FULLMASK                     (15U << CHN_FL_PARITY_POS)
#define CHN_FL_TX_NOTFULL_POS               8
#define CHN_FL_TX_NOTFULL                   (1U << CHN_FL_TX_NOTFULL_POS)
#define CHN_FL_RX_NOTEMPTY_POS              9
#define CHN_FL_RX_NOTEMPTY                  (1U << CHN_FL_RX_NOTEMPTY_POS)
#define CHN_FL_TX_END_POS                   10
#define CHN_FL_TX_END                       (1U << CHN_FL_TX_END_POS)
#define CHN_FL_RX_IDLE_POS                  11
#define CHN_FL_RX_IDLE                      (1U << CHN_FL_RX_IDLE_POS)
#define CHN_FL_RX_BREAK_POS                 12
#define CHN_FL_RX_BREAK                     (1U << CHN_FL_RX_BREAK_POS)
#define CHN_FL_CONNECTED_POS                13
#define CHN_FL_CONNECTED                    (1U << CHN_FL_CONNECTED_POS)
#define CHN_FL_DISCONNECTED_POS             14
#define CHN_FL_DISCONNECTED                 (1U << CHN_FL_DISCONNECTED_POS)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Replicated stream functions
 * @{
 */
/**
 * @brief       Channel blocking write.
 * @details     This function writes data from a buffer to a channel. If the
 *              channel is not ready to accept data then the calling thread is
 *              suspended.
 *
 * @param[in,out] ip            Pointer to a @p base_channel_c structure.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred.
 */
#define chnWrite(ip, bp, n)                                                 \
  streamWrite(ip, bp, n)

/**
 * @brief       Channel blocking read with timeout.
 * @details     The function reads data from a channel into a buffer. If the
 *              data is not available then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p base_channel_c structure.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred.
 */
#define chnRead(ip, bp, n)                                                  \
  streamRead(ip, bp, n)

/**
 * @brief       Channel blocking byte write with timeout.
 * @details     This function writes a byte value to a channel. If the channel
 *              is not ready to accept data then the calling thread is
 *              suspended.
 *
 * @param[in,out] ip            Pointer to a @p base_channel_c structure.
 * @param[in]     b             The byte value to be written to the channel.
 * @return                      The operation status.
 * @retval STM_OK               If the operation succeeded."
 * @retval STM_RESET            If the channel was reset."
 */
#define chnPut(ip, b)                                                       \
  streamPut(ip, b)

/**
 * @brief       Channel blocking byte read with timeout.
 * @details     This function reads a byte value from a channel. If the data is
 *              not available then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p base_channel_c structure.
 * @return                      A byte value from the channel.
 * @retval STM_RESET            If the channel was reset."
 */
#define chnGet(ip)                                                          \
  streamGet(ip)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of channel event flags.
 */
typedef eventflags_t chnflags_t;

/**
 * @interface   asynchronous_channel_i
 * @extends     sequential_stream_i
 *
 * @brief       Base I/O channel interface.
 * @details     This header defines an abstract interface useful to access
 *              generic I/O serial devices in a standardized way.
 *
 * @name        Interface @p asynchronous_channel_i structures
 * @{
 */

/**
 * @brief       Type of a base I/O channel interface.
 */
typedef struct asynchronous_channel asynchronous_channel_i;

/**
 * @brief       Interface @p asynchronous_channel_i virtual methods table.
 */
struct asynchronous_channel_vmt {
  /* Memory offset between this interface structure and begin of
     the implementing class structure.*/
  size_t instance_offset;
  /* From base_interface_i.*/
  /* From sequential_stream_i.*/
  size_t (*write)(void *ip, const uint8_t *bp, size_t n);
  size_t (*read)(void *ip, uint8_t *bp, size_t n);
  int (*put)(void *ip, uint8_t b);
  int (*get)(void *ip);
  int (*unget)(void *ip, int b);
  /* From asynchronous_channel_i.*/
  size_t (*writet)(void *ip, const uint8_t *bp, size_t n, sysinterval_t timeout);
  size_t (*readt)(void *ip, uint8_t *bp, size_t n, sysinterval_t timeout);
  msg_t (*putt)(void *ip, uint8_t b, sysinterval_t timeout);
  msg_t (*gett)(void *ip, sysinterval_t timeout);
  chnflags_t (*getclr)(void *ip, chnflags_t mask);
  msg_t (*ctl)(void *ip, unsigned int operation, void *arg);
};

/**
 * @brief       Structure representing a base I/O channel interface.
 */
struct asynchronous_channel {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct asynchronous_channel_vmt *vmt;
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
 * @name        Virtual methods of asynchronous_channel_i
 * @{
 */
/**
 * @brief       Channel blocking write with timeout.
 * @details     This function writes data from a buffer to a channel. If the
 *              channel is not ready to accept data then the calling thread is
 *              suspended.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 *
 * @api
 */
CC_FORCE_INLINE
static inline size_t chnWriteTimeout(void *ip, const uint8_t *bp, size_t n,
                                     sysinterval_t timeout) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->writet(ip, bp, n, timeout);
}

/**
 * @brief       Channel blocking read with timeout.
 * @details     The function reads data from a channel into a buffer. If the
 *              data is not available then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 *
 * @api
 */
CC_FORCE_INLINE
static inline size_t chnReadTimeout(void *ip, uint8_t *bp, size_t n,
                                    sysinterval_t timeout) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->readt(ip, bp, n, timeout);
}

/**
 * @brief       Channel blocking byte write with timeout.
 * @details     This function writes a byte value to a channel. If the channel
 *              is not ready to accept data then the calling thread is
 *              suspended.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     b             The byte value to be written to the channel.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The operation status.
 * @retval STM_OK               If the operation succeeded."
 * @retval STM_TIMEOUT          If the specified time expired."
 * @retval STM_RESET            If the channel was reset."
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t chnPutTimeout(void *ip, uint8_t b, sysinterval_t timeout) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->putt(ip, b, timeout);
}

/**
 * @brief       Channel blocking byte read with timeout.
 * @details     This function reads a byte value from a channel. If the data is
 *              not available then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      A byte value from the channel.
 * @retval STM_TIMEOUT          If the specified time expired."
 * @retval STM_RESET            If the channel was reset."
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t chnGetTimeout(void *ip, sysinterval_t timeout) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->gett(ip, timeout);
}

/**
 * @brief       Returns and clears pending event flags.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     mask          Mask of flags to be returned and cleared.
 * @return                      The cleared event flags.
 * @retval 0                    If no event flags were pending."
 *
 * @api
 */
CC_FORCE_INLINE
static inline chnflags_t chnGetAndClearFlags(void *ip, chnflags_t mask) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->getclr(ip, mask);
}

/**
 * @brief       Control operation on a channel.
 *
 * @param[in,out] ip            Pointer to a @p asynchronous_channel_i
 *                              instance.
 * @param[in]     operation     Control operation code
 * @param[in,out] arg           Operation argument.
 * @return                      The operation status.
 * @retval STM_OK               If the operation succeeded."
 * @retval STM_TIMEOUT          If the specified time expired."
 * @retval STM_RESET            If the channel was reset."
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t chnControl(void *ip, unsigned int operation, void *arg) {
  asynchronous_channel_i *self = (asynchronous_channel_i *)ip;

  return self->vmt->ctl(ip, operation, arg);
}
/** @} */

#endif /* HAL_CHANNELS_H */

/** @} */
