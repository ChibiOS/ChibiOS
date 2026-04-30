/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include <string.h>

#include "oop_chprintf.h"
#include "xshell.h"

static hal_buffered_sio_c bsio1;
static uint8_t rxbuf[32];
static uint8_t txbuf[32];

#define ADC_CHANNELS                        2U
#define ADC_CFG                             0U
#define ADC_LINEAR_GROUP                    0U
#define ADC_STREAM_GROUP                    1U
#define ADC_LINEAR_DEPTH                    1U
#define ADC_STREAM_DEPTH                    8U
#define ADC_STREAM_TIMEOUT                  TIME_MS2I(2000)
#define ADC_GPT_TRIGGER_PERIOD              100000U

static adcsample_t adc_linear_samples[ADC_CHANNELS * ADC_LINEAR_DEPTH];
static adcsample_t adc_stream_samples[ADC_CHANNELS * ADC_STREAM_DEPTH];

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE       THD_STACK_SIZE(2048)

static void adc_print_samples(xshell_t *xshp, const adcsample_t *samples,
                              size_t depth) {
  size_t i;

  for (i = 0U; i < depth; i++) {
    chprintf(xshp->stream, "%u: ch1=%u ch2=%u" XSHELL_NEWLINE_STR,
             (unsigned)i,
             (unsigned)samples[(i * ADC_CHANNELS) + 0U],
             (unsigned)samples[(i * ADC_CHANNELS) + 1U]);
  }
}

static bool adc_stop_requested(xshell_t *xshp) {
  msg_t msg;

  msg = chnGetTimeout((asynchronous_channel_i *)xshp->stream, TIME_IMMEDIATE);

  return msg != Q_TIMEOUT;
}

static void adc_linear(xshell_t *xshp) {
  msg_t msg;

  if (drvSelectCfgX(&ADCD1, ADC_CFG) == NULL) {
    chprintf(xshp->stream, "ADC linear configuration failed" XSHELL_NEWLINE_STR);
    return;
  }

  msg = adcConvert(&ADCD1, ADC_LINEAR_GROUP, adc_linear_samples,
                   ADC_LINEAR_DEPTH);
  if (msg != HAL_RET_SUCCESS) {
    chprintf(xshp->stream, "ADC linear conversion failed (%d)" XSHELL_NEWLINE_STR,
             msg);
    return;
  }

  adc_print_samples(xshp, adc_linear_samples, ADC_LINEAR_DEPTH);
}

static void adc_stream(xshell_t *xshp) {
  const adcsample_t *samples;
  driver_state_t state;
  msg_t msg;

  if (drvSelectCfgX(&ADCD1, ADC_CFG) == NULL) {
    chprintf(xshp->stream, "ADC stream configuration failed" XSHELL_NEWLINE_STR);
    return;
  }

  msg = adcStartConversionCircular(&ADCD1, ADC_STREAM_GROUP,
                                   adc_stream_samples, ADC_STREAM_DEPTH);
  if (msg != HAL_RET_SUCCESS) {
    chprintf(xshp->stream, "ADC stream start failed (%d)" XSHELL_NEWLINE_STR,
             msg);
    return;
  }

  chprintf(xshp->stream, "Streaming, press any key to stop" XSHELL_NEWLINE_STR);
  gptStartContinuous(&GPTD1, ADC_GPT_TRIGGER_PERIOD);

  state = HAL_DRV_STATE_HALF;
  while (!adc_stop_requested(xshp)) {
    msg = adcSynchronizeState(&ADCD1, state, ADC_STREAM_TIMEOUT);
    if (msg != MSG_OK) {
      chprintf(xshp->stream, "ADC stream stopped (%d)" XSHELL_NEWLINE_STR, msg);
      break;
    }

    if (state == HAL_DRV_STATE_HALF) {
      samples = &adc_stream_samples[0];
      state = HAL_DRV_STATE_FULL;
    }
    else {
      samples = &adc_stream_samples[(ADC_STREAM_DEPTH / 2U) * ADC_CHANNELS];
      state = HAL_DRV_STATE_HALF;
    }

    adc_print_samples(xshp, samples, ADC_STREAM_DEPTH / 2U);
  }

  gptStopTimer(&GPTD1);
  adcStopConversion(&ADCD1);
  chprintf(xshp->stream, "ADC stream stopped" XSHELL_NEWLINE_STR);
}

static void cmd_adc(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "adc linear|stream");
    return;
  }

  if (strcmp(argv[1], "linear") == 0) {
    adc_linear(xshp);
    return;
  }

  if (strcmp(argv[1], "stream") == 0) {
    adc_stream(xshp);
    return;
  }

  xshellUsage(xshp, "adc linear|stream");
}

static void cmd_halt(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc != 1) {
    xshellUsage(xshp, "halt");
    return;
  }

  chprintf(xshp->stream, XSHELL_NEWLINE_STR "halted");
  chThdSleepMilliseconds(10);
  chSysHalt("shell halt");
}

static void cmd_sbcrash(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "sbcrash");
    return;
  }

  chprintf(xshp->stream, "\r\n\nCrashing...\r\n");
  chThdSleepMilliseconds(20);

  /* Test for exception on interrupt.*/
  asm volatile ("mov r0, #64");
  asm volatile ("mov sp, r0");
  while (true) {
  }
}

static void cmd_sbexit(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "sbexit");
    return;
  }

  chprintf(xshp->stream, "\r\n\nExiting SandBox\r\n");
  chThdSleepMilliseconds(20);
  sbExit(MSG_OK);
}

static const xshell_command_t commands[] = {
  {"adc", cmd_adc},
  {"halt", cmd_halt},
  {"sbcrash", cmd_sbcrash},
  {"sbexit", cmd_sbexit},
  {NULL, NULL}
};

static const xshell_manager_config_t cfg1 = {
  .thread_name      = "shell",
  .banner           = XSHELL_DEFAULT_BANNER_STR,
  .prompt           = XSHELL_DEFAULT_PROMPT_STR,
  .commands         = commands,
  .use_heap         = true,
  .stack.size       = SHELL_WA_SIZE
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_STACK(thd1_stack, 256);
static THD_FUNCTION(thd1_func, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palTogglePad(IOPORT1, 0);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  xshell_manager_t sm1;

  /*
   * System initializations.
   * - Virtual HAL initialization.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  if (drvStart(&ADCD1, NULL) != HAL_RET_SUCCESS) {
    chSysHalt("ADCD1 failed");
  }
  if (drvStart(&GPTD1, NULL) != HAL_RET_SUCCESS) {
    chSysHalt("GPTD1 failed");
  }

  /*
   * Spawning a blinker thread.
   */
  static thread_t thd1;
  static const THD_DECL_STATIC(thd1_desc, "blinker", thd1_stack,
                               NORMALPRIO + 10, thd1_func, NULL, NULL);
  chThdSpawnRunning(&thd1, &thd1_desc);

  /*
   * Shell manager initialization.
   */
  xshellObjectInit(&sm1, &cfg1);

  /*
   * Starting a buffered SIO, it must behave exactly as a serial driver.
   */
  bsioObjectInit(&bsio1, &SIOD1,
                 rxbuf, sizeof rxbuf,
                 txbuf, sizeof txbuf);
  drvStart(&bsio1, NULL);

  /*
   * Normal main() thread activity, spawning shells.
   */
  while (true) {
    xshell_t *xshp = xshellSpawn(&sm1,
                                 (BaseSequentialStream *)oopGetIf(&bsio1, chn),
                                 NORMALPRIO + 1, NULL);
    chThdWait(&xshp->thread);
    chThdSleepMilliseconds(500);
  }
}
