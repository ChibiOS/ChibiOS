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

/**
 * @file    irq_storm.c
 * @brief   IRQ Storm stress test code.
 *
 * @addtogroup IRQ_STORM
 * @{
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "vt_storm.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static const vt_storm_config_t *config;

static virtual_timer_t watchdog;
static virtual_timer_t wrapper;
static virtual_timer_t sweeper0, sweeperm1, sweeperp1, sweeperm3, sweeperp3;
static virtual_timer_t guard0, guard1, guard2, guard3;
static virtual_timer_t continuous;
static volatile sysinterval_t delay;
static volatile bool saturated;
static uint32_t vtcus;

#if VT_STORM_CFG_HAMMERS
/*
 * GPTs configuration.
 */
static void hammer_gpt_cb(GPTDriver *gptp);
static const GPTConfig hammer_gpt_cfg = {
  1000000,              /* 1MHz timer clock.*/
  hammer_gpt_cb,    /* Timer callback.*/
  0,
  0
};
#endif

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void watchdog_cb(virtual_timer_t *vtp, void *p) {

  (void)vtp;
  (void)p;

  chSysLockFromISR();
  chVTResetI(&sweeper0);
  chVTResetI(&sweeperm1);
  chVTResetI(&sweeperp1);
  chVTResetI(&sweeperm3);
  chVTResetI(&sweeperp3);
  chVTResetI(&wrapper);
  chSysUnlockFromISR();

  saturated = true;
}

static void wrapper_cb(virtual_timer_t *vtp, void *p) {

  (void)vtp;
  (void)p;
}

static void sweeper0_cb(virtual_timer_t *vtp, void *p) {

  (void)p;

  chSysLockFromISR();
#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     r = rand() & 15;
     while (r--) {
       x++;
     }
   }
#endif
  chVTSetI(&wrapper, (sysinterval_t)-1, wrapper_cb, NULL);
  chVTSetI(vtp, delay, sweeper0_cb, NULL);
  chSysUnlockFromISR();
}

static void sweeperm1_cb(virtual_timer_t *vtp, void *p) {

  (void)p;

  chSysLockFromISR();
#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     r = rand() & 15;
     while (r--) {
       x++;
     }
   }
#endif
  chVTSetI(vtp, delay - 1, sweeperm1_cb, NULL);
  chSysUnlockFromISR();
}

static void sweeperp1_cb(virtual_timer_t *vtp, void *p) {

  (void)p;

  chSysLockFromISR();
#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     r = rand() & 15;
     while (r--) {
       x++;
     }
   }
#endif
  chVTSetI(vtp, delay + 1, sweeperp1_cb, NULL);
  chSysUnlockFromISR();
}

static void sweeperm3_cb(virtual_timer_t *vtp, void *p) {

  (void)p;

  chSysLockFromISR();
#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     r = rand() & 15;
     while (r--) {
       x++;
     }
   }
#endif
  chVTSetI(vtp, delay - 3, sweeperm3_cb, NULL);
  chSysUnlockFromISR();
}

static void sweeperp3_cb(virtual_timer_t *vtp, void *p) {

  (void)p;

  chSysLockFromISR();
#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     r = rand() & 15;
     while (r--) {
       x++;
     }
   }
#endif
  chVTSetI(vtp, delay + 3, sweeperp3_cb, NULL);
  chSysUnlockFromISR();
}

static void continuous_cb(virtual_timer_t *vtp, void *p) {

  (void)vtp;
  (void)p;
  vtcus++;

#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     chSysLockFromISR();
     r = rand() & 255;
     chSysUnlockFromISR();
     while (r--) {
       x++;
     }
   }
#endif
}

static void guard_cb(virtual_timer_t *vtp, void *p) {

  (void)vtp;
  (void)p;
}

#if VT_STORM_CFG_HAMMERS
/**
 * @brief   GPT callback.
 */
static void hammer_gpt_cb(GPTDriver *gptp) {

  (void)gptp;

#if VT_STORM_CFG_RANDOMIZE != FALSE
   /* Pseudo-random delay.*/
   {
     static volatile unsigned x = 0;
     unsigned r;

     chSysLockFromISR();
     r = rand() & 31;
     chSysUnlockFromISR();
     while (r--) {
       x++;
     }
   }
#endif
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   VT storm execution.
 *
 * @param[in] cfg       pointer to the test configuration structure
 *
 * @api
 */
void vt_storm_execute(const vt_storm_config_t *cfg) {
  unsigned i;

  config = cfg;

  /* Printing environment information.*/
  chprintf(cfg->out, "");
  chprintf(cfg->out, "\r\n*** ChibiOS/RT VT-STORM long duration test\r\n***\r\n");
  chprintf(cfg->out, "*** Kernel:       %s\r\n", CH_KERNEL_VERSION);
  chprintf(cfg->out, "*** Compiled:     %s\r\n", __DATE__ " - " __TIME__);
#ifdef PORT_COMPILER_NAME
  chprintf(cfg->out, "*** Compiler:     %s\r\n", PORT_COMPILER_NAME);
#endif
  chprintf(cfg->out, "*** Architecture: %s\r\n", PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(cfg->out, "*** Core Variant: %s\r\n", PORT_CORE_VARIANT_NAME);
#endif
  chprintf(cfg->out, "*** System Clock: %d\r\n", cfg->sysclk);
#ifdef PORT_INFO
  chprintf(cfg->out, "*** Port Info:    %s\r\n", PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(cfg->out, "*** Platform:     %s\r\n", PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(cfg->out, "*** Test Board:   %s\r\n", BOARD_NAME);
#endif
  chprintf(cfg->out, "***\r\n");
  chprintf(cfg->out, "*** Randomize:        %d\r\n", VT_STORM_CFG_RANDOMIZE);
  chprintf(cfg->out, "*** Hammers:          %d\r\n", VT_STORM_CFG_HAMMERS);
  chprintf(cfg->out, "*** Minimum Delay:    %d ticks\r\n", VT_STORM_CFG_MIN_DELAY);
  chprintf(cfg->out, "*** System Time size: %d bits\r\n", CH_CFG_ST_RESOLUTION);
  chprintf(cfg->out, "*** Intervals size:   %d bits\r\n", CH_CFG_INTERVALS_SIZE);
  chprintf(cfg->out, "*** SysTick:          %d Hz\r\n", CH_CFG_ST_FREQUENCY);
  chprintf(cfg->out, "*** Delta:            %d cycles\r\n", CH_CFG_ST_TIMEDELTA);
  chprintf(cfg->out, "\r\n");

#if VT_STORM_CFG_HAMMERS
  /* Starting hammer timers.*/
  gptStart(&GPTD3, &hammer_gpt_cfg);
  gptStart(&GPTD4, &hammer_gpt_cfg);
  gptStartContinuous(&GPTD3, 99);
  gptStartContinuous(&GPTD4, 101);
#endif

  for (i = 1; i <= VT_STORM_CFG_ITERATIONS; i++) {

    chprintf(cfg->out, "Iteration %d\r\n", i);
    chThdSleepS(TIME_MS2I(10));

    /* Starting continuous timer.*/
    vtcus = 0;

    delay = 120;
    saturated = false;
    do {
      /* Starting sweepers.*/
      chSysLock();
      chVTSetI(&watchdog, TIME_MS2I(501), watchdog_cb, NULL);
      chVTSetI(&sweeper0, delay, sweeper0_cb, NULL);
      chVTSetI(&sweeperm1, delay - 1, sweeperm1_cb, NULL);
      chVTSetI(&sweeperp1, delay + 1, sweeperp1_cb, NULL);
      chVTSetI(&sweeperm3, delay - 3, sweeperm3_cb, NULL);
      chVTSetI(&sweeperp3, delay + 3, sweeperp3_cb, NULL);
      chVTSetI(&wrapper, (sysinterval_t) - 1, wrapper_cb, NULL);
      chVTSetContinuousI(&continuous, TIME_US2I(50), continuous_cb, NULL);
      chVTSetI(&guard0, TIME_MS2I(250) + (CH_CFG_TIME_QUANTUM / 2), guard_cb, NULL);
      chVTSetI(&guard1, TIME_MS2I(250) + (CH_CFG_TIME_QUANTUM - 1), guard_cb, NULL);
      chVTSetI(&guard2, TIME_MS2I(250) + (CH_CFG_TIME_QUANTUM + 1), guard_cb, NULL);
      chVTSetI(&guard3, TIME_MS2I(250) + (CH_CFG_TIME_QUANTUM * 2), guard_cb, NULL);

      /* Letting them run for half second.*/
      chThdSleepS(TIME_MS2I(500));

      /* Stopping everything.*/
      chVTResetI(&watchdog);
      chVTResetI(&sweeper0);
      chVTResetI(&sweeperm1);
      chVTResetI(&sweeperp1);
      chVTResetI(&sweeperm3);
      chVTResetI(&sweeperp3);
      chVTResetI(&wrapper);
      chVTResetI(&continuous);
      chVTResetI(&guard0);
      chVTResetI(&guard1);
      chVTResetI(&guard2);
      chVTResetI(&guard3);
      chSysUnlock();

      if (saturated) {
        chprintf(cfg->out, "#");
        break;
      }

      palToggleLine(config->line);
      chprintf(cfg->out, ".");
      delay--;
    } while (delay >= VT_STORM_CFG_MIN_DELAY);

    if (saturated) {
      chprintf(cfg->out, "\r\nSaturated at %u uS", TIME_I2US(delay));
      chprintf(cfg->out, "\r\nContinuous ticks %u\r\n\r\n", vtcus);
    }
    else {
      chprintf(cfg->out, "\r\nNon saturated");
      chprintf(cfg->out, "\r\nContinuous ticks %u\r\n\r\n", vtcus);
    }
  }
}

/** @} */
