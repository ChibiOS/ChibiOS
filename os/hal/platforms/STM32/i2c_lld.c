/**
 * @file STM32/i2c_lld.c
 * @brief STM32 I2C subsystem low level driver source. Slave mode not implemented.
 * @addtogroup STM32_I2C
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "i2c_lld.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

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

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   TODO: Status bits translation.
 *
 * @param[in] sr        USART SR register value
 *
 * @return  The error flags.
 */
static i2cflags_t translate_i2c_errors(uint16_t sr) {
  i2cflags_t sts = 0;

  if (sr & USART_SR_ORE)
    sts |= UART_OVERRUN_ERROR;
  if (sr & USART_SR_PE)
    sts |= UART_PARITY_ERROR;
  if (sr & USART_SR_FE)
    sts |= UART_FRAMING_ERROR;
  if (sr & USART_SR_NE)
    sts |= UART_NOISE_ERROR;
  if (sr & USART_SR_LBD)
    sts |= UART_BREAK_DETECTED;
  return sts;
}


static void i2c_serve_error_interrupt(I2CDriver *i2cp) {
  // TODO:remove this stub and write normal handler
  // this is simply trap for errors
  while TRUE{
    translate_i2c_errors(i2cp->id_i2c->SR1);
  }
}

/* This function handle all regular interrupt conditions
 * TODO: 10 bit address handling here
 */
static void i2c_serve_event_interrupt(I2CDriver *i2cp) {
  int i = 0;
  int n = 0;
  int m = 0;

  if ((i2cp->id_state == I2C_READY) && (i2cp->id_i2c->SR1 & I2C_SR1_SB)){// start bit sent
    //i = i2cp->id_i2c->SR1;
    i2cp->id_state = I2C_MACTIVE;
    i2cp->id_i2c->DR = (i2cp->id_slave_config->slave_addr1 << 1) |
                        i2cp->id_slave_config->rw_bit; // write slave address in DR
    return;
  }

  // now "wait" interrupt with ADDR flag
  if ((i2cp->id_state == I2C_MACTIVE) && (i2cp->id_i2c->SR1 & I2C_SR1_ADDR)){// address successfully sent
    if(i2cp->id_i2c->SR2 & I2C_SR2_TRA){
      i2c_lld_txbyte(i2cp); // send first byte
      i2cp->id_state = I2C_MTRANSMIT; // change state
      return;
    }
    else {
      /* In order to generate the non-acknowledge pulse after the last received
       * data byte, the ACK bit must be cleared just after reading the second
       * last data byte (after second last RxNE event).
       */
      if (i2cp->id_slave_config->rxbytes > 1)
        i2cp->id_i2c->CR1 |= I2C_CR1_ACK; // set ACK bit
      i2cp->id_state = I2C_MRECEIVE; // change status
      return;
    }
  }

  // transmitting bytes one by one
  if ((i2cp->id_state == I2C_MTRANSMIT) && (i2cp->id_i2c->SR1 & I2C_SR1_TXE)){
    if (i2c_lld_txbyte(i2cp))
      i2cp->id_state = I2C_MWAIT_TF; // last byte written
    return;
  }

  //receiving bytes one by one
  if ((i2cp->id_state == I2C_MRECEIVE) && (i2cp->id_i2c->SR1 & I2C_SR1_RXNE)){
//    i = i2cp->id_i2c->SR1;
//    n = i2cp->id_i2c->SR2;
    if (i2c_lld_rxbyte(i2cp))
      i2cp->id_state = I2C_MWAIT_TF; // last byte read
//    i = i2cp->id_i2c->SR1;
//    n = i2cp->id_i2c->SR2;
    return;
  }

  // "wait" BTF bit in status register
//  if ((i2cp->id_state == I2C_MWAIT_TF) && (i2cp->id_i2c->SR1 & I2C_SR1_BTF)){
  if ((i2cp->id_state == I2C_MWAIT_TF) && (i2cp->id_i2c->SR1 & I2C_SR1_BTF)){
    chSysLockFromIsr();
    i2cp->id_i2c->CR2 &= (~I2C_CR2_ITEVTEN); // disable BTF interrupt
    i2cp->id_slave_config->id_callback(i2cp, i2cp->id_slave_config);

    i = i2cp->id_i2c->SR1;
    n = i2cp->id_i2c->SR2;
    m = i2cp->id_i2c->CR1;

    chSysUnlockFromIsr();
    return;
  }
  else{ // trap
    i = i2cp->id_i2c->SR1;
    n = i2cp->id_i2c->SR2;
    m = i2cp->id_i2c->CR1;
    return;
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
  RCC->APB1RSTR     = RCC_APB1RSTR_I2C1RST;           // reset I2C 1
  RCC->APB1RSTR     = 0;
  i2cObjectInit(&I2CD1);
  I2CD1.id_i2c     = I2C1;
#endif

#if STM32_I2C_USE_I2C2
  RCC->APB1RSTR     = RCC_APB1RSTR_I2C2RST;           // reset I2C 2
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

  /* If in stopped state then enables the I2C clock.*/
  if (i2cp->id_state == I2C_STOP) {
#if STM32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      NVICEnableVector(I2C1_EV_IRQn, STM32_I2C_I2C1_IRQ_PRIORITY);
      NVICEnableVector(I2C1_ER_IRQn, STM32_I2C_I2C1_IRQ_PRIORITY);
      RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C 1 clock enable
    }
#endif
#if STM32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      NVICEnableVector(I2C2_EV_IRQn, STM32_I2C2_IRQ_PRIORITY);
      NVICEnableVector(I2C2_ER_IRQn, STM32_I2C2_IRQ_PRIORITY);
      RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // I2C 2 clock enable
    }
#endif
  }

  /* I2C setup.*/
  i2cp->id_i2c->CR1 = I2C_CR1_SWRST; // reset i2c peripherial
  i2cp->id_i2c->CR1 = 0;

  i2cp->id_i2c->CR1 = i2cp->id_config->i2cc_cr1;
  i2cp->id_i2c->CR2 = i2cp->id_config->i2cc_cr2 |
                            I2C_CR2_ITERREN |
                            I2C_CR2_ITEVTEN |
                            I2C_CR2_ITBUFEN |
                            36; //TODO: replace this by macro calculation
  /* TODO:
   * 1. macro timing calculator
   * 2. parameter checker
   * 3. definitions in halconf.h: i2c-freq, i2c_mode, etc
   * 4. trise time calculator/checker
   */
  i2cp->id_i2c->CCR = i2cp->id_config->i2cc_ccr | 180;
  i2cp->id_i2c->TRISE = i2cp->id_config->i2cc_trise | 37;
  i2cp->id_i2c->CR1 |= 1; // enable interface
}

/**
 * @brief Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  /* If in ready state then disables the I2C clock.*/
  if (i2cp->id_state == I2C_READY) {
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


/* helper function, not API
 * write bytes in DR register
 * return TRUE if last byte written
 */
bool_t i2c_lld_txbyte(I2CDriver *i2cp) {
#define _txbufhead (i2cp->id_slave_config->txbufhead)
#define _txbytes (i2cp->id_slave_config->txbytes)
#define _txbuf (i2cp->id_slave_config->txbuf)

  if (_txbufhead < _txbytes){
    /* disable interrupt to avoid jumping to ISR */
    if ( _txbytes - _txbufhead == 1)
      i2cp->id_i2c->CR2 &= (~I2C_CR2_ITBUFEN);
    i2cp->id_i2c->DR = _txbuf[_txbufhead];
    (_txbufhead)++;
    return(FALSE);
  }
  _txbufhead = 0;
  return(TRUE); // last byte written
#undef _txbufhead
#undef _txbytes
#undef _txbuf
}


/* helper function, not API
 * read bytes from DR register
 * return TRUE if last byte read
 */
bool_t i2c_lld_rxbyte(I2CDriver *i2cp) {
  // temporal variables
#define _rxbuf     (i2cp->id_slave_config->rxbuf)
#define _rxbufhead (i2cp->id_slave_config->rxbufhead)
#define _rxdepth   (i2cp->id_slave_config->rxdepth)
#define _rxbytes   (i2cp->id_slave_config->rxbytes)

  /* In order to generate the non-acknowledge pulse after the last received
   * data byte, the ACK bit must be cleared just after reading the second
   * last data byte (after second last RxNE event).
   */
  if (_rxbufhead < _rxbytes){
    _rxbuf[_rxbufhead] = i2cp->id_i2c->DR;
    if ((_rxbytes - _rxbufhead) <= 2){
      i2cp->id_i2c->CR1 &= (~I2C_CR1_ACK);// clear ACK bit for automatically send NACK
    }
    (_rxbufhead)++;
    return(FALSE);
  }
  /* disable interrupt to avoid jumping to ISR */
  i2cp->id_i2c->CR2 &= (~I2C_CR2_ITBUFEN);

  _rxbuf[_rxbufhead] = i2cp->id_i2c->DR; // read last byte
  _rxbufhead = 0;
  return(TRUE); // last byte read

#undef _rxbuf
#undef _rxbufhead
#undef _rxdepth
#undef _rxbytes
}


void i2c_lld_master_start(I2CDriver *i2cp){
  i2cp->id_i2c->CR1 |= I2C_CR1_START;
  while (i2cp->id_i2c->CR1 & I2C_CR1_START);

  // enable interrupts
  i2cp->id_i2c->CR2 |= I2C_CR2_ITEVTEN;
  i2cp->id_i2c->CR2 |= I2C_CR2_ITBUFEN;
}

void i2c_lld_master_stop(I2CDriver *i2cp){
  i2cp->id_i2c->CR1 |= I2C_CR1_STOP;
  while (i2cp->id_i2c->CR1 & I2C_CR1_STOP);
}


void i2c_lld_master_transmitI(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  //TODO: check txbytes <= sizeof(i2cscfg->txbuf) here, or in hylevel API

  i2cp->id_slave_config = i2cscfg;
  i2cp->id_slave_config->rw_bit = I2C_WRITE;

  // generate start condition. Later transmission goes in background
  i2c_lld_master_start(i2cp);
}

void i2c_lld_master_receiveI(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  //TODO: check txbytes <= sizeof(i2cscfg->txbuf) here, or in hylevel API

  i2cp->id_slave_config = i2cscfg;
  i2cp->id_slave_config->rw_bit = I2C_READ;

  // generate (re)start condition. Later connection goes asynchronously
  i2c_lld_master_start(i2cp);
}



/**
 * @brief Transmits data ever the I2C bus as masteri2cp.
 *
 * @param[in] i2cp          pointer to the @p I2CDriver object
 * @param[in] i2cscfg       pointer to the @p I2CSlaveConfig object
 * @param[in] restart       bool. If TRUE then generate restart condition insted of stop
 */
void i2c_lld_master_transmit(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg, bool_t restart) {
	//TODO: check txbytes <= sizeof(i2cscfg->txbuf) here, or in hylevel API

  int i = 0;

  i2cp->id_slave_config = i2cscfg;
  i2cp->id_slave_config->rw_bit = I2C_WRITE;


  i2cp->id_i2c->CR1 |= I2C_CR1_START; // generate start condition
  while (!(i2cp->id_i2c->SR1 & I2C_SR1_SB)){
    i++; // wait Address sent
  }

  i2cp->id_i2c->DR = (i2cp->id_slave_config->slave_addr1 << 1) | I2C_WRITE; // write slave addres in DR
  while (!(i2cp->id_i2c->SR1 & I2C_SR1_ADDR)){
    i++; // wait Address sent
  }
  i = i2cp->id_i2c->SR2; // TODO: check is it need to read this register for I2C to proper functionality
  i = i2cp->id_i2c->SR1; //i2cp->id_i2c->SR1 &= (~I2C_SR1_ADDR); // clear ADDR bit

  // now write data byte by byte in DR register
  uint32_t n = 0;
  for (n = 0; n < i2cp->id_slave_config->txbytes; n++){
  	i2cp->id_i2c->DR = i2cscfg->txbuf[n];
  	while (!(i2cp->id_i2c->SR1 & I2C_SR1_TXE)){
  	  i++;
  	}
  }

  while (!(i2cp->id_i2c->SR1 & I2C_SR1_BTF)){
    i++;
  }

  if (restart){
    i2cp->id_i2c->CR1 |= I2C_CR1_START; // generate restart condition
    while (!(i2cp->id_i2c->SR1 & I2C_SR1_SB)){
      i++; // wait start bit
    }
  }
  else i2cp->id_i2c->CR1 |= I2C_CR1_STOP; // generate stop condition
}


/**
 * @brief   Receives data from the I2C bus.
 * @details Before receive data from I2C slave you must manually sent them some
 *          control bytes first (refer to you device datasheet).
 *
 * @param[in] i2cp          pointer to the @p I2CDriver object
 * @param[in] i2cscfg       pointer to the @p I2CSlaveConfig object
 */
void i2c_lld_master_receive(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg) {

  chSysLock();

  i2cp->id_slave_config = i2cscfg;

  uint16_t i = 0;
  uint16_t tmp = 0;

  // send slave addres with read-bit
  i2cp->id_i2c->DR = (i2cp->id_slave_config->slave_addr1 << 1) | I2C_READ;
  while (!(i2cp->id_i2c->SR1 & I2C_SR1_ADDR)){
    i++; // wait Address sent
  }
  i = i2cp->id_i2c->SR2; // TODO: check is it need to read this register for I2C to proper functionality
  i = i2cp->id_i2c->SR1; //i2cp->id_i2c->SR1 &= (~I2C_SR1_ADDR); // clear ADDR bit

  // set ACK bit
  i2cp->id_i2c->CR1 |= I2C_CR1_ACK;

  // collect data from slave
  for (i = 0; i < i2cp->id_slave_config->rxbytes; i++){
    if ((i2cp->id_slave_config->rxbytes - i) == 1){ // TODO: is it better <= in place of == ?
        // clear ACK bit for automatically send NACK
        i2cp->id_i2c->CR1 &= (~I2C_CR1_ACK);}
    while (!(i2cp->id_i2c->SR1 & I2C_SR1_RXNE)){
      tmp++;
    }
    i2cp->id_slave_config->rxbuf[i] = i2cp->id_i2c->DR;
  }
  // generate STOP
  i2cp->id_i2c->CR1 |= I2C_CR1_STOP;

  chSysUnlock();
}



#endif // HAL_USE_I2C
