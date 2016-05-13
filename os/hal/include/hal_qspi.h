/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    hal_qspi.h
 * @brief   QSPI Driver macros and structures.
 *
 * @addtogroup QSPI
 * @{
 */

#ifndef HAL_QSPI_H
#define HAL_QSPI_H

#if (HAL_USE_QSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Transfer options
 * @{
 */
#define QSPI_CFG_CMD_MASK                   (0xFFU << 0U)
#define QSPI_CFG_CMD(n)                     ((n) << 0U)
#define QSPI_CFG_CMD_MODE_MASK              (3U << 8U)
#define QSPI_CFG_CMD_MODE_NONE              (0U << 8U)
#define QSPI_CFG_CMD_MODE_ONE_LINE          (1U << 8U)
#define QSPI_CFG_CMD_MODE_TWO_LINES         (2U << 8U)
#define QSPI_CFG_CMD_MODE_FOUR_LINES        (3U << 8U)
#define QSPI_CFG_ADDR_MODE_MASK             (3U << 10U)
#define QSPI_CFG_ADDR_MODE_NONE             (0U << 10U)
#define QSPI_CFG_ADDR_MODE_ONE_LINE         (1U << 10U)
#define QSPI_CFG_ADDR_MODE_TWO_LINES        (2U << 10U)
#define QSPI_CFG_ADDR_MODE_FOUR_LINES       (3U << 10U)
#define QSPI_CFG_ADDR_SIZE_MASK             (3U << 12U)
#define QSPI_CFG_ADDR_SIZE_8                (0U << 12U)
#define QSPI_CFG_ADDR_SIZE_16               (1U << 12U)
#define QSPI_CFG_ADDR_SIZE_24               (2U << 12U)
#define QSPI_CFG_ADDR_SIZE_32               (3U << 12U)
#define QSPI_CFG_ALT_MODE_MASK              (3U << 14U)
#define QSPI_CFG_ALT_MODE_NONE              (0U << 14U)
#define QSPI_CFG_ALT_MODE_ONE_LINE          (1U << 14U)
#define QSPI_CFG_ALT_MODE_TWO_LINES         (2U << 14U)
#define QSPI_CFG_ALT_MODE_FOUR_LINES        (3U << 14U)
#define QSPI_CFG_ALT_SIZE_MASK              (3U << 16U)
#define QSPI_CFG_ALT_SIZE_8                 (0U << 16U)
#define QSPI_CFG_ALT_SIZE_16                (1U << 16U)
#define QSPI_CFG_ALT_SIZE_24                (2U << 16U)
#define QSPI_CFG_ALT_SIZE_32                (3U << 16U)
#define QSPI_CFG_DUMMY_CYCLES_MASK          (0x1FU << 18U)
#define QSPI_CFG_DUMMY_CYCLES(n)            ((n) << 18U)
#define QSPI_CFG_DATA_MODE_MASK             (3U << 24U)
#define QSPI_CFG_DATA_MODE_NONE             (0U << 24U)
#define QSPI_CFG_DATA_MODE_ONE_LINE         (1U << 24U)
#define QSPI_CFG_DATA_MODE_TWO_LINES        (2U << 24U)
#define QSPI_CFG_DATA_MODE_FOUR_LINES       (3U << 24U)
#define QSPI_CFG_F_MODE_MASK                (3U << 26U)
#define QSPI_CFG_F_MODE_INDIRECT_WRITE      (0U << 26U)
#define QSPI_CFG_F_MODE_INDIRECT_READ       (1U << 26U)
#define QSPI_CFG_F_MODE_MEMORY_MAPPED       (2U << 26U)
#define QSPI_CFG_F_MODE_FOUR_LINES          (3U << 26U)
#define QSPI_CFG_SIOO                       (1U << 28U)
#define QSPI_CFG_DDRM                       (1U << 31U)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    QSPI configuration options
 * @{
 */
/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(QSPI_USE_WAIT) || defined(__DOXYGEN__)
#define QSPI_USE_WAIT                       TRUE
#endif

/**
 * @brief   Enables the @p qspiAcquireBus() and @p qspiReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(QSPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define QSPI_USE_MUTUAL_EXCLUSION           TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  QSPI_UNINIT = 0,                  /**< Not initialized.                   */
  QSPI_STOP = 1,                    /**< Stopped.                           */
  QSPI_READY = 2,                   /**< Ready.                             */
  QSPI_ACTIVE = 3,                  /**< Exchanging data.                   */
  QSPI_COMPLETE = 4                 /**< Asynchronous operation complete.   */
} qspistate_t;

/**
 * @brief   Type of a QSPI command descriptor.
 */
typedef struct {
  uint32_t              cfg;
  uint32_t              addr;
  uint32_t              alt;
} qspi_command_t;

#include "hal_qspi_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Sends data over the QSPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of bytes to send or zero if no data phase
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @iclass
 */
#define qspiStartSendI(qspip, cmd, n, txbuf) {                              \
  (qspip)->state = QSPI_ACTIVE;                                             \
  qspi_lld_send(qspip, cmd, n, txbuf);                                      \
}

/**
 * @brief   Receives data from the QSPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of bytes to receive or zero if no data phase
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @iclass
 */
#define qspiStartReceiveI(qspip, cmd, n, rxbuf) {                           \
  (qspip)->state = QSPI_ACTIVE;                                             \
  qspi_lld_receive(qspip, cmd, n, rxbuf);                                   \
}
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
#if (QSPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
#define _qspi_wakeup_isr(qspip) {                                           \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(qspip)->thread, MSG_OK);                              \
  osalSysUnlockFromISR();                                                   \
}
#else /* !QSPI_USE_WAIT */
#define _qspi_wakeup_isr(qspip)
#endif /* !QSPI_USE_WAIT */

/**
 * @brief   Common ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
#define _qspi_isr_code(qspip) {                                             \
  if ((qspip)->config->end_cb) {                                            \
    (qspip)->state = QSPI_COMPLETE;                                         \
    (qspip)->config->end_cb(qspip);                                         \
    if ((qspip)->state == QSPI_COMPLETE)                                    \
      (qspip)->state = QSPI_READY;                                          \
  }                                                                         \
  else                                                                      \
    (qspip)->state = QSPI_READY;                                            \
  _qspi_wakeup_isr(qspip);                                                  \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void qspiInit(void);
  void qspiObjectInit(QSPIDriver *qspip);
  void qspiStart(QSPIDriver *qspip, const QSPIConfig *config);
  void qspiStop(QSPIDriver *qspip);
  void qspiStartSend(QSPIDriver *qspip, const qspi_command_t *cmdp,
                     size_t n, const uint8_t *txbuf);
  void qspiStartReceive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                        size_t n, uint8_t *rxbuf);
#if QSPI_USE_WAIT == TRUE
  void qspiSend(QSPIDriver *qspip, const qspi_command_t *cmdp,
                size_t n, const uint8_t *txbuf);
  void qspiReceive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                   size_t n, uint8_t *rxbuf);
#endif
#if QSPI_USE_MUTUAL_EXCLUSION == TRUE
  void qspiAcquireBus(QSPIDriver *qspip);
  void qspiReleaseBus(QSPIDriver *qspip);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_QSPI == TRUE */

#endif /* HAL_QSPI_H */

/** @} */
