/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    spi.h
 * @brief   SPI Driver macros and structures.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_H_
#define _SPI_H_

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Enables the "wait" APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_WAIT) || defined(__DOXYGEN__)
#define SPI_USE_WAIT                TRUE
#endif

/**
 * @brief   Enables the @p spiAcquireBus() and @p spiReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if SPI_USE_MUTUAL_EXCLUSION && !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "SPI_USE_MUTUAL_EXCLUSION requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  SPI_UNINIT = 0,                   /**< @brief Not initialized.            */
  SPI_STOP = 1,                     /**< @brief Stopped.                    */
  SPI_READY = 2,                    /**< @brief Ready.                      */
  SPI_SYNC= 3,                      /**< @brief Synchronizing.              */
  SPI_SELECTED = 4,                 /**< @brief Slave selected.             */
  SPI_ACTIVE = 5                    /**< @brief Exchanging data.            */
} spistate_t;

#include "spi_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
#define spiSelectI(spip) {                                                  \
  (spip)->spd_state = SPI_SELECTED;                                         \
  spi_lld_select(spip);                                                     \
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
#define spiUnselectI(spip) {                                                \
  (spip)->spd_state = SPI_READY;                                            \
  spi_lld_unselect(spip);                                                   \
}

/**
 * @brief   Emits a train of clock pulses on the SPI bus.
 * @details This asynchronous function starts the emission of a train of
 *          clock pulses without asserting any slave.
 * @note    This functionality is not usually required by the SPI protocol
 *          but it is required by initialization procedure of MMC/SD cards
 *          in SPI mode.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be clocked. The number of pulses
 *                      is equal to the number of words multiplied to the
 *                      configured word size in bits.
 *
 * @api
 */
#define spiSynchronizeI(spip, n) {                                          \
  (spip)->spd_state = SPI_SYNC;                                             \
  spi_lld_ignore(spip, n);                                                  \
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
#define spiIgnoreI(spip, n) {                                               \
  (spip)->spd_state = SPI_ACTIVE;                                           \
  spi_lld_ignore(spip, n);                                                  \
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
#define spiExchangeI(spip, n, txbuf, rxbuf) {                               \
  (spip)->spd_state = SPI_ACTIVE;                                           \
  spi_lld_exchange(spip, n, txbuf, rxbuf);                                  \
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
#define spiSendI(spip, n, txbuf) {                                          \
  (spip)->spd_state = SPI_ACTIVE;                                           \
  spi_lld_send(spip, n, txbuf);                                             \
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
#define spiReceiveI(spip, n, rxbuf) {                                       \
  (spip)->spd_state = SPI_ACTIVE;                                           \
  spi_lld_receive(spip, n, rxbuf);                                          \
}

#if SPI_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Awakens the thread waiting for operation completion, if any.
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
#define _spi_wakeup(spip) {                                                 \
  if ((spip)->spd_thread != NULL) {                                         \
    Thread *tp = (spip)->spd_thread;                                        \
    (spip)->spd_thread = NULL;                                              \
    tp->p_u.rdymsg = RDY_RESET;                                             \
    chSchReadyI(tp);                                                        \
  }                                                                         \
}

/**
 * @brief   Waits for operation completion.
 * @details This function waits for the driver to complete the current
 *          operation.
 * @pre     An operation must be running while the function is invoked.
 * @post    On exit the SPI driver is ready to accept more commands.
 * @note    No more than one thread can wait on a SPI driver using
 *          this function.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @sclass
 */
#define spiWaitS(spip) {                                                    \
  chDbgAssert((spip)->spd_thread == NULL,                                   \
              "spiWaitS(), #1", "already waiting");                         \
  (spip)->spd_thread = chThdSelf();                                         \
  chSchGoSleepS(THD_STATE_SUSPENDED);                                       \
}
#else /* !SPI_USE_WAIT */

/* No wakeup when wait functions are disabled.*/
#define _spi_wakeup(spip)

#endif /* !SPI_USE_WAIT */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void spiInit(void);
  void spiObjectInit(SPIDriver *spip);
  void spiStart(SPIDriver *spip, const SPIConfig *config);
  void spiStop(SPIDriver *spip);
  void spiSelect(SPIDriver *spip);
  void spiUnselect(SPIDriver *spip);
  void spiSynchronize(SPIDriver *spip, size_t n);
  void spiIgnore(SPIDriver *spip, size_t n);
  void spiExchange(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf);
  void spiSend(SPIDriver *spip, size_t n, const void *txbuf);
  void spiReceive(SPIDriver *spip, size_t n, void *rxbuf);
#if SPI_USE_WAIT
  void spiWait(SPIDriver *spip);
  void spiSynchronizeWait(SPIDriver *spip, size_t n);
  void spiIgnoreWait(SPIDriver *spip, size_t n);
  void spiExchangeWait(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf);
  void spiSendWait(SPIDriver *spip, size_t n, const void *txbuf);
  void spiReceiveWait(SPIDriver *spip, size_t n, void *rxbuf);
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION
  void spiAcquireBus(SPIDriver *spip);
  void spiReleaseBus(SPIDriver *spip);
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_SPI */

#endif /* _SPI_H_ */

/** @} */
