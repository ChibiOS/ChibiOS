/**
 * Maxim ADC has not so suitable default settings after startup.
 * So we will create init function to tune this ADC.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "max1236.h"

// Data buffers
static i2cblock_t max1236_rx_data[MAX1236_RX_DEPTH];
static i2cblock_t max1236_tx_data[MAX1236_TX_DEPTH];

/* Error trap */
static void i2c_max1236_error_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}


/* This callback raise up when transfer finished */
static void i2c_max1236_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  uint16_t ch1 = 0;
  uint16_t ch2 = 0;
  uint16_t ch3 = 0;
  uint16_t ch4 = 0;

  /* send stop */
  i2cMasterStop(i2cp);

  /* unlock bus */
  i2cReleaseBus(&I2CD2);

  /* get ADC data */
  ch1 = ((i2cscfg->rxbuf[0] & 0xF) << 8) + i2cscfg->rxbuf[1];
  ch2 = ((i2cscfg->rxbuf[2] & 0xF) << 8) + i2cscfg->rxbuf[3];
  ch3 = ((i2cscfg->rxbuf[4] & 0xF) << 8) + i2cscfg->rxbuf[5];
  ch4 = ((i2cscfg->rxbuf[6] & 0xF) << 8) + i2cscfg->rxbuf[7];
}


// ADC maxim MAX1236 config
static I2CSlaveConfig max1236 = {
  NULL, // first set to NULL. We will set this pointer to the function later.
  i2c_max1236_error_cb,
  max1236_rx_data,
  MAX1236_RX_DEPTH,
  0,
  0,
  max1236_tx_data,
  MAX1236_TX_DEPTH,
  0,
  0,
  0b0110100,
  FALSE,
};


/**
 * Initilization routine. See datasheet on page 13 to understand
 * how to initialize ADC.
 */
void init_max1236(void){
  /* lock bus */
  i2cAcquireBus(&I2CD2);

  /* this data we must send to IC to setup ADC settings */
  max1236.txbufhead = 0;
  max1236.txbytes = 2; // total 2 bytes to be sent
  max1236.txbuf[0] = 0b10000011; // config register content. Consult datasheet
  max1236.txbuf[1] = 0b00000111; // config register content. Consult datasheet

  // transmit out 2 bytes
  i2cMasterTransmit(&I2CD2, &max1236);
  while(I2CD2.id_state != I2C_READY) // wait
      chThdSleepMilliseconds(1);

  /* now add pointer to callback function */
  max1236.id_callback = i2c_max1236_cb;

  /*clear transmitting structures */
  max1236.txbytes = 0;
  max1236.txbufhead = 0;

  /* unlock bus */
  i2cReleaseBus(&I2CD2);
}


/* Now simply read 8 bytes to get all 4 ADC channels */
void read_max1236(void){
  /* tune receive buffer */
  max1236.rxbufhead = 0;
  max1236.rxbytes = 8;

  /* lock bus */
  i2cAcquireBus(&I2CD2);

  /* start reading */
  i2cMasterReceive(&I2CD2, &max1236);
}
