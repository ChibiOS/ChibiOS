/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/*
TODO:
write memtest function using ideas from http://www.memtest86.com/technical.htm
*/

#include "ch.h"
#include "hal.h"

#include "string.h"

#include "fsmc_sram.h"

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define USE_INFINITE_MEMTEST        FALSE

/*
 ******************************************************************************
 * EXTERNS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * PROTOTYPES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */
static uint32_t sram_check_buf[16 * 1024];
static uint32_t *sram_start = (uint32_t *)FSMC_Bank1_4_MAP;
static const size_t sram_size = 524288;

/*
 * SRAM driver configuration structure.
 */
static const SRAMConfig sram_cfg = {
    .btr = 2 << 8
};

/* benchmarking results in MiB/S */
double memset_speed_ext;
double memset_speed_int;
double memcpy_speed_ext2int;
double memcpy_speed_int2ext;

/*
 ******************************************************************************
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 */
/**
 *
 */
static void sram_benchmark(void){

  size_t i=0;
  time_measurement_t mem_tmu;

  /* memset speed ext */
  chTMObjectInit(&mem_tmu);
  chTMStartMeasurementX(&mem_tmu);
  memset(sram_start, 0x55, sram_size);
  memset(sram_start, 0x00, sram_size);
  chTMStopMeasurementX(&mem_tmu);
  memset_speed_ext = 1 / (mem_tmu.cumulative / (double)STM32_SYSCLK);

  /* memset speed int */
  chTMObjectInit(&mem_tmu);
  chTMStartMeasurementX(&mem_tmu);
  for (i=0; i<16; i++)
    memset(sram_check_buf, i, sizeof(sram_check_buf));
  chTMStopMeasurementX(&mem_tmu);
  memset_speed_int = 1 / (mem_tmu.cumulative / (double)STM32_SYSCLK);

  /* memcpy ext2int */
  chTMObjectInit(&mem_tmu);
  chTMStartMeasurementX(&mem_tmu);
  for (i=0; i<16; i++)
    memcpy(sram_check_buf, sram_start+ i * sizeof(sram_check_buf), sizeof(sram_check_buf));
  chTMStopMeasurementX(&mem_tmu);
  memcpy_speed_ext2int = 1 / (mem_tmu.cumulative / (double)STM32_SYSCLK);

  /* memcpy int2ext */
  chTMObjectInit(&mem_tmu);
  memset(sram_check_buf, 0xAA, sizeof(sram_check_buf));
  chTMStartMeasurementX(&mem_tmu);
  for (i=0; i<16; i++)
    memcpy(sram_start + i * sizeof(sram_check_buf), sram_check_buf, sizeof(sram_check_buf));
  chTMStopMeasurementX(&mem_tmu);
  memcpy_speed_int2ext = 1 / (mem_tmu.cumulative / (double)STM32_SYSCLK);
}

/**
 *
 */
#if USE_INFINITE_MEMTEST
static void memstest(void){
  while (true) {
    ;
  }
}
#endif /* USE_INFINITE_MEMTEST */

/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */

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

  fsmcSramInit();
  fsmcSramStart(&SRAMD4, &sram_cfg);
  sram_benchmark();

#if USE_INFINITE_MEMTEST
  memtest();
#endif

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}


