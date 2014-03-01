/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx I2C driver - Copyright (C) 2013 Marcin Jokel

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

/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */


/**
 * @file    LPC17xx/i2c_lld.h
 * @brief   LPC17xx I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if LPC17xx_I2C_USE_I2C0 || defined(__DOXYGEN__)
/** @brief I2C1 driver identifier.*/
I2CDriver I2CD1;
#endif

#if LPC17xx_I2C_USE_I2C1 || defined(__DOXYGEN__)
/** @brief I2C2 driver identifier.*/
I2CDriver I2CD2;
#endif

#if LPC17xx_I2C_USE_I2C2 || defined(__DOXYGEN__)
/** @brief I2C3 driver identifier.*/
I2CDriver I2CD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] msg       wakeup message
 *
 * @notapi
 */
#define wakeup_isr(i2cp, msg) {                                             \
  chSysLockFromIsr();                                                       \
  if ((i2cp)->thread != NULL) {                                             \
    Thread *tp = (i2cp)->thread;                                            \
    (i2cp)->thread = NULL;                                                  \
    tp->p_u.rdymsg = (msg);                                                 \
    chSchReadyI(tp);                                                        \
  }                                                                         \
  chSysUnlockFromIsr();                                                     \
}

/**
 * @brief   Handling of stalled I2C transactions.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_safety_timeout(void *p) {
  I2CDriver *i2cp = (I2CDriver *)p;

  chSysLockFromIsr();
  if (i2cp->thread) {
    Thread *tp = i2cp->thread;
    i2cp->thread = NULL;
    tp->p_u.rdymsg = RDY_TIMEOUT;
    chSchReadyI(tp);
  }
  chSysUnlockFromIsr();
}

/**
 * @brief   I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_serve_error_interrupt(I2CDriver *i2cp, uint32_t status) {
  i2cflags_t error = 0;

  switch (status) {
    case I2C_STATE_ARB_LOST:
      error = I2CD_ARBITRATION_LOST;
      break;
    case I2C_STATE_BUS_ERROR:
      error = I2CD_BUS_ERROR;
      break;
    case I2C_STATE_MS_SLAR_NACK:
    case I2C_STATE_MS_TDAT_NACK:
    case I2C_STATE_MS_SLAW_NACK:
      error = I2CD_ACK_FAILURE ;
      break;
  }

  /* If some error has been identified then sends wakes the waiting thread.*/
  i2cp->errors = error;
  wakeup_isr(i2cp, RDY_RESET);
}

/**
 * @brief   I2C serve interrupt handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_serve_interrupt(I2CDriver *i2cp) {
  uint32_t status;
  LPC_I2C_TypeDef *dp = i2cp->i2c;

  status = dp->STAT;
  switch(status) {
    case I2C_STATE_MS_START:    /*  A START condition has been transmitted. */
      if (i2cp->txbytes > 0) {
        dp->DAT = i2cp->addr;                  /* Write slave address with WR bit. */
      }
      else {
        dp->DAT = i2cp->addr | I2C_RD_BIT;     /* Write slave address with RD bit. */
      }

      dp->CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_SIC;   /* Clear START and SI bit. */
      break;

    case I2C_STATE_MS_SLAR_NACK: /* NOT ACK has been received, Master will be transmitted STOP. */
    case I2C_STATE_MS_TDAT_NACK: /* NOT ACK has been received, Master will be transmitted STOP. */
    case I2C_STATE_MS_SLAW_NACK: /* NOT ACK has been received, Master will be transmitted STOP. */
      dp->CONSET = I2C_CONSET_STO;             /* Set STOP bit. */
      dp->CONCLR = I2C_CONCLR_SIC;             /* Clear SI bit. */
      i2c_lld_serve_error_interrupt(i2cp, status);
      break;

    case I2C_STATE_MS_SLAW_ACK: /* SLA + W has been transmitted, ACK has been received. */
    case I2C_STATE_MS_TDAT_ACK: /* Data byte has been transmitted, ACK has been received. */
      if (i2cp->txbytes > 0) {
        dp->DAT = *i2cp->txbuf++;               /* Write data. */
        i2cp->txbytes--;
      }
      else {
        if (i2cp->rxbytes > 0) {
          dp->CONSET = I2C_CONSET_STO | I2C_CONSET_STA; /* Set START and STOP bit. */
        }                                                    /* STOP bit will be transmit, then START bit. */
        else {
          dp->CONSET = I2C_CONSET_STO;         /* Set STOP bit. */
          wakeup_isr(i2cp, RDY_OK);
        }
      }
      dp->CONCLR = I2C_CONCLR_SIC;             /* Clear SI bit. */
      break;

    case I2C_STATE_MS_SLAR_ACK: /* SLA + R has been transmitted, ACK has been received. */
    case I2C_STATE_MS_RDAT_ACK: /* Data byte has been received, ACK has been returned. */
      if (status == I2C_STATE_MS_RDAT_ACK) {
        *i2cp->rxbuf++ = dp->DAT;                /* Read data */
        i2cp->rxbytes--;
      }
      if (i2cp->rxbytes == 1) {
        dp->CONCLR = I2C_CONCLR_SIC | I2C_CONCLR_AAC; /* Clear SI and ACK bit. */
      }
      else {
        dp->CONSET = I2C_CONSET_AA;            /* Set ACK bit. */
        dp->CONCLR = I2C_CONCLR_SIC;           /* Clear SI bit. */
      }
      break;

    case I2C_STATE_MS_RDAT_NACK: /* Data byte has been received, NOT ACK has been returned. */
      *i2cp->rxbuf++ = dp->DAT;                /* Read data. */
      i2cp->rxbytes--;
      dp->CONSET = I2C_CONSET_STO;             /* Set STOP bit. */
      dp->CONCLR = I2C_CONCLR_SIC;             /* Clear SI bit. */
      wakeup_isr(i2cp, RDY_OK);
      break;

    case I2C_STATE_BUS_ERROR: /* Bus error. */
    case I2C_STATE_ARB_LOST:  /* Arbitration lost. */
      dp->CONCLR = I2C_CONCLR_SIC;             /* Clear SI bit. */
      i2c_lld_serve_error_interrupt(i2cp, status);
      break;
  }

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
#if LPC17xx_I2C_USE_I2C0 || defined(__DOXYGEN__)
/**
 * @brief   I2C0 event interrupt handler.
 *
 * @notapi
 */
CH_IRQ_HANDLER(Vector68) {

  CH_IRQ_PROLOGUE();
  i2c_lld_serve_interrupt(&I2CD1);
  CH_IRQ_EPILOGUE();
}
#endif

#if LPC17xx_I2C_USE_I2C1 || defined(__DOXYGEN__)
/**
 * @brief   I2C1 event interrupt handler.
 *
 * @notapi
 */
CH_IRQ_HANDLER(Vector6C) {

  CH_IRQ_PROLOGUE();
  i2c_lld_serve_interrupt(&I2CD2);
  CH_IRQ_EPILOGUE();
}
#endif

#if LPC17xx_I2C_USE_I2C2 || defined(__DOXYGEN__)
/**
 * @brief   I2C2 event interrupt handler.
 *
 * @notapi
 */
CH_IRQ_HANDLER(Vector70) {

  CH_IRQ_PROLOGUE();
  i2c_lld_serve_interrupt(&I2CD3);
  CH_IRQ_EPILOGUE();
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

#if LPC17xx_I2C_USE_I2C0
  i2cObjectInit(&I2CD1);
  I2CD1.thread = NULL;
  I2CD1.i2c    = LPC_I2C0;
#endif

#if LPC17xx_I2C_USE_I2C1
  i2cObjectInit(&I2CD2);
  I2CD2.thread = NULL;
  I2CD2.i2c    = LPC_I2C1;
#endif

#if LPC17xx_I2C_USE_I2C2
  i2cObjectInit(&I2CD3);
  I2CD3.thread = NULL;
  I2CD3.i2c    = LPC_I2C2;
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

  uint32_t i2cscl;
  uint32_t mulh, mull, div;
  LPC_I2C_TypeDef *dp = i2cp->i2c;

  /* If in stopped state then enables the I2C clock. */
  if (i2cp->state == I2C_STOP) {
#if LPC17xx_I2C_USE_I2C0
    if (&I2CD1 == i2cp) {
      LPC_SC->PCONP |= (1UL << 7);
      nvicEnableVector(I2C0_IRQn,
            CORTEX_PRIORITY_MASK(LPC17xx_I2C_I2C0_IRQ_PRIORITY));
    }
#endif

#if LPC17xx_I2C_USE_I2C1
    if (&I2CD2 == i2cp) {
      LPC_SC->PCONP |= (1UL << 19);
      nvicEnableVector(I2C1_IRQn,
            CORTEX_PRIORITY_MASK(LPC17xx_I2C_I2C1_IRQ_PRIORITY));
    }
#endif

#if LPC17xx_I2C_USE_I2C2
    if (&I2CD3 == i2cp) {
      LPC_SC->PCONP |= (1UL << 26);
      nvicEnableVector(I2C2_IRQn,
            CORTEX_PRIORITY_MASK(LPC17xx_I2C_I2C2_IRQ_PRIORITY));
    }
#endif
  }

  /* Make sure I2C peripheral is disabled */
  dp->CONCLR = I2C_CONCLR_ENC;

  /* Setup I2C clock parameters.*/
  i2cscl = (LPC17xx_PCLK/(i2cp->config->clock_timing));
  if (i2cp->config->mode == I2C_FAST_MODE) {
    div = 19;
    mull = 13;
    mulh = 6;
  } else if (i2cp->config->mode == I2C_FAST_MODE_PLUS) {
    div = 3;
    mull = 2;
    mulh = 1;
  } else { /* i2cp->config->mode == I2C_STANDARD_MODE */
    div = 2;
    mull = 1;
    mulh = 1;
  }

  dp->SCLH = (mulh * i2cscl) / div;
  dp->SCLL = (mull * i2cscl) / div;

  /* Enable I2C.*/
  dp->CONSET |= I2C_CONSET_EN;

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
    i2cp->i2c->CONCLR = I2C_CONCLR_ENC;
#if LPC17xx_I2C_USE_I2C0
 if (&I2CD1 == i2cp) {
   nvicDisableVector(I2C0_IRQn);
   LPC_SC->PCONP &= ~(1UL << 7);
 }
#endif

#if LPC17xx_I2C_USE_I2C1
 if (&I2CD2 == i2cp) {
   nvicDisableVector(I2C1_IRQn);
   LPC_SC->PCONP &= ~(1UL << 19);
 }
#endif

#if LPC17xx_I2C_USE_I2C2
 if (&I2CD3 == i2cp) {
   nvicDisableVector(I2C2_IRQn);
   LPC_SC->PCONP &= ~(1UL << 26);
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
 * @retval RDY_OK       if the function succeeded.
 * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval RDY_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     systime_t timeout) {
  LPC_I2C_TypeDef *dp = i2cp->i2c;
  VirtualTimer vt;

  i2cp->addr = addr << 1;
  /* Global timeout for the whole operation.*/
  if (timeout != TIME_INFINITE)
    chVTSetI(&vt, timeout, i2c_lld_safety_timeout, (void *)i2cp);

  /* Releases the lock from high level driver.*/
  chSysUnlock();

  /* Initializes driver fields */
  i2cp->errors = 0;
  i2cp->rxbuf = rxbuf;
  i2cp->rxbytes = rxbytes;

  /* This lock will be released in high level driver.*/
  chSysLock();

  /* Atomic check on the timer in order to make sure that a timeout didn't
     happen outside the critical zone.*/
  if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
    return RDY_TIMEOUT;

  /* Starts the operation.*/
  dp->CONSET = I2C_CONSET_STA;

  /* Waits for the operation completion or a timeout.*/
  i2cp->thread = chThdSelf();
  chSchGoSleepS(THD_STATE_SUSPENDED);
  if ((timeout != TIME_INFINITE) && chVTIsArmedI(&vt))
    chVTResetI(&vt);

  return chThdSelf()->p_u.rdymsg;
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
 * @retval RDY_OK       if the function succeeded.
 * @retval RDY_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval RDY_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      systime_t timeout) {
  LPC_I2C_TypeDef *dp = i2cp->i2c;
  VirtualTimer vt;

  i2cp->addr = addr << 1;
  /* Global timeout for the whole operation.*/
  if (timeout != TIME_INFINITE)
    chVTSetI(&vt, timeout, i2c_lld_safety_timeout, (void *)i2cp);

  /* Releases the lock from high level driver.*/
  chSysUnlock();

  /* Initializes driver fields */
  i2cp->errors = 0;
  i2cp->txbuf = txbuf;
  i2cp->txbytes = txbytes;
  i2cp->rxbuf = rxbuf;
  i2cp->rxbytes = rxbytes;

  /* This lock will be released in high level driver.*/
  chSysLock();

  /* Atomic check on the timer in order to make sure that a timeout didn't
     happen outside the critical zone.*/
  if ((timeout != TIME_INFINITE) && !chVTIsArmedI(&vt))
    return RDY_TIMEOUT;

  /* Starts the operation.*/
  dp->CONSET = I2C_CONSET_STA;

  /* Waits for the operation completion or a timeout.*/
  i2cp->thread = chThdSelf();
  chSchGoSleepS(THD_STATE_SUSPENDED);

  if ((timeout != TIME_INFINITE) && chVTIsArmedI(&vt))
    chVTResetI(&vt);

  return chThdSelf()->p_u.rdymsg;
}

#endif /* HAL_USE_I2C */

/** @} */
