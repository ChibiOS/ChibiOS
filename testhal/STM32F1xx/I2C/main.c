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
#include "lis3.h"
#include "tmp75.h"
#include "fake.h"


/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(BlinkWA, 128);
static msg_t Blink(void *arg) {
  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Accelerometer thread
 */
static WORKING_AREA(PollAccelThreadWA, 128);
static msg_t PollAccelThread(void *arg) {
  chRegSetThreadName("PollAccel");
  (void)arg;
  while (TRUE) {
//    chThdSleepMilliseconds(rand() & 31);
    chThdSleepMilliseconds(32);
    request_acceleration_data();
  }
  return 0;
}


/* Temperature polling thread */
static WORKING_AREA(PollTmp75ThreadWA, 128);
static msg_t PollTmp75Thread(void *arg) {
  chRegSetThreadName("PollTmp75");
  (void)arg;
  while (TRUE) {
//    chThdSleepMilliseconds(rand() & 31);
    chThdSleepMilliseconds(15);
    /* Call reading function */
    request_temperature();
  }
  return 0;
}


/* Temperature polling thread */
static WORKING_AREA(PollFakeThreadWA, 128);
static msg_t PollFakeThread(void *arg) {
  chRegSetThreadName("PollFake");
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(16);
    /* Call reading function */
    request_fake();
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

  chThdSleepMilliseconds(200);
  I2CInit_pns();

  /* Create accelerometer thread */
  chThdCreateStatic(PollAccelThreadWA,
          sizeof(PollAccelThreadWA),
          NORMALPRIO,
          PollAccelThread,
          NULL);

  /* Create temperature thread */
  chThdCreateStatic(PollTmp75ThreadWA,
          sizeof(PollTmp75ThreadWA),
          NORMALPRIO,
          PollTmp75Thread,
          NULL);

  /* Create not responding thread */
  chThdCreateStatic(PollFakeThreadWA,
          sizeof(PollFakeThreadWA),
          NORMALPRIO,
          PollFakeThread,
          NULL);

  /* Creates the blinker thread. */
  chThdCreateStatic(BlinkWA, sizeof(BlinkWA), HIGHPRIO, Blink, NULL);

  /* main loop that do nothing */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }

  return 0;
}
