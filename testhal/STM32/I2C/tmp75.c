/**
 * TMP75 is most simple I2C device in our case. It is already useful with
 * default settings after powerup.
 * You only must read 2 sequential bytes from it.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "tmp75.h"


// input buffer
static i2cblock_t tmp75_rx_data[TMP75_RX_DEPTH];
static i2cblock_t tmp75_tx_data[TMP75_TX_DEPTH];

// Simple error trap
static void i2c_tmp75_error_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}

/* This callback raise up when transfer finished */
static void i2c_tmp75_cb(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg){
  int16_t temperature = 0;

  /* Manually send stop signal to the bus. This is important! */
  i2cMasterStop(i2cp);
  /* unlock bus */
  i2cReleaseBus(&I2CD2);

  /* store temperature value */
  temperature = (i2cscfg->rxbuf[0] << 8) + i2cscfg->rxbuf[1];

}

// Fill TMP75 config.
static I2CSlaveConfig tmp75 = {
    i2c_tmp75_cb,
    i2c_tmp75_error_cb,
    tmp75_rx_data,
    TMP75_RX_DEPTH,
    0,
    0,
    tmp75_tx_data,
    TMP75_TX_DEPTH,
    0,
    0,
    0b1001000,
    FALSE,
};

/* This is main function. */
void request_temperature(void){
  tmp75.txbytes = 0;  // set to zero just to be safe

  /* tune receiving buffer */
  tmp75.rxbufhead = 0;// point to beginig of buffer
  tmp75.rxbytes = 2;  // we need read 2 bytes

  /* get exclusive access to the bus */
  i2cAcquireBus(&I2CD2);

  /* start receiving process in background and return */
  i2cMasterReceive(&I2CD2, &tmp75);
}


