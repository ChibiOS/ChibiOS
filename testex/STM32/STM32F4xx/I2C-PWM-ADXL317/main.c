/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "adxl317.h"

/*===========================================================================*/
/* ADXL317 related.                                                          */
/*===========================================================================*/

/*Before connecting the ADXL317 on the SDP-K1 move the Jumper P14 to 1.8V.*/

/*
 * I2C TX and RX buffers.
 */
static uint8_t txbuf[ADXL317_MAX_BUFF_SIZE];
static uint8_t rxbuf[ADXL317_MAX_BUFF_SIZE];

/* ADXL317 Driver: This object represent an ADXL317 instance */
static ADXL317Driver ADXL317D1;

static int32_t accraw[ADXL317_ACC_NUMBER_OF_AXES];

static float acccooked[ADXL317_ACC_NUMBER_OF_AXES];

static char axisID[ADXL317_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
  .op_mode          = OPMODE_I2C,
  .clock_speed      = 100000,
  .duty_cycle       = STD_DUTY_CYCLE
};

#if ADXL317_USE_ADVANCED || defined(__DOXYGEN__)
static const adxl317_acc_lp_t acc_lp = {
  .x                = ADXL317_ACC_X_LP_500,
  .y                = ADXL317_ACC_Y_LP_500,
  .z                = ADXL317_ACC_Z_LP_500
};

static const adxl317_acc_hp_t acc_hp = {
  .x                = ADXL317_ACC_X_HP_DISABLED,
  .y                = ADXL317_ACC_Y_HP_DISABLED,
  .z                = ADXL317_ACC_Z_HP_DISABLED
};
#endif /* ADXL317_USE_ADVANCED */

static ADXL317Config adxl317cfg = {
  .i2cp             = &I2CD1,
  .i2ccfg           = &i2ccfg,
  .accsensitivity   = NULL,
  .accbias          = NULL,
#if ADXL317_USE_ADVANCED
  .acclowpass       = acc_lp,
  .acchighpass      = acc_hp,
#endif /* ADXL317_USE_ADVANCED */
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

static BaseSequentialStream* chp = (BaseSequentialStream*) &SD5;

static PWMConfig pwmcfg = {
  .frequency = 30000000,
  .period = 10,
  .callback = NULL,
  .channels = {
    {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL},
    {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
    {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
    {.mode = PWM_OUTPUT_DISABLED, .callback = NULL}
  },
  .cr2 = 0,
  .bdtr = 0,
  .dier = 0
};

static THD_WORKING_AREA(waThreadBlinker, 128);
static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;

  chRegSetThreadName("blinker");

  while (true) {
    palSetLine(LINE_LED_RED);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED_RED);
    chThdSleepMilliseconds(200);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  sdStart(&SD5, NULL);

  /* Configuring I2C SCK and I2C SDA related GPIOs.*/
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN |
                 PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN |
                 PAL_STM32_PUPDR_PULLUP);

  /* Configuring PWM pin.*/
  palSetLineMode(LINE_ARD_D3, PAL_MODE_ALTERNATE(2)| PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_OTYPE_PUSHPULL);

  /* Creating LED thread.*/
  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO + 1,
                    ThreadBlinker, NULL);

  /* Starting PWM.*/
  pwmStart(&PWMD4, &pwmcfg);
  pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));

  /* ADXL317 Object Initialization.*/
  adxl317ObjectInit(&ADXL317D1, txbuf, rxbuf);

  /* Activates the ADXL317 driver.*/
  adxl317Start(&ADXL317D1, &adxl317cfg);

  /* Normal main() thread activity, printing MEMS data on the SDU1.*/
  while (true) {
    adxl317AccelerometerReadRaw(&ADXL317D1, accraw);
    chprintf(chp, "ADXL317 Accelerometer raw data...\r\n");
    for(i = 0; i < ADXL317_ACC_NUMBER_OF_AXES; i++) {
      chprintf(chp, "%c-axis: %d\r\n", axisID[i], accraw[i]);
    }

    adxl317AccelerometerReadCooked(&ADXL317D1, acccooked);
    chprintf(chp, "ADXL317 Accelerometer cooked data...\r\n");
    for(i = 0; i < ADXL317_ACC_NUMBER_OF_AXES; i++) {
      chprintf(chp, "%c-axis: %.3f\r\n", axisID[i], acccooked[i]);
    }
    chThdSleepMilliseconds(100);
  }
  adxl317Stop(&ADXL317D1);
}
