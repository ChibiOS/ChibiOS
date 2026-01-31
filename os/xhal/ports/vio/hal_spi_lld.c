/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sandbox/hal_spi_lld.c
 * @brief   Sandbox SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI1 driver identifier.*/
#if VIO_SPI_USE_VSPI1 || defined(__DOXYGEN__)
hal_spi_driver_c SPID1;
#endif

/** @brief SPI2 driver identifier.*/
#if VIO_SPI_USE_VSPI2 || defined(__DOXYGEN__)
hal_spi_driver_c SPID2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CC_FORCE_INLINE
static inline uint32_t __spi_vspi_init(uint32_t nvuart) {

  __syscall1r(226, VIO_CALL(SB_VSPI_INIT, nvuart));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __spi_vspi_deinit(uint32_t nvuart) {

  __syscall1r(226, VIO_CALL(SB_VSPI_DEINIT, nvuart));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __spi_vspi_selcfg(uint32_t nvuart, uint32_t ncfg,
                                         size_t n, void *p) {

  __syscall4r(226, VIO_CALL(SB_VSPI_SELCFG, nvuart), ncfg, n, p);
  return (uint32_t)r0;
}

/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 */
static void spi_lld_serve_interrupt(hal_spi_driver_c *spip, uint32_t nvrq) {
  uint32_t sts;

  sts = __sb_vrq_gcsts(nvrq);

  if ((sts & (1U << HAL_DRV_STATE_ERROR)) != 0U) {
    /* Transfer error interrupt, discards all others.*/
    __cbdrv_invoke_error_cb(spip);
    __spi_wakeup_isr(spip, HAL_RET_HW_FAILURE);
    return;
  }

  /* Operation finished interrupt.*/
  if ((sts & (1U << HAL_DRV_STATE_COMPLETE)) != 0U) {
    __cbdrv_invoke_complete_cb(spip);
    __spi_wakeup_isr(spip, MSG_OK);
    return;
  }

  /* Half buffer interrupt.*/
  if ((sts & (1U << HAL_DRV_STATE_HALF)) != 0U) {
    __cbdrv_invoke_half_cb(spip);
  }

  /* Full buffer interrupt.*/
  if ((sts & (1U << HAL_DRV_STATE_FULL)) != 0U) {
    __cbdrv_invoke_full_cb(spip);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_SPI_USE_VSPI1 || defined(__DOXYGEN__)
#if !defined(VIO_VSPI1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VSPI1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  spi_lld_serve_interrupt(&SPID1, VIO_VSPI1_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if VIO_SPI_USE_VSPI2 || defined(__DOXYGEN__)
#if !defined(VIO_VSPI2_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VPIO2_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  spi_lld_serve_interrupt(&SPID2, VIO_VSPI2_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

  /* Driver instances initialization.*/
#if VIO_SPI_USE_VSPI1 == TRUE
  spiObjectInit(&SPID1);
  SPID1.nvspi = 0U;
  __sb_vrq_seten(1U << VIO_VSPI1_IRQ);
#endif
#if VIO_SPI_USE_VSPI2 == TRUE
  spiObjectInit(&SPID2);
  SPID2.nvspi = 1U;
  __sb_vrq_seten(1U << VIO_VSPI2_IRQ);
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_start(hal_spi_driver_c *spip) {
  msg_t msg = HAL_RET_SUCCESS;

  /* Enables the peripheral.*/
  if (false) {
  }
#if VIO_SPI_USE_VSPI1 == TRUE
  else if (&SPID1 == spip) {
    msg = (msg_t)__spi_vspi_init(spip->nvspi);
  }
#endif
#if VIO_SPI_USE_VSPI2 == TRUE
  else if (&SPID2 == spip) {
    msg = (msg_t)__spi_vspi_init(spip->nvspi);
  }
#endif
  else {
    osalDbgAssert(false, "invalid SPI instance");
  }

  return msg;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 *
 * @notapi
 */
void spi_lld_stop(hal_spi_driver_c *spip) {
  msg_t msg = HAL_RET_SUCCESS;

  /* Disables the peripheral.*/
  if (false) {
  }
#if VIO_SPI_USE_VSPI1 == TRUE
  else if (&SPID1 == spip) {
    msg = __spi_vspi_deinit(spip->nvspi);
  }
#endif
#if VIO_SPI_USE_VSPI2 == TRUE
  else if (&SPID2 == spip) {
    msg = __spi_vspi_deinit(spip->nvspi);
  }
#endif
  else {
    osalDbgAssert(false, "invalid SIO instance");
  }

  osalDbgAssert(msg = HAL_RET_SUCCESS, "unexpected failure");
}

/**
 * @brief   SPI configuration.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] config    pointer to the @p hal_spi_config_t structure
 * @return              A pointer to the current configuration structure.
 *
 * @notapi
 */
const hal_spi_config_t *spi_lld_setcfg(hal_spi_driver_c *spip, const hal_spi_config_t *config) {

  (void)spip;
  (void)config;

  return NULL;
}

/**
 * @brief       Selects one of the pre-defined SPI configurations.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] cfgnum    driver configuration number
 * @return              The configuration pointer.
 *
 * @notapi
 */
const hal_spi_config_t *spi_lld_selcfg(hal_spi_driver_c *spip, unsigned cfgnum) {
  msg_t msg;

  msg = __spi_vspi_selcfg(spip->nvspi, cfgnum,
                          sizeof (hal_spi_config_t), &spip->cfgbuf);
  if (msg == HAL_RET_SUCCESS) {

    return &spip->cfgbuf;
  }

  return NULL;
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 *
 * @notapi
 */
void spi_lld_select(hal_spi_driver_c *spip) {

  __syscall1r(226, VIO_CALL(SB_VSPI_SELECT, spip->nvspi));
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 *
 * @notapi
 */
void spi_lld_unselect(hal_spi_driver_c *spip) {

  __syscall1r(226, VIO_CALL(SB_VSPI_UNSELECT, spip->nvspi));
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This synchronous function performs the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     In order to use this function the option @p SPI_USE_SYNCHRONIZATION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] n         number of words to be ignored
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_ignore(hal_spi_driver_c *spip, size_t n) {

  __syscall2r(226, VIO_CALL(SB_VSPI_PULSES, spip->nvspi), n);

  return (msg_t)r0;
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_exchange(hal_spi_driver_c *spip, size_t n,
                       const void *txbuf, void *rxbuf) {

  __syscall4r(226, VIO_CALL(SB_VSPI_EXCHANGE, spip->nvspi), n, txbuf, rxbuf);

  return (msg_t)r0;
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_send(hal_spi_driver_c *spip, size_t n, const void *txbuf) {

  __syscall3r(226, VIO_CALL(SB_VSPI_SEND, spip->nvspi), n, txbuf);

  return (msg_t)r0;
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_receive(hal_spi_driver_c *spip, size_t n, void *rxbuf) {

  __syscall3r(226, VIO_CALL(SB_VSPI_RECEIVE, spip->nvspi), n, rxbuf);

  return (msg_t)r0;
}

/**
 * @brief   Aborts the ongoing SPI operation, if any.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[out] sizep    pointer to the counter of frames not yet transferred
 *                      or @p NULL
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_stop_transfer(hal_spi_driver_c *spip, size_t *sizep) {

  __syscall2r(226, VIO_CALL(SB_VSPI_STOP, spip->nvspi), sizep);

  return (msg_t)r0;
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(hal_spi_driver_c *spip, uint16_t frame) {

  (void)spip;
  (void)frame;

  return 0;
}

#endif /* HAL_USE_SPI */

/** @} */
