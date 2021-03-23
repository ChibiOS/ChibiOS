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
 * @file    RP/rp2040.h
 * @brief   CMSIS device file for the RP2040.
 *
 * @defgroup CMSIS_RP2040 RP2040 Device File
 * @ingroup CMSIS_DEVICE
 * @{
 */
    
#ifndef RP2040_H
#define RP2040_H

#include <stdint.h>

#define __CM0PLUS_REV                   0U
#define __MPU_PRESENT                   0U
#define __VTOR_PRESENT                  1U
#define __NVIC_PRIO_BITS                2U
#define __Vendor_SysTickConfig          0U

/**
 * @brief   Interrupt vector numbers.
 */
typedef enum {
  NonMaskableInt_IRQn                   = -14,
  HardFault_IRQn                        = -13,
  SVC_IRQn                              = -5,
  PendSV_IRQn                           = -2,
  SysTick_IRQn                          = -1,
  Vector_0n                             = 0,
  Vector_1n                             = 1,
  Vector_2n                             = 2,
  Vector_3n                             = 3,
  Vector_4n                             = 4,
  Vector_5n                             = 5,
  Vector_6n                             = 6,
  Vector_7n                             = 7,
  Vector_8n                             = 8,
  Vector_9n                             = 9,
  Vector_10n                            = 10,
  Vector_11n                            = 11,
  Vector_12n                            = 12,
  Vector_13n                            = 13,
  Vector_14n                            = 14,
  Vector_15n                            = 15,
  Vector_16n                            = 16,
  Vector_17n                            = 17,
  Vector_18n                            = 18,
  Vector_19n                            = 19,
  Vector_20n                            = 20,
  Vector_21n                            = 21,
  Vector_22n                            = 22,
  Vector_23n                            = 23,
  Vector_24n                            = 24,
  Vector_25n                            = 25,
  Vector_26n                            = 26,
  Vector_27n                            = 27,
  Vector_28n                            = 28,
  Vector_29n                            = 29,
  Vector_30n                            = 30,
  Vector_31n                            = 31
} IRQn_Type;

#include "core_cm0plus.h"
#include "system_rp2040.h"

/**
 * @name    Peripheral structures
 * @{
 */
typedef struct {
  __IO uint32_t     TIMEHW;
  __IO uint32_t     TIMELW;
  __I  uint32_t     TIMEHR;
  __I  uint32_t     TIMELR;
  __IO uint32_t     ALARM0;
  __IO uint32_t     ALARM1;
  __IO uint32_t     ALARM2;
  __IO uint32_t     ALARM3;
  __IO uint32_t     ARMED;
  __I  uint32_t     TIMERAWH;
  __I  uint32_t     TIMERAWL;
  __IO uint32_t     DBGPAUSE;
  __IO uint32_t     PAUSE;
  __IO uint32_t     INTR;
  __IO uint32_t     INTE;
  __IO uint32_t     INTF;
  __I  uint32_t     INTS;
} TIMER_TypeDef;

typedef struct {
  __IO uint32_t     UARTDR;
  __IO uint32_t     UARTRSR;
  __I  uint32_t     resvd8;
  __I  uint32_t     resvdC;
  __I  uint32_t     resvd10;
  __I  uint32_t     resvd14;
  __I  uint32_t     UARTFR;
  __I  uint32_t     resvd1C;
  __IO uint32_t     UARTILPR;
  __IO uint32_t     UARTIBRD;
  __IO uint32_t     UARTFBRD;
  __IO uint32_t     UARTLCR_H;
  __IO uint32_t     UARTCR;
  __IO uint32_t     UARTIFLS;
  __IO uint32_t     UARTIMSC;
  __I  uint32_t     UARTRIS;
  __I  uint32_t     UARTMIS;
  __IO uint32_t     UARTICR;
  __IO uint32_t     UARTDMACR;
  __I  uint32_t     resvd4C[997];
  __I  uint32_t     UARTPERIPHID0;
  __I  uint32_t     UARTPERIPHID1;
  __I  uint32_t     UARTPERIPHID2;
  __I  uint32_t     UARTPERIPHID3;
  __I  uint32_t     UARTPCELLID0;
  __I  uint32_t     UARTPCELLID1;
  __I  uint32_t     UARTPCELLID2;
  __I  uint32_t     UARTPCELLID3;
} UART_TypeDef;
/** @} */

/**
 * @name    Base addresses
 * @{
 */
#define APBPERIPH_BASE                  0x40000000U
#define AHBPERIPH_BASE                  0x50000000U
#define __IOPORT_BASE                   0xA0000000U
#define __TIMER_BASE                    (APBPERIPH_BASE + 0x00054000)
#define __UART0_BASE                    (APBPERIPH_BASE + 0x00034000)
#define __UART1_BASE                    (APBPERIPH_BASE + 0x00038000)
/** @} */

/**
 * @name    Peripherals
 * @{
 */
#define TIMER                           ((TIMER_TypeDef *)__TIMER_BASE)
#define UART0                           ((UART_TypeDef *)__UART0_BASE)
#define UART1                           ((UART_TypeDef *)__UART1_BASE)
/** @} */

/**
 * @name    TIMER bits definitions
 */
#define TIMER_ARMED_ALARM0_Pos          0U
#define TIMER_ARMED_ALARM0_Msk          (1U << TIMER_ARMED_ALARM0_Pos)
#define TIMER_ARMED_ALARM0              TIMER_ARMED_ALARM0_Msk
#define TIMER_ARMED_ALARM1_Pos          1U
#define TIMER_ARMED_ALARM1_Msk          (1U << TIMER_ARMED_ALARM1_Pos)
#define TIMER_ARMED_ALARM1              TIMER_ARMED_ALARM1_Msk
#define TIMER_ARMED_ALARM2_Pos          2U
#define TIMER_ARMED_ALARM2_Msk          (1U << TIMER_ARMED_ALARM2_Pos)
#define TIMER_ARMED_ALARM2              TIMER_ARMED_ALARM2_Msk
#define TIMER_ARMED_ALARM3_Pos          3U
#define TIMER_ARMED_ALARM3_Msk          (1U << TIMER_ARMED_ALARM3_Pos)
#define TIMER_ARMED_ALARM3              TIMER_ARMED_ALARM3_Msk

#define TIMER_DBGPAUSE_DBG0_Pos         1U
#define TIMER_DBGPAUSE_DBG0_Msk         (1U << TIMER_DBGPAUSE_DBG0_Pos)
#define TIMER_DBGPAUSE_DBG0             TIMER_DBGPAUSE_DBG0_Msk
#define TIMER_DBGPAUSE_DBG1_Pos         2U
#define TIMER_DBGPAUSE_DBG1_Msk         (1U << TIMER_DBGPAUSE_DBG1_Pos)
#define TIMER_DBGPAUSE_DBG1             TIMER_DBGPAUSE_DBG1_Msk

#define TIMER_PAUSE_PAUSE_Pos           0U
#define TIMER_PAUSE_PAUSE_Msk           (1U << TIMER_PAUSE_PAUSE_Pos)
#define TIMER_PAUSE_PAUSE               TIMER_PAUSE_PAUSE_Msk

#define TIMER_INTR_ALARM0_Pos           0U
#define TIMER_INTR_ALARM0_Msk           (1U << TIMER_INTR_ALARM0_Pos)
#define TIMER_INTR_ALARM0               TIMER_INTR_ALARM0_Msk
#define TIMER_INTR_ALARM1_Pos           1U
#define TIMER_INTR_ALARM1_Msk           (1U << TIMER_INTR_ALARM1_Pos)
#define TIMER_INTR_ALARM1               TIMER_INTR_ALARM1_Msk
#define TIMER_INTR_ALARM2_Pos           2U
#define TIMER_INTR_ALARM2_Msk           (1U << TIMER_INTR_ALARM2_Pos)
#define TIMER_INTR_ALARM2               TIMER_INTR_ALARM2_Msk
#define TIMER_INTR_ALARM3_Pos           3U
#define TIMER_INTR_ALARM3_Msk           (1U << TIMER_INTR_ALARM3_Pos)
#define TIMER_INTR_ALARM3               TIMER_INTR_ALARM3_Msk

#define TIMER_INTE_ALARM0_Pos           0U
#define TIMER_INTE_ALARM0_Msk           (1U << TIMER_INTE_ALARM0_Pos)
#define TIMER_INTE_ALARM0               TIMER_INTE_ALARM0_Msk
#define TIMER_INTE_ALARM1_Pos           1U
#define TIMER_INTE_ALARM1_Msk           (1U << TIMER_INTE_ALARM1_Pos)
#define TIMER_INTE_ALARM1               TIMER_INTE_ALARM1_Msk
#define TIMER_INTE_ALARM2_Pos           2U
#define TIMER_INTE_ALARM2_Msk           (1U << TIMER_INTE_ALARM2_Pos)
#define TIMER_INTE_ALARM2               TIMER_INTE_ALARM2_Msk
#define TIMER_INTE_ALARM3_Pos           3U
#define TIMER_INTE_ALARM3_Msk           (1U << TIMER_INTE_ALARM3_Pos)
#define TIMER_INTE_ALARM3               TIMER_INTE_ALARM3_Msk

#define TIMER_INTF_ALARM0_Pos           0U
#define TIMER_INTF_ALARM0_Msk           (1U << TIMER_INTF_ALARM0_Pos)
#define TIMER_INTF_ALARM0               TIMER_INTF_ALARM0_Msk
#define TIMER_INTF_ALARM1_Pos           1U
#define TIMER_INTF_ALARM1_Msk           (1U << TIMER_INTF_ALARM1_Pos)
#define TIMER_INTF_ALARM1               TIMER_INTF_ALARM1_Msk
#define TIMER_INTF_ALARM2_Pos           2U
#define TIMER_INTF_ALARM2_Msk           (1U << TIMER_INTF_ALARM2_Pos)
#define TIMER_INTF_ALARM2               TIMER_INTF_ALARM2_Msk
#define TIMER_INTF_ALARM3_Pos           3U
#define TIMER_INTF_ALARM3_Msk           (1U << TIMER_INTF_ALARM3_Pos)
#define TIMER_INTF_ALARM3               TIMER_INTF_ALARM3_Msk

#define TIMER_INTS_ALARM0_Pos           0U
#define TIMER_INTS_ALARM0_Msk           (1U << TIMER_INTS_ALARM0_Pos)
#define TIMER_INTS_ALARM0               TIMER_INTS_ALARM0_Msk
#define TIMER_INTS_ALARM1_Pos           1U
#define TIMER_INTS_ALARM1_Msk           (1U << TIMER_INTS_ALARM1_Pos)
#define TIMER_INTS_ALARM1               TIMER_INTS_ALARM1_Msk
#define TIMER_INTS_ALARM2_Pos           2U
#define TIMER_INTS_ALARM2_Msk           (1U << TIMER_INTS_ALARM2_Pos)
#define TIMER_INTS_ALARM2               TIMER_INTS_ALARM2_Msk
#define TIMER_INTS_ALARM3_Pos           3U
#define TIMER_INTS_ALARM3_Msk           (1U << TIMER_INTS_ALARM3_Pos)
#define TIMER_INTS_ALARM3               TIMER_INTS_ALARM3_Msk
/** @} */

/**
 * @name    UART bits definitions
 */
#define UART_UARTDR_OE_Pos              11U
#define UART_UARTDR_OE_Msk              (1U << UART_UARTDR_OE_Pos)
#define UART_UARTDR_OE                  UART_UARTDR_OE_Msk
#define UART_UARTDR_BE_Pos              10U
#define UART_UARTDR_BE_Msk              (1U << UART_UARTDR_BE_Pos)
#define UART_UARTDR_BE                  UART_UARTDR_BE_Msk
#define UART_UARTDR_PE_Pos              9U
#define UART_UARTDR_PE_Msk              (1U << UART_UARTDR_PE_Pos)
#define UART_UARTDR_PE                  UART_UARTDR_PE_Msk
#define UART_UARTDR_FE_Pos              8U
#define UART_UARTDR_FE_Msk              (1U << UART_UARTDR_FE_Pos)
#define UART_UARTDR_FE                  UART_UARTDR_FE_Msk

#define UART_UARTRSR_OE_Pos             3U
#define UART_UARTRSR_OE_Msk             (1U << UART_UARTRSR_OE_Pos)
#define UART_UARTRSR_OE                 UART_UARTRSR_OE_Msk
#define UART_UARTRSR_BE_Pos             2U
#define UART_UARTRSR_BE_Msk             (1U << UART_UARTRSR_BE_Pos)
#define UART_UARTRSR_BE                 UART_UARTRSR_BE_Msk
#define UART_UARTRSR_PE_Pos             1U
#define UART_UARTRSR_PE_Msk             (1U << UART_UARTRSR_PE_Pos)
#define UART_UARTRSR_PE                 UART_UARTRSR_PE_Msk
#define UART_UARTRSR_FE_Pos             0U
#define UART_UARTRSR_FE_Msk             (1U << UART_UARTRSR_FE_Pos)
#define UART_UARTRSR_FE                 UART_UARTRSR_FE_Msk

#define UART_UARTFR_RI_Pos              8U
#define UART_UARTFR_RI_Msk              (1U << UART_UARTFR_RI_Pos)
#define UART_UARTFR_RI                  UART_UARTFR_RI_Msk
#define UART_UARTFR_TXFE_Pos            7U
#define UART_UARTFR_TXFE_Msk            (1U << UART_UARTFR_TXFE_Pos)
#define UART_UARTFR_TXFE                UART_UARTFR_TXFE_Msk
#define UART_UARTFR_RXFF_Pos            6U
#define UART_UARTFR_RXFF_Msk            (1U << UART_UARTFR_RXFF_Pos)
#define UART_UARTFR_RXFF                UART_UARTFR_RXFF_Msk
#define UART_UARTFR_TXFF_Pos            5U
#define UART_UARTFR_TXFF_Msk            (1U << UART_UARTFR_TXFF_Pos)
#define UART_UARTFR_TXFF                UART_UARTFR_TXFF_Msk
#define UART_UARTFR_RXFE_Pos            4U
#define UART_UARTFR_RXFE_Msk            (1U << UART_UARTFR_RXFE_Pos)
#define UART_UARTFR_RXFE                UART_UARTFR_RXFE_Msk
#define UART_UARTFR_BUSY_Pos            3U
#define UART_UARTFR_BUSY_Msk            (1U << UART_UARTFR_BUSY_Pos)
#define UART_UARTFR_BUSY                UART_UARTFR_BUSY_Msk
#define UART_UARTFR_DCD_Pos             2U
#define UART_UARTFR_DCD_Msk             (1U << UART_UARTFR_DCD_Pos)
#define UART_UARTFR_DCD                 UART_UARTFR_DCD_Msk
#define UART_UARTFR_DSR_Pos             1U
#define UART_UARTFR_DSR_Msk             (1U << UART_UARTFR_DSR_Pos)
#define UART_UARTFR_DSR                 UART_UARTFR_DSR_Msk
#define UART_UARTFR_CTS_Pos             0U
#define UART_UARTFR_CTS_Msk             (1U << UART_UARTFR_CTS_Pos)
#define UART_UARTFR_CTS                 UART_UARTFR_CTS_Msk

#define UART_UARTILPR_ILPDVSR_Pos       0U
#define UART_UARTILPR_ILPDVSR_Msk       (255U << UART_UARTILPR_ILPDVSR_Pos)
#define UART_UARTILPR_ILPDVSR(n)        ((n) << UART_UARTILPR_ILPDVSR_Pos)

#define UART_UARTIBRD_BAUD_DIVINT_Pos   0U
#define UART_UARTIBRD_BAUD_DIVINT_Msk   (0xFFFFU << UART_UARTIBRD_BAUD_DIVINT_Pos)
#define UART_UARTIBRD_BAUD_DIVINT(n)    ((n) << UART_UARTIBRD_BAUD_DIVINT_Pos)

#define UART_UARTFBRD_BAUD_DIVFRAC_Pos  0U
#define UART_UARTFBRD_BAUD_DIVFRAC_Msk  (63U << UART_UARTFBRD_BAUD_DIVFRAC_Pos)
#define UART_UARTFBRD_BAUD_DIVFRAC(n)   ((n) << UART_UARTFBRD_BAUD_DIVFRAC_Pos)
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RP2040_H */

/** @} */
