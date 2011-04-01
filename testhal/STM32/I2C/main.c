/**
 * Lets imagine that we have board with LIS3LV02DL accelerometer on channel #1
 * and MAX1236 ADC, TMP75 thermometer on channel #2.
 *
 * NOTE: I assume, that you have datasheets on all this stuff.
 *
 * NOTE: Also, I assume, that you know how to I2C works.
 *
 * In order from simplicity to complexity:
 *   TMP75
 *   MAX1236
 *   LIS3LV02DL
 *
 * Project splitted to separate source files for each device.
 *
 * Data from sensors we will be read from different thread sleeping different
 * amount of time.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "i2c_pns.h"
#include "tmp75.h"
#include "max1236.h"
#include "lis3.h"



/* Temperature polling thread */
static WORKING_AREA(PollTmp75ThreadWA, 128);
static msg_t PollTmp75Thread(void *arg) {
  (void)arg;
  systime_t time = chTimeNow();

  while (TRUE) {
    time += MS2ST(1000);
    /* Call reading function */
    request_temperature();
    chThdSleepUntil(time);
  }
  return 0;
}

/* MAX1236 polling thread */
static WORKING_AREA(PollMax1236ThreadWA, 128);
static msg_t PollMax1236Thread(void *arg) {
  (void)arg;
  systime_t time = chTimeNow();

  while (TRUE) {
    time += MS2ST(20);
    /* Call reading function */
    read_max1236();
    chThdSleepUntil(time);
  }
  return 0;
}


static WORKING_AREA(PollAccelThreadWA, 128);
static msg_t PollAccelThread(void *arg) {
  (void)arg;
  systime_t time = chTimeNow();

  while (TRUE) {
    time += MS2ST(2);
    request_acceleration_data();
    chThdSleepUntil(time);
  }
  return 0;
}




/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(void) {

  halInit();
  chSysInit();

  I2CInit_pns();

  /* Create temperature thread */
  chThdCreateStatic(PollTmp75ThreadWA,
          sizeof(PollTmp75ThreadWA),
          NORMALPRIO,
          PollTmp75Thread,
          NULL);


  /* Create max1236 thread */
  chThdCreateStatic(PollMax1236ThreadWA,
          sizeof(PollMax1236ThreadWA),
          NORMALPRIO,
          PollMax1236Thread,
          NULL);


  /* Create accelerometer thread */
  chThdCreateStatic(PollAccelThreadWA,
          sizeof(PollAccelThreadWA),
          HIGHPRIO,
          PollAccelThread,
          NULL);


  /* main loop that do nothing */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }

  return 0;
}
