/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file STM32/i2c_lld.c
 * @brief STM32 I2C subsystem low level driver source. Slave mode not implemented.
 * @addtogroup I2C
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "i2c_lld.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Datasheet notes.                                                          */
/*===========================================================================*/
/**
 * From reference manuals from ST:
 *
 * Note:
 * When the STOP, START or PEC bit is set, the software must NOT perform
 * any write access to I2C_CR1 before this bit is cleared by hardware.
 * Otherwise there is a  risk of setting a second STOP, START or PEC request.
 */

/*===========================================================================*/
/* Knowledge base.                                                            */
/*===========================================================================*/
/*
Not all system functions are usable in a given context.

The most restrictive type are the i-class, an I-class function is a function that must:
- Not access the "current" thread in any way (from an ISR the current thread
is random so it is meaningless).
- Not reschedule internally (from an ISR the reschedule is done at the end of
the ISR chain, rescheduling from within an ISR is forbidden because would
leave the IRQ stack not empty with all kind of funny consequences.
- Not try to change state for the current thread.
- Must be invoked between a lock() and an unlock() but never lock/unlock internally.

A bit less restrictive are the S-class that must simply:
- Be invoked between a lock() and an unlock() but never lock/unlock internally.
S-class can reschedule internally, access the current thread implicitly and
also change state so are not eligible for ISR context.

Normal functions can be invoked from thread context only but have no internal
restrictions.
*/

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/
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

#define I2C3_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C3_RX_DMA_STREAM,                        \
                       STM32_I2C3_RX_DMA_CHN)
#define I2C3_TX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2C_I2C3_TX_DMA_STREAM,                        \
                       STM32_I2C3_TX_DMA_CHN)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

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

/** @brief I2C2 driver identifier.*/
#if STM32_I2C_USE_I2C3 || defined(__DOXYGEN__)
I2CDriver I2CD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/
/* Debugging variables */
#if CH_DBG_ENABLE_ASSERTS
static volatile uint16_t dbgSR1 = 0;
static volatile uint16_t dbgSR2 = 0;
static volatile uint16_t dbgCR1 = 0;
static volatile uint16_t dbgCR2 = 0;
#endif /* CH_DBG_ENABLE_ASSERTS */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Return the last event value from I2C status registers.
 * @details Important but implicit function is clearing interrupts flags.
 * @note    Internal use only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static uint32_t i2c_get_event(I2CDriver *i2cp){
  uint16_t regSR1 = i2cp->id_i2c->SR1;
  uint16_t regSR2 = i2cp->id_i2c->SR2;
  #if CH_DBG_ENABLE_ASSERTS
    dbgSR1 = regSR1;
    dbgSR2 = regSR2;
  #endif /* CH_DBG_ENABLE_ASSERTS */

  return (I2C_EV_MASK & (regSR1 | (regSR2 << 16)));
}


/**
 * @brief I2C interrupts handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
static void i2c_serve_event_interrupt(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->id_i2c;

  switch(i2c_get_event(i2cp)){
  case I2C_EV5_MASTER_MODE_SELECT:
    dp->DR = i2cp->slave_addr;
    break;

  case I2C_EV6_MASTER_REC_MODE_SELECTED:
    dmaStreamEnable(i2cp->dmarx);
    i2cp->id_i2c->CR2 |= I2C_CR2_DMAEN | I2C_CR2_LAST;
    break;

  case I2C_EV6_MASTER_TRA_MODE_SELECTED:
    dmaStreamEnable(i2cp->dmatx);
    i2cp->id_i2c->CR2 |= I2C_CR2_DMAEN | I2C_CR2_LAST;
    break;

  case I2C_EV8_2_MASTER_BYTE_TRANSMITTED:
    /* catch BTF event after the end of transmission */
    if (i2cp->rxbytes > 1){
      /* start "read after write" operation */
      i2c_lld_master_receive(i2cp, (i2cp->slave_addr >> 1),
                            i2cp->rxbuf, i2cp->rxbytes);
      return;
    }
    else
      i2cp->id_i2c->CR1 |= I2C_CR1_STOP;
      _i2c_isr_code(i2cp, i2cp->id_slave_config);
    break;

  default:
    break;
  }
}


/**
 * @brief DMA rx end IRQ handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
static void i2c_lld_serve_rx_end_irq(I2CDriver *i2cp){
  dmaStreamDisable(i2cp->dmarx);

  i2cp->id_i2c->CR1 |= I2C_CR1_STOP;
  _i2c_isr_code(i2cp, i2cp->id_slave_config);
}


/**
 * @brief DMA tx enr IRQ handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
static void i2c_lld_serve_tx_end_irq(I2CDriver *i2cp){
  dmaStreamDisable(i2cp->dmatx);
}


/**
 * @brief I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
static void i2c_serve_error_interrupt(I2CDriver *i2cp) {
  i2cflags_t errors;

  chSysLockFromIsr();
  /* clear interrupt falgs just to be safe */
  dmaStreamDisable(i2cp->dmatx);
  dmaStreamDisable(i2cp->dmarx);
  dmaStreamClearInterrupt(i2cp->dmatx);
  dmaStreamClearInterrupt(i2cp->dmarx);
  chSysUnlockFromIsr();

  #define reg (i2cp->id_i2c)
  errors = I2CD_NO_ERROR;

  if(reg->SR1 & I2C_SR1_BERR) {                /* Bus error */
    reg->SR1 &= ~I2C_SR1_BERR;
    errors |= I2CD_BUS_ERROR;
  }
  if(reg->SR1 & I2C_SR1_ARLO) {                /* Arbitration lost */
    reg->SR1 &= ~I2C_SR1_ARLO;
    errors |= I2CD_ARBITRATION_LOST;
  }
  if(reg->SR1 & I2C_SR1_AF) {                  /* Acknowledge fail */
    reg->SR1 &= ~I2C_SR1_AF;
    reg->CR1 |= I2C_CR1_STOP;                  /* setting stop bit */
    errors |= I2CD_ACK_FAILURE;
  }
  if(reg->SR1 & I2C_SR1_OVR) {                 /* Overrun */
    reg->SR1 &= ~I2C_SR1_OVR;
    errors |= I2CD_OVERRUN;
  }
  if(reg->SR1 & I2C_SR1_PECERR) {              /* PEC error */
    reg->SR1 &= ~I2C_SR1_PECERR;
    errors |= I2CD_PEC_ERROR;
  }
  if(reg->SR1 & I2C_SR1_TIMEOUT) {             /* SMBus Timeout */
    reg->SR1 &= ~I2C_SR1_TIMEOUT;
    errors |= I2CD_TIMEOUT;
  }
  if(reg->SR1 & I2C_SR1_SMBALERT) {            /* SMBus alert */
    reg->SR1 &= ~I2C_SR1_SMBALERT;
    errors |= I2CD_SMB_ALERT;
  }

  if(errors != I2CD_NO_ERROR) {                /* send communication end signal */
    i2cp->errors |= errors;
    _i2c_isr_err_code(i2cp, i2cp->id_slave_config);
  }
  #undef reg
}


#if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
/**
 * @brief I2C1 event interrupt handler.
 */
CH_IRQ_HANDLER(I2C1_EV_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_event_interrupt(&I2CD1);
  CH_IRQ_EPILOGUE();
}
/**
 * @brief I2C1 error interrupt handler.
 */
CH_IRQ_HANDLER(I2C1_ER_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_error_interrupt(&I2CD1);
  CH_IRQ_EPILOGUE();
}
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
/**
 * @brief I2C2 event interrupt handler.
 */
CH_IRQ_HANDLER(I2C2_EV_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_event_interrupt(&I2CD2);
  CH_IRQ_EPILOGUE();
}
/**
 * @brief I2C2 error interrupt handler.
 */
CH_IRQ_HANDLER(I2C2_ER_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_error_interrupt(&I2CD2);
  CH_IRQ_EPILOGUE();
}
#endif /* STM32_I2C_USE_I2C2 */

#if STM32_I2C_USE_I2C3 || defined(__DOXYGEN__)
/**
 * @brief I2C3 event interrupt handler.
 */
CH_IRQ_HANDLER(I2C3_EV_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_event_interrupt(&I2CD3);
  CH_IRQ_EPILOGUE();
}
/**
 * @brief I2C3 error interrupt handler.
 */
CH_IRQ_HANDLER(I2C3_ER_IRQHandler) {
  CH_IRQ_PROLOGUE();
  i2c_serve_error_interrupt(&I2CD3);
  CH_IRQ_EPILOGUE();
}
#endif /* STM32_I2C_USE_I2C3 */


/**
 * @brief Low level I2C driver initialization.
 */
void i2c_lld_init(void) {

#if STM32_I2C_USE_I2C1
  i2cObjectInit(&I2CD1);
  I2CD1.id_i2c = I2C1;
  I2CD1.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C1_RX_DMA_STREAM);
  I2CD1.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C1_TX_DMA_STREAM);
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
  i2cObjectInit(&I2CD2);
  I2CD2.id_i2c = I2C2;
  I2CD2.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C2_RX_DMA_STREAM);
  I2CD2.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C2_TX_DMA_STREAM);
#endif /* STM32_I2C_USE_I2C2 */

#if STM32_I2C_USE_I2C3
  i2cObjectInit(&I2CD3);
  I2CD3.id_i2c = I2C3;
  I2CD3.dmarx  = STM32_DMA_STREAM(STM32_I2C_I2C3_RX_DMA_STREAM);
  I2CD3.dmatx  = STM32_DMA_STREAM(STM32_I2C_I2C3_TX_DMA_STREAM);
#endif /* STM32_I2C_USE_I2C3 */
}

/**
 * @brief Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_start(I2CDriver *i2cp) {

  i2cp->dmamode = STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE;

  if (i2cp->id_state == I2C_STOP) {         /* If in stopped state then enables the I2C clock.*/
#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {

      bool_t b;
      b = dmaStreamAllocate(i2cp->dmarx,
                            STM32_I2C_I2C1_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #3", "stream already allocated");
      b = dmaStreamAllocate(i2cp->dmatx,
                            STM32_I2C_I2C1_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #4", "stream already allocated");
      rccEnableI2C1(FALSE);
      NVICEnableVector(I2C1_EV_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
      NVICEnableVector(I2C1_ER_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));

      i2cp->dmamode |= STM32_DMA_CR_CHSEL(I2C1_RX_DMA_CHANNEL) | \
      	  	  	  	  	  STM32_DMA_CR_PL(STM32_I2C_I2C1_DMA_PRIORITY);
      __NOP();
    }
#endif /* STM32_I2C_USE_I2C1 */

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {

      bool_t b;
      b = dmaStreamAllocate(i2cp->dmarx,
                            STM32_I2C_I2C2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #3", "stream already allocated");
      b = dmaStreamAllocate(i2cp->dmatx,
                            STM32_I2C_I2C2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #4", "stream already allocated");
      rccEnableI2C2(FALSE);
      NVICEnableVector(I2C2_EV_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C2_IRQ_PRIORITY));
      NVICEnableVector(I2C2_ER_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C2_IRQ_PRIORITY));

      i2cp->dmamode |= STM32_DMA_CR_CHSEL(I2C2_RX_DMA_CHANNEL) |
                            STM32_DMA_CR_PL(STM32_I2C_I2C2_DMA_PRIORITY);
    }
#endif /* STM32_I2C_USE_I2C2 */

#if STM32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {

      bool_t b;
      b = dmaStreamAllocate(i2cp->dmarx,
                            STM32_I2C_I2C3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_rx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #3", "stream already allocated");
      b = dmaStreamAllocate(i2cp->dmatx,
                            STM32_I2C_I2C3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2c_lld_serve_tx_end_irq,
                            (void *)i2cp);
      chDbgAssert(!b, "uart_lld_start(), #4", "stream already allocated");
      rccEnableI2C3(FALSE);
      NVICEnableVector(I2C3_EV_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C3_IRQ_PRIORITY));
      NVICEnableVector(I2C3_ER_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C3_IRQ_PRIORITY));

      i2cp->dmamode |= STM32_DMA_CR_CHSEL(I2C3_RX_DMA_CHANNEL) |
                            STM32_DMA_CR_PL(STM32_I2C_I2C3_DMA_PRIORITY);
    }
#endif /* STM32_I2C_USE_I2C2 */

  }
  i2cp->dmamode |= STM32_DMA_CR_PSIZE_BYTE |
                   STM32_DMA_CR_MSIZE_BYTE |
                   STM32_DMA_CR_MINC |
                   STM32_DMA_CR_TCIE;
  dmaStreamSetPeripheral(i2cp->dmarx, &i2cp->id_i2c->DR);
  dmaStreamSetPeripheral(i2cp->dmatx, &i2cp->id_i2c->DR);

  i2cp->id_i2c->CR1 = I2C_CR1_SWRST;        /* reset i2c peripheral */
  i2cp->id_i2c->CR1 = 0;
  i2c_lld_set_clock(i2cp);
  i2c_lld_set_opmode(i2cp);

  i2cp->id_i2c->CR1 |= 1;                   /* enable interface */
}


/**
 * @brief Reset interface via RCC.
 */
void i2c_lld_reset(I2CDriver *i2cp){
  chDbgCheck((i2cp->id_state == I2C_STOP)||(i2cp->id_state == I2C_READY),
             "i2c_lld_reset: invalid state");

  #if STM32_I2C_USE_I2C1
  if (&I2CD1 == i2cp)
    rccResetI2C1();
  #endif /* STM32_I2C_USE_I2C1 */

  #if STM32_I2C_USE_I2C2
  if (&I2CD2 == i2cp)
    rccResetI2C2();
  #endif /* STM32_I2C_USE_I2C2 */

  #if STM32_I2C_USE_I2C3
  if (&I2CD3 == i2cp)
      rccResetI2C3();
  #endif /* STM32_I2C_USE_I2C3 */
}


/**
 * @brief Receive data via the I2C bus as master.
 * @details Number of receiving bytes must be more than 1 because of stm32
 *          hardware restrictions.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] slave_addr  slave device address
 * @param[in] rxbuf       pointer to the receive buffer
 * @param[in] rxbytes     number of bytes to be received
 */
void i2c_lld_master_receive(I2CDriver *i2cp, uint8_t slave_addr,
                            uint8_t *rxbuf, size_t rxbytes){

  uint32_t mode = 0;

  chDbgCheck((rxbytes > 1), "i2c_lld_master_receive");

  /* init driver fields */
  i2cp->slave_addr = (slave_addr << 1) | 0x01;    /* LSB = 1 -> receive */
  i2cp->rxbytes = rxbytes;
  i2cp->rxbuf = rxbuf;
  i2cp->errors = 0;

  mode = STM32_DMA_CR_DIR_P2M;
  // TODO: DMA error handling
  dmaStreamSetMemory0(i2cp->dmarx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->dmarx, rxbytes);
  dmaStreamSetMode(i2cp->dmarx, ((i2cp->dmamode) | mode));

  /* wait stop bit from previous transaction*/
  while(i2cp->id_i2c->CR1 & I2C_CR1_STOP)
    ;

  i2cp->id_i2c->CR2 |= I2C_CR2_ITERREN | I2C_CR2_ITEVTEN;
  i2cp->id_i2c->CR1 |= I2C_CR1_START | I2C_CR1_ACK;
}


/**
 * @brief Transmits data via the I2C bus as master.
 *
 * @details Number of receiving bytes must be 0 or more than 1 because of stm32
 *          hardware restrictions.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] slave_addr  slave device address
 * @param[in] txbuf       pointer to the transmit buffer
 * @param[in] txbytes     number of bytes to be transmitted
 * @param[in] rxbuf       pointer to the receive buffer
 * @param[in] rxbytes     number of bytes to be received
 */
void i2c_lld_master_transmit(I2CDriver *i2cp, uint8_t slave_addr,
                             uint8_t *txbuf, size_t txbytes,
                             uint8_t *rxbuf, size_t rxbytes){

  uint32_t mode = 0;

  chDbgCheck(((rxbytes == 0) || ((rxbytes > 1) && (rxbuf != NULL))),
      "i2cMasterTransmit");

  /* init driver fields */
  i2cp->slave_addr = (slave_addr << 1) & 0x00FE;         /* LSB = 0 -> write */
  i2cp->txbytes = txbytes;
  i2cp->rxbytes = rxbytes;
  i2cp->txbuf = txbuf;
  i2cp->rxbuf = rxbuf;
  i2cp->errors = 0;

  mode = STM32_DMA_CR_DIR_M2P;
  // TODO: DMA error handling
  dmaStreamSetMemory0(i2cp->dmatx, txbuf);
  dmaStreamSetTransactionSize(i2cp->dmatx, txbytes);
  dmaStreamSetMode(i2cp->dmatx, ((i2cp->dmamode) | mode));

  /* wait stop bit from previouse transaction*/
  while(i2cp->id_i2c->CR1 & I2C_CR1_STOP)
    ;

  i2cp->id_i2c->CR2 |= I2C_CR2_ITERREN | I2C_CR2_ITEVTEN;
  i2cp->id_i2c->CR1 |= I2C_CR1_START;
}


/**
 * @brief Set clock speed.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_set_clock(I2CDriver *i2cp) {
  volatile uint16_t regCCR, regCR2, freq, clock_div;
  volatile uint16_t pe_bit_saved;
  int32_t clock_speed = i2cp->id_config->clock_speed;
  i2cdutycycle_t duty = i2cp->id_config->duty_cycle;

  chDbgCheck((i2cp != NULL) && (clock_speed > 0) && (clock_speed <= 4000000),
             "i2c_lld_set_clock");

  /**************************************************************************
   * CR2 Configuration
   */
  regCR2 = i2cp->id_i2c->CR2;                   /* Get the I2Cx CR2 value */
  regCR2 &= (uint16_t)~I2C_CR2_FREQ;            /* Clear frequency FREQ[5:0] bits */
  freq = (uint16_t)(STM32_PCLK1 / 1000000);     /* Set frequency bits depending on pclk1 value */
#if   defined(STM32F4XX)
  chDbgCheck((freq >= 2) && (freq <= 42),
                "i2c_lld_set_clock() : Peripheral clock freq. out of range");
#elif defined(STM32L1XX_MD)
  chDbgCheck((freq >= 2) && (freq <= 32),
                "i2c_lld_set_clock() : Peripheral clock freq. out of range");
#elif defined(STM32F2XX)
  chDbgCheck((freq >= 2) && (freq <= 30),
                "i2c_lld_set_clock() : Peripheral clock freq. out of range");

#elif defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) || \
      defined(STM32F10X_HD_VL)
  chDbgCheck((freq >= 2) && (freq <= 24),
                  "i2c_lld_set_clock() : Peripheral clock freq. out of range");
#elif defined(STM32F10X_LD) || defined(STM32F10X_MD) ||                     \
      defined(STM32F10X_HD) || defined(STM32F10X_XL) ||                     \
      defined(STM32F10X_CL)
  chDbgCheck((freq >= 2) && (freq <= 36),
              "i2c_lld_set_clock() : Peripheral clock freq. out of range");
#else
#error "unspecified, unsupported or invalid STM32 platform"
#endif
  regCR2 |= freq;
  i2cp->id_i2c->CR2 = regCR2;

  /**************************************************************************
   * CCR Configuration
   */
  pe_bit_saved = (i2cp->id_i2c->CR1 & I2C_CR1_PE);
  i2cp->id_i2c->CR1 &= (uint16_t)~I2C_CR1_PE;                   /* Disable the selected I2C peripheral to configure TRISE */
  regCCR = 0;                                                   /* Clear F/S, DUTY and CCR[11:0] bits */
  clock_div = I2C_CCR_CCR;

  if (clock_speed <= 100000) {                                  /* Configure clock_div in standard mode */
    chDbgAssert(duty == STD_DUTY_CYCLE,
                "i2c_lld_set_clock(), #1",
                "Invalid standard mode duty cycle");
    clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 2));    /* Standard mode clock_div calculate: Tlow/Thigh = 1/1 */
    if (clock_div < 0x04) clock_div = 0x04;                     /* Test if CCR value is under 0x4, and set the minimum allowed value */
    regCCR |= (clock_div & I2C_CCR_CCR);                        /* Set clock_div value for standard mode */
    i2cp->id_i2c->TRISE = freq + 1;                             /* Set Maximum Rise Time for standard mode */
  }
  else if(clock_speed <= 400000) {                              /* Configure clock_div in fast mode */
    chDbgAssert((duty == FAST_DUTY_CYCLE_2) ||
                (duty == FAST_DUTY_CYCLE_16_9),
                "i2c_lld_set_clock(), #2",
                "Invalid fast mode duty cycle");
    if(duty == FAST_DUTY_CYCLE_2) {
      clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 3));  /* Fast mode clock_div calculate: Tlow/Thigh = 2/1 */
    }
    else if(duty == FAST_DUTY_CYCLE_16_9) {
      clock_div = (uint16_t)(STM32_PCLK1 / (clock_speed * 25)); /* Fast mode clock_div calculate: Tlow/Thigh = 16/9 */
      regCCR |= I2C_CCR_DUTY;                                   /* Set DUTY bit */
    }
    if(clock_div < 0x01) clock_div = 0x01;                      /* Test if CCR value is under 0x1, and set the minimum allowed value */
    regCCR |= (I2C_CCR_FS | (clock_div & I2C_CCR_CCR));         /* Set clock_div value and F/S bit for fast mode*/
    i2cp->id_i2c->TRISE = (freq * 300 / 1000) + 1;              /* Set Maximum Rise Time for fast mode */
  }
  chDbgAssert((clock_div <= I2C_CCR_CCR),
      "i2c_lld_set_clock(), #3", "Too low clock clock speed selected");

  i2cp->id_i2c->CCR = regCCR;                                   /* Write to I2Cx CCR */
  i2cp->id_i2c->CR1 |= pe_bit_saved;                            /* restore the I2C peripheral enabled state */
}


/**
 * @brief Set operation mode of I2C hardware.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_set_opmode(I2CDriver *i2cp) {
  i2copmode_t opmode = i2cp->id_config->op_mode;
  uint16_t regCR1;

  regCR1 = i2cp->id_i2c->CR1;             /* Get the I2Cx CR1 value */
  switch(opmode){
  case OPMODE_I2C:
    regCR1 &= (uint16_t)~(I2C_CR1_SMBUS|I2C_CR1_SMBTYPE);
    break;
  case OPMODE_SMBUS_DEVICE:
    regCR1 |= I2C_CR1_SMBUS;
    regCR1 &= (uint16_t)~(I2C_CR1_SMBTYPE);
    break;
  case OPMODE_SMBUS_HOST:
    regCR1 |= (I2C_CR1_SMBUS|I2C_CR1_SMBTYPE);
    break;
  }

  i2cp->id_i2c->CR1 = regCR1;             /* Write to I2Cx CR1 */
}


/**
 * @brief Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  if (i2cp->id_state != I2C_STOP) {  /* If in ready state then disables the I2C clock.*/

  dmaStreamDisable(i2cp->dmatx);
  dmaStreamDisable(i2cp->dmarx);
  dmaStreamClearInterrupt(i2cp->dmatx);
  dmaStreamClearInterrupt(i2cp->dmarx);
  dmaStreamRelease(i2cp->dmatx);
  dmaStreamRelease(i2cp->dmarx);

#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      NVICDisableVector(I2C1_EV_IRQn);
      NVICDisableVector(I2C1_ER_IRQn);
      rccDisableI2C1(FALSE);
    }
#endif

#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      NVICDisableVector(I2C2_EV_IRQn);
      NVICDisableVector(I2C2_ER_IRQn);
      rccDisableI2C2(FALSE);
    }
#endif

#if STM32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {
      NVICDisableVector(I2C3_EV_IRQn);
      NVICDisableVector(I2C3_ER_IRQn);
      rccDisableI2C3(FALSE);
    }
#endif
  }

  i2cp->id_state = I2C_STOP;
}


#endif /* HAL_USE_I2C */
