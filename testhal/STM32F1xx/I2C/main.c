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

#include "ch.h"
#include "hal.h"

#include "i2c_pns.h"
#include "lis3.h"



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
  (void)arg;
  systime_t time = chTimeNow();

  while (TRUE) {
    time += MS2ST(20);
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

  chThdSleepMilliseconds(1000);
  I2CInit_pns();

  /* Create accelerometer thread */
  chThdCreateStatic(PollAccelThreadWA,
          sizeof(PollAccelThreadWA),
          HIGHPRIO,
          PollAccelThread,
          NULL);

  /* Creates the blinker thread. */
  chThdCreateStatic(BlinkWA, sizeof(BlinkWA), LOWPRIO, Blink, NULL);

  /* main loop that do nothing */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }

  return 0;
}
