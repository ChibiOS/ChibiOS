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
 * @file    TIMv1/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"
#include "hardware/timer.h" // for timer0_hw


#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
#define ST_HANDLER                          SysTick_Handler
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
#define ST_HANDLER                          SysTick_Handler
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

typedef struct {
  uint32_t n;
  uint32_t prio;
} alarm_irq_t;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
#if (ST_LLD_NUM_ALARMS > 1) || defined(__DOXYGEN__)
static const alarm_irq_t alarm_irqs[ST_LLD_NUM_ALARMS] = {
  {RP_TIMER0_IRQ_0_NUMBER, RP_IRQ_TIMER0_ALARM0_PRIORITY},
  {RP_TIMER0_IRQ_1_NUMBER, RP_IRQ_TIMER0_ALARM1_PRIORITY},
  {RP_TIMER0_IRQ_2_NUMBER, RP_IRQ_TIMER0_ALARM2_PRIORITY},
  {RP_TIMER0_IRQ_3_NUMBER, RP_IRQ_TIMER0_ALARM3_PRIORITY}
};
#endif
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
#if !defined(ST_SYSTICK_SUPPRESS_ISR)
/**
 * @brief   SysTick interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SysTick_Handler) {

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
#if !defined(ST_TIMER_ALARM0_SUPPRESS_ISR)
/**
 * @brief   TIMER alarm 0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_TIMER0_IRQ_0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  osalDbgAssert((timer0_hw->ints & TIMER_INTS_ALARM_0_BITS) != 0U, "not pending");

  timer0_hw->intr = TIMER_INTR_ALARM_0_BITS;

#if defined(ST_LLD_ALARM0_STATIC_CB)
  ST_LLD_ALARM0_STATIC_CB();
#else
  if (st_callbacks[0] != NULL) {
    st_callbacks[0](0U);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(ST_TIMER_ALARM1_SUPPRESS_ISR)
/**
 * @brief   TIMER alarm 1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_TIMER0_IRQ_1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  osalDbgAssert((timer0_hw->ints & TIMER_INTS_ALARM_1_BITS) != 0U, "not pending");

  timer0_hw->intr = TIMER_INTR_ALARM_1_BITS;

#if defined(ST_LLD_ALARM1_STATIC_CB)
  ST_LLD_ALARM1_STATIC_CB();
#else
  if (st_callbacks[1] != NULL) {
    st_callbacks[1](1U);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(ST_TIMER_ALARM2_SUPPRESS_ISR)
/**
 * @brief   TIMER alarm 2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_TIMER0_IRQ_2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  osalDbgAssert((timer0_hw->ints & TIMER_INTS_ALARM_2_BITS) != 0U, "not pending");

  timer0_hw->intr = TIMER_INTR_ALARM_2_BITS;

#if defined(ST_LLD_ALARM2_STATIC_CB)
  ST_LLD_ALARM2_STATIC_CB();
#else
  if (st_callbacks[2] != NULL) {
    st_callbacks[2](2U);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(ST_TIMER_ALARM3_SUPPRESS_ISR)
/**
 * @brief   TIMER alarm 3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_TIMER0_IRQ_3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  osalDbgAssert((timer0_hw->ints & TIMER_INTS_ALARM_3_BITS) != 0U, "not pending");

  timer0_hw->intr = TIMER_INTR_ALARM_3_BITS;

#if defined(ST_LLD_ALARM3_STATIC_CB)
  ST_LLD_ALARM3_STATIC_CB();
#else
  if (st_callbacks[3] != NULL) {
    st_callbacks[3](3U);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  /* The timer needs to stop during debug or the virtual timers list would
     go out of sync.*/
  timer0_hw->dbgpause   = TIMER_DBGPAUSE_DBG0_BITS | TIMER_DBGPAUSE_DBG1_BITS;

  /* Comparators and counter initially at zero.*/
  timer0_hw->timelw     = 0U;
  timer0_hw->timehw     = 0U;
  timer0_hw->alarm[0]   = 0U;
  timer0_hw->alarm[1]   = 0U;
  timer0_hw->alarm[2]   = 0U;
  timer0_hw->alarm[3]   = 0U;
  timer0_hw->inte       = 0U;
  timer0_hw->intr       = TIMER_INTR_ALARM_3_BITS | TIMER_INTR_ALARM_2_BITS |
                      TIMER_INTR_ALARM_1_BITS | TIMER_INTR_ALARM_0_BITS;

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */
}

/**
 * @brief   Enables an alarm interrupt on the invoking core.
 * @note    Must be called before any other alarm-related function.
 *
 * @notapi
 */
void st_lld_bind(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  nvicEnableVector(RP_TIMER0_IRQ_0_NUMBER, RP_IRQ_TIMER0_ALARM0_PRIORITY);
#endif
#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
  uint32_t  timer_clk = RP_CORE_CLK;

  osalDbgAssert(timer_clk % OSAL_ST_FREQUENCY == 0U,
                "division remainder");
  osalDbgAssert((timer_clk / OSAL_ST_FREQUENCY) - 1U <= 0x00FFFFFFU,
                "prescaler range");

  /* Periodic systick mode, the Cortex-Mx internal systick timer is used
     in this mode.*/
  SysTick->LOAD = (timer_clk / OSAL_ST_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, RP_IRQ_SYSTICK_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
#if (ST_LLD_NUM_ALARMS > 1) || defined(__DOXYGEN__)
/**
 * @brief   Enables an alarm interrupt on the invoking core.
 * @note    Must be called before any other alarm-related function.
 *
 * @param[in] alarm     alarm channel number (0..ST_LLD_NUM_ALARMS-1)
 *
 * @notapi
 */
void st_lld_bind_alarm_n(unsigned alarm) {

  nvicEnableVector(alarm_irqs[alarm].n, alarm_irqs[alarm].prio);
}
#endif /* ST_LLD_NUM_ALARMS > 1 */
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */
