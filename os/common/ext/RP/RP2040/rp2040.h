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

#define UART_UARTLCR_H_SPS_Pos          7U
#define UART_UARTLCR_H_SPS_Msk          (1U << UART_UARTLCR_H_SPS_Pos)
#define UART_UARTLCR_H_SPS              UART_UARTLCR_H_SPS_Msk
#define UART_UARTLCR_H_WLEN_Pos         5U
#define UART_UARTLCR_H_WLEN_Msk         (1U << UART_UARTLCR_H_WLEN_Pos)
#define UART_UARTLCR_H_WLEN(n)          ((n) << UART_UARTLCR_H_WLEN_Pos)
#define UART_UARTLCR_H_WLEN_5BITS       UART_UARTLCR_H_WLEN(0U)
#define UART_UARTLCR_H_WLEN_6BITS       UART_UARTLCR_H_WLEN(1U)
#define UART_UARTLCR_H_WLEN_7BITS       UART_UARTLCR_H_WLEN(2U)
#define UART_UARTLCR_H_WLEN_8BITS       UART_UARTLCR_H_WLEN(3U)
#define UART_UARTLCR_H_FEN_Pos          4U
#define UART_UARTLCR_H_FEN_Msk          (1U << UART_UARTLCR_H_FEN_Pos)
#define UART_UARTLCR_H_FEN              UART_UARTLCR_H_FEN_Msk
#define UART_UARTLCR_H_STP2_Pos         3U
#define UART_UARTLCR_H_STP2_Msk         (1U << UART_UARTLCR_H_STP2_Pos)
#define UART_UARTLCR_H_STP2             UART_UARTLCR_H_STP2_Msk
#define UART_UARTLCR_H_EPS_Pos          2U
#define UART_UARTLCR_H_EPS_Msk          (1U << UART_UARTLCR_H_EPS_Pos)
#define UART_UARTLCR_H_EPS              UART_UARTLCR_H_EPS_Msk
#define UART_UARTLCR_H_PEN_Pos          1U
#define UART_UARTLCR_H_PEN_Msk          (1U << UART_UARTLCR_H_PEN_Pos)
#define UART_UARTLCR_H_PEN              UART_UARTLCR_H_PEN_Msk
#define UART_UARTLCR_H_BRK_Pos          0U
#define UART_UARTLCR_H_BRK_Msk          (1U << UART_UARTLCR_H_BRK_Pos)
#define UART_UARTLCR_H_BRK              UART_UARTLCR_H_BRK_Msk

#define UART_UARTCR_CTSEN_Pos           15U
#define UART_UARTCR_CTSEN_Msk           (1U << UART_UARTCR_CTSEN_Pos)
#define UART_UARTCR_CTSEN               UART_UARTCR_CTSEN_Msk
#define UART_UARTCR_RTSEN_Pos           14U
#define UART_UARTCR_RTSEN_Msk           (1U << UART_UARTCR_RTSEN_Pos)
#define UART_UARTCR_RTSEN               UART_UARTCR_RTSEN_Msk
#define UART_UARTCR_OUT2_Pos            13U
#define UART_UARTCR_OUT2_Msk            (1U << UART_UARTCR_OUT2_Pos)
#define UART_UARTCR_OUT2                UART_UARTCR_OUT2_Msk
#define UART_UARTCR_OUT1_Pos            12U
#define UART_UARTCR_OUT1_Msk            (1U << UART_UARTCR_OUT1_Pos)
#define UART_UARTCR_OUT1                UART_UARTCR_OUT1_Msk
#define UART_UARTCR_RTS_Pos             11U
#define UART_UARTCR_RTS_Msk             (1U << UART_UARTCR_RTS_Pos)
#define UART_UARTCR_RTS                 UART_UARTCR_RTS_Msk
#define UART_UARTCR_DTR_Pos             10U
#define UART_UARTCR_DTR_Msk             (1U << UART_UARTCR_DTR_Pos)
#define UART_UARTCR_DTR                 UART_UARTCR_DTR_Msk
#define UART_UARTCR_RXE_Pos             9U
#define UART_UARTCR_RXE_Msk             (1U << UART_UARTCR_RXE_Pos)
#define UART_UARTCR_RXE                 UART_UARTCR_RXE_Msk
#define UART_UARTCR_TXE_Pos             8U
#define UART_UARTCR_TXE_Msk             (1U << UART_UARTCR_TXE_Pos)
#define UART_UARTCR_TXE                 UART_UARTCR_TXE_Msk
#define UART_UARTCR_LBE_Pos             7U
#define UART_UARTCR_LBE_Msk             (1U << UART_UARTCR_LBE_Pos)
#define UART_UARTCR_LBE                 UART_UARTCR_LBE_Msk
#define UART_UARTCR_SIRLP_Pos           2U
#define UART_UARTCR_SIRLP_Msk           (1U << UART_UARTCR_SIRLP_Pos)
#define UART_UARTCR_SIRLP               UART_UARTCR_SIRLP_Msk
#define UART_UARTCR_SIREN_Pos           1U
#define UART_UARTCR_SIREN_Msk           (1U << UART_UARTCR_SIREN_Pos)
#define UART_UARTCR_SIREN               UART_UARTCR_SIREN_Msk
#define UART_UARTCR_UARTEN_Pos          0U
#define UART_UARTCR_UARTEN_Msk          (1U << UART_UARTCR_UARTEN_Pos)
#define UART_UARTCR_UARTEN              UART_UARTCR_UARTEN_Msk

#define UART_UARTIFLS_RXIFLSEL_Pos      3U
#define UART_UARTIFLS_RXIFLSEL_Msk      (1U << UART_UARTIFLS_RXIFLSEL_Pos)
#define UART_UARTIFLS_RXIFLSEL(n)       ((n) << UART_UARTIFLS_RXIFLSEL_Pos)
#define UART_UARTIFLS_RXIFLSEL_1_8F     UART_UARTIFLS_RXIFLSEL(0U)
#define UART_UARTIFLS_RXIFLSEL_1_4F     UART_UARTIFLS_RXIFLSEL(1U)
#define UART_UARTIFLS_RXIFLSEL_1_2F     UART_UARTIFLS_RXIFLSEL(2U)
#define UART_UARTIFLS_RXIFLSEL_3_4F     UART_UARTIFLS_RXIFLSEL(3U)
#define UART_UARTIFLS_RXIFLSEL_7_8F     UART_UARTIFLS_RXIFLSEL(4U)

#define UART_UARTIFLS_TXIFLSEL_Pos      3U
#define UART_UARTIFLS_TXIFLSEL_Msk      (1U << UART_UARTIFLS_TXIFLSEL_Pos)
#define UART_UARTIFLS_TXIFLSEL(n)       ((n) << UART_UARTIFLS_TXIFLSEL_Pos)
#define UART_UARTIFLS_TXIFLSEL_1_8E     UART_UARTIFLS_TXIFLSEL(0U)
#define UART_UARTIFLS_TXIFLSEL_1_4E     UART_UARTIFLS_TXIFLSEL(1U)
#define UART_UARTIFLS_TXIFLSEL_1_2E     UART_UARTIFLS_TXIFLSEL(2U)
#define UART_UARTIFLS_TXIFLSEL_3_4E     UART_UARTIFLS_TXIFLSEL(3U)
#define UART_UARTIFLS_TXIFLSEL_7_8E     UART_UARTIFLS_TXIFLSEL(4U)

#define UART_UARTIMSC_OEIM_Pos          10U
#define UART_UARTIMSC_OEIM_Msk          (1U << UART_UARTIMSC_OEIM_Pos)
#define UART_UARTIMSC_OEIM              UART_UARTIMSC_OEIM_Msk
#define UART_UARTIMSC_BEIM_Pos          9U
#define UART_UARTIMSC_BEIM_Msk          (1U << UART_UARTIMSC_BEIM_Pos)
#define UART_UARTIMSC_BEIM              UART_UARTIMSC_BEIM_Msk
#define UART_UARTIMSC_PEIM_Pos          8U
#define UART_UARTIMSC_PEIM_Msk          (1U << UART_UARTIMSC_PEIM_Pos)
#define UART_UARTIMSC_PEIM              UART_UARTIMSC_PEIM_Msk
#define UART_UARTIMSC_FEIM_Pos          7U
#define UART_UARTIMSC_FEIM_Msk          (1U << UART_UARTIMSC_FEIM_Pos)
#define UART_UARTIMSC_FEIM              UART_UARTIMSC_FEIM_Msk
#define UART_UARTIMSC_RTIM_Pos          6U
#define UART_UARTIMSC_RTIM_Msk          (1U << UART_UARTIMSC_RTIM_Pos)
#define UART_UARTIMSC_RTIM              UART_UARTIMSC_RTIM_Msk
#define UART_UARTIMSC_TXIM_Pos          5U
#define UART_UARTIMSC_TXIM_Msk          (1U << UART_UARTIMSC_TXIM_Pos)
#define UART_UARTIMSC_TXIM              UART_UARTIMSC_TXIM_Msk
#define UART_UARTIMSC_RXIM_Pos          4U
#define UART_UARTIMSC_RXIM_Msk          (1U << UART_UARTIMSC_RXIM_Pos)
#define UART_UARTIMSC_RXIM              UART_UARTIMSC_RXIM_Msk
#define UART_UARTIMSC_DSRMIM_Pos        3U
#define UART_UARTIMSC_DSRMIM_Msk        (1U << UART_UARTIMSC_DSRMIM_Pos)
#define UART_UARTIMSC_DSRMIM            UART_UARTIMSC_DSRMIM_Msk
#define UART_UARTIMSC_DCDMIM_Pos        2U
#define UART_UARTIMSC_DCDMIM_Msk        (1U << UART_UARTIMSC_DCDMIM_Pos)
#define UART_UARTIMSC_DCDMIM            UART_UARTIMSC_DCDMIM_Msk
#define UART_UARTIMSC_CTSMIM_Pos        1U
#define UART_UARTIMSC_CTSMIM_Msk        (1U << UART_UARTIMSC_CTSMIM_Pos)
#define UART_UARTIMSC_CTSMIM            UART_UARTIMSC_CTSMIM_Msk
#define UART_UARTIMSC_RIMIM_Pos         0U
#define UART_UARTIMSC_RIMIM_Msk         (1U << UART_UARTIMSC_RIMIM_Pos)
#define UART_UARTIMSC_RIMIM             UART_UARTIMSC_RIMIM_Msk

#define UART_UARTRIS_OERIS_Pos          10U
#define UART_UARTRIS_OERIS_Msk          (1U << UART_UARTRIS_OERIS_Pos)
#define UART_UARTRIS_OERIS              UART_UARTRIS_OERIS_Msk
#define UART_UARTRIS_BERIS_Pos          9U
#define UART_UARTRIS_BERIS_Msk          (1U << UART_UARTRIS_BERIS_Pos)
#define UART_UARTRIS_BERIS              UART_UARTRIS_BERIS_Msk
#define UART_UARTRIS_PERIS_Pos          8U
#define UART_UARTRIS_PERIS_Msk          (1U << UART_UARTRIS_PERIS_Pos)
#define UART_UARTRIS_PERIS              UART_UARTRIS_PERIS_Msk
#define UART_UARTRIS_FERIS_Pos          7U
#define UART_UARTRIS_FERIS_Msk          (1U << UART_UARTRIS_FERIS_Pos)
#define UART_UARTRIS_FERIS              UART_UARTRIS_FERIS_Msk
#define UART_UARTRIS_RTRIS_Pos          6U
#define UART_UARTRIS_RTRIS_Msk          (1U << UART_UARTRIS_RTRIS_Pos)
#define UART_UARTRIS_RTRIS              UART_UARTRIS_RTRIS_Msk
#define UART_UARTRIS_TXRIS_Pos          5U
#define UART_UARTRIS_TXRIS_Msk          (1U << UART_UARTRIS_TXRIS_Pos)
#define UART_UARTRIS_TXRIS              UART_UARTRIS_TXRIS_Msk
#define UART_UARTRIS_RXRIS_Pos          4U
#define UART_UARTRIS_RXRIS_Msk          (1U << UART_UARTRIS_RXRIS_Pos)
#define UART_UARTRIS_RXRIS              UART_UARTRIS_RXRIS_Msk
#define UART_UARTRIS_DSRRMIS_Pos        3U
#define UART_UARTRIS_DSRRMIS_Msk        (1U << UART_UARTRIS_DSRRMIS_Pos)
#define UART_UARTRIS_DSRRMIS            UART_UARTRIS_DSRRMIS_Msk
#define UART_UARTRIS_DCDRMIS_Pos        2U
#define UART_UARTRIS_DCDRMIS_Msk        (1U << UART_UARTRIS_DCDRMIS_Pos)
#define UART_UARTRIS_DCDRMIS            UART_UARTRIS_DCDRMIS_Msk
#define UART_UARTRIS_CTSRMIS_Pos        1U
#define UART_UARTRIS_CTSRMIS_Msk        (1U << UART_UARTRIS_CTSRMIS_Pos)
#define UART_UARTRIS_CTSRMIS            UART_UARTRIS_CTSRMIS_Msk
#define UART_UARTRIS_RIRMIS_Pos         0U
#define UART_UARTRIS_RIRMIS_Msk         (1U << UART_UARTRIS_RIRMIS_Pos)
#define UART_UARTRIS_RIRMIS             UART_UARTIMSC_RIRMIS_Msk

#define UART_UARTICR_OEIC_Pos           10U
#define UART_UARTICR_OEIC_Msk           (1U << UART_UARTICR_OEIC_Pos)
#define UART_UARTICR_OEIM               UART_UARTICR_OEIC_Msk
#define UART_UARTICR_BEIC_Pos           9U
#define UART_UARTICR_BEIC_Msk           (1U << UART_UARTICR_BEIC_Pos)
#define UART_UARTICR_BEIM               UART_UARTICR_BEIC_Msk
#define UART_UARTICR_PEIC_Pos           8U
#define UART_UARTICR_PEIC_Msk           (1U << UART_UARTICR_PEIC_Pos)
#define UART_UARTICR_PEIM               UART_UARTICR_PEIC_Msk
#define UART_UARTICR_FEIC_Pos           7U
#define UART_UARTICR_FEIC_Msk           (1U << UART_UARTICR_FEIC_Pos)
#define UART_UARTICR_FEIM               UART_UARTICR_FEIC_Msk
#define UART_UARTICR_RTIC_Pos           6U
#define UART_UARTICR_RTIC_Msk           (1U << UART_UARTICR_RTIC_Pos)
#define UART_UARTICR_RTIM               UART_UARTICR_RTIC_Msk
#define UART_UARTICR_TXIC_Pos           5U
#define UART_UARTICR_TXIC_Msk           (1U << UART_UARTICR_TXIC_Pos)
#define UART_UARTICR_TXIM               UART_UARTICR_TXIC_Msk
#define UART_UARTICR_RXIC_Pos           4U
#define UART_UARTICR_RXIC_Msk           (1U << UART_UARTICR_RXIC_Pos)
#define UART_UARTICR_RXIM               UART_UARTICR_RXIC_Msk
#define UART_UARTICR_DSRMIC_Pos         3U
#define UART_UARTICR_DSRMIC_Msk         (1U << UART_UARTICR_DSRMIC_Pos)
#define UART_UARTICR_DSRMIM             UART_UARTICR_DSRMIC_Msk
#define UART_UARTICR_DCDMIC_Pos         2U
#define UART_UARTICR_DCDMIC_Msk         (1U << UART_UARTICR_DCDMIC_Pos)
#define UART_UARTICR_DCDMIM             UART_UARTICR_DCDMIC_Msk
#define UART_UARTICR_CTSMIC_Pos         1U
#define UART_UARTICR_CTSMIC_Msk         (1U << UART_UARTICR_CTSMIC_Pos)
#define UART_UARTICR_CTSMIM             UART_UARTICR_CTSMIC_Msk
#define UART_UARTICR_RIMIC_Pos          0U
#define UART_UARTICR_RIMIC_Msk          (1U << UART_UARTICR_RIMIC_Pos)
#define UART_UARTICR_RIMIM              UART_UARTICR_RIMIC_Msk

#define UART_UARTDMACR_DMAONERR_Pos     2U
#define UART_UARTDMACR_DMAONERR_Msk     (1U << UART_UARTDMACR_DMAONERR_Pos)
#define UART_UARTDMACR_DMAONERR         UART_UARTDMACR_DMAONERR_Msk
#define UART_UARTDMACR_TXDMAE_Pos       1U
#define UART_UARTDMACR_TXDMAE_Msk       (1U << UART_UARTDMACR_TXDMAE_Pos)
#define UART_UARTDMACR_TXDMAE           UART_UARTDMACR_TXDMAE_Msk
#define UART_UARTDMACR_RXDMAE_Pos       0U
#define UART_UARTDMACR_RXDMAE_Msk       (1U << UART_UARTDMACR_RXDMAE_Pos)
#define UART_UARTDMACR_RXDMAE           UART_UARTDMACR_RXDMAE_Msk
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RP2040_H */

/** @} */
