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
 * @file    QUADSPIv1/hal_wspi_lld.c
 * @brief   STM32 WSPI subsystem low level driver source.
 *
 * @addtogroup WSPI
 * @{
 */

#include "hal.h"

#if HAL_USE_WSPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define QUADSPI1_DMA_CHANNEL                                                \
  STM32_DMA_GETCHANNEL(STM32_WSPI_QUADSPI1_DMA_STREAM,                      \
                       STM32_QUADSPI1_DMA_CHN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief QUADSPI1 driver identifier.*/
#if STM32_WSPI_USE_QUADSPI1 || defined(__DOXYGEN__)
hal_wspi_driver_c WSPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const hal_wspi_config_t wspi_default_config = {
  .dcr                       = STM32_WSPI_DEFAULT_DCR
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared DMA service routine.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void wspi_lld_serve_dma_interrupt(hal_wspi_driver_c *wspip,
                                         uint32_t flags) {

  /* DMA errors handling.*/
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0U) {
#if defined(STM32_WSPI_DMA_ERROR_HOOK)
    STM32_WSPI_DMA_ERROR_HOOK(wspip);
#endif
    dmaStreamDisable(wspip->dma);
    wspip->qspi->CR &= ~QUADSPI_CR_DMAEN;
    _wspi_isr_error_code(wspip);
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level WSPI driver initialization.
 *
 * @notapi
 */
void wspi_lld_init(void) {

#if STM32_WSPI_USE_QUADSPI1
  wspiObjectInit(&WSPID1);
  WSPID1.qspi    = QUADSPI;
  WSPID1.dma     = NULL;
  WSPID1.dmamode = STM32_DMA_CR_CHSEL(QUADSPI1_DMA_CHANNEL) |
                   STM32_DMA_CR_PL(STM32_WSPI_QUADSPI1_DMA_PRIORITY) |
                   STM32_DMA_CR_PSIZE_BYTE |
                   STM32_DMA_CR_MSIZE_BYTE |
                   STM32_DMA_CR_MINC |
                   STM32_DMA_CR_DMEIE |
                   STM32_DMA_CR_TEIE;
#endif
}

/**
 * @brief   Configures and activates the WSPI peripheral.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t wspi_lld_start(hal_wspi_driver_c *wspip) {

  if (wspip->config == NULL) {
    wspip->config = wspi_lld_setcfg(wspip, NULL);
    if (wspip->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  {
#if STM32_WSPI_USE_QUADSPI1
    if (&WSPID1 == wspip) {
      wspip->dma = dmaStreamAlloc(STM32_WSPI_QUADSPI1_DMA_STREAM,
                                  STM32_WSPI_QUADSPI1_DMA_IRQ_PRIORITY,
                                  (stm32_dmaisr_t)wspi_lld_serve_dma_interrupt,
                                  (void *)wspip);
      if (wspip->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
      rccEnableQUADSPI1(true);
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(wspip->dma, STM32_DMAMUX1_QUADSPI);
#endif
    }
#endif

    dmaStreamSetPeripheral(wspip->dma, &wspip->qspi->DR);
  }

  wspip->qspi->DCR = __wspi_getfield(wspip, dcr);
  wspip->qspi->CR  = ((STM32_WSPI_QUADSPI1_PRESCALER_VALUE - 1U) << 24U) |
                     QUADSPI_CR_TCIE | QUADSPI_CR_DMAEN | QUADSPI_CR_EN;
  wspip->qspi->FCR = QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF |
                     QUADSPI_FCR_CSMF | QUADSPI_FCR_CTOF;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the WSPI peripheral.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 *
 * @notapi
 */
void wspi_lld_stop(hal_wspi_driver_c *wspip) {

  if (wspip->dma != NULL) {
    dmaStreamDisable(wspip->dma);
  }

  wspip->qspi->CR = 0U;

  if (wspip->dma != NULL) {
    dmaStreamFree(wspip->dma);
    wspip->dma = NULL;
  }

#if STM32_WSPI_USE_QUADSPI1
  if (&WSPID1 == wspip) {
    rccDisableQUADSPI1();
  }
#endif
}

/**
 * @brief   Applies a WSPI configuration.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] config    pointer to the configuration structure
 * @return              The current configuration pointer.
 *
 * @notapi
 */
const hal_wspi_config_t *wspi_lld_setcfg(hal_wspi_driver_c *wspip,
                                         const hal_wspi_config_t *config) {

  (void)wspip;

  if (config == NULL) {
    config = &wspi_default_config;
  }

  return config;
}

/**
 * @brief   Selects one of the pre-defined WSPI configurations.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] cfgnum    driver configuration number
 * @return              The configuration pointer.
 *
 * @notapi
 */
const hal_wspi_config_t *wspi_lld_selcfg(hal_wspi_driver_c *wspip,
                                         unsigned cfgnum) {

  if (cfgnum > 0U) {
    return NULL;
  }

  return wspi_lld_setcfg(wspip, NULL);
}

/**
 * @brief   Serves the QUADSPI interrupt.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 *
 * @notapi
 */
void wspi_lld_serve_interrupt(hal_wspi_driver_c *wspip) {
  bool was_receive;

  was_receive = (wspip->state == WSPI_STATE_RECEIVE);

  wspip->qspi->FCR = QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF |
                     QUADSPI_FCR_CSMF | QUADSPI_FCR_CTOF;

  _wspi_isr_complete_code(wspip);

  while ((wspip->dma != NULL) &&
         (dmaStreamGetTransactionSize(wspip->dma) > 0U)) {
  }

  if (wspip->dma != NULL) {
    dmaStreamClearInterrupt(wspip->dma);
    dmaStreamDisable(wspip->dma);
  }

#if defined(STM32L471xx) || defined(STM32L475xx) ||                         \
    defined(STM32L476xx) || defined(STM32L486xx)
  if (was_receive) {
    while ((wspip->qspi->SR & QUADSPI_SR_BUSY) != 0U) {
      (void)wspip->qspi->DR;
    }
  }
#else
  (void)was_receive;
#endif
}

/**
 * @brief   Sends a command without data phase.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] cmdp      pointer to the command descriptor
 *
 * @notapi
 */
void wspi_lld_command(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp) {

#if STM32_USE_STM32_D1_WORKAROUND == TRUE
  if ((cmdp->cfg & (WSPI_CFG_ADDR_MODE_MASK | WSPI_CFG_ALT_MODE_MASK)) == 0U) {
    wspip->qspi->DLR = 0U;
    wspip->qspi->ABR = cmdp->cmd;
    wspip->qspi->CCR = (cmdp->cfg & WSPI_CFG_CMD_MODE_MASK) << 6U;
    return;
  }
#endif

  wspip->qspi->DLR = 0U;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR = cmdp->addr;
  }
}

/**
 * @brief   Sends a command with data over the WSPI bus.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void wspi_lld_send(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp,
                   size_t n, const uint8_t *txbuf) {

  dmaStreamSetMemory0(wspip->dma, txbuf);
  dmaStreamSetTransactionSize(wspip->dma, n);
  dmaStreamSetMode(wspip->dma, wspip->dmamode | STM32_DMA_CR_DIR_M2P);

  wspip->qspi->DLR = n - 1U;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR = cmdp->addr;
  }

  dmaStreamEnable(wspip->dma);
}

/**
 * @brief   Sends a command then receives data over the WSPI bus.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void wspi_lld_receive(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp,
                      size_t n, uint8_t *rxbuf) {

  dmaStreamSetMemory0(wspip->dma, rxbuf);
  dmaStreamSetTransactionSize(wspip->dma, n);
  dmaStreamSetMode(wspip->dma, wspip->dmamode | STM32_DMA_CR_DIR_P2M);

  wspip->qspi->DLR = n - 1U;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg |
                     QUADSPI_CCR_DUMMY_CYCLES(cmdp->dummy) |
                     QUADSPI_CCR_FMODE_0;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR = cmdp->addr;
  }

  dmaStreamEnable(wspip->dma);
}

#if WSPI_SUPPORTS_MEMMAP == TRUE || defined(__DOXYGEN__)
/**
 * @brief   Maps in memory space a WSPI flash device.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[out] addrp    pointer to the mapped flash base address or @p NULL
 *
 * @notapi
 */
void wspi_lld_map_flash(hal_wspi_driver_c *wspip,
                        const wspi_command_t *cmdp,
                        uint8_t **addrp) {

  wspip->qspi->CR &= ~QUADSPI_CR_DMAEN;

  wspip->qspi->DLR = 0U;
  wspip->qspi->ABR = 0U;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg |
                     QUADSPI_CCR_DUMMY_CYCLES(cmdp->dummy) |
                     QUADSPI_CCR_FMODE_1 | QUADSPI_CCR_FMODE_0;

  if (addrp != NULL) {
    *addrp = (uint8_t *)0x90000000U;
  }
}

/**
 * @brief   Unmaps from memory space a WSPI flash device.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 *
 * @notapi
 */
void wspi_lld_unmap_flash(hal_wspi_driver_c *wspip) {

  wspip->qspi->CR |= QUADSPI_CR_ABORT;
  while ((wspip->qspi->CR & QUADSPI_CR_ABORT) != 0U) {
  }

  wspip->qspi->CR |= QUADSPI_CR_DMAEN;
}
#endif /* WSPI_SUPPORTS_MEMMAP == TRUE */

#endif /* HAL_USE_WSPI */

/** @} */
