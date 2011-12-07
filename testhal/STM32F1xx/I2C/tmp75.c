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
static uint8_t tmp75_rx_data[TMP75_RX_DEPTH];

/* temperature value */
static int16_t temperature = 0;


#define tmp75_addr 0b1001000

/* This is main function. */
void request_temperature(void){
  int16_t t_int = 0, t_frac = 0;

  i2cAcquireBus(&I2CD1);
  i2cMasterReceive(&I2CD1, tmp75_addr, tmp75_rx_data, 2);
  i2cReleaseBus(&I2CD1);

  t_int = tmp75_rx_data[0] * 100;
  t_frac = (tmp75_rx_data[1] * 100) >> 8;
  temperature = t_int + t_frac;
}


