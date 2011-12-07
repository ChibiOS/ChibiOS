/**
 * Not responding slave test
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "fake.h"


/* input buffer */
static uint8_t rx_data[2];

/* temperature value */
static int16_t temperature = 0;


#define addr 0b1001100

/* This is main function. */
void request_fake(void){
  i2cflags_t errors = 0;

  i2cAcquireBus(&I2CD1);
  errors = i2cMasterReceive(&I2CD1, addr, rx_data, 2);
  i2cReleaseBus(&I2CD1);

  if (errors == I2CD_ACK_FAILURE){
    __NOP();
  }
  else{
    temperature = (rx_data[0] << 8) + rx_data[1];
  }
}


