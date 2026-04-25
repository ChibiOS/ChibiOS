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
 * @file    I2Cv4/hal_i2c_lld.c
 * @brief   STM32 I2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */

#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Channels wrapper */
#if defined(STM32_DMA3_PRESENT)
#define STM32_I2C_I2C1_DMA_CHANNEL          STM32_I2C_I2C1_DMA3_CHANNEL
#define STM32_I2C_I2C2_DMA_CHANNEL          STM32_I2C_I2C2_DMA3_CHANNEL
#define STM32_I2C_I2C3_DMA_CHANNEL          STM32_I2C_I2C3_DMA3_CHANNEL
#define STM32_I2C_I2C4_DMA_CHANNEL          STM32_I2C_I2C4_DMA3_CHANNEL
#endif

/* Common GPDMA CR settings.*/
#define I2C_DMA3_CR_COMMON(i2cp)                                           \
  (STM32_DMA3_CCR_PRIO((uint32_t)(i2cp)->dprio)    |                       \
   STM32_DMA3_CCR_LAP_MEM                          |                       \
   STM32_DMA3_CCR_TOIE                             |                       \
   STM32_DMA3_CCR_USEIE                            |                       \
   STM32_DMA3_CCR_ULEIE                            |                       \
   STM32_DMA3_CCR_DTEIE                            |                       \
   STM32_DMA3_CTR1_DDW_BYTE                        |                       \
   STM32_DMA3_CTR1_SDW_BYTE)

/* Common DMA CR settings.*/
#define I2C_DMA_CR_COMMON(i2cp)                                             \
  (STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE |                      \
   STM32_DMA_CR_MINC       | STM32_DMA_CR_DMEIE      |                      \
   STM32_DMA_CR_TEIE       | STM32_DMA_CR_TCIE)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define I2C_ERROR_MASK                                                      \
  ((uint32_t)(I2C_ISR_BERR | I2C_ISR_ARLO | I2C_ISR_OVR | I2C_ISR_PECERR |  \
              I2C_ISR_TIMEOUT | I2C_ISR_ALERT))

#define I2C_INT_MASK                                                        \
  ((uint32_t)(I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF |    \
              I2C_ISR_ADDR | I2C_ISR_RXNE | I2C_ISR_TXIS))

#define I2C_MAX_XFR_BYTES   (I2C_CR2_NBYTES >> I2C_CR2_NBYTES_Pos)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2C1 driver identifier.*/
#if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
hal_i2c_driver_c I2CD1;
#endif

/** @brief I2C2 driver identifier.*/
#if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
hal_i2c_driver_c I2CD2;
#endif

/** @brief I2C3 driver identifier.*/
#if STM32_I2C_USE_I2C3 || defined(__DOXYGEN__)
hal_i2c_driver_c I2CD3;
#endif

/** @brief I2C4 driver identifier.*/
#if STM32_I2C_USE_I2C4 || defined(__DOXYGEN__)
hal_i2c_driver_c I2CD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if STM32_I2C_USE_DMA == TRUE
__STATIC_FORCEINLINE void i2c_dma_alloc(hal_i2c_driver_c *i2cp,
                                        uint32_t channel,
                                        uint32_t irqprio) {

#if defined(STM32_DMA3_PRESENT)
  i2cp->dma = dma3ChannelAlloc(channel, irqprio, NULL, (void *)i2cp);
#else
  i2cp->dma = dmaStreamAlloc(channel, irqprio, NULL, (void *)i2cp);
#endif
}

__STATIC_FORCEINLINE void i2c_dma_release(hal_i2c_driver_c *i2cp) {

#if defined(STM32_DMA3_PRESENT)
  dma3ChannelFree(i2cp->dma);
#else
  dmaStreamFree(i2cp->dma);
#endif
}

__STATIC_FORCEINLINE void i2c_dma_disable(hal_i2c_driver_c *i2cp) {

#if defined(STM32_DMA3_PRESENT)
  dma3ChannelDisable(i2cp->dma);
#else
  dmaStreamDisable(i2cp->dma);
#endif
}

__STATIC_FORCEINLINE void i2c_dma_enable_tx(hal_i2c_driver_c *i2cp) {

#if defined(STM32_DMA3_PRESENT)
  dma3ChannelSetSource(i2cp->dma, i2cp->txptr);
  dma3ChannelSetDestination(i2cp->dma, &i2cp->i2c->TXDR);
  dma3ChannelSetTransactionSize(i2cp->dma, i2cp->txbytes);
  dma3ChannelSetMode(i2cp->dma,
                      I2C_DMA3_CR_COMMON(i2cp),
                      (i2c_lld_getcfg(i2cp)->dtr1tx         | STM32_DMA3_CTR1_DAP_PER  |
                       STM32_DMA3_CTR1_SAP_MEM      | STM32_DMA3_CTR1_SINC),
                      (i2c_lld_getcfg(i2cp)->dtr2tx         | STM32_DMA3_CTR2_DREQ     |
                       STM32_DMA3_CTR2_REQSEL(i2cp->dreqtx)),
                       0U);
  dma3ChannelEnable(i2cp->dma);
#else
  dmaSetRequestSource(i2cp->dma, i2cp->dreqtx);
  dmaStreamSetMode(i2cp->dma,
                   I2C_DMA_CR_COMMON(i2cp)                      |
                   STM32_DMA_CR_DIR_M2P                         |
                   STM32_DMA_CR_PL(i2cp->dprio) |
                   STM32_DMA_CR_CHSEL(i2cp->dreqtx));
  dmaStreamSetTransactionSize(i2cp->dma, i2cp->txbytes);
  dmaStreamSetPeripheral(i2cp->dma, &i2cp->i2c->TXDR);
  dmaStreamSetMemory0(i2cp->dma, i2cp->txptr);
  dmaStreamEnable(i2cp->dma);
#endif
}

__STATIC_FORCEINLINE void i2c_dma_enable_rx(hal_i2c_driver_c *i2cp) {

#if defined(STM32_DMA3_PRESENT)
  dma3ChannelSetSource(i2cp->dma, &i2cp->i2c->RXDR);
  dma3ChannelSetDestination(i2cp->dma, i2cp->rxptr);
  dma3ChannelSetTransactionSize(i2cp->dma, i2cp->rxbytes);
  dma3ChannelSetMode(i2cp->dma,
                      I2C_DMA3_CR_COMMON(i2cp),
                      (i2c_lld_getcfg(i2cp)->dtr1rx         | STM32_DMA3_CTR1_DAP_MEM  |
                       STM32_DMA3_CTR1_SAP_PER      | STM32_DMA3_CTR1_DINC),
                      (i2c_lld_getcfg(i2cp)->dtr2rx         |
                       STM32_DMA3_CTR2_REQSEL(i2cp->dreqrx)),
                       0U);
  dma3ChannelEnable(i2cp->dma);
#else
  dmaSetRequestSource(i2cp->dma, i2cp->dreqrx);
  dmaStreamSetMode(i2cp->dma,
                   I2C_DMA_CR_COMMON(i2cp)                      |
                   STM32_DMA_CR_DIR_P2M                         |
                   STM32_DMA_CR_PL(i2cp->dprio) |
                   STM32_DMA_CR_CHSEL(i2cp->dreqrx));
  dmaStreamSetTransactionSize(i2cp->dma, i2cp->rxbytes);
  dmaStreamSetPeripheral(i2cp->dma, &i2cp->i2c->RXDR);
  dmaStreamSetMemory0(i2cp->dma, i2cp->rxptr);
  dmaStreamEnable(i2cp->dma);
#endif
}
#endif /* STM32_I2C_USE_DMA == TRUE */

/**
 * @brief   Slave address setup.
 * @note    The RW bit is set to zero internally.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] addr      slave device address
 *
 * @notapi
 */
static void i2c_lld_set_address(hal_i2c_driver_c *i2cp, i2caddr_t addr) {
  I2C_TypeDef *dp = i2cp->i2c;

  /* Address alignment depends on the addressing mode selected.*/
  if ((i2c_lld_getcfg(i2cp)->cr2 & I2C_CR2_ADD10) == 0U)
    dp->CR2 = (uint32_t)addr << 1U;
  else
    dp->CR2 = (uint32_t)addr;
}

/**
 * @brief   I2C RX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
static void i2c_lld_setup_rx_transfer(hal_i2c_driver_c *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t reload;
  size_t n;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  n = i2cp->rxbytes;
  if (n > (size_t)I2C_MAX_XFR_BYTES) {
    n = (size_t)I2C_MAX_XFR_BYTES;
    reload = I2C_CR2_RELOAD;
  }
  else {
    reload = 0U;
  }

  /* Configures the CR2 registers with both the calculated and static
     settings.*/
  dp->CR2 = (dp->CR2 & ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD)) | i2c_lld_getcfg(i2cp)->cr2 |
            I2C_CR2_RD_WRN | (n << I2C_CR2_NBYTES_Pos) | reload;
}

/**
 * @brief   I2C TX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
static void i2c_lld_setup_tx_transfer(hal_i2c_driver_c *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t reload;
  size_t n;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  n = i2cp->txbytes;
  if (n > (size_t)I2C_MAX_XFR_BYTES) {
    n = (size_t)I2C_MAX_XFR_BYTES;
    reload = I2C_CR2_RELOAD;
  }
  else {
    reload = 0U;
  }

  /* Configures the CR2 registers with both the calculated and static
     settings.*/
  dp->CR2 = (dp->CR2 & ~(I2C_CR2_NBYTES | I2C_CR2_RELOAD)) | i2c_lld_getcfg(i2cp)->cr2 |
            (n << I2C_CR2_NBYTES_Pos) | reload;
}

/**
 * @brief   Aborts an I2C transaction.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
static void i2c_lld_abort_operation(hal_i2c_driver_c *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  if (dp->CR1 & I2C_CR1_PE) {
    /* Stops the I2C peripheral.*/
    dp->CR1 &= ~I2C_CR1_PE;
    while (dp->CR1 & I2C_CR1_PE) {
      /* Waiting for PE to become zero.*/
    }
    dp->CR1 |= I2C_CR1_PE;
  }

#if STM32_I2C_USE_DMA == TRUE
  /* Stops the associated DMA channel.*/
  i2c_dma_disable(i2cp);
#else
  dp->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE);
#endif
}

/**
 * @brief   I2C shared ISR code.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] isr       content of the ISR register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_events(hal_i2c_driver_c *i2cp, uint32_t isr) {
  I2C_TypeDef *dp = i2cp->i2c;

#if STM32_I2C_USE_DMA == FALSE
  uint32_t cr1 = dp->CR1;
#endif

  /* Special case of a received NACK, the transfer is aborted.*/
  if ((isr & I2C_ISR_NACKF) != 0U) {

#if STM32_I2C_USE_DMA == TRUE
    /* Stops the associated DMA channel.*/
    i2c_dma_disable(i2cp);
#endif

    /* Error flag.*/
    i2cp->errors |= I2C_ACK_FAILURE;

    /* Transaction finished sending the STOP.*/
    dp->CR2 |= I2C_CR2_STOP;

    /* Make sure no more interrupts.*/
    dp->CR1 &= ~(I2C_CR1_TCIE | I2C_CR1_TXIE | I2C_CR1_RXIE);

    /* Errors are signaled to the upper layer.*/
    __i2c_error_isr(i2cp);

    return;
  }

#if STM32_I2C_USE_DMA == FALSE
  /* Handling of data transfer if the DMA mode is disabled.*/
  {
    if (i2cp->state == HAL_DRV_STATE_ACTIVE) {
      /* Transmission phase.*/
      if (((cr1 & I2C_CR1_TXIE) != 0U) && ((isr & I2C_ISR_TXIS) != 0U)) {
        dp->TXDR = (uint32_t)*i2cp->txptr;
        i2cp->txptr++;
        i2cp->txbytes--;
        if (i2cp->txbytes == 0U) {
          dp->CR1 &= ~I2C_CR1_TXIE;
        }
      }
    }
    else {
      /* Receive phase.*/
      if (((cr1 & I2C_CR1_RXIE) != 0U) && ((isr & I2C_ISR_RXNE) != 0U)) {
        *i2cp->rxptr = (uint8_t)dp->RXDR;
        i2cp->rxptr++;
        i2cp->rxbytes--;
        if (i2cp->rxbytes == 0U) {
          dp->CR1 &= ~I2C_CR1_RXIE;
        }
      }
    }
  }
#endif

  /* Partial transfer handling, restarting the transfer and returning.*/
  if ((isr & I2C_ISR_TCR) != 0U) {
    if (i2cp->state == HAL_DRV_STATE_ACTIVE) {
#if STM32_I2C_USE_DMA == TRUE
      i2cp->txbytes -= (size_t)I2C_MAX_XFR_BYTES;
#endif
      i2c_lld_setup_tx_transfer(i2cp);
    }
    else {
#if STM32_I2C_USE_DMA == TRUE
      i2cp->rxbytes -= (size_t)I2C_MAX_XFR_BYTES;
#endif
      i2c_lld_setup_rx_transfer(i2cp);
    }
    return;
  }

  /* The following condition is true if a transfer phase has been completed.*/
  if ((isr & I2C_ISR_TC) != 0U) {
    if (i2cp->state == HAL_DRV_STATE_ACTIVE) {
      /* End of the transmit phase.*/

#if STM32_I2C_USE_DMA == TRUE
      /* Disabling DMA channel.*/
      i2c_dma_disable(i2cp);
#endif

      /* Starting receive phase if necessary.*/
      if (i2cp->rxbytes > 0U) {
        /* Setting up the peripheral.*/
        i2c_lld_setup_rx_transfer(i2cp);

#if STM32_I2C_USE_DMA == TRUE
        /* Setup DMA for RX.*/
        i2c_dma_enable_rx(i2cp);
#else
        /* RX interrupt enabled.*/
        dp->CR1 |= I2C_CR1_RXIE;
#endif

        /* Starts the read operation.*/
        dp->CR2 |= I2C_CR2_START;

        /* State change.*/
        i2cp->state = I2C_ACTIVE_RX;

        /* Note, returning because the transaction is not over yet.*/
        return;
      }
    }
    else {
      /* End of the receive phase.*/
#if STM32_I2C_USE_DMA == TRUE
      /* Disabling DMA channel.*/
      i2c_dma_disable(i2cp);
#endif
    }

    /* Transaction finished sending the STOP.*/
    dp->CR2 |= I2C_CR2_STOP;

    /* Make sure no more 'Transfer Complete' interrupts.*/
    dp->CR1 &= ~I2C_CR1_TCIE;

    /* Normal transaction end.*/
    __i2c_complete_isr(i2cp);
  }
}

/**
 * @brief   I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] isr       content of the ISR register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_errors(hal_i2c_driver_c *i2cp, uint32_t isr) {

#if STM32_I2C_USE_DMA == TRUE
  /* Clears DMA interrupt flags just to be safe.*/
  i2c_dma_disable(i2cp);
#else
  /* Disabling RX and TX interrupts.*/
  i2cp->i2c->CR1 &= ~(I2C_CR1_TXIE | I2C_CR1_RXIE);
#endif

  if (isr & I2C_ISR_BERR) {
    i2cp->errors |= I2C_BUS_ERROR;
  }

  if (isr & I2C_ISR_ARLO) {
    i2cp->errors |= I2C_ARBITRATION_LOST;
  }

  if (isr & I2C_ISR_OVR) {
    i2cp->errors |= I2C_OVERRUN;
  }

  if (isr & I2C_ISR_TIMEOUT) {
    i2cp->errors |= I2C_TIMEOUT;
  }

  /* If some error has been identified then reports it to the upper layer.*/
  if (i2cp->errors != I2C_NO_ERROR) {
    __i2c_error_isr(i2cp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if STM32_I2C_USE_I2C1
  i2cObjectInit(&I2CD1);
  I2CD1.i2c    = I2C1;
#if STM32_I2C_USE_DMA == TRUE
  I2CD1.dma    = NULL;
  I2CD1.dprio  = STM32_I2C_I2C1_DMA_PRIORITY;
#if defined(STM32_DMA3_PRESENT)
  I2CD1.dreqtx = STM32_DMA3_REQ_I2C1_TX;
  I2CD1.dreqrx = STM32_DMA3_REQ_I2C1_RX;
#else /* Assuming old DMAs.*/
  I2CD1.dreqtx = STM32_DMAMUX1_I2C1_TX;
  I2CD1.dreqrx = STM32_DMAMUX1_I2C1_RX;
#endif
#endif /* STM32_I2C_USE_DMA == TRUE */
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
  i2cObjectInit(&I2CD2);
  I2CD2.i2c    = I2C2;
#if STM32_I2C_USE_DMA == TRUE
  I2CD2.dma    = NULL;
  I2CD2.dprio  = STM32_I2C_I2C2_DMA_PRIORITY;
#if defined(STM32_DMA3_PRESENT)
  I2CD2.dreqtx = STM32_DMA3_REQ_I2C2_TX;
  I2CD2.dreqrx = STM32_DMA3_REQ_I2C2_RX;
#else /* Assuming old DMAs.*/
  I2CD2.dreqtx = STM32_DMAMUX1_I2C2_TX;
  I2CD2.dreqrx = STM32_DMAMUX1_I2C2_RX;
#endif
#endif /* STM32_I2C_USE_DMA == TRUE */
#endif /* STM32_I2C_USE_I2C2 */

#if STM32_I2C_USE_I2C3
  i2cObjectInit(&I2CD3);
  I2CD3.i2c    = I2C3;
#if STM32_I2C_USE_DMA == TRUE
  I2CD3.dma    = NULL;
  I2CD3.dprio  = STM32_I2C_I2C3_DMA_PRIORITY;
#if defined(STM32_DMA3_PRESENT)
  I2CD3.dreqtx = STM32_DMA3_REQ_I2C3_TX;
  I2CD3.dreqrx = STM32_DMA3_REQ_I2C3_RX;
#else /* Assuming old DMAs.*/
  I2CD3.dreqtx = STM32_DMAMUX1_I2C3_TX;
  I2CD3.dreqrx = STM32_DMAMUX1_I2C3_RX;
#endif
#endif /* STM32_I2C_USE_DMA == TRUE */
#endif /* STM32_I2C_USE_I2C3 */

#if STM32_I2C_USE_I2C4
  i2cObjectInit(&I2CD4);
  I2CD4.i2c    = I2C4;
#if STM32_I2C_USE_DMA == TRUE
  I2CD4.dma    = NULL;
  I2CD4.dprio  = STM32_I2C_I2C4_DMA_PRIORITY;
#if defined(STM32_DMA3_PRESENT)
  I2CD4.dreqtx = STM32_DMA3_REQ_I2C4_TX;
  I2CD4.dreqrx = STM32_DMA3_REQ_I2C4_RX;
#else /* Assuming old DMAs.*/
  I2CD4.dreqtx = STM32_DMAMUX1_I2C4_TX;
  I2CD4.dreqrx = STM32_DMAMUX1_I2C4_RX;
#endif
#endif /* STM32_I2C_USE_DMA == TRUE */
#endif /* STM32_I2C_USE_I2C4 */
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_start(hal_i2c_driver_c *i2cp) {
  const hal_i2c_config_t *config = (const hal_i2c_config_t *)i2cp->config;
  I2C_TypeDef *dp = i2cp->i2c;

  if (config == NULL) {
    config = i2c_lld_selcfg(i2cp, 0U);
  }
  if (config == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  i2cp->config = config;

  /* Make sure I2C peripheral is disabled */
  dp->CR1 &= ~I2C_CR1_PE;

  /* Enabling the I2C and DMA clocks.*/
  {

#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {

      rccResetI2C1();
      rccEnableI2C1(true);

#if STM32_I2C_USE_DMA == TRUE
      i2c_dma_alloc(i2cp, STM32_I2C_I2C1_DMA_CHANNEL, STM32_IRQ_I2C1_PRIORITY);
      if (i2cp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
#endif
    }
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {

      rccResetI2C2();
      rccEnableI2C2(true);

#if STM32_I2C_USE_DMA == TRUE
      i2c_dma_alloc(i2cp, STM32_I2C_I2C2_DMA_CHANNEL, STM32_IRQ_I2C2_PRIORITY);
      if (i2cp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
#endif
    }
#endif /* STM32_I2C_USE_I2C2 */

#if STM32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {

      rccResetI2C3();
      rccEnableI2C3(true);

#if STM32_I2C_USE_DMA == TRUE
      i2c_dma_alloc(i2cp, STM32_I2C_I2C3_DMA_CHANNEL, STM32_IRQ_I2C3_PRIORITY);
      if (i2cp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
#endif
    }
#endif /* STM32_I2C_USE_I2C3 */

#if STM32_I2C_USE_I2C4
    if (&I2CD4 == i2cp) {

      rccResetI2C4();
      rccEnableI2C4(true);

#if STM32_I2C_USE_DMA == TRUE
      i2c_dma_alloc(i2cp, STM32_I2C_I2C4_DMA_CHANNEL, STM32_IRQ_I2C4_PRIORITY);
      if (i2cp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
#endif
    }
#endif /* STM32_I2C_USE_I2C4 */
  }


  /* Reset i2c peripheral, the TCIE bit will be handled separately.*/
  dp->CR1 = i2c_lld_getcfg(i2cp)->cr1 |
#if STM32_I2C_USE_DMA == TRUE
            I2C_CR1_TXDMAEN | I2C_CR1_RXDMAEN | /* Enable only if using DMA */
#endif
            I2C_CR1_ERRIE | I2C_CR1_NACKIE;

  /* Setup I2C parameters.*/
  dp->TIMINGR = i2c_lld_getcfg(i2cp)->timingr;

  /* Ready to go.*/
  dp->CR1 |= I2C_CR1_PE;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
void i2c_lld_stop(hal_i2c_driver_c *i2cp) {

  /* If not in stopped state then disables the I2C clock.*/
  if (i2cp->state != HAL_DRV_STATE_STOP) {

    /* I2C disable.*/
    i2c_lld_abort_operation(i2cp);
#if STM32_I2C_USE_DMA == TRUE
    i2c_dma_release(i2cp);
#endif

#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {

      rccDisableI2C1();
    }
#endif

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {

      rccDisableI2C2();
    }
#endif

#if STM32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {

      rccDisableI2C3();
    }
#endif

#if STM32_I2C_USE_I2C4
    if (&I2CD4 == i2cp) {

      rccDisableI2C4();
    }
#endif
  }
}

/**
 * @brief   Verifies an I2C configuration.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] config    pointer to the I2C configuration
 * @return              The verified configuration.
 * @retval NULL         if the configuration is invalid.
 *
 * @notapi
 */
const hal_i2c_config_t *i2c_lld_setcfg(hal_i2c_driver_c *i2cp,
                                       const hal_i2c_config_t *config) {
  (void)i2cp;

  if (config == NULL) {
    return i2c_lld_selcfg(i2cp, 0U);
  }

  return config;
}

/**
 * @brief   Selects an I2C configuration.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] cfgnum    configuration index
 * @return              The selected configuration.
 * @retval NULL         if the configuration is not available.
 *
 * @notapi
 */
const hal_i2c_config_t *i2c_lld_selcfg(hal_i2c_driver_c *i2cp,
                                       unsigned cfgnum) {
  (void)i2cp;

#if I2C_USE_CONFIGURATIONS == TRUE
  extern const i2c_configurations_t i2c_configurations;

  if (cfgnum < i2c_configurations.cfgsnum) {
    return &i2c_configurations.cfgs[cfgnum];
  }
#else
  (void)cfgnum;
#endif

  return NULL;
}

/**
 * @brief   Low level callback configuration hook.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] cb        callback pointer
 *
 * @notapi
 */
void i2c_lld_set_callback(hal_i2c_driver_c *i2cp, drv_cb_t cb) {

  (void)i2cp;
  (void)cb;
}

/**
 * @brief   Stops the ongoing I2C transfer.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_stop_transfer(hal_i2c_driver_c *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  /* Stops the transaction and disables data/transfer interrupts. Error
     interrupts are left enabled for the started peripheral. */
  dp->CR2 |= I2C_CR2_STOP;
  dp->CR1 &= ~(I2C_CR1_TCIE | I2C_CR1_TXIE | I2C_CR1_RXIE);

#if STM32_I2C_USE_DMA == TRUE
  i2c_dma_disable(i2cp);
#endif

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @return              The operation status.
 * @retval HAL_RET_SUCCESS     if the function succeeded.
 * @retval HAL_RET_HW_BUSY     if the I2C bus is busy.
 *
 * @notapi
 */
msg_t i2c_lld_start_master_receive(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                   uint8_t *rxbuf, size_t rxbytes) {
  I2C_TypeDef *dp = i2cp->i2c;

  i2cp->txptr   = NULL;
  i2cp->txbytes = (size_t)0;
  i2cp->rxptr   = rxbuf;
  i2cp->rxbytes = rxbytes;

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  if ((dp->ISR & I2C_ISR_BUSY) != 0U) {
    return HAL_RET_HW_BUSY;
  }

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Setting up the peripheral.*/
  i2c_lld_setup_rx_transfer(i2cp);

#if STM32_I2C_USE_DMA == TRUE
  /* Setup DMA for RX.*/
  i2c_dma_enable_rx(i2cp);

  /* Transfer complete interrupt enabled.*/
  dp->CR1 |= I2C_CR1_TCIE;
#else

  /* Transfer complete and RX interrupts enabled.*/
  dp->CR1 |= I2C_CR1_TCIE | I2C_CR1_RXIE;
#endif

  /* Starts the operation.*/
  dp->CR2 |= I2C_CR2_START;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @return              The operation status.
 * @retval HAL_RET_SUCCESS     if the function succeeded.
 * @retval HAL_RET_HW_BUSY     if the I2C bus is busy.
 *
 * @notapi
 */
msg_t i2c_lld_start_master_transmit(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                    const uint8_t *txbuf, size_t txbytes,
                                    uint8_t *rxbuf, size_t rxbytes) {
  I2C_TypeDef *dp = i2cp->i2c;

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  /* Transaction setup.*/
  i2cp->txptr   = txbuf;
  i2cp->txbytes = txbytes;
  i2cp->rxptr   = rxbuf;
  i2cp->rxbytes = rxbytes;

  if ((dp->ISR & I2C_ISR_BUSY) != 0U) {
    return HAL_RET_HW_BUSY;
  }

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Preparing the transfer.*/
  i2c_lld_setup_tx_transfer(i2cp);

#if STM32_I2C_USE_DMA == TRUE
  /* Setup DMA for TX.*/
  i2c_dma_enable_tx(i2cp);

  /* Transfer complete interrupt enabled.*/
  dp->CR1 |= I2C_CR1_TCIE;
#else
  /* Transfer complete and TX interrupts enabled.*/
  dp->CR1 |= I2C_CR1_TCIE | I2C_CR1_TXIE;
#endif

  /* Starts the operation.*/
  dp->CR2 |= I2C_CR2_START;

  return HAL_RET_SUCCESS;
}

#if (STM32_I2C_SINGLE_IRQ == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   I2C events interrupt handler.
 *
 * @notapi
 */
void i2c_lld_serve_global_interrupt(hal_i2c_driver_c *i2cp) {
  uint32_t isr = i2cp->i2c->ISR;

  /* Clearing all IRQ bits.*/
  i2cp->i2c->ICR = isr;

  if ((isr & I2C_ERROR_MASK) != 0U) {
    i2c_lld_serve_errors(i2cp, isr);
  }
  else if ((isr & I2C_INT_MASK) != 0U) {
    i2c_lld_serve_events(i2cp, isr);
  }
}

#else /* STM32_I2C_SINGLE_IRQ == FALSE */
/**
 * @brief   I2C events interrupt handler.
 *
 * @notapi
 */
void i2c_lld_serve_ev_interrupt(hal_i2c_driver_c *i2cp) {
  uint32_t isr = i2cp->i2c->ISR;

  /* Clearing events IRQ bits.*/
  i2cp->i2c->ICR = isr & I2C_INT_MASK;

  i2c_lld_serve_events(i2cp, isr);
}

/**
 * @brief   I2C errors interrupt handler.
 *
 * @notapi
 */
void i2c_lld_serve_er_interrupt(hal_i2c_driver_c *i2cp) {
  uint32_t isr = i2cp->i2c->ISR;

  /* Clearing error IRQ bits.*/
  i2cp->i2c->ICR = isr & I2C_ERROR_MASK;

  i2c_lld_serve_errors(i2cp, isr);
}
#endif /* STM32_I2C_SINGLE_IRQ == FALSE */

#endif /* HAL_USE_I2C */

/** @} */
