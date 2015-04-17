/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    STM32/I2Cv2/i2c_lld.c
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

#define DMAMODE_COMMON                                                      \
  (STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE |                      \
   STM32_DMA_CR_MINC       | STM32_DMA_CR_DMEIE      |                      \
   STM32_DMA_CR_TEIE       | STM32_DMA_CR_TCIE)

#define I2C1_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C1_RX_DMA_STREAM,                        \
                       STM32_I2C1_RX_DMA_CHN)

#define I2C1_TX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C1_TX_DMA_STREAM,                        \
                       STM32_I2C1_TX_DMA_CHN)

#define I2C2_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C2_RX_DMA_STREAM,                        \
                       STM32_I2C2_RX_DMA_CHN)

#define I2C2_TX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C2_TX_DMA_STREAM,                        \
                       STM32_I2C2_TX_DMA_CHN)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define I2C_ERROR_MASK                                                      \
  ((uint32_t)(I2C_ISR_BERR | I2C_ISR_ARLO | I2C_ISR_OVR | I2C_ISR_PECERR |  \
              I2C_ISR_TIMEOUT | I2C_ISR_ALERT))

#define I2C_INT_MASK                                                        \
  ((uint32_t)(I2C_ISR_TCR | I2C_ISR_TC | I2C_ISR_STOPF | I2C_ISR_NACKF |    \
              I2C_ISR_ADDR | I2C_ISR_RXNE | I2C_ISR_TXIS))

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2C1 driver identifier.*/
#if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/** @brief I2C2 driver identifier.*/
#if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
I2CDriver I2CD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Slave address setup.
 * @note    The RW bit is set to zero internally.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 *
 * @notapi
 */
static void i2c_lld_set_address(I2CDriver *i2cp, i2caddr_t addr) {
  I2C_TypeDef *dp = i2cp->i2c;

  /* Address alignment depends on the addressing mode selected.*/
  if ((i2cp->config->cr2 & I2C_CR2_ADD10) == 0U)
    dp->CR2 = ((uint32_t)addr & 0x7FU) << 1U;
  else
    dp->CR2 = (uint32_t)addr;
}

/**
 * @brief   I2C RX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         size of the transfer
 *
 * @notapi
 */
static void i2c_lld_setup_rx_transfer(I2CDriver *i2cp, size_t n) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t reload;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  if (n > 255U) {
    i2cp->tsize = n - 255U;
    n = 255U;
    reload = I2C_CR2_RELOAD;
  }
  else {
    i2cp->tsize = 0;
    reload = 0;
  }

  /* Configures the CR2 registers with both the calculated and static
     settings.*/
  dp->CR2 = (dp->CR2 & ~I2C_CR2_NBYTES) | i2cp->config->cr2 | I2C_CR2_RD_WRN |
            (n << 16U) | reload;
}

/**
 * @brief   I2C TX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         size of the transfer
 *
 * @notapi
 */
static void i2c_lld_setup_tx_transfer(I2CDriver *i2cp, size_t n) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t reload;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  if (n > 255U) {
    i2cp->tsize = n - 255U;
    n = 255U;
    reload = I2C_CR2_RELOAD;
  }
  else {
    i2cp->tsize = 0;
    reload = 0;
  }

  /* Configures the CR2 registers with both the calculated and static
     settings.*/
  dp->CR2 = (dp->CR2 & ~I2C_CR2_NBYTES) | i2cp->config->cr2 |
            (n << 16U) | reload;
}

/**
 * @brief   Aborts an I2C transaction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_abort_operation(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  if (dp->CR1 & I2C_CR1_PE) {
    /* Stops the I2C peripheral.*/
    dp->CR1 &= ~I2C_CR1_PE;
    while (dp->CR1 & I2C_CR1_PE)
      dp->CR1 &= ~I2C_CR1_PE;
    dp->CR1 |= I2C_CR1_PE;
  }

  /* Stops the associated DMA streams.*/
  dmaStreamDisable(i2cp->dmatx);
  dmaStreamDisable(i2cp->dmarx);
}

/**
 * @brief   I2C shared ISR code.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] isr       content of the ISR register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_interrupt(I2CDriver *i2cp, uint32_t isr) {
  I2C_TypeDef *dp = i2cp->i2c;

  /* Special case of a received NACK, the transfer is aborted.*/
  if ((isr & I2C_ISR_NACKF) != 0U) {
    /* Stops the associated DMA streams.*/
    dmaStreamDisable(i2cp->dmatx);
    dmaStreamDisable(i2cp->dmarx);

    /* Error flag.*/
    i2cp->errors |= I2C_ACK_FAILURE;
  }
  else {
    /* Partial transfer handling, restarting the transfer and returning.*/
    if ((isr & I2C_ISR_TCR) != 0U) {
      if (i2cp->state == I2C_ACTIVE_TX) {
        i2c_lld_setup_tx_transfer(i2cp, i2cp->tsize);
      }
      else {
        i2c_lld_setup_rx_transfer(i2cp, i2cp->tsize);
      }
      return;
    }

    /* The following condition is true if a transfer phase has been completed.*/
    if ((isr & I2C_ISR_TC) != 0U) {
      if (i2cp->state == I2C_ACTIVE_TX) {
        /* End of the transmit phase.*/
        size_t rxbytes = dmaStreamGetTransactionSize(i2cp->dmarx);

        /* Disabling TX DMA channel.*/
        dmaStreamDisable(i2cp->dmatx);

        /* Starting receive phase if necessary.*/
        if (rxbytes > 0U) {
          /* Setting up the peripheral.*/
          i2c_lld_setup_rx_transfer(i2cp, rxbytes);

          /* Enabling RX DMA.*/
          dmaStreamEnable(i2cp->dmarx);

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

        /* Disabling RX DMA channel.*/
        dmaStreamDisable(i2cp->dmarx);
      }
    }
  }

  /* Transaction finished sending the STOP.*/
  dp->CR2 |= I2C_CR2_STOP;

  /* Make sure no more 'Transfer Complete' interrupts.*/
  dp->CR1 &= ~I2C_CR1_TCIE;

  /* Thread wakeup at transaction end.*/
  if (i2cp->errors) {
    /* Errors are signaled to the upper layer.*/
    _i2c_wakeup_error_isr(i2cp);
  }
  else {
    /* Normal transaction end.*/
    _i2c_wakeup_isr(i2cp);
  }
}

/**
 * @brief   I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] isr       content of the ISR register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_error_interrupt(I2CDriver *i2cp, uint32_t isr) {

  /* Clears interrupt flags just to be safe.*/
  dmaStreamDisable(i2cp->dmatx);
  dmaStreamDisable(i2cp->dmarx);

  if (isr & I2C_ISR_BERR)
    i2cp->errors |= I2C_BUS_ERROR;

  if (isr & I2C_ISR_ARLO)
    i2cp->errors |= I2C_ARBITRATION_LOST;

  if (isr & I2C_ISR_OVR)
    i2cp->errors |= I2C_OVERRUN;

  if (isr & I2C_ISR_TIMEOUT)
    i2cp->errors |= I2C_TIMEOUT;

  /* If some error has been identified then sends wakes the waiting thread.*/
  if (i2cp->errors != I2C_NO_ERROR)
    _i2c_wakeup_error_isr(i2cp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
#if defined(STM32_I2C1_GLOBAL_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   I2C1 event interrupt handler.
 *
 * @notapi
 */
OSAL_IRQ_HANDLER(STM32_I2C1_GLOBAL_HANDLER) {
  uint32_t isr = I2CD1.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD1.i2c->ICR = isr;

  if (isr & I2C_ERROR_MASK)
    i2c_lld_serve_error_interrupt(&I2CD1, isr);
  else if (isr & I2C_INT_MASK)
    i2c_lld_serve_interrupt(&I2CD1, isr);

  OSAL_IRQ_EPILOGUE();
}

#elif defined(STM32_I2C1_EVENT_HANDLER) && defined(STM32_I2C1_ERROR_HANDLER)
OSAL_IRQ_HANDLER(STM32_I2C1_EVENT_HANDLER) {
  uint32_t isr = I2CD1.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD1.i2c->ICR = isr & I2C_INT_MASK;

  i2c_lld_serve_interrupt(&I2CD1, isr);

  OSAL_IRQ_EPILOGUE();
}

OSAL_IRQ_HANDLER(STM32_I2C1_ERROR_HANDLER) {
  uint32_t isr = I2CD1.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD1.i2c->ICR = isr & I2C_ERROR_MASK;

  i2c_lld_serve_error_interrupt(&I2CD1, isr);

  OSAL_IRQ_EPILOGUE();
}

#else
#error "I2C1 interrupt handlers not defined"
#endif
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
#if defined(STM32_I2C2_GLOBAL_HANDLER) || defined(__DOXYGEN__)
/**
 * @brief   I2C2 event interrupt handler.
 *
 * @notapi
 */
OSAL_IRQ_HANDLER(STM32_I2C2_GLOBAL_HANDLER) {
  uint32_t isr = I2CD2.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD2.i2c->ICR = isr;

  if (isr & I2C_ERROR_MASK)
    i2c_lld_serve_error_interrupt(&I2CD2, isr);
  else if (isr & I2C_INT_MASK)
    i2c_lld_serve_interrupt(&I2CD2, isr);

  OSAL_IRQ_EPILOGUE();
}

#elif defined(STM32_I2C2_EVENT_HANDLER) && defined(STM32_I2C2_ERROR_HANDLER)
OSAL_IRQ_HANDLER(STM32_I2C2_EVENT_HANDLER) {
  uint32_t isr = I2CD2.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD2.i2c->ICR = isr & I2C_INT_MASK;

  i2c_lld_serve_interrupt(&I2CD2, isr);

  OSAL_IRQ_EPILOGUE();
}

OSAL_IRQ_HANDLER(STM32_I2C2_ERROR_HANDLER) {
  uint32_t isr = I2CD2.i2c->ISR;

  OSAL_IRQ_PROLOGUE();

  /* Clearing IRQ bits.*/
  I2CD2.i2c->ICR = isr & I2C_ERROR_MASK;

  i2c_lld_serve_error_interrupt(&I2CD2, isr);

  OSAL_IRQ_EPILOGUE();
}

#else
#error "I2C2 interrupt handlers not defined"
#endif
#endif /* STM32_I2C_USE_I2C2 */

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
  I2CD1.thread = NULL;
  I2CD1.i2c    = I2C1;
  I2CD1.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C1_RX_DMA_STREAM);
  I2CD1.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C1_TX_DMA_STREAM);
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
  i2cObjectInit(&I2CD2);
  I2CD2.thread = NULL;
  I2CD2.i2c    = I2C2;
  I2CD2.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C2_RX_DMA_STREAM);
  I2CD2.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C2_TX_DMA_STREAM);
#endif /* STM32_I2C_USE_I2C2 */
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  i2cp->txdmamode = DMAMODE_COMMON | STM32_DMA_CR_DIR_M2P;
  i2cp->rxdmamode = DMAMODE_COMMON | STM32_DMA_CR_DIR_P2M;

  /* Make sure I2C peripheral is disabled */
  dp->CR1 &= ~I2C_CR1_PE;

  /* If in stopped state then enables the I2C and DMA clocks.*/
  if (i2cp->state == I2C_STOP) {

#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      bool b;

      rccResetI2C1();
      b = dmaStreamAllocate(i2cp->dmarx,
                            STM32_I2C_I2C1_IRQ_PRIORITY,
                            NULL,
                            (void *)i2cp);
      osalDbgAssert(!b, "stream already allocated");
      b = dmaStreamAllocate(i2cp->dmatx,
                            STM32_I2C_I2C1_IRQ_PRIORITY,
                            NULL,
                            (void *)i2cp);
      osalDbgAssert(!b, "stream already allocated");
      rccEnableI2C1(FALSE);

#if defined(STM32_I2C1_GLOBAL_NUMBER) || defined(__DOXYGEN__)
      nvicEnableVector(STM32_I2C1_GLOBAL_NUMBER, STM32_I2C_I2C1_IRQ_PRIORITY);
#elif defined(STM32_I2C1_EVENT_NUMBER) && defined(STM32_I2C1_ERROR_NUMBER)
      nvicEnableVector(STM32_I2C1_EVENT_NUMBER, STM32_I2C_I2C1_IRQ_PRIORITY);
      nvicEnableVector(STM32_I2C1_ERROR_NUMBER, STM32_I2C_I2C1_IRQ_PRIORITY);
#else
#error "I2C1 interrupt numbers not defined"
#endif

      i2cp->rxdmamode |= STM32_DMA_CR_CHSEL(I2C1_RX_DMA_CHANNEL) |
                         STM32_DMA_CR_PL(STM32_I2C_I2C1_DMA_PRIORITY);
      i2cp->txdmamode |= STM32_DMA_CR_CHSEL(I2C1_TX_DMA_CHANNEL) |
                         STM32_DMA_CR_PL(STM32_I2C_I2C1_DMA_PRIORITY);
    }
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      bool b;

      rccResetI2C2();
      b = dmaStreamAllocate(i2cp->dmarx,
                            STM32_I2C_I2C2_IRQ_PRIORITY,
                            NULL,
                            (void *)i2cp);
      osalDbgAssert(!b, "stream already allocated");
      b = dmaStreamAllocate(i2cp->dmatx,
                            STM32_I2C_I2C2_IRQ_PRIORITY,
                            NULL,
                            (void *)i2cp);
      osalDbgAssert(!b, "stream already allocated");
      rccEnableI2C2(FALSE);

#if defined(STM32_I2C2_GLOBAL_NUMBER) || defined(__DOXYGEN__)
      nvicEnableVector(STM32_I2C2_GLOBAL_NUMBER, STM32_I2C_I2C2_IRQ_PRIORITY);
#elif defined(STM32_I2C2_EVENT_NUMBER) && defined(STM32_I2C2_ERROR_NUMBER)
      nvicEnableVector(STM32_I2C2_EVENT_NUMBER, STM32_I2C_I2C2_IRQ_PRIORITY);
      nvicEnableVector(STM32_I2C2_ERROR_NUMBER, STM32_I2C_I2C2_IRQ_PRIORITY);
#else
#error "I2C2 interrupt numbers not defined"
#endif

      i2cp->rxdmamode |= STM32_DMA_CR_CHSEL(I2C2_RX_DMA_CHANNEL) |
                         STM32_DMA_CR_PL(STM32_I2C_I2C2_DMA_PRIORITY);
      i2cp->txdmamode |= STM32_DMA_CR_CHSEL(I2C2_TX_DMA_CHANNEL) |
                         STM32_DMA_CR_PL(STM32_I2C_I2C2_DMA_PRIORITY);
    }
#endif /* STM32_I2C_USE_I2C2 */
  }

  /* I2C registers pointed by the DMA.*/
  dmaStreamSetPeripheral(i2cp->dmarx, &dp->RXDR);
  dmaStreamSetPeripheral(i2cp->dmatx, &dp->TXDR);

  /* Reset i2c peripheral, the TCIE bit will be handled separately.*/
  dp->CR1 = i2cp->config->cr1 | I2C_CR1_ERRIE | I2C_CR1_NACKIE |
            I2C_CR1_TXDMAEN | I2C_CR1_RXDMAEN;

  /* Set slave address field (master mode) */
  dp->CR2 = (i2cp->config->cr2 & ~I2C_CR2_SADD);

  /* Setup I2C parameters.*/
  dp->TIMINGR = i2cp->config->timingr;

  /* Ready to go.*/
  dp->CR1 |= I2C_CR1_PE;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  /* If not in stopped state then disables the I2C clock.*/
  if (i2cp->state != I2C_STOP) {

    /* I2C disable.*/
    i2c_lld_abort_operation(i2cp);
    dmaStreamRelease(i2cp->dmatx);
    dmaStreamRelease(i2cp->dmarx);

#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
#if defined(STM32_I2C1_GLOBAL_NUMBER) || defined(__DOXYGEN__)
      nvicDisableVector(STM32_I2C1_GLOBAL_NUMBER);
#elif defined(STM32_I2C1_EVENT_NUMBER) && defined(STM32_I2C1_ERROR_NUMBER)
      nvicDisableVector(STM32_I2C1_EVENT_NUMBER);
      nvicDisableVector(STM32_I2C1_ERROR_NUMBER);
#else
#error "I2C1 interrupt numbers not defined"
#endif

      rccDisableI2C1(FALSE);
    }
#endif

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
#if defined(STM32_I2C2_GLOBAL_NUMBER) || defined(__DOXYGEN__)
      nvicDisableVector(STM32_I2C2_GLOBAL_NUMBER);
#elif defined(STM32_I2C2_EVENT_NUMBER) && defined(STM32_I2C2_ERROR_NUMBER)
      nvicDisableVector(STM32_I2C2_EVENT_NUMBER);
      nvicDisableVector(STM32_I2C2_ERROR_NUMBER);
#else
#error "I2C2 interrupt numbers not defined"
#endif

      rccDisableI2C2(FALSE);
    }
#endif
  }
}

/**
 * @brief   Receives data via the I2C bus as master.
 * @details Number of receiving bytes must be more than 1 on STM32F1x. This is
 *          hardware restriction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     systime_t timeout) {
  msg_t msg;
  I2C_TypeDef *dp = i2cp->i2c;
  systime_t start, end;

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  /* Releases the lock from high level driver.*/
  osalSysUnlock();

  /* RX DMA setup.*/
  dmaStreamSetMode(i2cp->dmarx, i2cp->rxdmamode);
  dmaStreamSetMemory0(i2cp->dmarx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->dmarx, rxbytes);

  /* Calculating the time window for the timeout on the busy bus condition.*/
  start = osalOsGetSystemTimeX();
  end = start + OSAL_MS2ST(STM32_I2C_BUSY_TIMEOUT);

  /* Waits until BUSY flag is reset or, alternatively, for a timeout
     condition.*/
  while (true) {
    osalSysLock();

    /* If the bus is not busy then the operation can continue, note, the
       loop is exited in the locked state.*/
    if ((dp->ISR & I2C_ISR_BUSY) == 0)
      break;

    /* If the system time went outside the allowed window then a timeout
       condition is returned.*/
    if (!osalOsIsTimeWithinX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Setting up the peripheral.*/
  i2c_lld_setup_rx_transfer(i2cp, rxbytes);

  /* Enabling RX DMA.*/
  dmaStreamEnable(i2cp->dmarx);

  /* Transfer complete interrupt enabled.*/
  dp->CR1 |= I2C_CR1_TCIE;

  /* Starts the operation.*/
  dp->CR2 |= I2C_CR2_START;

  /* Waits for the operation completion or a timeout.*/
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  /* In case of a software timeout a STOP is sent as an extreme attempt
     to release the bus.*/
  if (msg == MSG_TIMEOUT) {
    dp->CR2 |= I2C_CR2_STOP;
  }

  return msg;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 * @details Number of receiving bytes must be 0 or more than 1 on STM32F1x.
 *          This is hardware restriction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      systime_t timeout) {
  msg_t msg;
  I2C_TypeDef *dp = i2cp->i2c;
  systime_t start, end;

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  /* Releases the lock from high level driver.*/
  osalSysUnlock();

  /* TX DMA setup.*/
  dmaStreamSetMode(i2cp->dmatx, i2cp->txdmamode);
  dmaStreamSetMemory0(i2cp->dmatx, txbuf);
  dmaStreamSetTransactionSize(i2cp->dmatx, txbytes);

  /* RX DMA setup, note, rxbytes can be zero but we write the value anyway.*/
  dmaStreamSetMode(i2cp->dmarx, i2cp->rxdmamode);
  dmaStreamSetMemory0(i2cp->dmarx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->dmarx, rxbytes);

  /* Calculating the time window for the timeout on the busy bus condition.*/
  start = osalOsGetSystemTimeX();
  end = start + OSAL_MS2ST(STM32_I2C_BUSY_TIMEOUT);

  /* Waits until BUSY flag is reset or, alternatively, for a timeout
     condition.*/
  while (true) {
    osalSysLock();

    /* If the bus is not busy then the operation can continue, note, the
       loop is exited in the locked state.*/
    if ((dp->ISR & I2C_ISR_BUSY) == 0)
      break;

    /* If the system time went outside the allowed window then a timeout
       condition is returned.*/
    if (!osalOsIsTimeWithinX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Preparing the transfer.*/
  i2c_lld_setup_tx_transfer(i2cp, txbytes);

  /* Enabling TX DMA.*/
  dmaStreamEnable(i2cp->dmatx);

  /* Transfer complete interrupt enabled.*/
  dp->CR1 |= I2C_CR1_TCIE;

  /* Starts the operation.*/
  dp->CR2 |= I2C_CR2_START;

  /* Waits for the operation completion or a timeout.*/
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  /* In case of a software timeout a STOP is sent as an extreme attempt
     to release the bus.*/
  if (msg == MSG_TIMEOUT) {
    dp->CR2 |= I2C_CR2_STOP;
  }

  return msg;
}

#endif /* HAL_USE_I2C */

/** @} */
