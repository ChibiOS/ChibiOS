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
 * @file    OCTOSPIv3/hal_wspi_lld.c
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

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief OCTOSPI1 driver identifier.*/
#if STM32_WSPI_USE_OCTOSPI1 || defined(__DOXYGEN__)
hal_wspi_driver_c WSPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const hal_wspi_config_t wspi_default_config = {
  .dcr1                      = STM32_WSPI_DEFAULT_DCR1,
  .dcr2                      = STM32_WSPI_DEFAULT_DCR2,
  .dcr3                      = STM32_WSPI_DEFAULT_DCR3,
  .dcr4                      = STM32_WSPI_DEFAULT_DCR4
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for completion of previous operation.
 */
static inline void wspi_lld_sync(hal_wspi_driver_c *wspip) {

  while ((wspip->ospi->SR & OCTOSPI_SR_BUSY) != 0U) {
  }
}

/**
 * @brief   Shared DMA3 service routine.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] csr       content of the CxSR register
 */
static void wspi_lld_serve_dma_interrupt(void *p, uint32_t csr) {
  hal_wspi_driver_c *wspip = (hal_wspi_driver_c *)p;
  bool data_transfer;

  data_transfer = (wspip->state == WSPI_STATE_SEND) ||
                  (wspip->state == WSPI_STATE_RECEIVE);

  if ((csr & STM32_DMA3_CSR_ERRORS) != 0U) {
#if defined(STM32_WSPI_DMA_ERROR_HOOK)
    STM32_WSPI_DMA_ERROR_HOOK(wspip);
#endif
    if (data_transfer && (wspip->dmachp != NULL)) {
      (void)dma3ChannelDisable(wspip->dmachp);
    }
    wspip->ospi->CR &= ~OCTOSPI_CR_DMAEN;
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

#if STM32_WSPI_USE_OCTOSPI1
  wspiObjectInit(&WSPID1);
  WSPID1.extra_tcr = 0U
#if STM32_WSPI_OCTOSPI1_SSHIFT
                     | OCTOSPI_TCR_SSHIFT
#endif
#if STM32_WSPI_OCTOSPI1_DHQC
                     | OCTOSPI_TCR_DHQC
#endif
                     ;
  WSPID1.ospi      = OCTOSPI1;
  WSPID1.dmachp    = NULL;
  WSPID1.dreq      = STM32_DMA3_REQ_OSPI1;
  WSPID1.dprio     = STM32_WSPI_OCTOSPI1_DMA_PRIORITY;
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
  uint32_t dcr2 = 0U;

  if (wspip->config == NULL) {
    wspip->config = wspi_lld_setcfg(wspip, NULL);
    if (wspip->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  dcr2 = STM32_DCR2_PRESCALER(STM32_WSPI_OCTOSPI1_PRESCALER_VALUE - 1U);

  {
#if STM32_WSPI_USE_OCTOSPI1
    if (&WSPID1 == wspip) {
      wspip->dmachp = dma3ChannelAlloc(STM32_WSPI_OCTOSPI1_DMA3_CHANNEL,
                                       STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY,
                                       wspi_lld_serve_dma_interrupt,
                                       (void *)wspip);
      if (wspip->dmachp == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
      rccEnableOCTOSPI1(true);
    }
#endif
  }

  wspip->ospi->DCR1 = __wspi_getfield(wspip, dcr1);
  wspip->ospi->DCR2 = __wspi_getfield(wspip, dcr2) | dcr2;
  wspip->ospi->DCR3 = __wspi_getfield(wspip, dcr3);
  wspip->ospi->DCR4 = __wspi_getfield(wspip, dcr4);
  wspip->ospi->CR   = OCTOSPI_CR_TEIE | OCTOSPI_CR_TCIE |
                      OCTOSPI_CR_DMAEN | OCTOSPI_CR_EN;
  wspip->ospi->FCR  = OCTOSPI_FCR_CTEF | OCTOSPI_FCR_CTCF |
                      OCTOSPI_FCR_CSMF | OCTOSPI_FCR_CTOF;

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

  wspi_lld_sync(wspip);
  wspip->ospi->CR = 0U;

  if (wspip->dmachp != NULL) {
    (void)dma3ChannelDisable(wspip->dmachp);
    dma3ChannelFree(wspip->dmachp);
    wspip->dmachp = NULL;
  }

#if STM32_WSPI_USE_OCTOSPI1
  if (&WSPID1 == wspip) {
    rccDisableOCTOSPI1();
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
 * @brief   Serves the OCTOSPI interrupt.
 *
 * @param[in] wspip     pointer to the @p hal_wspi_driver_c object
 *
 * @notapi
 */
void wspi_lld_serve_interrupt(hal_wspi_driver_c *wspip) {
  bool data_transfer;
  uint32_t sr;

  data_transfer = (wspip->state == WSPI_STATE_SEND) ||
                  (wspip->state == WSPI_STATE_RECEIVE);
  sr = wspip->ospi->SR;
  wspip->ospi->FCR = OCTOSPI_FCR_CTEF | OCTOSPI_FCR_CTCF |
                     OCTOSPI_FCR_CSMF | OCTOSPI_FCR_CTOF;

  if ((sr & OCTOSPI_SR_TEF) != 0U) {
    if (data_transfer && (wspip->dmachp != NULL)) {
      (void)dma3ChannelDisable(wspip->dmachp);
    }
    wspip->ospi->CR &= ~OCTOSPI_CR_DMAEN;
    _wspi_isr_error_code(wspip);
    return;
  }

  _wspi_isr_complete_code(wspip);

  while (data_transfer && (wspip->dmachp != NULL) &&
         (dma3ChannelGetTransactionSize(wspip->dmachp) > 0U)) {
  }

  if (data_transfer && (wspip->dmachp != NULL)) {
    (void)dma3ChannelDisable(wspip->dmachp);
  }
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

  wspip->ospi->CR &= ~OCTOSPI_CR_FMODE;
  wspip->ospi->DLR = 0U;
  wspip->ospi->TCR = cmdp->dummy | wspip->extra_tcr;
  wspip->ospi->CCR = cmdp->cfg;
  wspip->ospi->ABR = cmdp->alt;
  wspip->ospi->IR  = cmdp->cmd;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->ospi->AR = cmdp->addr;
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
  uint32_t ccr = STM32_DMA3_CCR_PRIO(wspip->dprio) |
                 STM32_DMA3_CCR_USEIE |
                 STM32_DMA3_CCR_ULEIE |
                 STM32_DMA3_CCR_DTEIE;

  dma3ChannelSetSource(wspip->dmachp, txbuf);
  dma3ChannelSetDestination(wspip->dmachp, &wspip->ospi->DR);
  dma3ChannelSetTransactionSize(wspip->dmachp, n);
  dma3ChannelSetMode(wspip->dmachp,
                     ccr,
                     STM32_DMA3_CTR1_DAP_PER |
                     STM32_DMA3_CTR1_DDW_BYTE |
                     STM32_DMA3_CTR1_SAP_MEM |
                     STM32_DMA3_CTR1_SINC |
                     STM32_DMA3_CTR1_SDW_BYTE,
                     STM32_DMA3_CTR2_REQSEL(wspip->dreq),
                     0U);

  wspip->ospi->CR &= ~OCTOSPI_CR_FMODE;
  wspip->ospi->DLR = n - 1U;
  wspip->ospi->TCR = cmdp->dummy | wspip->extra_tcr;
  wspip->ospi->CCR = cmdp->cfg;
  wspip->ospi->ABR = cmdp->alt;
  wspip->ospi->IR  = cmdp->cmd;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->ospi->AR = cmdp->addr;
  }

  dma3ChannelEnable(wspip->dmachp);
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
  uint32_t ccr = STM32_DMA3_CCR_PRIO(wspip->dprio) |
                 STM32_DMA3_CCR_USEIE |
                 STM32_DMA3_CCR_ULEIE |
                 STM32_DMA3_CCR_DTEIE;

  dma3ChannelSetSource(wspip->dmachp, &wspip->ospi->DR);
  dma3ChannelSetDestination(wspip->dmachp, rxbuf);
  dma3ChannelSetTransactionSize(wspip->dmachp, n);
  dma3ChannelSetMode(wspip->dmachp,
                     ccr,
                     STM32_DMA3_CTR1_DAP_MEM |
                     STM32_DMA3_CTR1_DINC |
                     STM32_DMA3_CTR1_DDW_BYTE |
                     STM32_DMA3_CTR1_SAP_PER |
                     STM32_DMA3_CTR1_SDW_BYTE,
                     STM32_DMA3_CTR2_REQSEL(wspip->dreq),
                     0U);

  wspip->ospi->CR  = (wspip->ospi->CR & ~OCTOSPI_CR_FMODE) | OCTOSPI_CR_FMODE_0;
  wspip->ospi->DLR = n - 1U;
  wspip->ospi->TCR = cmdp->dummy | wspip->extra_tcr;
  wspip->ospi->CCR = cmdp->cfg;
  wspip->ospi->ABR = cmdp->alt;
  wspip->ospi->IR  = cmdp->cmd;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->ospi->AR = cmdp->addr;
  }

  dma3ChannelEnable(wspip->dmachp);
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

  wspip->ospi->CR  = OCTOSPI_CR_FMODE_1 | OCTOSPI_CR_FMODE_0 | OCTOSPI_CR_EN;
  wspip->ospi->TCR = cmdp->dummy | wspip->extra_tcr;
  wspip->ospi->CCR = cmdp->cfg;
  wspip->ospi->IR  = cmdp->cmd;
  wspip->ospi->ABR = 0U;
  wspip->ospi->AR  = 0U;
  wspip->ospi->WTCR = 0U;
  wspip->ospi->WCCR = 0U;
  wspip->ospi->WIR  = 0U;
  wspip->ospi->WABR = 0U;

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

  wspip->ospi->CR |= OCTOSPI_CR_ABORT;
  while ((wspip->ospi->CR & OCTOSPI_CR_ABORT) != 0U) {
  }

  wspip->ospi->CR = OCTOSPI_CR_TEIE | OCTOSPI_CR_TCIE |
                    OCTOSPI_CR_DMAEN | OCTOSPI_CR_EN;
}
#endif /* WSPI_SUPPORTS_MEMMAP == TRUE */

#endif /* HAL_USE_WSPI */

/** @} */
