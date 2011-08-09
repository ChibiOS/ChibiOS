/**
 * TMP75 is most simple I2C device in our case. It is already useful with
 * default settings after powerup.
 * You only must read 2 sequential bytes from it.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "tmp75.h"


/* input buffer */
static i2cblock_t tmp75_rx_data[TMP75_RX_DEPTH];

/* temperature value */
static int16_t temperature = 0;

/* Simple error trap */
static void i2c_tmp75_error_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void)i2cscfg;
  int status = 0;
  status = i2cp->id_i2c->SR1;
  while(TRUE);
}

/* This callback raise up when transfer finished */
static void i2c_tmp75_cb(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg){
  (void)*i2cp;
  (void)*i2cscfg;
  /* store temperature value */
}

/* Fill TMP75 config. */
static const I2CSlaveConfig tmp75 = {
    i2c_tmp75_cb,
    i2c_tmp75_error_cb,
};

#define tmp75_addr 0b1001000

/* This is main function. */
void request_temperature(void){
  i2cAcquireBus(&I2CD2);
  i2cMasterReceive(&I2CD2, &tmp75, tmp75_addr, tmp75_rx_data, 2);
  i2cReleaseBus(&I2CD2);
  temperature = (tmp75_rx_data[0] << 8) + tmp75_rx_data[1];
}


