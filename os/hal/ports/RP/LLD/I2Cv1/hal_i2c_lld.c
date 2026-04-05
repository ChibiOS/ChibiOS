/*
    ChibiOS - Copyright (C) 2022 Stefan Kerkmann.
    ChibiOS - Copyright (C) 2021 Hanya.
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
 * @file    I2Cv1/hal_i2c_lld.c
 * @brief   RP I2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */

#include "hal.h"

#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   I2C0 driver identifier.
 */
#if (RP_I2C_USE_I2C0 == TRUE) || defined(__DOXYGEN__)
I2CDriver I2CD0;
#endif

/**
 * @brief   I2C1 driver identifier.
 */
#if (RP_I2C_USE_I2C1 == TRUE) || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#define I2C_OVERRUN_ERRORS \
  (I2C_IC_INTR_STAT_R_RX_OVER | I2C_IC_INTR_STAT_R_RX_UNDER | \
   I2C_IC_INTR_STAT_R_TX_OVER)

#define I2C_ERROR_INTERRUPTS \
  (I2C_IC_INTR_MASK_M_TX_ABRT | I2C_IC_INTR_MASK_M_TX_OVER | \
   I2C_IC_INTR_MASK_M_RX_OVER | I2C_IC_INTR_MASK_M_RX_UNDER)

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief    Calculates the waiting time which is 10 times the SCL period
 *           duration.
 */
static sysinterval_t i2c_lld_get_wait_time(I2CDriver *i2cp) {
  uint32_t baudrate = i2cp->config->baudrate;
  if (baudrate <= 100000U) {
    return OSAL_US2I(100);
  } else if (baudrate <= 400000U) {
    return OSAL_US2I(25);
  } else {
    return OSAL_US2I(10);
  }
}

/**
 * @brief    Tries to disable the I2C peripheral.
 */
static msg_t i2c_lld_disableS(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  systime_t start, end;
  msg_t ok = MSG_OK;

  /* Calculating the time window for the timeout on the enable condition. */
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, i2c_lld_get_wait_time(i2cp));

  dp->ENABLE &= ~I2C_IC_ENABLE_ENABLE;

  osalSysUnlock();
  while ((dp->ENABLESTATUS & I2C_IC_ENABLE_STATUS_IC_EN) != 0U) {
    osalSysLock();
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      ok = MSG_TIMEOUT;
    }
    osalSysUnlock();

    if (ok != MSG_OK) {
      break;
    }
  }
  osalSysLock();

  return ok;
}

/**
 * @brief    Aborts any ongoing transmission of the I2C peripheral.
 */
static msg_t i2c_lld_abort_transmissionS(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  systime_t start, end;
  msg_t ok = MSG_OK;

  /* Calculating the time window for the timeout on the abort condition. */
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, i2c_lld_get_wait_time(i2cp));

  /* Disable all interrupts as a pre-caution. */
  dp->INTRMASK = 0U;
  /* Enable peripheral to issue abort. */
  dp->ENABLE |= I2C_IC_ENABLE_ENABLE;
  /* Issue abort. */
  dp->ENABLE |= I2C_IC_ENABLE_ABORT;

  osalSysUnlock();
  /* Wait for the transmission to be aborted. */
  while ((dp->RAWINTRSTAT & I2C_IC_INTR_STAT_R_TX_ABRT) == 0U) {
    osalSysLock();
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      ok = MSG_TIMEOUT;
    }
    osalSysUnlock();
    if (ok != MSG_OK) {
      break;
    }
  }
  osalSysLock();

  (void)dp->CLRTXABRT;
  return ok;
}

/**
 * @brief    Handles transmission errors by waking the sleeping thread
 *           and setting the error reasons.
 */
static void i2c_lld_handle_errors(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  if (dp->TXABRTSOURCE & I2C_IC_TX_ABRT_SOURCE_ARB_LOST) {
    i2cp->errors |= I2C_ARBITRATION_LOST;
  }

  if (dp->RAWINTRSTAT & I2C_OVERRUN_ERRORS) {
    i2cp->errors |= I2C_OVERRUN;
  }

  /* Disable all interrupts. */
  dp->INTRMASK = 0U;
  (void)dp->CLRINTR;
  (void)dp->CLRTXABRT;
  _i2c_wakeup_error_isr(i2cp);
}

/**
 * @brief    Calculates and set up clock settings.
 */
static void i2c_lld_setup_frequency(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;

  halfreq_t freq_in = halClockGetPointX(RP_CLK_SYS);

  const uint32_t baudrate = i2cp->config->baudrate;
  const uint32_t period = (freq_in + baudrate / 2U) / baudrate;

  uint32_t lcnt, hcnt, sda_tx_hold_count;

  if (baudrate <= 100000U) {
    /* Standard Mode: H: 4000ns, L: 4700ns ~ 0.54. */
    lcnt = period * 54U / 100U;
  } else if (baudrate <= 400000U) {
    /* Fast Mode: H: 600ns, L: 1300ns ~ 0.68. */
    lcnt = period * 68U / 100U;
  } else {
    /* Fast Mode Plus: H: 500ns, L: 760ns ~ 0.60. */
    lcnt = period * 60U / 100U;
  }
  hcnt = period - lcnt;

  osalDbgAssert((lcnt >= 8) &&
                (lcnt <= I2C_IC_FS_SCL_LCNT) &&
                (hcnt >= 8) &&
                (hcnt <= I2C_IC_FS_SCL_HCNT),
                "Invalid I2C clock parameters.");

  if (baudrate < 1000000U) {
    /* Standard and Fast Mode:
       sda_tx_hold_count = freq_in [cycles/s] * 300ns * (1s / 1e9ns)
       Reduce 300/1e9 to 3/1e7 to avoid numbers that don't fit in u32.
       Add 1 to avoid division truncation. */
    sda_tx_hold_count = ((freq_in * 3U) / 10000000U) + 1U;
  } else {
    /* Fast Mode Plus:
       sda_tx_hold_count = freq_in [cycles/s] * 120ns * (1s / 1e9ns)
       Reduce 120/1e9 to 3/25e6 to avoid numbers that don't fit in u32.
       Add 1 to avoid division truncation. */
    sda_tx_hold_count = ((freq_in * 3U) / 25000000U) + 1U;
  }
  osalDbgCheck(sda_tx_hold_count <= lcnt - 2U);

  dp->FSSCLHCNT = hcnt & I2C_IC_FS_SCL_HCNT;
  dp->FSSCLLCNT = lcnt & I2C_IC_FS_SCL_LCNT;
  dp->FSSPKLEN = (lcnt < 16 ? 1 : lcnt / 16) & I2C_IC_FS_SPKLEN_IC_FS_SPKLEN;
  dp->SDAHOLD = sda_tx_hold_count & I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD;
}

/**
 * @brief    Requests data to be received, actual reception is done in the
 *           interrupt handler.
 */
static void i2c_lld_request_data(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t data = I2C_IC_DATA_CMD_CMD;

  /* RP Designware I2C peripheral has FIFO depth of 16 elements. As we
     specify that the TX_EMPTY interrupt only fires if the TX FIFO is
     truly empty we don't need to check the current fill level. */
  uint32_t batch = i2cp->rxbytes < 16U ? (uint32_t)i2cp->rxbytes : 16U;

  if (i2cp->send_restart) {
    data |= I2C_IC_DATA_CMD_RESTART;
    i2cp->send_restart = false;
  }

  /* Setup RX FIFO trigger level to only trigger when the batch has been
     completely received. Therefore we don't need to check if there are any
     bytes still pending to be received. */
  dp->RXTL = batch > 1U ? batch - 1U : 0U;

  while (batch > 0U) {
    /* Send STOP after last byte. */
    if (i2cp->rxbytes == 1U) {
      data |= I2C_IC_DATA_CMD_STOP;
    }
    dp->DATACMD = data;

    batch--;
    i2cp->rxbytes--;
    data = I2C_IC_DATA_CMD_CMD;
  }

  /* Clear TX FIFO empty interrupt, it will be re-activated when data has
     been received. */
  dp->CLR.INTRMASK = I2C_IC_INTR_MASK_M_TX_EMPTY;
  /* Enable RX FULL interrupt to process received data. */
  dp->SET.INTRMASK = I2C_IC_INTR_STAT_R_RX_FULL;
}

/**
 * @brief    Fills TX FIFO with data to be sent.
 */
static void i2c_lld_transmit_data(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t data;

  while (i2cp->txbytes > 0U && dp->STATUS & I2C_IC_STATUS_TFNF) {
    data = (uint32_t)*i2cp->txptr;

    /* Send STOP after last byte. */
    if (i2cp->txbytes == 1U) {
      data |= I2C_IC_DATA_CMD_STOP;
    }
    dp->DATACMD = data;

    i2cp->txptr++;
    i2cp->txbytes--;
  }

  /* Nothing more to send, disable TX FIFO empty interrupt. */
  if (i2cp->txbytes == 0U) {
    dp->CLR.INTRMASK = I2C_IC_INTR_MASK_M_TX_EMPTY;
  }
}

/**
 * @brief   I2C shared ISR code.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
static void i2c_lld_serve_interrupt(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->i2c;
  uint32_t intr = dp->INTRSTAT;

  /* Transmission error detected. */
  if (intr & I2C_ERROR_INTERRUPTS) {
    return i2c_lld_handle_errors(i2cp);
  }

  /* If the TX FIFO is empty we can request or send more data. */
  if (intr & I2C_IC_INTR_STAT_R_TX_EMPTY) {
    if (i2cp->state == I2C_ACTIVE_TX) {
      i2c_lld_transmit_data(i2cp);
    } else {
      i2c_lld_request_data(i2cp);
    }
    return;
  }

  if (intr & I2C_IC_INTR_STAT_R_RX_FULL) {
    while (dp->STATUS & I2C_IC_STATUS_RFNE) {
      /* Read out received data. */
      *i2cp->rxptr = (uint8_t)dp->DATACMD;
      i2cp->rxptr++;
    }

    if (i2cp->rxbytes == 0U) {
      /* Everything is received, therefore disable all FIFO IRQs. */
      dp->CLR.INTRMASK = I2C_IC_INTR_MASK_M_RX_FULL |
                          I2C_IC_INTR_STAT_R_TX_EMPTY;
    } else {
      /* Enable TX FIFO empty IRQ to request more data. */
      dp->SET.INTRMASK = I2C_IC_INTR_STAT_R_TX_EMPTY;
    }
    return;
  }

  if (intr & I2C_IC_INTR_STAT_R_STOP_DET) {
    /* Clear irq flag. */
    (void)dp->CLRSTOPDET;
    if (i2cp->state == I2C_ACTIVE_TX && i2cp->rxbytes > 0U) {
      /* State change to RX. */
      i2cp->state = I2C_ACTIVE_RX;
      /* Restart communication. */
      i2cp->send_restart = true;

      /* Enable TX FIFO empty IRQ to request more data to be received. */
      dp->SET.INTRMASK = I2C_IC_INTR_STAT_R_TX_EMPTY;
      return;
    }
  }

  /* Transmission complete, disable and clear all interrupts. */
  dp->INTRMASK = 0U;
  (void)dp->CLRINTR;
  _i2c_wakeup_isr(i2cp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (RP_I2C_USE_I2C0 == TRUE) || defined(__DOXYGEN__)

OSAL_IRQ_HANDLER(RP_I2C0_IRQ_HANDLER) {
  OSAL_IRQ_PROLOGUE();

  i2c_lld_serve_interrupt(&I2CD0);

  OSAL_IRQ_EPILOGUE();
}

#endif

#if (RP_I2C_USE_I2C1 == TRUE) || defined(__DOXYGEN__)

OSAL_IRQ_HANDLER(RP_I2C1_IRQ_HANDLER) {
  OSAL_IRQ_PROLOGUE();

  i2c_lld_serve_interrupt(&I2CD1);

  OSAL_IRQ_EPILOGUE();
}

#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if RP_I2C_USE_I2C0 == TRUE
  i2cObjectInit(&I2CD0);
  I2CD0.i2c = I2C0;
  I2CD0.thread = NULL;

  /* Reset I2C. */
  rp_peripheral_reset(RESETS_ALLREG_I2C0);
#endif

#if RP_I2C_USE_I2C1 == TRUE
  i2cObjectInit(&I2CD1);
  I2CD1.i2c = I2C1;
  I2CD1.thread = NULL;

  /* Reset I2C. */
  rp_peripheral_reset(RESETS_ALLREG_I2C1);
#endif
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

  if (i2cp->state == I2C_STOP) {

#if RP_I2C_USE_I2C0 == TRUE
    if (&I2CD0 == i2cp) {
      rp_peripheral_unreset(RESETS_ALLREG_I2C0);

      nvicEnableVector(RP_I2C0_IRQ_NUMBER, RP_IRQ_I2C0_PRIORITY);
    }
#endif

#if RP_I2C_USE_I2C1 == TRUE
    if (&I2CD1 == i2cp) {
      rp_peripheral_unreset(RESETS_ALLREG_I2C1);

      nvicEnableVector(RP_I2C1_IRQ_NUMBER, RP_IRQ_I2C1_PRIORITY);
    }
#endif
  }

  /* Disable I2C peripheral for setup phase. */
  if (i2c_lld_disableS(i2cp) != MSG_OK) {
    return;
  }

  dp->CON = I2C_IC_CON_IC_SLAVE_DISABLE |
            I2C_IC_CON_IC_RESTART_EN |
#if RP_I2C_ADDRESS_MODE_10BIT == TRUE
            I2C_IC_CON_IC_10BITADDR_MASTER |
#endif
            (2U << I2C_IC_CON_SPEED_Pos) | /* Always Fast Mode. */
            I2C_IC_CON_MASTER_MODE |
            I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE |
            I2C_IC_CON_TX_EMPTY_CTRL;

  dp->RXTL = 0U;
  dp->TXTL = 0U;

  i2c_lld_setup_frequency(i2cp);

  /* Clear interrupt mask. */
  dp->INTRMASK = 0U;

  /* Enable peripheral. */
  dp->ENABLE = I2C_IC_ENABLE_ENABLE;

  /* Clear interrupts. */
  (void)dp->CLRINTR;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  if (i2cp->state != I2C_STOP) {
    if (i2c_lld_disableS(i2cp) != MSG_OK) {
      i2c_lld_abort_transmissionS(i2cp);
    }
#if RP_I2C_USE_I2C0 == TRUE
    if (&I2CD0 == i2cp) {
      nvicDisableVector(RP_I2C0_IRQ_NUMBER);

      rp_peripheral_reset(RESETS_ALLREG_I2C0);
    }
#endif

#if RP_I2C_USE_I2C1 == TRUE
    if (&I2CD1 == i2cp) {
      nvicDisableVector(RP_I2C1_IRQ_NUMBER);

      rp_peripheral_reset(RESETS_ALLREG_I2C1);
    }
#endif
  }
}

/**
 * @brief   Receives data via the I2C bus as master.
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
                                     sysinterval_t timeout) {
  msg_t msg;
  systime_t start, end;
  I2C_TypeDef *dp = i2cp->i2c;

  /* Releases the lock from high level driver. */
  osalSysUnlock();

  /* Calculating the time window for the timeout on the busy bus condition. */
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, OSAL_MS2I(RP_I2C_BUSY_TIMEOUT));

  while (true) {
    osalSysLock();

    if ((dp->STATUS & I2C_IC_STATUS_ACTIVITY) == 0) {
      break;
    }

    /* Attempt to clear activity bit. */
    (void)dp->CLRACTIVITY;

    /* If the system time went outside the allowed window then a timeout
       condition is returned. */
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  i2cp->txbytes = 0U;
  i2cp->txptr = NULL;
  i2cp->rxbytes = rxbytes;
  i2cp->rxptr = rxbuf;
  i2cp->send_restart = false;

  /* Disable I2C peripheral during setup phase. */
  msg = i2c_lld_disableS(i2cp);
  if (msg != MSG_OK) {
    return msg;
  }

  /* Set address. */
  dp->TAR = addr & I2C_IC_TAR_IC_TAR_Msk;

  /* Clear interrupt mask. */
  dp->INTRMASK = 0U;

  /* Enable peripheral. */
  dp->ENABLE = I2C_IC_ENABLE_ENABLE;

  /* Clear interrupts. */
  (void)dp->CLRINTR;

  /* Set interrupt mask. */
  dp->INTRMASK = I2C_IC_INTR_MASK_M_STOP_DET |
                 I2C_IC_INTR_MASK_M_TX_EMPTY |
                 I2C_ERROR_INTERRUPTS;

  /* Waits for the operation completion or a timeout. */
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  if (msg == MSG_TIMEOUT) {
    /* Disable and clear interrupts. */
    dp->INTRMASK = 0U;
    (void)dp->CLRINTR;
  }

  return msg;
}

/**
 * @brief   Transmits data via the I2C bus as master.
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
                                      sysinterval_t timeout) {

  msg_t msg;
  systime_t start, end;
  I2C_TypeDef *dp = i2cp->i2c;

  /* Releases the lock from high level driver. */
  osalSysUnlock();

  /* Calculating the time window for the timeout on the busy bus condition. */
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, OSAL_MS2I(RP_I2C_BUSY_TIMEOUT));

  while (true) {
    osalSysLock();

    if ((dp->STATUS & I2C_IC_STATUS_ACTIVITY) == 0U) {
      break;
    }
    /* Attempt to clear activity bit. */
    (void)dp->CLRACTIVITY;

    /* If the system time went outside the allowed window then a timeout
       condition is returned. */
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  i2cp->txbytes = txbytes;
  i2cp->txptr = txbuf;
  i2cp->rxbytes = rxbytes;
  i2cp->rxptr = rxbuf;
  i2cp->send_restart = false;

  /* Disable I2C peripheral during setup phase. */
  msg = i2c_lld_disableS(i2cp);
  if (msg != MSG_OK) {
    return msg;
  }

  /* Set target address. */
  dp->TAR = addr & I2C_IC_TAR_IC_TAR;

  /* Clear interrupt mask. */
  dp->INTRMASK = 0U;

  /* Enable peripheral. */
  dp->ENABLE = I2C_IC_ENABLE_ENABLE;

  /* Clear interrupts. */
  (void)dp->CLRINTR;

  /* Set interrupt mask. */
  dp->INTRMASK = I2C_IC_INTR_MASK_M_STOP_DET |
                 I2C_IC_INTR_MASK_M_TX_EMPTY |
                 I2C_ERROR_INTERRUPTS;

  /* Waits for the operation completion or a timeout. */
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  if (msg == MSG_TIMEOUT) {
    /* Disable and clear interrupts. */
    dp->INTRMASK = 0U;
    (void)dp->CLRINTR;
  }

  return msg;
}

#endif /* HAL_USE_I2C == TRUE */

/** @} */
