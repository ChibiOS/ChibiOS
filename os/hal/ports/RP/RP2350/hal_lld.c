/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    RP2350/hal_lld.c
 * @brief   RP2350 HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

#include <stdio.h>
#include "pico/stdlib.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_rp2040.h.
 */
uint32_t SystemCoreClock;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if RP_CORE1_START == TRUE
/* Courtesy of Pico-SDK.*/
static void start_core1(void) {

  //multicore_launch_core1(); //from picosdk multicore.c/h, no work without adjusting pico-sdk.2350.mk a bunch, testing and more.
  extern uint32_t RP_CORE1_STACK_END;
  extern uint32_t RP_CORE1_VECTORS_TABLE;
  extern void RP_CORE1_ENTRY_POINT(void);
  static const uint32_t cmd_sequence[] = {0, 0, 1,
                             (uint32_t)&RP_CORE1_VECTORS_TABLE,
                             (uint32_t)&RP_CORE1_STACK_END,
                             (uint32_t)RP_CORE1_ENTRY_POINT};

  return; //skip the below code for now, as it is not working yet.
//   unsigned seq;

// #if 0
//   /* Resetting core1.*/
//   PSM_SET->FRCE_OFF = PSM_ANY_PROC1;
//   while ((PSM->FRCE_OFF & PSM_ANY_PROC1) == 0U) {
//   }
//   PSM_CLR->FRCE_OFF = PSM_ANY_PROC1;
// #endif

//   /* Starting core 1.*/
//   seq = 0;
//   return;// fifoFlushRead still not working, so we just skip teh entire below bit..
//   do {
//     uint32_t response;
//     uint32_t cmd = cmd_sequence[seq];

//     /* Flushing the FIFO state before sending a zero.*/
//     if (!cmd) {
//       fifoFlushRead();
//     }
//     //fifoBlockingWrite(cmd);  //this is a blocking call and prevents booting core1 right now
//     response = 0;//fifoBlockingRead(); //this is a blocking call and prevents booting core1 right now
//     /* Checking response, going forward or back to first step.*/
//     seq = cmd == response ? seq + 1U : 0U;
//   } while (seq < count_of(cmd_sequence));

    uint seq = 0;
    do {
        uint cmd = cmd_sequence[seq];
        // we drain before sending a 0
        if (!cmd) {
            //multicore_fifo_drain();
            fifoFlushRead();
            //__sev(); // core 1 may be waiting for fifo space
            __ASM volatile ("sev");

        }
        //multicore_fifo_push_blocking(cmd);
        fifoBlockingWrite(cmd);
        //uint32_t response = multicore_fifo_pop_blocking();
        uint32_t response = fifoBlockingRead();
        // move to next state on correct response otherwise start over
        seq = cmd == response ? seq + 1 : 0;
    } while (seq < count_of(cmd_sequence));

}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if RP_NO_INIT == FALSE
  //clocks_init(); //sdk1
  //runtime_init_clocks();//no worky? missing includes.? buzz todo karl had this line in.
  //stdio_init_all(); //init all the pico-sdk.

  SystemCoreClock = RP_CORE_CLK;

   // buzz 
  hal_lld_peripheral_unreset(RESETS_ALLREG_BUSCTRL);
  hal_lld_peripheral_unreset(RESETS_ALLREG_SYSINFO);
  hal_lld_peripheral_unreset(RESETS_ALLREG_SYSCFG);


#endif /* RP_NO_INIT */

  /* Common subsystems initialization.*/
  irqInit();
#if defined(RP_DMA_REQUIRED)
  dmaInit();
#endif

#if RP_CORE1_START == TRUE
  start_core1();
#endif
}

/** @} */
