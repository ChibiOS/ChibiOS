#include "ch.h"
#include "hal.h"

#include "i2c_pns.h"

#include "lis3.h"
#include "tmp75.h"
#include "max1236.h"

/* I2C1 */
static I2CConfig i2cfg1 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
    0,
    0,
    0,
    0,
};

/* I2C2 */
static I2CConfig i2cfg2 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
    0,
    0,
    0,
    0,
};



void I2CInit_pns(void){

  i2cInit();

  i2cStart(&I2CD1, &i2cfg1);
  while(I2CD1.id_state != I2C_READY){ // wait ready status
      chThdSleepMilliseconds(1);
  }

  i2cStart(&I2CD2, &i2cfg2);
  while(I2CD2.id_state != I2C_READY){ // wait ready status
      chThdSleepMilliseconds(1);
  }

  /* tune ports for I2C1*/
  palSetPadMode(IOPORT2, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
  palSetPadMode(IOPORT2, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);

  /* tune ports for I2C2*/
  palSetPadMode(IOPORT2, 10, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
  palSetPadMode(IOPORT2, 11, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);


  /* startups. Pauses added just to be safe */
  init_max1236();
  chThdSleepMilliseconds(100);

  init_lis3();
  chThdSleepMilliseconds(100);
}


