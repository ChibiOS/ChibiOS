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
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "chprintf.h"
#include "chsmc.h"

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(600);
  }
}

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

/*
 *  Dummy trust service thread.
 */
static THD_WORKING_AREA(waDummyTrustedService, 512);
static THD_FUNCTION(DummyTrustedService, arg) {
  (void) arg;

  msg_t msg;
  smc_service_t *svcp;
  chRegSetThreadName("DTS");

  /*
   * Register the trust service
   */
  svcp = smcRegisterMeAsService("DummyTrustedService");
  if (svcp == NULL) {
    /*
     * Error: the service is already registered
     * or memory is exhausted.
     */
    return;
  }
  /*
   * Wait and process requests
   */
  while (true) {
    msg = smcServiceWaitRequest(svcp, MSG_OK);
    if (msg == MSG_OK && svcp->svc_datalen > 0) {
      *((char *)svcp->svc_data + svcp->svc_datalen - 1) = '\0';
#if 1
      chprintf((BaseSequentialStream*)&SD1,
          "My non secure 'alter ego' has a request.\r\n");
      chprintf((BaseSequentialStream*)&SD1,
          "She tells: '");
#endif
      chprintf((BaseSequentialStream*)&SD1, (char *)svcp->svc_data);
      chprintf((BaseSequentialStream*)&SD1, "'\r\n");
    }
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  uint32_t n;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   *   The foreign interrupts are disabled up to the trampoline in the non secure world
   */
  halInit();
  chSysInit();
  smcInit();

  /*
   * Activates the serial driver 0 using the driver default configuration.
   */
  sdStart(&SD1, &sdcfg);

  /* Redirecting  UART0 RX on PD2 and UART0 TX on PD3. */
  palSetGroupMode(PIOD, PAL_PORT_BIT(2) | PAL_PORT_BIT(3), 0U,
                  PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO-64, Thread1, NULL);
  /*
   * Creates the dummy service thread.
   */
  n = 0;
  chThdCreateStatic(waDummyTrustedService, sizeof(waDummyTrustedService), NORMALPRIO-32,
      DummyTrustedService, (void *)n);
  ++n;

  /*
   * The DDR memory is divided in 4 regions. Each region is 2MB large.
   * The first region is split in two areas, each 1MB large.
   * The lower area of this first region is non secure.
   * All the rest of the regions space is secured.
   */
  mtxSetSlaveRegionSize(MATRIX0, H64MX_SLAVE_DDR_PORT0, MATRIX_AREA_SIZE_2M, REGION_0_MSK);
  mtxSetSlaveRegionSize(MATRIX0, H64MX_SLAVE_DDR_PORT1, MATRIX_AREA_SIZE_2M, REGION_0_MSK);

  mtxSetSlaveSplitAddr(MATRIX0, H64MX_SLAVE_DDR_PORT0, MATRIX_AREA_SIZE_1M, REGION_0_MSK);
  mtxSetSlaveSplitAddr(MATRIX0, H64MX_SLAVE_DDR_PORT0, MATRIX_AREA_SIZE_2M,
      REGION_1_MSK | REGION_2_MSK | REGION_3_MSK);
  mtxSetSlaveSplitAddr(MATRIX0, H64MX_SLAVE_DDR_PORT1, MATRIX_AREA_SIZE_1M, REGION_0_MSK);
  mtxSetSlaveSplitAddr(MATRIX0, H64MX_SLAVE_DDR_PORT1, MATRIX_AREA_SIZE_2M,
      REGION_1_MSK | REGION_2_MSK | REGION_3_MSK);

  mtxConfigSlaveSec(MATRIX0, H64MX_SLAVE_DDR_PORT0,
      mtxRegionLansech(REGION_0, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_1, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_2, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_3, UPPER_AREA_SECURABLE),
      mtxRegionRdnsech(REGION_0, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_1, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_2, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_3, NOT_SECURE_READ),
      mtxRegionWrnsech(REGION_0, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_1, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_2, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_3, NOT_SECURE_WRITE));
  mtxConfigSlaveSec(MATRIX0, H64MX_SLAVE_DDR_PORT1,
      mtxRegionLansech(REGION_0, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_1, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_2, UPPER_AREA_SECURABLE) |
      mtxRegionLansech(REGION_3, UPPER_AREA_SECURABLE),
      mtxRegionRdnsech(REGION_0, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_1, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_2, NOT_SECURE_READ) |
      mtxRegionRdnsech(REGION_3, NOT_SECURE_READ),
      mtxRegionWrnsech(REGION_0, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_1, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_2, NOT_SECURE_WRITE) |
      mtxRegionWrnsech(REGION_3, NOT_SECURE_WRITE));

  /*
   * Wait that all services are initialized
   */
  smcWaitServicesStarted(n);
  /*
   * Jump in the NON SECURE world
   * This 'main' thread become the non secure environment as view by
   * the secure world.
   */
  chprintf((BaseSequentialStream*)&SD1, "Jumping in the non secure world\n\r");
  _ns_trampoline(NSEC_MEMORY_START);
  /*
   * It never goes here
   */
}
