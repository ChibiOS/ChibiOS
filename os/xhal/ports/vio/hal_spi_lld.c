/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
SPIDriver SPID1;
#endif

/** @brief SPI2 driver identifier.*/
#if VIO_SPI_USE_VSPI2 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const hal_spi_config_t spi_default_config = {
  .ncfg  = 0U
};

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
static inline uint32_t __spi_vspi_setcfg(uint32_t nvuart, uint32_t ncfg) {

  __syscall2r(226, VIO_CALL(SB_VSPI_SETCFG, nvuart), ncfg);
  return (uint32_t)r0;
}

/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void sio_lld_serve_interrupt(SPIDriver *spip) {

  /* DMA errors handling.*/
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
#if defined(STM32_SPI_DMA_ERROR_HOOK)
    /* Hook first, if defined.*/
    STM32_SPI_DMA_ERROR_HOOK(spip);
#endif
    /* Stopping DMAs.*/
    dmaStreamDisable(spip->dmatx);
    dmaStreamDisable(spip->dmarx);

    /* Reporting the failure.*/
    spip->sts |= SPI_STS_FAILED | SPI_STS_RXDMA_FAIL;
    __spi_isr_error_code(spip, HAL_RET_HW_FAILURE);
  }
  else if ((__spi_getfield(spip, mode) & SPI_MODE_CIRCULAR) != 0U) {
    if ((flags & STM32_DMA_ISR_HTIF) != 0U) {
      /* Half buffer interrupt.*/
      __spi_isr_half_code(spip);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0U) {
      /* End buffer interrupt.*/
      __spi_isr_full_code(spip);
    }
  }
  else {
    /* Stopping DMAs.*/
    dmaStreamDisable(spip->dmatx);
    dmaStreamDisable(spip->dmarx);

    /* Operation finished interrupt.*/
    __spi_isr_complete_code(spip);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_SPI_USE_VSPI1 || defined(__DOXYGEN__)
#if !defined(VIO_VSPI1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VSIO1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  sio_lld_serve_interrupt(&SIOD1);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if VIO_SPI_USE_VSPI2 || defined(__DOXYGEN__)
#if !defined(VIO_VSPI2_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VSIO2_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  sio_lld_serve_interrupt(&SIOD2);

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
#if SB_SPI_USE_VSPI1 == TRUE
  sioObjectInit(&SPID1);
  SPID1.sts    = 0U;
  SPID1.nvuart = 0U;
  __sb_vrq_seten(1U << VIO_VSPI1_IRQ);
#endif
#if SB_SPI_USE_VSPI2 == TRUE
  sioObjectInit(&SPID2);
  SPID2.sts    = 0U;
  SPID2.nvuart = 1U;
  __sb_vrq_seten(1U << VIO_VSPI2_IRQ);
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_start(SPIDriver *spip) {
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

  /* Status cleared.*/
  spip->sts = (drv_status_t)0;

  /* Configures the peripheral.*/
  spi_lld_configure(spip, &default_config);

  return msg;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {
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
const hal_spi_config_t *spi_lld_configure(hal_spi_driver_c *spip,
                                          const hal_spi_config_t *config) {

  return __spi_vspi_setcfg(spip->nspi, config->ncfg);
}

/**
 * @brief       Implementation of method @p drvGetStatusX().
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 *
 * @notapi
 */
drv_status_t spi_lld_get_status(hal_spi_driver_c *spip) {

  __syscall1r(98, VIO_CALL(SB_VSPI_GETSTS, siop->nvuart));
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief       Implementation of method @p drvGetAndClearStatusI().
 *
 * @param[in] spip      pointer to the @p hal_spi_driver_c object
 * @param[in] mask      flags to be returned and cleared
 *
 * @notapi
 */
drv_status_t spi_lld_get_clear_status(hal_spi_driver_c *spip,
                                      drv_status_t mask) {

  __syscall2r(226, VIO_CALL(SB_VSPI_GETCLRSTS, spip->nvspi), mask);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LLD) || defined(__DOXYGEN__)
/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {

  /* No implementation on VIO.*/
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {

  /* No implementation on VIO.*/
}
#endif

/**
 * @brief   Ignores data on the SPI bus.
 * @details This synchronous function performs the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     In order to use this function the option @p SPI_USE_SYNCHRONIZATION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_ignore(SPIDriver *spip, size_t n) {

  __syscall2r(226, VIO_CALL(SB_VSPI_PULSES, spip->nvspi), n);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_exchange(SPIDriver *spip, size_t n,
                       const void *txbuf, void *rxbuf) {

  __syscall4r(226, VIO_CALL(SB_VSPI_EXCHANGE, spip->nvspi), n, txbuf, rxbuf);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  __syscall3r(226, VIO_CALL(SB_VSPI_SEND, spip->nvspi), n, txbuf);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  __syscall3r(226, VIO_CALL(SB_VSPI_RECEIVE, spip->nvspi), n, rxbuf);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief   Aborts the ongoing SPI operation, if any.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[out] sizep    pointer to the counter of frames not yet transferred
 *                      or @p NULL
 * @return              The operation status.
 *
 * @notapi
 */
msg_t spi_lld_stop_transfer(SPIDriver *spip, size_t *sizep) {

  __syscall2r(226, VIO_CALL(SB_VSPI_STOP, spip->nvspi), sizep);
  osalDbgAssert(r0 != (uint32_t)-1, "unexpected failure");

  return (sioevents_t)r0;
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {

}

#endif /* HAL_USE_SPI */

/** @} */
