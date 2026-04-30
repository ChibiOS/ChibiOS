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
#include "sb.h"

#include "oop_chprintf.h"
#include "oop_nullstreams.h"

#include "startup_defs.h"

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

/*===========================================================================*/
/* VIO-related.                                                              */
/*===========================================================================*/

static vio_gpio_units_t sb1_gpio_units = {
  .n        = 1U,
  .units = {
    [0] = {
      .permissions  = VIO_GPIO_PERM_WRITE,
      .port         = GPIOA,
      .mask         = 1U,
      .offset       = GPIOA_LED_GREEN
    }
  }
};

static sio_configurations_t sb1_uart_configurations = {
  .cfgsnum      = 1U,
  .cfgs = {
    [0]         = SIO_DEFAULT_CONFIGURATION
  }
};

static vio_uart_units_t sb1_uart_units = {
  .n        = 1U,
  .units = {
    [0] = {
      .siop  = &LPSIOD1,
      .vrqsb = &sbx1,
      .vrqn  = 8
    }
  }
};

static const gpt_configurations_t sb1_gpt_configurations = {
  .cfgsnum                     = 1U,
  .cfgs = {
    [0] = {
      .frequency               = 1000000U,
      .cr2                     = TIM_CR2_MMS_1,
      .dier                    = 0U
    }
  }
};

static vio_gpt_units_t sb1_gpt_units = {
  .n                           = 1U,
  .units = {
    [0] = {
      .gptp                    = &GPTD4,
      .vrqsb                   = &sbx1,
      .vrqn                    = 13
    }
  }
};

static const adc_conversion_groups_t sb1_adc_groups = {
  .grpsnum                     = 2U,
  .grps                        = {
    [0] = {
      .num_channels            = 2U,
      .cfgr                    = ADC_CFGR_EXTEN_RISING |
                                 ADC_CFGR_EXTSEL_SRC(12),
      .cfgr2                   = 0U,
      .tr1                     = ADC_TR_DISABLED,
      .tr2                     = ADC_TR_DISABLED,
      .tr3                     = ADC_TR_DISABLED,
      .awd2cr                  = 0U,
      .awd3cr                  = 0U,
      .smpr                    = {
        ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) |
        ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5),
        0U
      },
      .sqr                     = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) |
        ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
        0U,
        0U,
        0U
      }
    },
    [1] = {
      .num_channels            = 2U,
      .cfgr                    = ADC_CFGR_EXTEN_RISING |
                                 ADC_CFGR_EXTSEL_SRC(12),
      .cfgr2                   = 0U,
      .tr1                     = ADC_TR_DISABLED,
      .tr2                     = ADC_TR_DISABLED,
      .tr3                     = ADC_TR_DISABLED,
      .awd2cr                  = 0U,
      .awd3cr                  = 0U,
      .smpr                    = {
        ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) |
        ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5),
        0U
      },
      .sqr                     = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) |
        ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
        0U,
        0U,
        0U
      }
    }
  }
};

const adc_configurations_t sb1_adc_configurations = {
  .cfgsnum                     = 1U,
  .cfgs = {
    [0] = {
      .grps                    = &sb1_adc_groups,
      .difsel                  = 0U
    }
  }
};

static vio_adc_units_t sb1_adc_units = {
  .n                           = 1U,
  .units = {
    [0] = {
      .adcp                    = &ADCD1,
      .config                  = &sb1_adc_configurations.cfgs[0],
      .vrqsb                   = &sbx1,
      .vrqn                    = 12
    }
  }
};

static vio_conf_t vio_config1 = {
  .gpios        = &sb1_gpio_units,
  .adcs         = &sb1_adc_units,
  .adcconfs     = &sb1_adc_configurations,
  .gpts         = &sb1_gpt_units,
  .gptconfs     = &sb1_gpt_configurations,
  .uarts        = &sb1_uart_units,
  .uartconfs    = &sb1_uart_configurations
};

/*===========================================================================*/
/* SB-related.                                                               */
/*===========================================================================*/

/* Privileged stacks for sandboxes.*/
static SB_STACK(sbx1stk);

/* Arguments and environments for SB1.*/
static const char *sbx1_argv[] = {
  "sbx1",
  NULL
};

static const char *sbx1_envp[] = {
  NULL
};

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

static void start_sb1(void) {
  thread_t *utp;

  /* Starting sandboxed thread 1.*/
  utp = sbStart(&sbx1, NORMALPRIO-10, sbx1stk, sbx1_argv, sbx1_envp);
  if (utp == NULL) {
    chSysHalt("sbx1 failed");
  }
}

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t el1;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   * - SandBox manager initialization.
   */
  halInit();
  chSysInit();
  sbHostInit();

  /* Pins used by the ADC test code in the sandbox.*/
  palSetPadMode(GPIOA, 0U, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 1U, PAL_MODE_INPUT_ANALOG);

  /*
   * Sandbox objects initialization, regions are assigned explicitly.
   */
  sbObjectInit(&sbx1);
  sbSetRegion(&sbx1, 0, STARTUP_FLASH1_BASE, STARTUP_FLASH1_SIZE, SB_REG_IS_CODE);
  sbSetRegion(&sbx1, 1, STARTUP_RAM1_BASE,   STARTUP_RAM1_SIZE, SB_REG_IS_DATA);
  sbSetVirtualIO(&sbx1, &vio_config1);

  /* Starting sandboxed threads.*/
  start_sb1();

  /*
   * Listening to sandbox events.
   */
  chEvtRegister(&sb.termination_es, &el1, (eventid_t)0);

  /*
   * Normal main() thread activity, in this demo it checks for sandboxes state.
   */
  while (true) {

    /* Waiting for a sandbox event or timeout.*/
    if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(500)) != (eventmask_t)0) {

      if (!sbIsThreadRunningX(&sbx1)) {
        msg_t msg = sbWait(&sbx1);

        /* Re-starting the driver because the sandbox stops it on exit.*/
        if (drvStart(&LPSIOD1, NULL) != MSG_OK) {
          chSysHalt("LPSIOD1 failed");
        }
        chprintf(oopGetIf(&LPSIOD1, chn), "SB1 terminated: 0x%08x\r\n", msg);
        chThdSleepMilliseconds(10);
        drvStop(&LPSIOD1);

        start_sb1();
      }
    }
  }
}
