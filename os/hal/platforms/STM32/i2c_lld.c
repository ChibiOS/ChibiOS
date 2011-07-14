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
 * From RM0008.pdf
 *
 * Note:
 * When the STOP, START or PEC bit is set, the software must NOT perform
 * any write access to I2C_CR1 before this bit is cleared by hardware.
 * Otherwise there is a risk of setting a second STOP, START or PEC request.
 */


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
/* Driver local variables.                                                   */
/*===========================================================================*/

/* Debugging variables */
#if CH_DBG_ENABLE_ASSERTS
static volatile uint16_t dbgSR1 = 0;
static volatile uint16_t dbgSR2 = 0;
static volatile uint16_t dbgCR1 = 0;
static volatile uint16_t dbgCR2 = 0;

static uint32_t polling_time_worst = 0;
static uint32_t polling_time_begin = 0;
static uint32_t polling_time_delta = 0;

#endif /* CH_DBG_ENABLE_ASSERTS */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if STM32_I2C_USE_POLLING_WAIT
#else
VirtualTimer i2c_waiting_vt;
#endif /* STM32_I2C_USE_POLLING_WAIT */


/* defines for convenience purpose */
#define txBuffp (i2cp->txbuff_p)
#define rxBuffp (i2cp->rxbuff_p)

/**
 * @brief   Function for I2C debugging purpose.
 * @note    Internal use only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#if CH_DBG_ENABLE_ASSERTS
void _i2c_unhandled_case(I2CDriver *i2cp){
  dbgCR1 = i2cp->id_i2c->CR1;
  dbgCR2 = i2cp->id_i2c->CR2;
  chDbgAssert((dbgSR1 + dbgSR2) == 0,
        "i2c_serve_event_interrupt(), #1",
        "unhandled case");
}
#else
#define _i2c_unhandled_case(i2cp)
#endif /* CH_DBG_ENABLE_ASSERTS */

/**
 * @brief   Return the last event value from I2C status registers.
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
 * @brief   Handle the flags/interrupts.
 * @note    Internal use only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void _i2c_ev6_master_rec_mode_selected(I2CDriver *i2cp){
  I2C_TypeDef *dp = i2cp->id_i2c;

  switch(i2cp->flags & EV6_SUBEV_MASK) {

  case I2C_EV6_3_MASTER_REC_1BTR_MODE_SELECTED: /* only an single byte to receive */
    dp->CR1 &= (uint16_t)~I2C_CR1_ACK;          /* Clear ACK */
    dp->CR1 |= I2C_CR1_STOP;                    /* Program the STOP */
    break;

  case I2C_EV6_1_MASTER_REC_2BTR_MODE_SELECTED: /* only two bytes to receive */
    dp->CR1 &= (uint16_t)~I2C_CR1_ACK;          /* Clear ACK */
    dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;      /* Disable the ITBUF in order to have only the BTF interrupt */
    break;

  default: /* more than 2 bytes to receive */
    break;
  }
}

/**
 * @brief   Handle cases of 2 or 3 bytes receiving.
 * @note    Internal use only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void _i2c_ev7_master_rec_byte_qued(I2CDriver *i2cp){
  I2C_TypeDef *dp = i2cp->id_i2c;

  switch(i2cp->flags & EV7_SUBEV_MASK) {

  case I2C_EV7_2_MASTER_REC_3BYTES_TO_PROCESS:
    /* Only for case of three bytes to be received.
     * DataN-2 and DataN-1 already received. */
    dp->CR1 &= (uint16_t)~I2C_CR1_ACK;  /* Clear ACK */
    *rxBuffp = dp->DR;                  /* Read the DataN-2. This clear the RXE & BFT flags and launch the DataN exception in the shift register (ending the SCL stretch) */
    rxBuffp++;
    chSysLockFromIsr();
    dp->CR1 |= I2C_CR1_STOP;            /* Program the STOP */
    *rxBuffp = dp->DR;                  /* Read the DataN-1 */
    chSysUnlockFromIsr();
    rxBuffp++;
    i2cp->rxbytes -= 2;                 /* Decrement the number of readed bytes */
    i2cp->flags = 0;
    dp->CR2 |= I2C_CR2_ITBUFEN;         /* ready for read DataN. Enable interrupt for next (and last) RxNE event*/
    break;

  case I2C_EV7_3_MASTER_REC_2BYTES_TO_PROCESS:
    /* only for case of two bytes to be received
     * DataN-1 and DataN are received */
    dp->CR2 &= (uint16_t)~I2C_CR2_ITEVTEN;
    dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
    chSysLockFromIsr();
    dp->CR1 |= I2C_CR1_STOP;                    /* Program the STOP */
    *rxBuffp = dp->DR;                          /* Read the DataN-1*/
    rxBuffp++;
    *rxBuffp = dp->DR;                          /* Read the DataN*/
    chSysUnlockFromIsr();
    i2cp->rxbytes = 0;
    i2cp->flags = 0;
    _i2c_isr_code(i2cp, i2cp->id_slave_config); /* Portable I2C ISR code defined in the high level driver. */
    break;

  case I2C_FLG_MASTER_RECEIVER:
    /* Some times in hi load scenarions it is possible to "miss" interrupt
     * because STM32 I2C has OR'ed interrupt sources. This case handle that
     * scenario. */
    if (i2cp->rxbytes > 3){
      *rxBuffp = dp->DR;
      rxBuffp++;
      (i2cp->rxbytes)--;
    }
    else
      _i2c_unhandled_case(i2cp);
    break;

  default:
    _i2c_unhandled_case(i2cp);
    break;
  }
}

/**
 * @brief   Main I2C interrupt handler.
 * @note    Internal use only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_serve_event_interrupt(I2CDriver *i2cp) {
  I2C_TypeDef *dp = i2cp->id_i2c;

  switch(i2c_get_event(i2cp)) {

  case I2C_EV5_MASTER_MODE_SELECT:
    i2cp->flags &= ~I2C_FLG_HEADER_SENT;
    dp->DR = i2cp->slave_addr1;
    break;

  case I2C_EV9_MASTER_ADDR_10BIT:
    if(i2cp->flags & I2C_FLG_MASTER_RECEIVER) {
      i2cp->slave_addr1 |= 0x01;
      i2cp->flags |= I2C_FLG_HEADER_SENT;
    }
    dp->DR = i2cp->slave_addr2;
    break;

  /**************************************************************************
   * Master Transmitter part
   */
  case I2C_EV6_MASTER_TRA_MODE_SELECTED:
    if(i2cp->flags & I2C_FLG_HEADER_SENT){
      dp->CR1 |= I2C_CR1_START;             /* re-send the start in 10-Bit address mode */
      break;
    }
    txBuffp = (uint8_t*)i2cp->txbuf;        /* Initialize the transmit buffer pointer */
    i2cp->txbytes--;
    if(i2cp->txbytes == 0) {                /* If no further data to be sent, disable the I2C ITBUF in order to not have a TxE interrupt */
      dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
    }
    dp->DR = *txBuffp;                      /* EV8_1 write the first data */
    txBuffp++;
    break;

  case I2C_EV8_MASTER_BYTE_TRANSMITTING:
    if(i2cp->txbytes > 0) {
      i2cp->txbytes--;
      if(i2cp->txbytes == 0) {              /* If no further data to be sent, disable the ITBUF in order to not have a TxE interrupt */
        dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
      }
      dp->DR = *txBuffp;
      txBuffp++;
    }
    break;

  case I2C_EV8_2_MASTER_BYTE_TRANSMITTED:
    dp->CR2 &= (uint16_t)~I2C_CR2_ITEVTEN;        /* Disable ITEVT In order to not have again a BTF IT */
    if (i2cp->rxbytes == 0){                      /* if nothing to read then generate stop */
      dp->CR1 |= I2C_CR1_STOP;
      _i2c_isr_code(i2cp, i2cp->id_slave_config); /* Portable I2C ISR code defined in the high level driver. */
    }
    else{                                         /* start reading operation */
      i2c_lld_master_transceive(i2cp);
    }
    break;

  /**************************************************************************
   * Master Receiver part
   */
  case I2C_EV6_MASTER_REC_MODE_SELECTED:
    _i2c_ev6_master_rec_mode_selected(i2cp);
    rxBuffp = i2cp->rxbuf;                        /* Initialize receive buffer pointer */
    break;

  case I2C_EV7_MASTER_REC_BYTE_RECEIVED:
    if(i2cp->rxbytes > 3) {
      *rxBuffp = dp->DR;                          /* Read the data register */
      rxBuffp++;
      i2cp->rxbytes--;
      if(i2cp->rxbytes == 3){                     /* Disable the ITBUF in order to have only the BTF interrupt */
        dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
        i2cp->flags |= I2C_FLG_3BTR;
      }
    }
    else if (i2cp->rxbytes == 3){                 /* Disable the ITBUF in order to have only the BTF interrupt */
      dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
      i2cp->flags |= I2C_FLG_3BTR;
    }
    break;

  case I2C_EV7_MASTER_REC_BYTE_QUEUED:
    _i2c_ev7_master_rec_byte_qued(i2cp);
    break;

  default:                                        /* only 1 byte must to be read to complete trasfer. Stop already sent to bus. */
    chDbgAssert((i2cp->rxbytes) == 1,
          "i2c_serve_event_interrupt(), #1",
          "more than 1 byte to be received");
    *rxBuffp = dp->DR;                            /* Read the data register */
    i2cp->rxbytes = 0;
    dp->CR2 &= (uint16_t)~I2C_CR2_ITEVTEN;        /* disable interrupts */
    dp->CR2 &= (uint16_t)~I2C_CR2_ITBUFEN;
    _i2c_isr_code(i2cp, i2cp->id_slave_config);   /* Portable I2C ISR code defined in the high level driver.*/
    break;
  }
}


static void i2c_serve_error_interrupt(I2CDriver *i2cp) {
  i2cflags_t flags;
  I2C_TypeDef *reg;

  reg = i2cp->id_i2c;
  flags = I2CD_NO_ERROR;

  if(reg->SR1 & I2C_SR1_BERR) {                /* Bus error */
    reg->SR1 &= ~I2C_SR1_BERR;
    flags |= I2CD_BUS_ERROR;
  }
  if(reg->SR1 & I2C_SR1_ARLO) {                /* Arbitration lost */
    reg->SR1 &= ~I2C_SR1_ARLO;
    flags |= I2CD_ARBITRATION_LOST;
  }
  if(reg->SR1 & I2C_SR1_AF) {                  /* Acknowledge fail */
    reg->SR1 &= ~I2C_SR1_AF;
    reg->CR1 |= I2C_CR1_STOP;                  /* setting stop bit */
    while(i2cp->id_i2c->CR1 & I2C_CR1_STOP)
      ;
    flags |= I2CD_ACK_FAILURE;
  }
  if(reg->SR1 & I2C_SR1_OVR) {                 /* Overrun */
    reg->SR1 &= ~I2C_SR1_OVR;
    flags |= I2CD_OVERRUN;
  }
  if(reg->SR1 & I2C_SR1_PECERR) {              /* PEC error */
    reg->SR1 &= ~I2C_SR1_PECERR;
    flags |= I2CD_PEC_ERROR;
  }
  if(reg->SR1 & I2C_SR1_TIMEOUT) {             /* SMBus Timeout */
    reg->SR1 &= ~I2C_SR1_TIMEOUT;
    flags |= I2CD_TIMEOUT;
  }
  if(reg->SR1 & I2C_SR1_SMBALERT) {            /* SMBus alert */
    reg->SR1 &= ~I2C_SR1_SMBALERT;
    flags |= I2CD_SMB_ALERT;
  }

  if(flags != I2CD_NO_ERROR) {                /* send communication end signal */
    chSysLockFromIsr();
    i2cAddFlagsI(i2cp, flags);
    chSysUnlockFromIsr();
    _i2c_isr_err_code(i2cp, i2cp->id_slave_config);
  }
}


#if STM32_I2C_USE_I2C1 || defined(__DOXYGEN__)
/**
 * @brief I2C1 event interrupt handler.
 */
CH_IRQ_HANDLER(VectorBC) {

  CH_IRQ_PROLOGUE();
  i2c_serve_event_interrupt(&I2CD1);
  CH_IRQ_EPILOGUE();
}

/**
 * @brief I2C1 error interrupt handler.
 */
CH_IRQ_HANDLER(VectorC0) {

  CH_IRQ_PROLOGUE();
  i2c_serve_error_interrupt(&I2CD1);
  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_I2C_USE_I2C2 || defined(__DOXYGEN__)
/**
 * @brief I2C2 event interrupt handler.
 */
CH_IRQ_HANDLER(VectorC4) {

  CH_IRQ_PROLOGUE();
  i2c_serve_event_interrupt(&I2CD2);
  CH_IRQ_EPILOGUE();
}

/**
 * @brief I2C2 error interrupt handler.
 */
CH_IRQ_HANDLER(VectorC8) {

  CH_IRQ_PROLOGUE();
  i2c_serve_error_interrupt(&I2CD2);
  CH_IRQ_EPILOGUE();
}
#endif

/**
 * @brief Low level I2C driver initialization.
 */
void i2c_lld_init(void) {

#if STM32_I2C_USE_I2C1
  RCC->APB1RSTR     = RCC_APB1RSTR_I2C1RST; /* reset I2C 1 */
  RCC->APB1RSTR     = 0;
  i2cObjectInit(&I2CD1);
  I2CD1.id_i2c     = I2C1;
#endif

#if STM32_I2C_USE_I2C2
  RCC->APB1RSTR     = RCC_APB1RSTR_I2C2RST; /* reset I2C 2 */
  RCC->APB1RSTR     = 0;
  i2cObjectInit(&I2CD2);
  I2CD2.id_i2c     = I2C2;
#endif
}

/**
 * @brief Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_start(I2CDriver *i2cp) {
  if (i2cp->id_state == I2C_STOP) {         /* If in stopped state then enables the I2C clock.*/
#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      NVICEnableVector(I2C1_EV_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
      NVICEnableVector(I2C1_ER_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;   /* I2C 1 clock enable */
    }
#endif
#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      NVICEnableVector(I2C2_EV_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
      NVICEnableVector(I2C2_ER_IRQn,
          CORTEX_PRIORITY_MASK(STM32_I2C_I2C1_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;   /* I2C 2 clock enable */
    }
#endif
  }

  i2cp->id_i2c->CR1 = I2C_CR1_SWRST;        /* reset i2c peripherial */
  i2cp->id_i2c->CR1 = 0;
  i2c_lld_set_clock(i2cp);
  i2c_lld_set_opmode(i2cp);
  i2cp->id_i2c->CR1 |= 1;                   /* enable interface */
}

void i2c_lld_reset(I2CDriver *i2cp){
  chDbgCheck((i2cp->id_state == I2C_STOP)||(i2cp->id_state == I2C_READY),
             "i2c_lld_reset: invalid state");

  RCC->APB1RSTR     = RCC_APB1RSTR_I2C1RST; /* reset I2C 1 */
  RCC->APB1RSTR     = 0;
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
  chDbgCheck((freq >= 2) && (freq <= 36),
              "i2c_lld_set_clock() : Peripheral clock freq. out of range");
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
 * @brief Set own address.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_set_own_address(I2CDriver *i2cp) {
  /* TODO: dual address mode */

  i2cp->id_i2c->OAR1 |= 1 << 14;

  if (&(i2cp->id_config->own_addr_10) == NULL){       /* only 7-bit address */
    i2cp->id_i2c->OAR1 &= (~I2C_OAR1_ADDMODE);
    i2cp->id_i2c->OAR1 |= i2cp->id_config->own_addr_7 << 1;
  }
  else {
    chDbgAssert((i2cp->id_config->own_addr_10 < 1024),
        "i2c_lld_set_own_address(), #1", "10-bit address longer then 10 bit")
    i2cp->id_i2c->OAR1 |= I2C_OAR1_ADDMODE;
    i2cp->id_i2c->OAR1 |= i2cp->id_config->own_addr_10;
  }
}


/**
 * @brief Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_stop(I2CDriver *i2cp) {
  if (i2cp->id_state == I2C_READY) {  /* If in ready state then disables the I2C clock.*/
#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      NVICDisableVector(I2C1_EV_IRQn);
      NVICDisableVector(I2C1_ER_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
    }
#endif
#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      NVICDisableVector(I2C2_EV_IRQn);
      NVICDisableVector(I2C2_ER_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;
    }
#endif
  }

  i2cp->id_state = I2C_STOP;
}

/**
 * @brief Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] slave_addr  Slave device address. Bits 0-9 contain slave
 *                        device address. Bit 15 must be set to 1 if 10-bit
 *                        addressing modes used. Otherwise	keep it cleared.
 *                        Bits 10-14 unused.
 * @param[in] txbuf       pointer to the transmit buffer
 * @param[in] txbytes     number of bytes to be transmitted
 * @param[in] rxbuf       pointer to the receive buffer
 * @param[in] rxbytes     number of bytes to be received
 */
void i2c_lld_master_transmit(I2CDriver *i2cp, uint16_t slave_addr,
    uint8_t *txbuf, size_t txbytes, uint8_t *rxbuf, size_t rxbytes) {

  i2cp->slave_addr = slave_addr;
  i2cp->txbytes = txbytes;
  i2cp->rxbytes = rxbytes;
  i2cp->txbuf = txbuf;
  i2cp->rxbuf = rxbuf;

  if(slave_addr & 0x8000){                                    /* 10-bit mode used */
    i2cp->slave_addr1 = ((slave_addr >>7) & 0x0006);          /* add the two msb of 10-bit address to the header */
    i2cp->slave_addr1 |= 0xF0;                                /* add the header bits with LSB = 0 -> write */
    i2cp->slave_addr2 = slave_addr & 0x00FF;                  /* the remaining 8 bit of 10-bit address */
  }
  else{
    i2cp->slave_addr1 = ((slave_addr <<1) & 0x00FE);          /* LSB = 0 -> write */
  }

  i2cp->flags = 0;
  i2cp->errors = 0;

  #if CH_DBG_ENABLE_ASSERTS
    polling_time_begin = PWMD4.tim->CNT;
  #endif
  while(i2cp->id_i2c->CR1 & I2C_CR1_STOP)
    ;
  #if CH_DBG_ENABLE_ASSERTS
    polling_time_delta = PWMD4.tim->CNT - polling_time_begin;
    if (polling_time_delta > polling_time_worst)
      polling_time_worst = polling_time_delta;
  #endif

  i2cp->id_i2c->CR1 &= ~I2C_CR1_POS;
  i2cp->id_i2c->CR1 |= I2C_CR1_START;                         /* send start bit */
  i2cp->id_i2c->CR2 |= (I2C_CR2_ITERREN|I2C_CR2_ITEVTEN|I2C_CR2_ITBUFEN); /* enable ERR, EVT & BUF ITs */
}


/**
 * @brief Receives data from the I2C bus.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] slave_addr  Slave device address. Bits 0-9 contain slave
 *                        device address. Bit 15 must be set to 1 if 10-bit
 *                        addressing modes used. Otherwise	keep it cleared.
 *                        Bits 10-14 unused.
 * @param[in] rxbuf       pointer to the receive buffer
 * @param[in] rxbytes     number of bytes to be received
 */
void i2c_lld_master_receive(I2CDriver *i2cp, uint16_t slave_addr,
    uint8_t *rxbuf, size_t rxbytes){

  chDbgAssert((i2cp->id_i2c->SR1 + i2cp->id_i2c->SR2) == 0,
        "i2c_lld_master_receive(), #1",
        "some interrupt sources not clear");

	i2cp->slave_addr = slave_addr;
	i2cp->rxbytes = rxbytes;
	i2cp->rxbuf = rxbuf;

  if(slave_addr & 0x8000){                            /* 10-bit mode used */
    i2cp->slave_addr1 = ((slave_addr >>7) & 0x0006);  /* add the two msb of 10-bit address to the header */
    i2cp->slave_addr1 |= 0xF0;                        /* add the header bits (the LSB -> 1 will be add to second */
    i2cp->slave_addr2 = slave_addr & 0x00FF;          /* the remaining 8 bit of 10-bit address */
  }
  else{
    i2cp->slave_addr1 = ((slave_addr <<1) | 0x01);    /* LSB = 1 -> receive */
  }

  i2cp->flags = I2C_FLG_MASTER_RECEIVER;
  i2cp->errors = 0;

  #if CH_DBG_ENABLE_ASSERTS
    polling_time_begin = PWMD4.tim->CNT;
  #endif
  while(i2cp->id_i2c->CR1 & I2C_CR1_STOP)
    ;
  #if CH_DBG_ENABLE_ASSERTS
    polling_time_delta = PWMD4.tim->CNT - polling_time_begin;
    if (polling_time_delta > polling_time_worst)
      polling_time_worst = polling_time_delta;
  #endif

  i2cp->id_i2c->CR1 |= I2C_CR1_ACK;                   /* acknowledge returned */
  i2cp->id_i2c->CR1 &= ~I2C_CR1_POS;

  if(i2cp->rxbytes == 1) {                            /* Only one byte to be received */
    i2cp->flags |= I2C_FLG_1BTR;
  }
  else if(i2cp->rxbytes == 2) {                       /* Only two bytes to be received */
    i2cp->flags |= I2C_FLG_2BTR;
    i2cp->id_i2c->CR1 |= I2C_CR1_POS;                 /* Acknowledge Position */
  }

  i2cp->id_i2c->CR1 |= I2C_CR1_START;                 /* send start bit */
  i2cp->id_i2c->CR2 |= (I2C_CR2_ITERREN|I2C_CR2_ITEVTEN|I2C_CR2_ITBUFEN); /* enable ERR, EVT & BUF ITs */
}


/**
 * @brief Realize read-though-write behavior.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_master_transceive(I2CDriver *i2cp){

  chDbgAssert((i2cp != NULL) && (i2cp->slave_addr1 != 0) &&\
      (i2cp->rxbytes > 0) && (i2cp->rxbuf != NULL),
      "i2c_lld_master_transceive(), #1",
      "");

  i2cp->flags = I2C_FLG_MASTER_RECEIVER;
  i2cp->errors = 0;
  i2cp->id_i2c->CR1 |= I2C_CR1_ACK;                       /* acknowledge returned */
  i2cp->id_i2c->CR1 &= ~I2C_CR1_POS;

  if(i2cp->slave_addr & 0x8000){                          /* 10-bit mode used */
    i2cp->slave_addr1 = ((i2cp->slave_addr >>7) & 0x0006);/* add the two msb of 10-bit address to the header */
    i2cp->slave_addr1 |= 0xF0;                            /* add the header bits (the LSB -> 1 will be add to second */
    i2cp->slave_addr2 = i2cp->slave_addr & 0x00FF;        /* the remaining 8 bit of 10-bit address */
  }
  else{
    i2cp->slave_addr1 |= 0x01;
  }

  if(i2cp->rxbytes == 1) {                                /* Only one byte to be received */
    i2cp->flags |= I2C_FLG_1BTR;
  }
  else if(i2cp->rxbytes == 2) {                           /* Only two bytes to be received */
    i2cp->flags |= I2C_FLG_2BTR;
    i2cp->id_i2c->CR1 |= I2C_CR1_POS;                     /* Acknowledge Position */
  }

  i2cp->id_i2c->CR1 |= I2C_CR1_START;                     /* send start bit */

  uint32_t timeout = I2C_START_TIMEOUT;
  while((i2cp->id_i2c->CR1 & I2C_CR1_START) && timeout--)
    ;
  chDbgAssert(timeout <= I2C_START_TIMEOUT,
      "i2c_lld_master_receive(), #1", "time is out");

  i2cp->id_i2c->CR2 |= (I2C_CR2_ITERREN|I2C_CR2_ITEVTEN|I2C_CR2_ITBUFEN);  /* enable ERR, EVT & BUF ITs */
}

#undef rxBuffp
#undef txBuffp

#endif /* HAL_USE_I2C */
