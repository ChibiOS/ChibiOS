/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    RP/rp2350.h
 * @brief   CMSIS device file for the RP2350.
 *
 * @defgroup CMSIS_RP2350 RP2350 Device File
 * @ingroup CMSIS_DEVICE
 * @{
 */

#ifndef RP2350_H
#define RP2350_H

#include <stdint.h>

#define __CM33_REV                        1U 
#define __MPU_PRESENT                     1U
#define __VTOR_PRESENT                    1U
#define __NVIC_PRIO_BITS                  4U
#define __Vendor_SysTickConfig            0U
#define __FPU_PRESENT                     1U
#define __DSP_PRESENT                     1U
#define __SAU_PRESENT                     1U
#define __SAUREGION_PRESENT               8U

/**
 * @brief   Interrupt vector numbers.
 * @note    See RP2350 Datasheet 3.2 Interrupts (Table 95)
 */
typedef enum {
  NonMaskableInt_IRQn                   = -14,
  HardFault_IRQn                        = -13,
  MemoryManagement_IRQn                 = -12,
  BusFault_IRQn                         = -11,
  UsageFault_IRQn                       = -10,
  SecureFault_IRQn                      = -9,
  SVCall_IRQn                           = -5,
  DebugMonitor_IRQn                     = -4,
  PendSV_IRQn                           = -2,
  SysTick_IRQn                          = -1,
  TIMER0_IRQ_0n                         = 0,
  TIMER0_IRQ_1n                         = 1,
  TIMER0_IRQ_2n                         = 2,
  TIMER0_IRQ_3n                         = 3,
  TIMER1_IRQ_0n                         = 4,
  TIMER1_IRQ_1n                         = 5,
  TIMER1_IRQ_2n                         = 6,
  TIMER1_IRQ_3n                         = 7,
  PWM_IRQ_WRAP_0n                       = 8,
  PWM_IRQ_WRAP_1n                       = 9,
  DMA_IRQ_0n                            = 10,
  DMA_IRQ_1n                            = 11,
  DMA_IRQ_2n                            = 12,
  DMA_IRQ_3n                            = 13,
  USBCTRL_IRQn                          = 14,
  PIO0_IRQ_0n                           = 15,
  PIO0_IRQ_1n                           = 16,
  PIO1_IRQ_0n                           = 17,
  PIO1_IRQ_1n                           = 18,
  PIO2_IRQ_0n                           = 19,
  PIO2_IRQ_1n                           = 20,
  IO_IRQ_BANK0n                         = 21,
  IO_IRQ_BANK0_NSn                      = 22,
  IO_IRQ_QSPIn                          = 23,
  IO_IRQ_QSPI_NSn                       = 24,
  SIO_IRQ_FIFOn                         = 25,
  SIO_IRQ_BELLn                         = 26,
  SIO_IRQ_FIFO_NSn                      = 27,
  SIO_IRQ_BELL_NSn                      = 28,
  SIO_IRQ_MTIMECMPn                     = 29,
  CLOCKS_IRQn                           = 30,
  SPI0_IRQn                             = 31,
  SPI1_IRQn                             = 32,
  UART0_IRQn                            = 33,
  UART1_IRQn                            = 34,
  ADC_IRQ_FIFOn                         = 35,
  I2C0_IRQn                             = 36,
  I2C1_IRQn                             = 37,
  OTP_IRQn                              = 38,
  TRNG_IRQn                             = 39,
  PROC0_IRQ_CTIn                        = 40,
  PROC1_IRQ_CTIn                        = 41,
  PLL_SYS_IRQn                          = 42,
  PLL_USB_IRQn                          = 43,
  POWMAN_IRQ_POWn                       = 44,
  POWMAN_IRQ_TIMERn                     = 45,
  SPARE_IRQ_0n                          = 46,
  SPARE_IRQ_1n                          = 47,
  SPARE_IRQ_2n                          = 48,
  SPARE_IRQ_3n                          = 49,
  SPARE_IRQ_4n                          = 50,
  SPARE_IRQ_5n                          = 51
} IRQn_Type;

#define SVC_IRQn                          SVCall_IRQn

#include "core_cm33.h"
#include "system_rp2350.h"

/**
 * @name    Peripheral structures
 * @{
 */
typedef struct {
  __IO uint32_t         READ_ADDR;
  __IO uint32_t         WRITE_ADDR;
  __IO uint32_t         TRANS_COUNT;
  __IO uint32_t         CTRL_TRIG;
  __IO uint32_t         AL1_CTRL;
  __IO uint32_t         AL1_READ_ADDR;
  __IO uint32_t         AL1_WRITE_ADDR;
  __IO uint32_t         AL1_TRANSFER_COUNT_TRIG;
  __IO uint32_t         AL2_CTRL;
  __IO uint32_t         AL2_TRANSFER_COUNT;
  __IO uint32_t         AL2_READ_ADDR;
  __IO uint32_t         AL2_WRITE_ADDR_TRIG;
  __IO uint32_t         AL3_CTRL;
  __IO uint32_t         AL3_WRITE_ADDR;
  __IO uint32_t         AL3_TRANSFER_COUNT;
  __IO uint32_t         AL3_READ_ADDR_TRIG;
} DMA_Channel_TypeDef;

typedef struct {
  __I  uint32_t         CTDREQ;
  __I  uint32_t         TCR;
  __I  uint32_t         resvd8[14];
} DMA_Debug_TypeDef;

typedef struct {
  DMA_Channel_TypeDef   CH[16];
  __IO uint32_t         INTR;
  __IO uint32_t         INTE0;
  __IO uint32_t         INTF0;
  __IO uint32_t         INTS0;
  __IO uint32_t         INTR1;
  __IO uint32_t         INTE1;
  __IO uint32_t         INTF1;
  __IO uint32_t         INTS1;
  __IO uint32_t         INTR2;
  __IO uint32_t         INTE2;
  __IO uint32_t         INTF2;
  __IO uint32_t         INTS2;
  __IO uint32_t         INTR3;
  __IO uint32_t         INTE3;
  __IO uint32_t         INTF3;
  __IO uint32_t         INTS3;
  __IO uint32_t         TIMER[4];
  __IO uint32_t         MULTI_CHAN_TRIGGER;
  __IO uint32_t         SNIFF_CTRL;
  __IO uint32_t         SNIFF_DATA;
  __I  uint32_t         resvd45C;
  __I  uint32_t         FIFO_LEVELS;
  __IO uint32_t         CHAN_ABORT;
  __I  uint32_t         N_CHANNELS;
  __I  uint32_t         resvd46C[5];
  __IO uint32_t         SECCFG_CH[16];
  __IO uint32_t         SECCFG_IRQ[4];
  __IO uint32_t         SECCFG_MISC;
  __I  uint32_t         resvd4D4[203];
  DMA_Debug_TypeDef     CH_DBG[16];
  __I  uint32_t         resvdC00[256];
  struct {
    DMA_Channel_TypeDef CH[16];
    __IO uint32_t       INTR;
    __IO uint32_t       INTE0;
    __IO uint32_t       INTF0;
    __IO uint32_t       INTS0;
    __IO uint32_t       INTR1;
    __IO uint32_t       INTE1;
    __IO uint32_t       INTF1;
    __IO uint32_t       INTS1;
    __IO uint32_t       INTR2;
    __IO uint32_t       INTE2;
    __IO uint32_t       INTF2;
    __IO uint32_t       INTS2;
    __IO uint32_t       INTR3;
    __IO uint32_t       INTE3;
    __IO uint32_t       INTF3;
    __IO uint32_t       INTS3;
    __IO uint32_t       TIMER[4];
    __IO uint32_t       MULTI_CHAN_TRIGGER;
    __IO uint32_t       SNIFF_CTRL;
    __IO uint32_t       SNIFF_DATA;
    __I  uint32_t       resvd45C;
    __I  uint32_t       FIFO_LEVELS;
    __IO uint32_t       CHAN_ABORT;
    __I  uint32_t       N_CHANNELS;
    __I  uint32_t       resvd46C[5];
    __IO uint32_t       SECCFG_CH[16];
    __IO uint32_t       SECCFG_IRQ[4];
    __IO uint32_t       SECCFG_MISC;
    __I  uint32_t       resvd4D4[203];
    DMA_Debug_TypeDef   CH_DBG[16];
    __I  uint32_t       resvdC00[256];
  } XOR;
  struct {
    DMA_Channel_TypeDef CH[16];
    __IO uint32_t       INTR;
    __IO uint32_t       INTE0;
    __IO uint32_t       INTF0;
    __IO uint32_t       INTS0;
    __IO uint32_t       INTR1;
    __IO uint32_t       INTE1;
    __IO uint32_t       INTF1;
    __IO uint32_t       INTS1;
    __IO uint32_t       INTR2;
    __IO uint32_t       INTE2;
    __IO uint32_t       INTF2;
    __IO uint32_t       INTS2;
    __IO uint32_t       INTR3;
    __IO uint32_t       INTE3;
    __IO uint32_t       INTF3;
    __IO uint32_t       INTS3;
    __IO uint32_t       TIMER[4];
    __IO uint32_t       MULTI_CHAN_TRIGGER;
    __IO uint32_t       SNIFF_CTRL;
    __IO uint32_t       SNIFF_DATA;
    __I  uint32_t       resvd45C;
    __I  uint32_t       FIFO_LEVELS;
    __IO uint32_t       CHAN_ABORT;
    __I  uint32_t       N_CHANNELS;
    __I  uint32_t       resvd46C[5];
    __IO uint32_t       SECCFG_CH[16];
    __IO uint32_t       SECCFG_IRQ[4];
    __IO uint32_t       SECCFG_MISC;
    __I  uint32_t       resvd4D4[203];
    DMA_Debug_TypeDef   CH_DBG[16];
    __I  uint32_t       resvdC00[256];
  } SET;
  struct {
    DMA_Channel_TypeDef CH[16];
    __IO uint32_t       INTR;
    __IO uint32_t       INTE0;
    __IO uint32_t       INTF0;
    __IO uint32_t       INTS0;
    __IO uint32_t       INTR1;
    __IO uint32_t       INTE1;
    __IO uint32_t       INTF1;
    __IO uint32_t       INTS1;
    __IO uint32_t       INTR2;
    __IO uint32_t       INTE2;
    __IO uint32_t       INTF2;
    __IO uint32_t       INTS2;
    __IO uint32_t       INTR3;
    __IO uint32_t       INTE3;
    __IO uint32_t       INTF3;
    __IO uint32_t       INTS3;
    __IO uint32_t       TIMER[4];
    __IO uint32_t       MULTI_CHAN_TRIGGER;
    __IO uint32_t       SNIFF_CTRL;
    __IO uint32_t       SNIFF_DATA;
    __I  uint32_t       resvd45C;
    __I  uint32_t       FIFO_LEVELS;
    __IO uint32_t       CHAN_ABORT;
    __I  uint32_t       N_CHANNELS;
    __I  uint32_t       resvd46C[5];
    __IO uint32_t       SECCFG_CH[16];
    __IO uint32_t       SECCFG_IRQ[4];
    __IO uint32_t       SECCFG_MISC;
    __I  uint32_t       resvd4D4[203];
    DMA_Debug_TypeDef   CH_DBG[16];
    __I  uint32_t       resvdC00[256];
  } CLR;
} DMA_TypeDef;

typedef struct {
  struct {
    __I  uint32_t       STATUS;
    __IO uint32_t       CTRL;
  } GPIO[48];
  __I  uint32_t         resvd180[32];
  __IO uint32_t         INTR[6];
  __I  uint32_t         resvd218[2];
  struct {
    __IO uint32_t       INTE[6];
    __I  uint32_t       resvd18[2];
    __IO uint32_t       INTF[6];
    __I  uint32_t       resvd38[2];
    __I  uint32_t       INTS[6];
    __I  uint32_t       resvd58[2];
  } PROC[2];
  __IO uint32_t         DW_INTE[6];
  __I  uint32_t         resvd2F8[2];
  __IO uint32_t         DW_INTF[6];
  __I  uint32_t         resvd318[2];
  __I  uint32_t         DW_INTS[6];
} IOUSER_TypeDef;

typedef struct {
  struct {
    __I  uint32_t       STATUS;
    __IO uint32_t       CTRL;
  } GPIO[6];
  __IO uint32_t         INTR[1];
  struct {
    __IO uint32_t       INTE[1];
    __IO uint32_t       INTF[1];
    __I  uint32_t       INTS[1];
  } PROC[2];
  __IO uint32_t         DW_INTE[1];
  __IO uint32_t         DW_INTF[1];
  __I  uint32_t         DW_INTS[1];
} IOQSPI_TypeDef;

typedef struct {
  __IO uint32_t         VOLTAGE_SELECT;
  __IO uint32_t         GPIO[48];
  __IO uint32_t         SWCLK;
  __IO uint32_t         SWD;
} PADS_TypeDef;

typedef struct {
  __IO uint32_t         VOLTAGE_SELECT;
  __IO uint32_t         GPIO[6];
} PADS_QSPI_TypeDef;

typedef struct {
  __IO uint32_t         FRCE_ON;
  __IO uint32_t         FRCE_OFF;
  __IO uint32_t         WDSEL;
  __I  uint32_t         DONE;
  __I  uint32_t         resvd10[1020];
  struct {
    __IO uint32_t       FRCE_ON;
    __IO uint32_t       FRCE_OFF;
    __IO uint32_t       WDSEL;
    __I  uint32_t       DONE;
    __I  uint32_t       resvd10[1020];
  } XOR;
  struct {
    __IO uint32_t       FRCE_ON;
    __IO uint32_t       FRCE_OFF;
    __IO uint32_t       WDSEL;
    __I  uint32_t       DONE;
    __I  uint32_t       resvd10[1020];
  } SET;
  struct {
    __IO uint32_t       FRCE_ON;
    __IO uint32_t       FRCE_OFF;
    __IO uint32_t       WDSEL;
    __I  uint32_t       DONE;
    __I  uint32_t       resvd10[1020];
  } CLR;
} PSM_TypeDef;

typedef struct {
  __IO uint32_t         RESET;
  __IO uint32_t         WDSEL;
  __I  uint32_t         RESET_DONE;
  __I  uint32_t         resvdC[1021];
  struct {
    __IO uint32_t       RESET;
    __IO uint32_t       WDSEL;
    __I  uint32_t       RESET_DONE;
    __I  uint32_t       resvdC[1021];
  } XOR;
  struct {
    __IO uint32_t       RESET;
    __IO uint32_t       WDSEL;
    __I  uint32_t       RESET_DONE;
    __I  uint32_t       resvdC[1021];
  } SET;
  struct {
    __IO uint32_t       RESET;
    __IO uint32_t       WDSEL;
    __I  uint32_t       RESET_DONE;
    __I  uint32_t       resvdC[1021];
  } CLR;
} RESETS_TypeDef;

/**
 * @brief   SIO peripheral.
 * @note    RP2350 interleaves base GPIO and GPIO_HI registers differently
 *          from RP2040. The base GPIO registers control GPIO 0-31, while
 *          GPIO_HI registers control GPIO 32-47 plus QSPI and USB.
 */
typedef struct {
  __I  uint32_t         CPUID;
  __I  uint32_t         GPIO_IN;
  __I  uint32_t         GPIO_HI_IN;
  __I  uint32_t         resvdC;
  __IO uint32_t         GPIO_OUT;
  __IO uint32_t         GPIO_HI_OUT;
  __IO uint32_t         GPIO_OUT_SET;
  __IO uint32_t         GPIO_HI_OUT_SET;
  __IO uint32_t         GPIO_OUT_CLR;
  __IO uint32_t         GPIO_HI_OUT_CLR;
  __IO uint32_t         GPIO_OUT_XOR;
  __IO uint32_t         GPIO_HI_OUT_XOR;
  __IO uint32_t         GPIO_OE;
  __IO uint32_t         GPIO_HI_OE;
  __IO uint32_t         GPIO_OE_SET;
  __IO uint32_t         GPIO_HI_OE_SET;
  __IO uint32_t         GPIO_OE_CLR;
  __IO uint32_t         GPIO_HI_OE_CLR;
  __IO uint32_t         GPIO_OE_XOR;
  __IO uint32_t         GPIO_HI_OE_XOR;
  __IO uint32_t         FIFO_ST;
  __IO uint32_t         FIFO_WR;
  __I  uint32_t         FIFO_RD;
  __IO uint32_t         SPINLOCK_ST;
  __I  uint32_t         resvd60[8];
  struct {
    __IO uint32_t       ACCUM0;
    __IO uint32_t       ACCUM1;
    __IO uint32_t       BASE0;
    __IO uint32_t       BASE1;
    __IO uint32_t       BASE2;
    __I  uint32_t       POP_LANE0;
    __I  uint32_t       POP_LANE1;
    __I  uint32_t       POP_FULL;
    __I  uint32_t       PEEK_LANE0;
    __I  uint32_t       PEEK_LANE1;
    __I  uint32_t       PEEK_FULL;
    __IO uint32_t       CTRL_LANE0;
    __IO uint32_t       CTRL_LANE1;
    __IO uint32_t       ACCUM0_ADD;
    __IO uint32_t       ACCUM1_ADD;
    __IO uint32_t       BASE_1AND0;
  } INTERP[2];
  __IO uint32_t         SPINLOCK[32];
  __IO uint32_t         DOORBELL_OUT_SET;
  __IO uint32_t         DOORBELL_OUT_CLR;
  __I  uint32_t         DOORBELL_IN_SET;
  __I  uint32_t         DOORBELL_IN_CLR;
  __IO uint32_t         PERI_NONSEC;
  __I  uint32_t         resvd194[3];
  __IO uint32_t         RISCV_SOFTIRQ;
  __IO uint32_t         MTIME_CTRL;
  __I  uint32_t         resvd1A8[2];
  __IO uint32_t         MTIME;
  __IO uint32_t         MTIMEH;
  __IO uint32_t         MTIMECMP;
  __IO uint32_t         MTIMECMPH;
  __IO uint32_t         TMDS_CTRL;
  __IO uint32_t         TMDS_WDATA;
  __I  uint32_t         TMDS_PEEK_SINGLE;
  __I  uint32_t         TMDS_POP_SINGLE;
  __I  uint32_t         TMDS_PEEK_DOUBLE_L0;
  __I  uint32_t         TMDS_POP_DOUBLE_L0;
  __I  uint32_t         TMDS_PEEK_DOUBLE_L1;
  __I  uint32_t         TMDS_POP_DOUBLE_L1;
  __I  uint32_t         TMDS_PEEK_DOUBLE_L2;
  __I  uint32_t         TMDS_POP_DOUBLE_L2;
} SIO_TypeDef;

typedef struct {
  __IO uint32_t         SSPCR0;
  __IO uint32_t         SSPCR1;
  __IO uint32_t         SSPDR;
  __I  uint32_t         SSPSR;
  __IO uint32_t         SSPCPSR;
  __IO uint32_t         SSPIMSC;
  __I  uint32_t         SSPRIS;
  __I  uint32_t         SSPMIS;
  __IO uint32_t         SSPICR;
  __IO uint32_t         SSPDMACR;
  __I  uint32_t         resvd28[1006];
  __I  uint32_t         SSPPERIPHID0;
  __I  uint32_t         SSPPERIPHID1;
  __I  uint32_t         SSPPERIPHID2;
  __I  uint32_t         SSPPERIPHID3;
  __I  uint32_t         SSPPCELLID0;
  __I  uint32_t         SSPPCELLID1;
  __I  uint32_t         SSPPCELLID2;
  __I  uint32_t         SSPPCELLID3;
  struct {
    __IO uint32_t       SSPCR0;
    __IO uint32_t       SSPCR1;
    __IO uint32_t       SSPDR;
    __I  uint32_t       SSPSR;
    __IO uint32_t       SSPCPSR;
    __IO uint32_t       SSPIMSC;
    __I  uint32_t       SSPRIS;
    __I  uint32_t       SSPMIS;
    __IO uint32_t       SSPICR;
    __IO uint32_t       SSPDMACR;
    __I  uint32_t       resvd28[1006];
    __I  uint32_t       SSPPERIPHID0;
    __I  uint32_t       SSPPERIPHID1;
    __I  uint32_t       SSPPERIPHID2;
    __I  uint32_t       SSPPERIPHID3;
    __I  uint32_t       SSPPCELLID0;
    __I  uint32_t       SSPPCELLID1;
    __I  uint32_t       SSPPCELLID2;
    __I  uint32_t       SSPPCELLID3;
  } XOR;
  struct {
    __IO uint32_t       SSPCR0;
    __IO uint32_t       SSPCR1;
    __IO uint32_t       SSPDR;
    __I  uint32_t       SSPSR;
    __IO uint32_t       SSPCPSR;
    __IO uint32_t       SSPIMSC;
    __I  uint32_t       SSPRIS;
    __I  uint32_t       SSPMIS;
    __IO uint32_t       SSPICR;
    __IO uint32_t       SSPDMACR;
    __I  uint32_t       resvd28[1006];
    __I  uint32_t       SSPPERIPHID0;
    __I  uint32_t       SSPPERIPHID1;
    __I  uint32_t       SSPPERIPHID2;
    __I  uint32_t       SSPPERIPHID3;
    __I  uint32_t       SSPPCELLID0;
    __I  uint32_t       SSPPCELLID1;
    __I  uint32_t       SSPPCELLID2;
    __I  uint32_t       SSPPCELLID3;
  } SET;
  struct {
    __IO uint32_t       SSPCR0;
    __IO uint32_t       SSPCR1;
    __IO uint32_t       SSPDR;
    __I  uint32_t       SSPSR;
    __IO uint32_t       SSPCPSR;
    __IO uint32_t       SSPIMSC;
    __I  uint32_t       SSPRIS;
    __I  uint32_t       SSPMIS;
    __IO uint32_t       SSPICR;
    __IO uint32_t       SSPDMACR;
    __I  uint32_t       resvd28[1006];
    __I  uint32_t       SSPPERIPHID0;
    __I  uint32_t       SSPPERIPHID1;
    __I  uint32_t       SSPPERIPHID2;
    __I  uint32_t       SSPPERIPHID3;
    __I  uint32_t       SSPPCELLID0;
    __I  uint32_t       SSPPCELLID1;
    __I  uint32_t       SSPPCELLID2;
    __I  uint32_t       SSPPCELLID3;
  } CLR;
} SPI_TypeDef;

typedef struct {
  __IO uint32_t         TIMEHW;
  __IO uint32_t         TIMELW;
  __I  uint32_t         TIMEHR;
  __I  uint32_t         TIMELR;
  __IO uint32_t         ALARM[4];
  __IO uint32_t         ARMED;
  __I  uint32_t         TIMERAWH;
  __I  uint32_t         TIMERAWL;
  __IO uint32_t         DBGPAUSE;
  __IO uint32_t         PAUSE;
  __IO uint32_t         LOCKED;
  __IO uint32_t         SOURCE;
  __IO uint32_t         INTR;
  __IO uint32_t         INTE;
  __IO uint32_t         INTF;
  __I  uint32_t         INTS;
  __I  uint32_t         resvd4C[1005];
  struct {
    __IO uint32_t       TIMEHW;
    __IO uint32_t       TIMELW;
    __I  uint32_t       TIMEHR;
    __I  uint32_t       TIMELR;
    __IO uint32_t       ALARM[4];
    __IO uint32_t       ARMED;
    __I  uint32_t       TIMERAWH;
    __I  uint32_t       TIMERAWL;
    __IO uint32_t       DBGPAUSE;
    __IO uint32_t       PAUSE;
    __IO uint32_t       LOCKED;
    __IO uint32_t       SOURCE;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __I  uint32_t       INTS;
    __I  uint32_t       resvd4C[1005];
  } XOR;
  struct {
    __IO uint32_t       TIMEHW;
    __IO uint32_t       TIMELW;
    __I  uint32_t       TIMEHR;
    __I  uint32_t       TIMELR;
    __IO uint32_t       ALARM[4];
    __IO uint32_t       ARMED;
    __I  uint32_t       TIMERAWH;
    __I  uint32_t       TIMERAWL;
    __IO uint32_t       DBGPAUSE;
    __IO uint32_t       PAUSE;
    __IO uint32_t       LOCKED;
    __IO uint32_t       SOURCE;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __I  uint32_t       INTS;
    __I  uint32_t       resvd4C[1005];
  } SET;
  struct {
    __IO uint32_t       TIMEHW;
    __IO uint32_t       TIMELW;
    __I  uint32_t       TIMEHR;
    __I  uint32_t       TIMELR;
    __IO uint32_t       ALARM[4];
    __IO uint32_t       ARMED;
    __I  uint32_t       TIMERAWH;
    __I  uint32_t       TIMERAWL;
    __IO uint32_t       DBGPAUSE;
    __IO uint32_t       PAUSE;
    __IO uint32_t       LOCKED;
    __IO uint32_t       SOURCE;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __I  uint32_t       INTS;
    __I  uint32_t       resvd4C[1005];
  } CLR;
} TIMER_TypeDef;

typedef struct {
  __IO uint32_t         UARTDR;
  __IO uint32_t         UARTRSR;
  __I  uint32_t         resvd8[4];
  __I  uint32_t         UARTFR;
  __I  uint32_t         resvd1C;
  __IO uint32_t         UARTILPR;
  __IO uint32_t         UARTIBRD;
  __IO uint32_t         UARTFBRD;
  __IO uint32_t         UARTLCR_H;
  __IO uint32_t         UARTCR;
  __IO uint32_t         UARTIFLS;
  __IO uint32_t         UARTIMSC;
  __I  uint32_t         UARTRIS;
  __I  uint32_t         UARTMIS;
  __IO uint32_t         UARTICR;
  __IO uint32_t         UARTDMACR;
  __I  uint32_t         resvd4C[997];
  __I  uint32_t         UARTPERIPHID0;
  __I  uint32_t         UARTPERIPHID1;
  __I  uint32_t         UARTPERIPHID2;
  __I  uint32_t         UARTPERIPHID3;
  __I  uint32_t         UARTPCELLID0;
  __I  uint32_t         UARTPCELLID1;
  __I  uint32_t         UARTPCELLID2;
  __I  uint32_t         UARTPCELLID3;
  struct {
    __IO uint32_t       UARTDR;
    __IO uint32_t       UARTRSR;
    __I  uint32_t       resvd8[4];
    __I  uint32_t       UARTFR;
    __I  uint32_t       resvd1C;
    __IO uint32_t       UARTILPR;
    __IO uint32_t       UARTIBRD;
    __IO uint32_t       UARTFBRD;
    __IO uint32_t       UARTLCR_H;
    __IO uint32_t       UARTCR;
    __IO uint32_t       UARTIFLS;
    __IO uint32_t       UARTIMSC;
    __I  uint32_t       UARTRIS;
    __I  uint32_t       UARTMIS;
    __IO uint32_t       UARTICR;
    __IO uint32_t       UARTDMACR;
    __I  uint32_t       resvd4C[997];
    __I  uint32_t       UARTPERIPHID0;
    __I  uint32_t       UARTPERIPHID1;
    __I  uint32_t       UARTPERIPHID2;
    __I  uint32_t       UARTPERIPHID3;
    __I  uint32_t       UARTPCELLID0;
    __I  uint32_t       UARTPCELLID1;
    __I  uint32_t       UARTPCELLID2;
    __I  uint32_t       UARTPCELLID3;
  } XOR;
  struct {
    __IO uint32_t       UARTDR;
    __IO uint32_t       UARTRSR;
    __I  uint32_t       resvd8[4];
    __I  uint32_t       UARTFR;
    __I  uint32_t       resvd1C;
    __IO uint32_t       UARTILPR;
    __IO uint32_t       UARTIBRD;
    __IO uint32_t       UARTFBRD;
    __IO uint32_t       UARTLCR_H;
    __IO uint32_t       UARTCR;
    __IO uint32_t       UARTIFLS;
    __IO uint32_t       UARTIMSC;
    __I  uint32_t       UARTRIS;
    __I  uint32_t       UARTMIS;
    __IO uint32_t       UARTICR;
    __IO uint32_t       UARTDMACR;
    __I  uint32_t       resvd4C[997];
    __I  uint32_t       UARTPERIPHID0;
    __I  uint32_t       UARTPERIPHID1;
    __I  uint32_t       UARTPERIPHID2;
    __I  uint32_t       UARTPERIPHID3;
    __I  uint32_t       UARTPCELLID0;
    __I  uint32_t       UARTPCELLID1;
    __I  uint32_t       UARTPCELLID2;
    __I  uint32_t       UARTPCELLID3;
  } SET;
  struct {
    __IO uint32_t       UARTDR;
    __IO uint32_t       UARTRSR;
    __I  uint32_t       resvd8[4];
    __I  uint32_t       UARTFR;
    __I  uint32_t       resvd1C;
    __IO uint32_t       UARTILPR;
    __IO uint32_t       UARTIBRD;
    __IO uint32_t       UARTFBRD;
    __IO uint32_t       UARTLCR_H;
    __IO uint32_t       UARTCR;
    __IO uint32_t       UARTIFLS;
    __IO uint32_t       UARTIMSC;
    __I  uint32_t       UARTRIS;
    __I  uint32_t       UARTMIS;
    __IO uint32_t       UARTICR;
    __IO uint32_t       UARTDMACR;
    __I  uint32_t       resvd4C[997];
    __I  uint32_t       UARTPERIPHID0;
    __I  uint32_t       UARTPERIPHID1;
    __I  uint32_t       UARTPERIPHID2;
    __I  uint32_t       UARTPERIPHID3;
    __I  uint32_t       UARTPCELLID0;
    __I  uint32_t       UARTPCELLID1;
    __I  uint32_t       UARTPCELLID2;
    __I  uint32_t       UARTPCELLID3;
  } CLR;
} UART_TypeDef;

typedef struct {
  __IO uint32_t         CON;
  __IO uint32_t         TAR;
  __IO uint32_t         SAR;
  __I  uint32_t         resvdC;
  __IO uint32_t         DATACMD;
  __IO uint32_t         SSSCLHCNT;
  __IO uint32_t         SSSCLLCNT;
  __IO uint32_t         FSSCLHCNT;
  __IO uint32_t         FSSCLLCNT;
  __I  uint32_t         resvd24[2];
  __IO uint32_t         INTRSTAT;
  __IO uint32_t         INTRMASK;
  __IO uint32_t         RAWINTRSTAT;
  __IO uint32_t         RXTL;
  __IO uint32_t         TXTL;
  __IO uint32_t         CLRINTR;
  __IO uint32_t         CLRRXUNDER;
  __IO uint32_t         CLRRXOVER;
  __IO uint32_t         CLRTXOVER;
  __IO uint32_t         CLRRDREQ;
  __IO uint32_t         CLRTXABRT;
  __IO uint32_t         CLRRXDONE;
  __IO uint32_t         CLRACTIVITY;
  __IO uint32_t         CLRSTOPDET;
  __IO uint32_t         CLRSTARTDET;
  __IO uint32_t         CLRGENCALL;
  __IO uint32_t         ENABLE;
  __IO uint32_t         STATUS;
  __IO uint32_t         TXFLR;
  __IO uint32_t         RXFLR;
  __IO uint32_t         SDAHOLD;
  __IO uint32_t         TXABRTSOURCE;
  __IO uint32_t         SLVDATANACKONLY;
  __IO uint32_t         DMACR;
  __IO uint32_t         DMATDLR;
  __IO uint32_t         DMARDLR;
  __IO uint32_t         SDASETUP;
  __IO uint32_t         ACKGENERALCALL;
  __IO uint32_t         ENABLESTATUS;
  __IO uint32_t         FSSPKLEN;
  __I  uint32_t         resvdA4;
  __IO uint32_t         CLRRESTARTDET;
  __I  uint32_t         resvdac[981];
  struct {
    __IO uint32_t       CON;
    __IO uint32_t       TAR;
    __IO uint32_t       SAR;
    __I  uint32_t       resvdC;
    __IO uint32_t       DATACMD;
    __IO uint32_t       SSSCLHCNT;
    __IO uint32_t       SSSCLLCNT;
    __IO uint32_t       FSSCLHCNT;
    __IO uint32_t       FSSCLLCNT;
    __I  uint32_t       resvd24[2];
    __IO uint32_t       INTRSTAT;
    __IO uint32_t       INTRMASK;
    __IO uint32_t       RAWINTRSTAT;
    __IO uint32_t       RXTL;
    __IO uint32_t       TXTL;
    __IO uint32_t       CLRINTR;
    __IO uint32_t       CLRRXUNDER;
    __IO uint32_t       CLRRXOVER;
    __IO uint32_t       CLRTXOVER;
    __IO uint32_t       CLRRDREQ;
    __IO uint32_t       CLRTXABRT;
    __IO uint32_t       CLRRXDONE;
    __IO uint32_t       CLRACTIVITY;
    __IO uint32_t       CLRSTOPDET;
    __IO uint32_t       CLRSTARTDET;
    __IO uint32_t       CLRGENCALL;
    __IO uint32_t       ENABLE;
    __IO uint32_t       STATUS;
    __IO uint32_t       TXFLR;
    __IO uint32_t       RXFLR;
    __IO uint32_t       SDAHOLD;
    __IO uint32_t       TXABRTSOURCE;
    __IO uint32_t       SLVDATANACKONLY;
    __IO uint32_t       DMACR;
    __IO uint32_t       DMATDLR;
    __IO uint32_t       DMARDLR;
    __IO uint32_t       SDASETUP;
    __IO uint32_t       ACKGENERALCALL;
    __IO uint32_t       ENABLESTATUS;
    __IO uint32_t       FSSPKLEN;
    __I  uint32_t       resvdA4;
    __IO uint32_t       CLRRESTARTDET;
    __I  uint32_t       resvdac[981];
  } XOR;
  struct {
    __IO uint32_t       CON;
    __IO uint32_t       TAR;
    __IO uint32_t       SAR;
    __I  uint32_t       resvdC;
    __IO uint32_t       DATACMD;
    __IO uint32_t       SSSCLHCNT;
    __IO uint32_t       SSSCLLCNT;
    __IO uint32_t       FSSCLHCNT;
    __IO uint32_t       FSSCLLCNT;
    __I  uint32_t       resvd24[2];
    __IO uint32_t       INTRSTAT;
    __IO uint32_t       INTRMASK;
    __IO uint32_t       RAWINTRSTAT;
    __IO uint32_t       RXTL;
    __IO uint32_t       TXTL;
    __IO uint32_t       CLRINTR;
    __IO uint32_t       CLRRXUNDER;
    __IO uint32_t       CLRRXOVER;
    __IO uint32_t       CLRTXOVER;
    __IO uint32_t       CLRRDREQ;
    __IO uint32_t       CLRTXABRT;
    __IO uint32_t       CLRRXDONE;
    __IO uint32_t       CLRACTIVITY;
    __IO uint32_t       CLRSTOPDET;
    __IO uint32_t       CLRSTARTDET;
    __IO uint32_t       CLRGENCALL;
    __IO uint32_t       ENABLE;
    __IO uint32_t       STATUS;
    __IO uint32_t       TXFLR;
    __IO uint32_t       RXFLR;
    __IO uint32_t       SDAHOLD;
    __IO uint32_t       TXABRTSOURCE;
    __IO uint32_t       SLVDATANACKONLY;
    __IO uint32_t       DMACR;
    __IO uint32_t       DMATDLR;
    __IO uint32_t       DMARDLR;
    __IO uint32_t       SDASETUP;
    __IO uint32_t       ACKGENERALCALL;
    __IO uint32_t       ENABLESTATUS;
    __IO uint32_t       FSSPKLEN;
    __I  uint32_t       resvdA4;
    __IO uint32_t       CLRRESTARTDET;
    __I  uint32_t       resvdac[981];
  } SET;
  struct {
    __IO uint32_t       CON;
    __IO uint32_t       TAR;
    __IO uint32_t       SAR;
    __I  uint32_t       resvdC;
    __IO uint32_t       DATACMD;
    __IO uint32_t       SSSCLHCNT;
    __IO uint32_t       SSSCLLCNT;
    __IO uint32_t       FSSCLHCNT;
    __IO uint32_t       FSSCLLCNT;
    __I  uint32_t       resvd24[2];
    __IO uint32_t       INTRSTAT;
    __IO uint32_t       INTRMASK;
    __IO uint32_t       RAWINTRSTAT;
    __IO uint32_t       RXTL;
    __IO uint32_t       TXTL;
    __IO uint32_t       CLRINTR;
    __IO uint32_t       CLRRXUNDER;
    __IO uint32_t       CLRRXOVER;
    __IO uint32_t       CLRTXOVER;
    __IO uint32_t       CLRRDREQ;
    __IO uint32_t       CLRTXABRT;
    __IO uint32_t       CLRRXDONE;
    __IO uint32_t       CLRACTIVITY;
    __IO uint32_t       CLRSTOPDET;
    __IO uint32_t       CLRSTARTDET;
    __IO uint32_t       CLRGENCALL;
    __IO uint32_t       ENABLE;
    __IO uint32_t       STATUS;
    __IO uint32_t       TXFLR;
    __IO uint32_t       RXFLR;
    __IO uint32_t       SDAHOLD;
    __IO uint32_t       TXABRTSOURCE;
    __IO uint32_t       SLVDATANACKONLY;
    __IO uint32_t       DMACR;
    __IO uint32_t       DMATDLR;
    __IO uint32_t       DMARDLR;
    __IO uint32_t       SDASETUP;
    __IO uint32_t       ACKGENERALCALL;
    __IO uint32_t       ENABLESTATUS;
    __IO uint32_t       FSSPKLEN;
    __I  uint32_t       resvdA4;
    __IO uint32_t       CLRRESTARTDET;
    __I  uint32_t       resvdac[981];
  } CLR;
} I2C_TypeDef;

typedef struct {
  __IO uint32_t         CS;
  __IO uint32_t         RESULT;
  __IO uint32_t         FCS;
  __IO uint32_t         FIFO;
  __IO uint32_t         DIV;
  __IO uint32_t         INTR;
  __IO uint32_t         INTE;
  __IO uint32_t         INTF;
  __IO uint32_t         INTS;
  __I  uint32_t         resvd24[1015];
  struct {
    __IO uint32_t       CS;
    __IO uint32_t       RESULT;
    __IO uint32_t       FCS;
    __IO uint32_t       FIFO;
    __IO uint32_t       DIV;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd24[1015];
  } XOR;
  struct {
    __IO uint32_t       CS;
    __IO uint32_t       RESULT;
    __IO uint32_t       FCS;
    __IO uint32_t       FIFO;
    __IO uint32_t       DIV;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd24[1015];
  } SET;
  struct {
    __IO uint32_t       CS;
    __IO uint32_t       RESULT;
    __IO uint32_t       FCS;
    __IO uint32_t       FIFO;
    __IO uint32_t       DIV;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd24[1015];
  } CLR;
} ADC_TypeDef;

typedef struct {
  struct {
    __IO uint32_t       CSR;
    __IO uint32_t       DIV;
    __IO uint32_t       CTR;
    __IO uint32_t       CC;
    __IO uint32_t       TOP;
  } CH[12];
  __IO uint32_t         EN;
  __IO uint32_t         INTR;
  __IO uint32_t         IRQ0_INTE;
  __IO uint32_t         IRQ0_INTF;
  __I  uint32_t         IRQ0_INTS;
  __IO uint32_t         IRQ1_INTE;
  __IO uint32_t         IRQ1_INTF;
  __I  uint32_t         IRQ1_INTS;
  __I  uint32_t         resvdpwm[968];
  struct {
    struct {
      __IO uint32_t     CSR;
      __IO uint32_t     DIV;
      __IO uint32_t     CTR;
      __IO uint32_t     CC;
      __IO uint32_t     TOP;
    } CH[12];
    __IO uint32_t       EN;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvdpwm[968];
  } XOR;
  struct {
    struct {
      __IO uint32_t     CSR;
      __IO uint32_t     DIV;
      __IO uint32_t     CTR;
      __IO uint32_t     CC;
      __IO uint32_t     TOP;
    } CH[12];
    __IO uint32_t       EN;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvdpwm[968];
  } SET;
  struct {
    struct {
      __IO uint32_t     CSR;
      __IO uint32_t     DIV;
      __IO uint32_t     CTR;
      __IO uint32_t     CC;
      __IO uint32_t     TOP;
    } CH[12];
    __IO uint32_t       EN;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvdpwm[968];
  } CLR;
} PWM_TypeDef;

typedef struct {
  __IO uint32_t         DEVADDRCTRL;
  __IO uint32_t         INTEPADDRCTRL[15];
  __IO uint32_t         MAINCTRL;
  __IO uint32_t         SOFRW;
  __I  uint32_t         SOFRD;
  __IO uint32_t         SIECTRL;
  __IO uint32_t         SIESTATUS;
  __IO uint32_t         INTEPCTRL;
  __IO uint32_t         BUFSTATUS;
  __IO uint32_t         BUFCPUSHOULDHANDLE;
  __IO uint32_t         ABORT;
  __IO uint32_t         ABORTDONE;
  __IO uint32_t         EPSTALLARM;
  __IO uint32_t         NAKPOLL;
  __IO uint32_t         EPNAKSTALLSTATUS;
  __IO uint32_t         MUXING;
  __IO uint32_t         PWR;
  __IO uint32_t         PHYDIRECT;
  __IO uint32_t         PHYDIRECTOVERRIDE;
  __IO uint32_t         PHYTRIM;
  __IO uint32_t         LINESTATETUNING;
  __IO uint32_t         INTR;
  __IO uint32_t         INTE;
  __IO uint32_t         INTF;
  __IO uint32_t         INTS;
  __I  uint32_t         resvd[985];
  struct {
    __IO uint32_t       DEVADDRCTRL;
    __IO uint32_t       INTEPADDRCTRL[15];
    __IO uint32_t       MAINCTRL;
    __IO uint32_t       SOFRW;
    __I  uint32_t       SOFRD;
    __IO uint32_t       SIECTRL;
    __IO uint32_t       SIESTATUS;
    __IO uint32_t       INTEPCTRL;
    __IO uint32_t       BUFSTATUS;
    __IO uint32_t       BUFCPUSHOULDHANDLE;
    __IO uint32_t       ABORT;
    __IO uint32_t       ABORTDONE;
    __IO uint32_t       EPSTALLARM;
    __IO uint32_t       NAKPOLL;
    __IO uint32_t       EPNAKSTALLSTATUS;
    __IO uint32_t       MUXING;
    __IO uint32_t       PWR;
    __IO uint32_t       PHYDIRECT;
    __IO uint32_t       PHYDIRECTOVERRIDE;
    __IO uint32_t       PHYTRIM;
    __IO uint32_t       LINESTATETUNING;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd[985];
  } XOR;
  struct {
    __IO uint32_t       DEVADDRCTRL;
    __IO uint32_t       INTEPADDRCTRL[15];
    __IO uint32_t       MAINCTRL;
    __IO uint32_t       SOFRW;
    __I  uint32_t       SOFRD;
    __IO uint32_t       SIECTRL;
    __IO uint32_t       SIESTATUS;
    __IO uint32_t       INTEPCTRL;
    __IO uint32_t       BUFSTATUS;
    __IO uint32_t       BUFCPUSHOULDHANDLE;
    __IO uint32_t       ABORT;
    __IO uint32_t       ABORTDONE;
    __IO uint32_t       EPSTALLARM;
    __IO uint32_t       NAKPOLL;
    __IO uint32_t       EPNAKSTALLSTATUS;
    __IO uint32_t       MUXING;
    __IO uint32_t       PWR;
    __IO uint32_t       PHYDIRECT;
    __IO uint32_t       PHYDIRECTOVERRIDE;
    __IO uint32_t       PHYTRIM;
    __IO uint32_t       LINESTATETUNING;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd[985];
  } SET;
  struct {
    __IO uint32_t       DEVADDRCTRL;
    __IO uint32_t       INTEPADDRCTRL[15];
    __IO uint32_t       MAINCTRL;
    __IO uint32_t       SOFRW;
    __I  uint32_t       SOFRD;
    __IO uint32_t       SIECTRL;
    __IO uint32_t       SIESTATUS;
    __IO uint32_t       INTEPCTRL;
    __IO uint32_t       BUFSTATUS;
    __IO uint32_t       BUFCPUSHOULDHANDLE;
    __IO uint32_t       ABORT;
    __IO uint32_t       ABORTDONE;
    __IO uint32_t       EPSTALLARM;
    __IO uint32_t       NAKPOLL;
    __IO uint32_t       EPNAKSTALLSTATUS;
    __IO uint32_t       MUXING;
    __IO uint32_t       PWR;
    __IO uint32_t       PHYDIRECT;
    __IO uint32_t       PHYDIRECTOVERRIDE;
    __IO uint32_t       PHYTRIM;
    __IO uint32_t       LINESTATETUNING;
    __IO uint32_t       INTR;
    __IO uint32_t       INTE;
    __IO uint32_t       INTF;
    __IO uint32_t       INTS;
    __I  uint32_t       resvd[985];
  } CLR;
} USB_TypeDef;

/**
 * @brief   RP2350 WATCHDOG peripheral.
 * @note    RP2350 moved TICK to TICKS peripheral.
 */
typedef struct {
  __IO uint32_t         CTRL;
  __O  uint32_t         LOAD;
  __I  uint32_t         REASON;
  __IO uint32_t         SCRATCH[8];
  __I  uint32_t         resvd2c[1013];
  struct {
    __IO uint32_t       CTRL;
    __O  uint32_t       LOAD;
    __I  uint32_t       REASON;
    __IO uint32_t       SCRATCH[8];
    __I  uint32_t       resvd2c[1013];
  } XOR;
  struct {
    __IO uint32_t       CTRL;
    __O  uint32_t       LOAD;
    __I  uint32_t       REASON;
    __IO uint32_t       SCRATCH[8];
    __I  uint32_t       resvd2c[1013];
  } SET;
  struct {
    __IO uint32_t       CTRL;
    __O  uint32_t       LOAD;
    __I  uint32_t       REASON;
    __IO uint32_t       SCRATCH[8];
    __I  uint32_t       resvd2c[1013];
  } CLR;
} WATCHDOG_TypeDef;

typedef struct {
  __IO uint32_t         CTRL;
  __IO uint32_t         CYCLES;
  __I  uint32_t         COUNT;
} TICKS_TICK_TypeDef;

typedef struct {
  TICKS_TICK_TypeDef    TICK[6];
} TICKS_TypeDef;

typedef struct {
  __IO uint32_t         CTRL;
  __I  uint32_t         FSTAT;
  __IO uint32_t         FDEBUG;
  __I  uint32_t         FLEVEL;
  __IO uint32_t         TXF[4];
  __I  uint32_t         RXF[4];
  __IO uint32_t         IRQ;
  __IO uint32_t         IRQ_FORCE;
  __IO uint32_t         INPUT_SYNC_BYPASS;
  __IO uint32_t         DBG_PADOUT;
  __IO uint32_t         DBG_PADOE;
  __IO uint32_t         DBG_CFGINFO;
  __IO uint32_t         INSTR_MEM[32];
  struct {
    __IO uint32_t       CLKDIV;
    __IO uint32_t       EXECCTRL;
    __IO uint32_t       SHIFTCTRL;
    __I  uint32_t       ADDR;
    __IO uint32_t       INSTR;
    __IO uint32_t       PINCTRL;
  } SM[4];
  __IO uint32_t         RXF_PUTGET[4];
  __I  uint32_t         GPIOBASE;
  __IO uint32_t         INTR;
  __IO uint32_t         IRQ0_INTE;
  __IO uint32_t         IRQ0_INTF;
  __I  uint32_t         IRQ0_INTS;
  __IO uint32_t         IRQ1_INTE;
  __IO uint32_t         IRQ1_INTF;
  __I  uint32_t         IRQ1_INTS;
  __I  uint32_t         resvd164[935];
  struct {
    __IO uint32_t       CTRL;
    __I  uint32_t       FSTAT;
    __IO uint32_t       FDEBUG;
    __I  uint32_t       FLEVEL;
    __IO uint32_t       TXF[4];
    __I  uint32_t       RXF[4];
    __IO uint32_t       IRQ;
    __IO uint32_t       IRQ_FORCE;
    __IO uint32_t       INPUT_SYNC_BYPASS;
    __IO uint32_t       DBG_PADOUT;
    __IO uint32_t       DBG_PADOE;
    __IO uint32_t       DBG_CFGINFO;
    __IO uint32_t       INSTR_MEM[32];
    struct {
      __IO uint32_t     CLKDIV;
      __IO uint32_t     EXECCTRL;
      __IO uint32_t     SHIFTCTRL;
      __I  uint32_t     ADDR;
      __IO uint32_t     INSTR;
      __IO uint32_t     PINCTRL;
    } SM[4];
    __IO uint32_t       RXF_PUTGET[4];
    __I  uint32_t       GPIOBASE;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvd164[935];
  } XOR;
  struct {
    __IO uint32_t       CTRL;
    __I  uint32_t       FSTAT;
    __IO uint32_t       FDEBUG;
    __I  uint32_t       FLEVEL;
    __IO uint32_t       TXF[4];
    __I  uint32_t       RXF[4];
    __IO uint32_t       IRQ;
    __IO uint32_t       IRQ_FORCE;
    __IO uint32_t       INPUT_SYNC_BYPASS;
    __IO uint32_t       DBG_PADOUT;
    __IO uint32_t       DBG_PADOE;
    __IO uint32_t       DBG_CFGINFO;
    __IO uint32_t       INSTR_MEM[32];
    struct {
      __IO uint32_t     CLKDIV;
      __IO uint32_t     EXECCTRL;
      __IO uint32_t     SHIFTCTRL;
      __I  uint32_t     ADDR;
      __IO uint32_t     INSTR;
      __IO uint32_t     PINCTRL;
    } SM[4];
    __IO uint32_t       RXF_PUTGET[4];
    __I  uint32_t       GPIOBASE;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvd164[935];
  } SET;
  struct {
    __IO uint32_t       CTRL;
    __I  uint32_t       FSTAT;
    __IO uint32_t       FDEBUG;
    __I  uint32_t       FLEVEL;
    __IO uint32_t       TXF[4];
    __I  uint32_t       RXF[4];
    __IO uint32_t       IRQ;
    __IO uint32_t       IRQ_FORCE;
    __IO uint32_t       INPUT_SYNC_BYPASS;
    __IO uint32_t       DBG_PADOUT;
    __IO uint32_t       DBG_PADOE;
    __IO uint32_t       DBG_CFGINFO;
    __IO uint32_t       INSTR_MEM[32];
    struct {
      __IO uint32_t     CLKDIV;
      __IO uint32_t     EXECCTRL;
      __IO uint32_t     SHIFTCTRL;
      __I  uint32_t     ADDR;
      __IO uint32_t     INSTR;
      __IO uint32_t     PINCTRL;
    } SM[4];
    __IO uint32_t       RXF_PUTGET[4];
    __I  uint32_t       GPIOBASE;
    __IO uint32_t       INTR;
    __IO uint32_t       IRQ0_INTE;
    __IO uint32_t       IRQ0_INTF;
    __I  uint32_t       IRQ0_INTS;
    __IO uint32_t       IRQ1_INTE;
    __IO uint32_t       IRQ1_INTF;
    __I  uint32_t       IRQ1_INTS;
    __I  uint32_t       resvd164[935];
  } CLR;
} PIO_TypeDef;

typedef struct {
  __IO uint32_t         CTRL;
  __I  uint32_t         STATUS;
  __IO uint32_t         DORMANT;
  __IO uint32_t         STARTUP;
  __I  uint32_t         resvd10[3];
  __IO uint32_t         COUNT;
} XOSC_TypeDef;

typedef struct {
  __IO uint32_t         CS;
  __IO uint32_t         PWR;
  __IO uint32_t         FBDIV_INT;
  __IO uint32_t         PRIM;
  __IO uint32_t         INTR;
  __IO uint32_t         INTE;
  __IO uint32_t         INTF;
  __I  uint32_t         INTS;
} PLL_TypeDef;

typedef struct {
  __IO uint32_t         CTRL;
  __IO uint32_t         DIV;
  __I  uint32_t         SELECTED;
} CLOCKS_CLK_TypeDef;

typedef struct {
  __IO uint32_t         CTRL;
  __I  uint32_t         STATUS;
} CLOCKS_RESUS_TypeDef;

typedef struct {
  __IO uint32_t         REF_KHZ;
  __IO uint32_t         MIN_KHZ;
  __IO uint32_t         MAX_KHZ;
  __IO uint32_t         DELAY;
  __IO uint32_t         INTERVAL;
  __IO uint32_t         SRC;
  __I  uint32_t         STATUS;
  __I  uint32_t         RESULT;
} CLOCKS_FC0_TypeDef;

typedef struct {
  CLOCKS_CLK_TypeDef    CLK[10];
  CLOCKS_RESUS_TypeDef  RESUS;
  CLOCKS_FC0_TypeDef    FC0;
  __IO uint32_t         WAKE_EN0;
  __IO uint32_t         WAKE_EN1;
  __IO uint32_t         SLEEP_EN0;
  __IO uint32_t         SLEEP_EN1;
  __I  uint32_t         ENABLED0;
  __I  uint32_t         ENABLED1;
  __I  uint32_t         INTR;
  __IO uint32_t         INTE;
  __IO uint32_t         INTF;
  __I  uint32_t         INTS;
} CLOCKS_TypeDef;

/**
 * @brief   QMI (QSPI Memory Interface) register block.
 * @note    Controls execute-in-place (XIP) flash access for RP2350.
 */
typedef struct {
  __IO uint32_t         DIRECT_CSR;
  __IO uint32_t         DIRECT_TX;
  __I  uint32_t         DIRECT_RX;
  __IO uint32_t         M0_TIMING;
  __IO uint32_t         M0_RFMT;
  __IO uint32_t         M0_RCMD;
  __IO uint32_t         M0_WFMT;
  __IO uint32_t         M0_WCMD;
  __IO uint32_t         M1_TIMING;
  __IO uint32_t         M1_RFMT;
  __IO uint32_t         M1_RCMD;
  __IO uint32_t         M1_WFMT;
  __IO uint32_t         M1_WCMD;
  __IO uint32_t         ATRANS[8];
} QMI_TypeDef;
/** @} */

/**
 * @name    Base addresses
 * @{
 */
#define __APBPERIPH_BASE                  0x40000000U
#define __AHBPERIPH_BASE                  0x50000000U
#define __IOPORT_BASE                     0xD0000000U

#define __SYSINFO_BASE                    (__APBPERIPH_BASE + 0x00000000U)
#define __SYSCFG_BASE                     (__APBPERIPH_BASE + 0x00008000U)
#define __CLOCKS_BASE                     (__APBPERIPH_BASE + 0x00010000U)
#define __PSM_BASE                        (__APBPERIPH_BASE + 0x00018000U)
#define __RESETS_BASE                     (__APBPERIPH_BASE + 0x00020000U)
#define __IOUSER0_BASE                    (__APBPERIPH_BASE + 0x00028000U)
#define __IOQSPI_BASE                     (__APBPERIPH_BASE + 0x00030000U)
#define __PADSUSER0_BASE                  (__APBPERIPH_BASE + 0x00038000U)
#define __PADSQSPI_BASE                   (__APBPERIPH_BASE + 0x00040000U)
#define __XOSC_BASE                       (__APBPERIPH_BASE + 0x00048000U)
#define __PLL_SYS_BASE                    (__APBPERIPH_BASE + 0x00050000U)
#define __PLL_USB_BASE                    (__APBPERIPH_BASE + 0x00058000U)
#define __ACCESSCTRL_BASE                 (__APBPERIPH_BASE + 0x00060000U)
#define __BUSCTRL_BASE                    (__APBPERIPH_BASE + 0x00068000U)
#define __UART0_BASE                      (__APBPERIPH_BASE + 0x00070000U)
#define __UART1_BASE                      (__APBPERIPH_BASE + 0x00078000U)
#define __SPI0_BASE                       (__APBPERIPH_BASE + 0x00080000U)
#define __SPI1_BASE                       (__APBPERIPH_BASE + 0x00088000U)
#define __I2C0_BASE                       (__APBPERIPH_BASE + 0x00090000U)
#define __I2C1_BASE                       (__APBPERIPH_BASE + 0x00098000U)
#define __ADC_BASE                        (__APBPERIPH_BASE + 0x000A0000U)
#define __PWM_BASE                        (__APBPERIPH_BASE + 0x000A8000U)
#define __TIMER0_BASE                     (__APBPERIPH_BASE + 0x000B0000U)
#define __TIMER1_BASE                     (__APBPERIPH_BASE + 0x000B8000U)
#define __HSTX_CTRL_BASE                  (__APBPERIPH_BASE + 0x000C0000U)
#define __XIP_CTRL_BASE                   (__APBPERIPH_BASE + 0x000C8000U)
#define __QMI_BASE                        (__APBPERIPH_BASE + 0x000D0000U)
#define __WATCHDOG_BASE                   (__APBPERIPH_BASE + 0x000D8000U)
#define __BOOTRAM_BASE                    (__APBPERIPH_BASE + 0x000E0000U)
#define __ROSC_BASE                       (__APBPERIPH_BASE + 0x000E8000U)
#define __TRNG_BASE                       (__APBPERIPH_BASE + 0x000F0000U)
#define __SHA256_BASE                     (__APBPERIPH_BASE + 0x000F8000U)
#define __POWMAN_BASE                     (__APBPERIPH_BASE + 0x00100000U)
#define __TICKS_BASE                      (__APBPERIPH_BASE + 0x00108000U)
#define __OTP_BASE                        (__APBPERIPH_BASE + 0x00120000U)
#define __OTP_DATA_BASE                   (__APBPERIPH_BASE + 0x00130000U)
#define __OTP_DATA_RAW_BASE               (__APBPERIPH_BASE + 0x00134000U)
#define __GLITCH_DETECTOR_BASE            (__APBPERIPH_BASE + 0x00158000U)
#define __TBMAN_BASE                      (__APBPERIPH_BASE + 0x00160000U)

#define __DMA_BASE                        (__AHBPERIPH_BASE + 0x00000000U)
#define __USB_DPRAM_BASE                  (__AHBPERIPH_BASE + 0x00100000U)
#define __USB_BASE                        (__AHBPERIPH_BASE + 0x00110000U)
#define __PIO0_BASE                       (__AHBPERIPH_BASE + 0x00200000U)
#define __PIO1_BASE                       (__AHBPERIPH_BASE + 0x00300000U)
#define __PIO2_BASE                       (__AHBPERIPH_BASE + 0x00400000U)
#define __XIP_AUX_BASE                    (__AHBPERIPH_BASE + 0x00500000U)
#define __HSTX_FIFO_BASE                  (__AHBPERIPH_BASE + 0x00600000U)
#define __CORESIGHT_TRACE_BASE            (__AHBPERIPH_BASE + 0x00700000U)

#define __SIO_BASE                        (__IOPORT_BASE    + 0x00000000U)
/** @} */

/**
 * @name    Peripherals
 * @{
 */
#define DMA                               ((DMA_TypeDef *)        __DMA_BASE)
#define IO_BANK0                          ((IOUSER_TypeDef *)     __IOUSER0_BASE)
#define IO_QSPI                           ((IOQSPI_TypeDef *)     __IOQSPI_BASE)
#define PADS_BANK0                        ((PADS_TypeDef *)       __PADSUSER0_BASE)
#define PADS_QSPI                         ((PADS_QSPI_TypeDef *)  __PADSQSPI_BASE)
#define PSM                               ((PSM_TypeDef *)        __PSM_BASE)
#define RESETS                            ((RESETS_TypeDef *)     __RESETS_BASE)
#define SIO                               ((SIO_TypeDef *)        __SIO_BASE)
#define TIMER0                            ((TIMER_TypeDef *)      __TIMER0_BASE)
#define TIMER1                            ((TIMER_TypeDef *)      __TIMER1_BASE)
#define UART0                             ((UART_TypeDef *)       __UART0_BASE)
#define UART1                             ((UART_TypeDef *)       __UART1_BASE)
#define SPI0                              ((SPI_TypeDef *)        __SPI0_BASE)
#define SPI1                              ((SPI_TypeDef *)        __SPI1_BASE)
#define I2C0                              ((I2C_TypeDef *)        __I2C0_BASE)
#define I2C1                              ((I2C_TypeDef *)        __I2C1_BASE)
#define ADC                               ((ADC_TypeDef *)        __ADC_BASE)
#define PWM                               ((PWM_TypeDef *)        __PWM_BASE)
#define PIO0                              ((PIO_TypeDef *)        __PIO0_BASE)
#define PIO1                              ((PIO_TypeDef *)        __PIO1_BASE)
#define PIO2                              ((PIO_TypeDef *)        __PIO2_BASE)
#define USB                               ((USB_TypeDef *)        __USB_BASE)
#define WATCHDOG                          ((WATCHDOG_TypeDef *)   __WATCHDOG_BASE)
#define PLL_SYS                           ((PLL_TypeDef *)        __PLL_SYS_BASE)
#define PLL_USB                           ((PLL_TypeDef *)        __PLL_USB_BASE)
#define XOSC                              ((XOSC_TypeDef *)       __XOSC_BASE)
#define CLOCKS                            ((CLOCKS_TypeDef *)     __CLOCKS_BASE)
#define TICKS                             ((TICKS_TypeDef *)      __TICKS_BASE)
#define QMI                               ((QMI_TypeDef *)        __QMI_BASE)
/** @} */

/**
 * @name    DMA bits definitions
 * @note    See RP2350 Datasheet 12.6.10 DMA List of Registers
 * @{
 */
#define DMA_CTRL_TRIG_AHB_ERROR           (1U << 31)
#define DMA_CTRL_TRIG_READ_ERROR          (1U << 30)
#define DMA_CTRL_TRIG_WRITE_ERROR         (1U << 29)
#define DMA_CTRL_TRIG_BUSY                (1U << 26)
#define DMA_CTRL_TRIG_SNIFF_EN            (1U << 25)
#define DMA_CTRL_TRIG_BSWAP               (1U << 24)
#define DMA_CTRL_TRIG_IRQ_QUIET           (1U << 23)
#define DMA_CTRL_TRIG_TREQ_SEL_Pos        17U
#define DMA_CTRL_TRIG_TREQ_SEL_Msk        (0x3FU << DMA_CTRL_TRIG_TREQ_SEL_Pos)
#define DMA_CTRL_TRIG_TREQ_SEL(n)         ((n) << DMA_CTRL_TRIG_TREQ_SEL_Pos)
#define DMA_CTRL_TRIG_TREQ_PIO0_TX0       DMA_CTRL_TRIG_TREQ_SEL(0x00U)
#define DMA_CTRL_TRIG_TREQ_PIO0_TX1       DMA_CTRL_TRIG_TREQ_SEL(0x01U)
#define DMA_CTRL_TRIG_TREQ_PIO0_TX2       DMA_CTRL_TRIG_TREQ_SEL(0x02U)
#define DMA_CTRL_TRIG_TREQ_PIO0_TX3       DMA_CTRL_TRIG_TREQ_SEL(0x03U)
#define DMA_CTRL_TRIG_TREQ_PIO0_RX0       DMA_CTRL_TRIG_TREQ_SEL(0x04U)
#define DMA_CTRL_TRIG_TREQ_PIO0_RX1       DMA_CTRL_TRIG_TREQ_SEL(0x05U)
#define DMA_CTRL_TRIG_TREQ_PIO0_RX2       DMA_CTRL_TRIG_TREQ_SEL(0x06U)
#define DMA_CTRL_TRIG_TREQ_PIO0_RX3       DMA_CTRL_TRIG_TREQ_SEL(0x07U)
#define DMA_CTRL_TRIG_TREQ_PIO1_TX0       DMA_CTRL_TRIG_TREQ_SEL(0x08U)
#define DMA_CTRL_TRIG_TREQ_PIO1_TX1       DMA_CTRL_TRIG_TREQ_SEL(0x09U)
#define DMA_CTRL_TRIG_TREQ_PIO1_TX2       DMA_CTRL_TRIG_TREQ_SEL(0x0AU)
#define DMA_CTRL_TRIG_TREQ_PIO1_TX3       DMA_CTRL_TRIG_TREQ_SEL(0x0BU)
#define DMA_CTRL_TRIG_TREQ_PIO1_RX0       DMA_CTRL_TRIG_TREQ_SEL(0x0CU)
#define DMA_CTRL_TRIG_TREQ_PIO1_RX1       DMA_CTRL_TRIG_TREQ_SEL(0x0DU)
#define DMA_CTRL_TRIG_TREQ_PIO1_RX2       DMA_CTRL_TRIG_TREQ_SEL(0x0EU)
#define DMA_CTRL_TRIG_TREQ_PIO1_RX3       DMA_CTRL_TRIG_TREQ_SEL(0x0FU)
#define DMA_CTRL_TRIG_TREQ_PIO2_TX0       DMA_CTRL_TRIG_TREQ_SEL(0x10U)
#define DMA_CTRL_TRIG_TREQ_PIO2_TX1       DMA_CTRL_TRIG_TREQ_SEL(0x11U)
#define DMA_CTRL_TRIG_TREQ_PIO2_TX2       DMA_CTRL_TRIG_TREQ_SEL(0x12U)
#define DMA_CTRL_TRIG_TREQ_PIO2_TX3       DMA_CTRL_TRIG_TREQ_SEL(0x13U)
#define DMA_CTRL_TRIG_TREQ_PIO2_RX0       DMA_CTRL_TRIG_TREQ_SEL(0x14U)
#define DMA_CTRL_TRIG_TREQ_PIO2_RX1       DMA_CTRL_TRIG_TREQ_SEL(0x15U)
#define DMA_CTRL_TRIG_TREQ_PIO2_RX2       DMA_CTRL_TRIG_TREQ_SEL(0x16U)
#define DMA_CTRL_TRIG_TREQ_PIO2_RX3       DMA_CTRL_TRIG_TREQ_SEL(0x17U)
#define DMA_CTRL_TRIG_TREQ_SPI0_TX        DMA_CTRL_TRIG_TREQ_SEL(0x18U)
#define DMA_CTRL_TRIG_TREQ_SPI0_RX        DMA_CTRL_TRIG_TREQ_SEL(0x19U)
#define DMA_CTRL_TRIG_TREQ_SPI1_TX        DMA_CTRL_TRIG_TREQ_SEL(0x1AU)
#define DMA_CTRL_TRIG_TREQ_SPI1_RX        DMA_CTRL_TRIG_TREQ_SEL(0x1BU)
#define DMA_CTRL_TRIG_TREQ_UART0_TX       DMA_CTRL_TRIG_TREQ_SEL(0x1CU)
#define DMA_CTRL_TRIG_TREQ_UART0_RX       DMA_CTRL_TRIG_TREQ_SEL(0x1DU)
#define DMA_CTRL_TRIG_TREQ_UART1_TX       DMA_CTRL_TRIG_TREQ_SEL(0x1EU)
#define DMA_CTRL_TRIG_TREQ_UART1_RX       DMA_CTRL_TRIG_TREQ_SEL(0x1FU)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP0      DMA_CTRL_TRIG_TREQ_SEL(0x20U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP1      DMA_CTRL_TRIG_TREQ_SEL(0x21U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP2      DMA_CTRL_TRIG_TREQ_SEL(0x22U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP3      DMA_CTRL_TRIG_TREQ_SEL(0x23U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP4      DMA_CTRL_TRIG_TREQ_SEL(0x24U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP5      DMA_CTRL_TRIG_TREQ_SEL(0x25U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP6      DMA_CTRL_TRIG_TREQ_SEL(0x26U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP7      DMA_CTRL_TRIG_TREQ_SEL(0x27U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP8      DMA_CTRL_TRIG_TREQ_SEL(0x28U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP9      DMA_CTRL_TRIG_TREQ_SEL(0x29U)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP10     DMA_CTRL_TRIG_TREQ_SEL(0x2AU)
#define DMA_CTRL_TRIG_TREQ_PWM_WRAP11     DMA_CTRL_TRIG_TREQ_SEL(0x2BU)
#define DMA_CTRL_TRIG_TREQ_I2C0_TX        DMA_CTRL_TRIG_TREQ_SEL(0x2CU)
#define DMA_CTRL_TRIG_TREQ_I2C0_RX        DMA_CTRL_TRIG_TREQ_SEL(0x2DU)
#define DMA_CTRL_TRIG_TREQ_I2C1_TX        DMA_CTRL_TRIG_TREQ_SEL(0x2EU)
#define DMA_CTRL_TRIG_TREQ_I2C1_RX        DMA_CTRL_TRIG_TREQ_SEL(0x2FU)
#define DMA_CTRL_TRIG_TREQ_ADC            DMA_CTRL_TRIG_TREQ_SEL(0x30U)
#define DMA_CTRL_TRIG_TREQ_XIP_STREAM     DMA_CTRL_TRIG_TREQ_SEL(0x31U)
#define DMA_CTRL_TRIG_TREQ_XIP_QMITX      DMA_CTRL_TRIG_TREQ_SEL(0x32U)
#define DMA_CTRL_TRIG_TREQ_XIP_QMIRX      DMA_CTRL_TRIG_TREQ_SEL(0x33U)
#define DMA_CTRL_TRIG_TREQ_HSTX           DMA_CTRL_TRIG_TREQ_SEL(0x34U)
#define DMA_CTRL_TRIG_TREQ_CORESIGHT      DMA_CTRL_TRIG_TREQ_SEL(0x35U)
#define DMA_CTRL_TRIG_TREQ_SHA256         DMA_CTRL_TRIG_TREQ_SEL(0x36U)
#define DMA_CTRL_TRIG_TREQ_TIMER0         DMA_CTRL_TRIG_TREQ_SEL(0x3BU)
#define DMA_CTRL_TRIG_TREQ_TIMER1         DMA_CTRL_TRIG_TREQ_SEL(0x3CU)
#define DMA_CTRL_TRIG_TREQ_TIMER2         DMA_CTRL_TRIG_TREQ_SEL(0x3DU)
#define DMA_CTRL_TRIG_TREQ_TIMER3         DMA_CTRL_TRIG_TREQ_SEL(0x3EU)
#define DMA_CTRL_TRIG_TREQ_PERMANENT      DMA_CTRL_TRIG_TREQ_SEL(0x3FU)
#define DMA_CTRL_TRIG_CHAIN_TO_Pos        13U
#define DMA_CTRL_TRIG_CHAIN_TO_Msk        (0xFU << DMA_CTRL_TRIG_CHAIN_TO_Pos)
#define DMA_CTRL_TRIG_CHAIN_TO(n)         ((n) << DMA_CTRL_TRIG_CHAIN_TO_Pos)
#define DMA_CTRL_TRIG_RING_SEL            (1U << 12)
#define DMA_CTRL_TRIG_RING_SIZE_Pos       8U
#define DMA_CTRL_TRIG_RING_SIZE_Msk       (0xFU << DMA_CTRL_TRIG_RING_SIZE_Pos)
#define DMA_CTRL_TRIG_RING_SIZE(n)        ((n) << DMA_CTRL_TRIG_RING_SIZE_Pos)
#define DMA_CTRL_TRIG_INCR_WRITE_REV      (1U << 7)
#define DMA_CTRL_TRIG_INCR_WRITE          (1U << 6)
#define DMA_CTRL_TRIG_INCR_READ_REV       (1U << 5)
#define DMA_CTRL_TRIG_INCR_READ           (1U << 4)
#define DMA_CTRL_TRIG_DATA_SIZE_Pos       2U
#define DMA_CTRL_TRIG_DATA_SIZE_Msk       (3U << DMA_CTRL_TRIG_DATA_SIZE_Pos)
#define DMA_CTRL_TRIG_DATA_SIZE(n)        ((n) << DMA_CTRL_TRIG_DATA_SIZE_Pos)
#define DMA_CTRL_TRIG_DATA_SIZE_BYTE      DMA_CTRL_TRIG_DATA_SIZE(0U)
#define DMA_CTRL_TRIG_DATA_SIZE_HWORD     DMA_CTRL_TRIG_DATA_SIZE(1U)
#define DMA_CTRL_TRIG_DATA_SIZE_WORD      DMA_CTRL_TRIG_DATA_SIZE(2U)
#define DMA_CTRL_TRIG_HIGH_PRIORITY       (1U << 1)
#define DMA_CTRL_TRIG_EN                  (1U << 0)
/** @} */

/**
 * @name    PSM bits definitions (dangerous)
 * @note    See RP2350 Datasheet 7.4.4 PSM List of Registers
 * @{
 */
#define PSM_ANY_PROC1                     (1U << 24)
#define PSM_ANY_PROC0                     (1U << 23)
#define PSM_ANY_ACCESSCTRL                (1U << 22)
#define PSM_ANY_SIO                       (1U << 21)
#define PSM_ANY_XIP                       (1U << 20)
#define PSM_ANY_SRAM9                     (1U << 19)
#define PSM_ANY_SRAM8                     (1U << 18)
#define PSM_ANY_SRAM7                     (1U << 17)
#define PSM_ANY_SRAM6                     (1U << 16)
#define PSM_ANY_SRAM5                     (1U << 15)
#define PSM_ANY_SRAM4                     (1U << 14)
#define PSM_ANY_SRAM3                     (1U << 13)
#define PSM_ANY_SRAM2                     (1U << 12)
#define PSM_ANY_SRAM1                     (1U << 11)
#define PSM_ANY_SRAM0                     (1U << 10)
#define PSM_ANY_BOOTRAM                   (1U << 9)
#define PSM_ANY_ROM                       (1U << 8)
#define PSM_ANY_BUSFABRIC                 (1U << 7)
#define PSM_ANY_PSM_READY                 (1U << 6)
#define PSM_ANY_CLOCKS                    (1U << 5)
#define PSM_ANY_RESETS                    (1U << 4)
#define PSM_ANY_XOSC                      (1U << 3)
#define PSM_ANY_ROSC                      (1U << 2)
#define PSM_ANY_OTP                       (1U << 1)
#define PSM_ANY_PROC_COLD                 (1U << 0)
/** @} */

/**
 * @name    RESETS bits definitions
 * @note    See RP2350 Datasheet 7.5.3 Resets List of Registers
 * @{
 */
#define RESETS_ALLREG_USBCTRL             (1U << 28)
#define RESETS_ALLREG_UART1               (1U << 27)
#define RESETS_ALLREG_UART0               (1U << 26)
#define RESETS_ALLREG_TRNG                (1U << 25)
#define RESETS_ALLREG_TIMER1              (1U << 24)
#define RESETS_ALLREG_TIMER0              (1U << 23)
#define RESETS_ALLREG_TBMAN               (1U << 22)
#define RESETS_ALLREG_SYSINFO             (1U << 21)
#define RESETS_ALLREG_SYSCFG              (1U << 20)
#define RESETS_ALLREG_SPI1                (1U << 19)
#define RESETS_ALLREG_SPI0                (1U << 18)
#define RESETS_ALLREG_SHA256              (1U << 17)
#define RESETS_ALLREG_PWM                 (1U << 16)
#define RESETS_ALLREG_PLL_USB             (1U << 15)
#define RESETS_ALLREG_PLL_SYS             (1U << 14)
#define RESETS_ALLREG_PIO2                (1U << 13)
#define RESETS_ALLREG_PIO1                (1U << 12)
#define RESETS_ALLREG_PIO0                (1U << 11)
#define RESETS_ALLREG_PADS_QSPI           (1U << 10)
#define RESETS_ALLREG_PADS_BANK0          (1U << 9)
#define RESETS_ALLREG_JTAG                (1U << 8)
#define RESETS_ALLREG_IO_QSPI             (1U << 7)
#define RESETS_ALLREG_IO_BANK0            (1U << 6)
#define RESETS_ALLREG_I2C1                (1U << 5)
#define RESETS_ALLREG_I2C0                (1U << 4)
#define RESETS_ALLREG_HSTX                (1U << 3)
#define RESETS_ALLREG_DMA                 (1U << 2)
#define RESETS_ALLREG_BUSCTRL             (1U << 1)
#define RESETS_ALLREG_ADC                 (1U << 0)
/** @} */

/**
 * @name    SIO bits definitions
 * @note    See RP2350 Datasheet 3.1.11 SIO List of Registers
 * @{
 */
#define SIO_FIFO_ST_VLD_Pos               0U
#define SIO_FIFO_ST_VLD_Msk               (1U << SIO_FIFO_ST_VLD_Pos)
#define SIO_FIFO_ST_VLD                   SIO_FIFO_ST_VLD_Msk
#define SIO_FIFO_ST_RDY_Pos               1U
#define SIO_FIFO_ST_RDY_Msk               (1U << SIO_FIFO_ST_RDY_Pos)
#define SIO_FIFO_ST_RDY                   SIO_FIFO_ST_RDY_Msk
#define SIO_FIFO_ST_WOF_Pos               2U
#define SIO_FIFO_ST_WOF_Msk               (1U << SIO_FIFO_ST_WOF_Pos)
#define SIO_FIFO_ST_WOF                   SIO_FIFO_ST_WOF_Msk
#define SIO_FIFO_ST_ROE_Pos               3U
#define SIO_FIFO_ST_ROE_Msk               (1U << SIO_FIFO_ST_ROE_Pos)
#define SIO_FIFO_ST_ROE                   SIO_FIFO_ST_ROE_Msk
/** @} */

/**
 * @name    SPI bits definitions
 * @note    See RP2350 Datasheet 12.3.5 SPI List of Registers
 * @{
 */
#define SPI_SSPCR0_SCR_Pos                8U
#define SPI_SSPCR0_SCR_Msk                (255U << SPI_SSPCR0_SCR_Pos)
#define SPI_SSPCR0_SCR(n)                 ((n) << SPI_SSPCR0_SCR_Pos)
#define SPI_SSPCR0_SPH_Pos                7U
#define SPI_SSPCR0_SPH_Msk                (1U << SPI_SSPCR0_SPH_Pos)
#define SPI_SSPCR0_SPH                    SPI_SSPCR0_SPH_Msk
#define SPI_SSPCR0_SPO_Pos                6U
#define SPI_SSPCR0_SPO_Msk                (1U << SPI_SSPCR0_SPO_Pos)
#define SPI_SSPCR0_SPO                    SPI_SSPCR0_SPO_Msk
#define SPI_SSPCR0_FRF_Pos                4U
#define SPI_SSPCR0_FRF_Msk                (3U << SPI_SSPCR0_FRF_Pos)
#define SPI_SSPCR0_FRF(n)                 ((n) << SPI_SSPCR0_FRF_Pos)
#define SPI_SSPCR0_FRF_MOTOROLA           SPI_SSPCR0_FRF(0U)
#define SPI_SSPCR0_FRF_TI                 SPI_SSPCR0_FRF(1U)
#define SPI_SSPCR0_FRF_NATIONAL           SPI_SSPCR0_FRF(2U)
#define SPI_SSPCR0_DSS_Pos                0U
#define SPI_SSPCR0_DSS_Msk                (15U << SPI_SSPCR0_DSS_Pos)
#define SPI_SSPCR0_DSS(n)                 ((n) << SPI_SSPCR0_DSS_Pos)
#define SPI_SSPCR0_DSS_4BIT               SPI_SSPCR0_DSS(3U)
#define SPI_SSPCR0_DSS_5BIT               SPI_SSPCR0_DSS(4U)
#define SPI_SSPCR0_DSS_6BIT               SPI_SSPCR0_DSS(5U)
#define SPI_SSPCR0_DSS_7BIT               SPI_SSPCR0_DSS(6U)
#define SPI_SSPCR0_DSS_8BIT               SPI_SSPCR0_DSS(7U)
#define SPI_SSPCR0_DSS_9BIT               SPI_SSPCR0_DSS(8U)
#define SPI_SSPCR0_DSS_10BIT              SPI_SSPCR0_DSS(9U)
#define SPI_SSPCR0_DSS_11BIT              SPI_SSPCR0_DSS(10U)
#define SPI_SSPCR0_DSS_12BIT              SPI_SSPCR0_DSS(11U)
#define SPI_SSPCR0_DSS_13BIT              SPI_SSPCR0_DSS(12U)
#define SPI_SSPCR0_DSS_14BIT              SPI_SSPCR0_DSS(13U)
#define SPI_SSPCR0_DSS_15BIT              SPI_SSPCR0_DSS(14U)
#define SPI_SSPCR0_DSS_16BIT              SPI_SSPCR0_DSS(15U)

#define SPI_SSPCR1_SOD_Pos                3U
#define SPI_SSPCR1_SOD_Msk                (1U << SPI_SSPCR1_SOD_Pos)
#define SPI_SSPCR1_SOD                    SPI_SSPCR1_SOD_Msk
#define SPI_SSPCR1_MS_Pos                 2U
#define SPI_SSPCR1_MS_Msk                 (1U << SPI_SSPCR1_MS_Pos)
#define SPI_SSPCR1_MS                     SPI_SSPCR1_MS_Msk
#define SPI_SSPCR1_SSE_Pos                1U
#define SPI_SSPCR1_SSE_Msk                (1U << SPI_SSPCR1_SSE_Pos)
#define SPI_SSPCR1_SSE                    SPI_SSPCR1_SSE_Msk
#define SPI_SSPCR1_LBM_Pos                0U
#define SPI_SSPCR1_LBM_Msk                (1U << SPI_SSPCR1_LBM_Pos)
#define SPI_SSPCR1_LBM                    SPI_SSPCR1_LBM_Msk

#define SPI_SSPSR_BSY_Pos                 4U
#define SPI_SSPSR_BSY_Msk                 (1U << SPI_SSPSR_BSY_Pos)
#define SPI_SSPSR_BSY                     SPI_SSPSR_BSY_Msk
#define SPI_SSPSR_RFF_Pos                 3U
#define SPI_SSPSR_RFF_Msk                 (1U << SPI_SSPSR_RFF_Pos)
#define SPI_SSPSR_RFF                     SPI_SSPSR_RFF_Msk
#define SPI_SSPSR_RNE_Pos                 2U
#define SPI_SSPSR_RNE_Msk                 (1U << SPI_SSPSR_RNE_Pos)
#define SPI_SSPSR_RNE                     SPI_SSPSR_RNE_Msk
#define SPI_SSPSR_TNF_Pos                 1U
#define SPI_SSPSR_TNF_Msk                 (1U << SPI_SSPSR_TNF_Pos)
#define SPI_SSPSR_TNF                     SPI_SSPSR_TNF_Msk
#define SPI_SSPSR_TFE_Pos                 0U
#define SPI_SSPSR_TFE_Msk                 (1U << SPI_SSPSR_TFE_Pos)
#define SPI_SSPSR_TFE                     SPI_SSPSR_TFE_Msk

#define SPI_SSPCPSR_CPSDVSR_Pos           0U
#define SPI_SSPCPSR_CPSDVSR_Msk           (255U << SPI_SSPCPSR_CPSDVSR_Pos)
#define SPI_SSPCPSR_CPSDVSR(n)            ((n) << SPI_SSPCPSR_CPSDVSR_Pos)

#define SPI_SSPIMSC_TXIM_Pos              3U
#define SPI_SSPIMSC_TXIM_Msk              (1U << SPI_SSPIMSC_TXIM_Pos)
#define SPI_SSPIMSC_TXIM                  SPI_SSPIMSC_TXIM_Msk
#define SPI_SSPIMSC_RXIM_Pos              2U
#define SPI_SSPIMSC_RXIM_Msk              (1U << SPI_SSPIMSC_RXIM_Pos)
#define SPI_SSPIMSC_RXIM                  SPI_SSPIMSC_RXIM_Msk
#define SPI_SSPIMSC_RTIM_Pos              1U
#define SPI_SSPIMSC_RTIM_Msk              (1U << SPI_SSPIMSC_RTIM_Pos)
#define SPI_SSPIMSC_RTIM                  SPI_SSPIMSC_RTIM_Msk
#define SPI_SSPIMSC_RORIM_Pos             0U
#define SPI_SSPIMSC_RORIM_Msk             (1U << SPI_SSPIMSC_RORIM_Pos)
#define SPI_SSPIMSC_RORIM                 SPI_SSPIMSC_RORIM_Msk

#define SPI_SSPDMACR_TXDMAE_Pos           1U
#define SPI_SSPDMACR_TXDMAE_Msk           (1U << SPI_SSPDMACR_TXDMAE_Pos)
#define SPI_SSPDMACR_TXDMAE               SPI_SSPDMACR_TXDMAE_Msk
#define SPI_SSPDMACR_RXDMAE_Pos           0U
#define SPI_SSPDMACR_RXDMAE_Msk           (1U << SPI_SSPDMACR_RXDMAE_Pos)
#define SPI_SSPDMACR_RXDMAE               SPI_SSPDMACR_RXDMAE_Msk
/** @} */

/**
 * @name    TIMER bits definitions
 * @note    See RP2350 Datasheet 12.8.5 TIMER List of Registers
 * @{
 */
#define TIMER_ARMED_ALARM0_Pos            0U
#define TIMER_ARMED_ALARM0_Msk            (1U << TIMER_ARMED_ALARM0_Pos)
#define TIMER_ARMED_ALARM0                TIMER_ARMED_ALARM0_Msk
#define TIMER_ARMED_ALARM1_Pos            1U
#define TIMER_ARMED_ALARM1_Msk            (1U << TIMER_ARMED_ALARM1_Pos)
#define TIMER_ARMED_ALARM1                TIMER_ARMED_ALARM1_Msk
#define TIMER_ARMED_ALARM2_Pos            2U
#define TIMER_ARMED_ALARM2_Msk            (1U << TIMER_ARMED_ALARM2_Pos)
#define TIMER_ARMED_ALARM2                TIMER_ARMED_ALARM2_Msk
#define TIMER_ARMED_ALARM3_Pos            3U
#define TIMER_ARMED_ALARM3_Msk            (1U << TIMER_ARMED_ALARM3_Pos)
#define TIMER_ARMED_ALARM3                TIMER_ARMED_ALARM3_Msk

#define TIMER_DBGPAUSE_DBG0_Pos           1U
#define TIMER_DBGPAUSE_DBG0_Msk           (1U << TIMER_DBGPAUSE_DBG0_Pos)
#define TIMER_DBGPAUSE_DBG0               TIMER_DBGPAUSE_DBG0_Msk
#define TIMER_DBGPAUSE_DBG1_Pos           2U
#define TIMER_DBGPAUSE_DBG1_Msk           (1U << TIMER_DBGPAUSE_DBG1_Pos)
#define TIMER_DBGPAUSE_DBG1               TIMER_DBGPAUSE_DBG1_Msk

#define TIMER_INTR_ALARM0_Pos             0U
#define TIMER_INTR_ALARM0_Msk             (1U << TIMER_INTR_ALARM0_Pos)
#define TIMER_INTR_ALARM0                 TIMER_INTR_ALARM0_Msk
#define TIMER_INTR_ALARM1_Pos             1U
#define TIMER_INTR_ALARM1_Msk             (1U << TIMER_INTR_ALARM1_Pos)
#define TIMER_INTR_ALARM1                 TIMER_INTR_ALARM1_Msk
#define TIMER_INTR_ALARM2_Pos             2U
#define TIMER_INTR_ALARM2_Msk             (1U << TIMER_INTR_ALARM2_Pos)
#define TIMER_INTR_ALARM2                 TIMER_INTR_ALARM2_Msk
#define TIMER_INTR_ALARM3_Pos             3U
#define TIMER_INTR_ALARM3_Msk             (1U << TIMER_INTR_ALARM3_Pos)
#define TIMER_INTR_ALARM3                 TIMER_INTR_ALARM3_Msk

#define TIMER_INTS_ALARM0_Pos             0U
#define TIMER_INTS_ALARM0_Msk             (1U << TIMER_INTS_ALARM0_Pos)
#define TIMER_INTS_ALARM0                 TIMER_INTS_ALARM0_Msk
#define TIMER_INTS_ALARM1_Pos             1U
#define TIMER_INTS_ALARM1_Msk             (1U << TIMER_INTS_ALARM1_Pos)
#define TIMER_INTS_ALARM1                 TIMER_INTS_ALARM1_Msk
#define TIMER_INTS_ALARM2_Pos             2U
#define TIMER_INTS_ALARM2_Msk             (1U << TIMER_INTS_ALARM2_Pos)
#define TIMER_INTS_ALARM2                 TIMER_INTS_ALARM2_Msk
#define TIMER_INTS_ALARM3_Pos             3U
#define TIMER_INTS_ALARM3_Msk             (1U << TIMER_INTS_ALARM3_Pos)
#define TIMER_INTS_ALARM3                 TIMER_INTS_ALARM3_Msk
/** @} */

/**
 * @name    UART bits definitions
 * @note    See RP2350 Datasheet 12.1.8 UART List of Registers
 * @{
 */
#define UART_UARTDR_OE_Pos                11U
#define UART_UARTDR_OE_Msk                (1U << UART_UARTDR_OE_Pos)
#define UART_UARTDR_OE                    UART_UARTDR_OE_Msk
#define UART_UARTDR_BE_Pos                10U
#define UART_UARTDR_BE_Msk                (1U << UART_UARTDR_BE_Pos)
#define UART_UARTDR_BE                    UART_UARTDR_BE_Msk
#define UART_UARTDR_PE_Pos                9U
#define UART_UARTDR_PE_Msk                (1U << UART_UARTDR_PE_Pos)
#define UART_UARTDR_PE                    UART_UARTDR_PE_Msk
#define UART_UARTDR_FE_Pos                8U
#define UART_UARTDR_FE_Msk                (1U << UART_UARTDR_FE_Pos)
#define UART_UARTDR_FE                    UART_UARTDR_FE_Msk

#define UART_UARTRSR_OE_Pos               3U
#define UART_UARTRSR_OE_Msk               (1U << UART_UARTRSR_OE_Pos)
#define UART_UARTRSR_OE                   UART_UARTRSR_OE_Msk
#define UART_UARTRSR_BE_Pos               2U
#define UART_UARTRSR_BE_Msk               (1U << UART_UARTRSR_BE_Pos)
#define UART_UARTRSR_BE                   UART_UARTRSR_BE_Msk
#define UART_UARTRSR_PE_Pos               1U
#define UART_UARTRSR_PE_Msk               (1U << UART_UARTRSR_PE_Pos)
#define UART_UARTRSR_PE                   UART_UARTRSR_PE_Msk
#define UART_UARTRSR_FE_Pos               0U
#define UART_UARTRSR_FE_Msk               (1U << UART_UARTRSR_FE_Pos)
#define UART_UARTRSR_FE                   UART_UARTRSR_FE_Msk

#define UART_UARTFR_RI_Pos                8U
#define UART_UARTFR_RI_Msk                (1U << UART_UARTFR_RI_Pos)
#define UART_UARTFR_RI                    UART_UARTFR_RI_Msk
#define UART_UARTFR_TXFE_Pos              7U
#define UART_UARTFR_TXFE_Msk              (1U << UART_UARTFR_TXFE_Pos)
#define UART_UARTFR_TXFE                  UART_UARTFR_TXFE_Msk
#define UART_UARTFR_RXFF_Pos              6U
#define UART_UARTFR_RXFF_Msk              (1U << UART_UARTFR_RXFF_Pos)
#define UART_UARTFR_RXFF                  UART_UARTFR_RXFF_Msk
#define UART_UARTFR_TXFF_Pos              5U
#define UART_UARTFR_TXFF_Msk              (1U << UART_UARTFR_TXFF_Pos)
#define UART_UARTFR_TXFF                  UART_UARTFR_TXFF_Msk
#define UART_UARTFR_RXFE_Pos              4U
#define UART_UARTFR_RXFE_Msk              (1U << UART_UARTFR_RXFE_Pos)
#define UART_UARTFR_RXFE                  UART_UARTFR_RXFE_Msk
#define UART_UARTFR_BUSY_Pos              3U
#define UART_UARTFR_BUSY_Msk              (1U << UART_UARTFR_BUSY_Pos)
#define UART_UARTFR_BUSY                  UART_UARTFR_BUSY_Msk
#define UART_UARTFR_DCD_Pos               2U
#define UART_UARTFR_DCD_Msk               (1U << UART_UARTFR_DCD_Pos)
#define UART_UARTFR_DCD                   UART_UARTFR_DCD_Msk
#define UART_UARTFR_DSR_Pos               1U
#define UART_UARTFR_DSR_Msk               (1U << UART_UARTFR_DSR_Pos)
#define UART_UARTFR_DSR                   UART_UARTFR_DSR_Msk
#define UART_UARTFR_CTS_Pos               0U
#define UART_UARTFR_CTS_Msk               (1U << UART_UARTFR_CTS_Pos)
#define UART_UARTFR_CTS                   UART_UARTFR_CTS_Msk

#define UART_UARTILPR_ILPDVSR_Pos         0U
#define UART_UARTILPR_ILPDVSR_Msk         (255U << UART_UARTILPR_ILPDVSR_Pos)
#define UART_UARTILPR_ILPDVSR(n)          ((n) << UART_UARTILPR_ILPDVSR_Pos)

#define UART_UARTIBRD_BAUD_DIVINT_Pos     0U
#define UART_UARTIBRD_BAUD_DIVINT_Msk     (0xFFFFU << UART_UARTIBRD_BAUD_DIVINT_Pos)
#define UART_UARTIBRD_BAUD_DIVINT(n)      ((n) << UART_UARTIBRD_BAUD_DIVINT_Pos)

#define UART_UARTFBRD_BAUD_DIVFRAC_Pos    0U
#define UART_UARTFBRD_BAUD_DIVFRAC_Msk    (63U << UART_UARTFBRD_BAUD_DIVFRAC_Pos)
#define UART_UARTFBRD_BAUD_DIVFRAC(n)     ((n) << UART_UARTFBRD_BAUD_DIVFRAC_Pos)

#define UART_UARTLCR_H_SPS_Pos            7U
#define UART_UARTLCR_H_SPS_Msk            (1U << UART_UARTLCR_H_SPS_Pos)
#define UART_UARTLCR_H_SPS                UART_UARTLCR_H_SPS_Msk
#define UART_UARTLCR_H_WLEN_Pos           5U
#define UART_UARTLCR_H_WLEN_Msk           (3U << UART_UARTLCR_H_WLEN_Pos)
#define UART_UARTLCR_H_WLEN(n)            ((n) << UART_UARTLCR_H_WLEN_Pos)
#define UART_UARTLCR_H_WLEN_5BITS         UART_UARTLCR_H_WLEN(0U)
#define UART_UARTLCR_H_WLEN_6BITS         UART_UARTLCR_H_WLEN(1U)
#define UART_UARTLCR_H_WLEN_7BITS         UART_UARTLCR_H_WLEN(2U)
#define UART_UARTLCR_H_WLEN_8BITS         UART_UARTLCR_H_WLEN(3U)
#define UART_UARTLCR_H_FEN_Pos            4U
#define UART_UARTLCR_H_FEN_Msk            (1U << UART_UARTLCR_H_FEN_Pos)
#define UART_UARTLCR_H_FEN                UART_UARTLCR_H_FEN_Msk
#define UART_UARTLCR_H_STP2_Pos           3U
#define UART_UARTLCR_H_STP2_Msk           (1U << UART_UARTLCR_H_STP2_Pos)
#define UART_UARTLCR_H_STP2               UART_UARTLCR_H_STP2_Msk
#define UART_UARTLCR_H_EPS_Pos            2U
#define UART_UARTLCR_H_EPS_Msk            (1U << UART_UARTLCR_H_EPS_Pos)
#define UART_UARTLCR_H_EPS                UART_UARTLCR_H_EPS_Msk
#define UART_UARTLCR_H_PEN_Pos            1U
#define UART_UARTLCR_H_PEN_Msk            (1U << UART_UARTLCR_H_PEN_Pos)
#define UART_UARTLCR_H_PEN                UART_UARTLCR_H_PEN_Msk
#define UART_UARTLCR_H_BRK_Pos            0U
#define UART_UARTLCR_H_BRK_Msk            (1U << UART_UARTLCR_H_BRK_Pos)
#define UART_UARTLCR_H_BRK                UART_UARTLCR_H_BRK_Msk

#define UART_UARTCR_CTSEN_Pos             15U
#define UART_UARTCR_CTSEN_Msk             (1U << UART_UARTCR_CTSEN_Pos)
#define UART_UARTCR_CTSEN                 UART_UARTCR_CTSEN_Msk
#define UART_UARTCR_RTSEN_Pos             14U
#define UART_UARTCR_RTSEN_Msk             (1U << UART_UARTCR_RTSEN_Pos)
#define UART_UARTCR_RTSEN                 UART_UARTCR_RTSEN_Msk
#define UART_UARTCR_OUT2_Pos              13U
#define UART_UARTCR_OUT2_Msk              (1U << UART_UARTCR_OUT2_Pos)
#define UART_UARTCR_OUT2                  UART_UARTCR_OUT2_Msk
#define UART_UARTCR_OUT1_Pos              12U
#define UART_UARTCR_OUT1_Msk              (1U << UART_UARTCR_OUT1_Pos)
#define UART_UARTCR_OUT1                  UART_UARTCR_OUT1_Msk
#define UART_UARTCR_RTS_Pos               11U
#define UART_UARTCR_RTS_Msk               (1U << UART_UARTCR_RTS_Pos)
#define UART_UARTCR_RTS                   UART_UARTCR_RTS_Msk
#define UART_UARTCR_DTR_Pos               10U
#define UART_UARTCR_DTR_Msk               (1U << UART_UARTCR_DTR_Pos)
#define UART_UARTCR_DTR                   UART_UARTCR_DTR_Msk
#define UART_UARTCR_RXE_Pos               9U
#define UART_UARTCR_RXE_Msk               (1U << UART_UARTCR_RXE_Pos)
#define UART_UARTCR_RXE                   UART_UARTCR_RXE_Msk
#define UART_UARTCR_TXE_Pos               8U
#define UART_UARTCR_TXE_Msk               (1U << UART_UARTCR_TXE_Pos)
#define UART_UARTCR_TXE                   UART_UARTCR_TXE_Msk
#define UART_UARTCR_LBE_Pos               7U
#define UART_UARTCR_LBE_Msk               (1U << UART_UARTCR_LBE_Pos)
#define UART_UARTCR_LBE                   UART_UARTCR_LBE_Msk
#define UART_UARTCR_SIRLP_Pos             2U
#define UART_UARTCR_SIRLP_Msk             (1U << UART_UARTCR_SIRLP_Pos)
#define UART_UARTCR_SIRLP                 UART_UARTCR_SIRLP_Msk
#define UART_UARTCR_SIREN_Pos             1U
#define UART_UARTCR_SIREN_Msk             (1U << UART_UARTCR_SIREN_Pos)
#define UART_UARTCR_SIREN                 UART_UARTCR_SIREN_Msk
#define UART_UARTCR_UARTEN_Pos            0U
#define UART_UARTCR_UARTEN_Msk            (1U << UART_UARTCR_UARTEN_Pos)
#define UART_UARTCR_UARTEN                UART_UARTCR_UARTEN_Msk

#define UART_UARTIFLS_RXIFLSEL_Pos        3U
#define UART_UARTIFLS_RXIFLSEL_Msk        (7U << UART_UARTIFLS_RXIFLSEL_Pos)
#define UART_UARTIFLS_RXIFLSEL(n)         ((n) << UART_UARTIFLS_RXIFLSEL_Pos)
#define UART_UARTIFLS_RXIFLSEL_1_8F       UART_UARTIFLS_RXIFLSEL(0U)
#define UART_UARTIFLS_RXIFLSEL_1_4F       UART_UARTIFLS_RXIFLSEL(1U)
#define UART_UARTIFLS_RXIFLSEL_1_2F       UART_UARTIFLS_RXIFLSEL(2U)
#define UART_UARTIFLS_RXIFLSEL_3_4F       UART_UARTIFLS_RXIFLSEL(3U)
#define UART_UARTIFLS_RXIFLSEL_7_8F       UART_UARTIFLS_RXIFLSEL(4U)

#define UART_UARTIFLS_TXIFLSEL_Pos        0U
#define UART_UARTIFLS_TXIFLSEL_Msk        (7U << UART_UARTIFLS_TXIFLSEL_Pos)
#define UART_UARTIFLS_TXIFLSEL(n)         ((n) << UART_UARTIFLS_TXIFLSEL_Pos)
#define UART_UARTIFLS_TXIFLSEL_1_8E       UART_UARTIFLS_TXIFLSEL(0U)
#define UART_UARTIFLS_TXIFLSEL_1_4E       UART_UARTIFLS_TXIFLSEL(1U)
#define UART_UARTIFLS_TXIFLSEL_1_2E       UART_UARTIFLS_TXIFLSEL(2U)
#define UART_UARTIFLS_TXIFLSEL_3_4E       UART_UARTIFLS_TXIFLSEL(3U)
#define UART_UARTIFLS_TXIFLSEL_7_8E       UART_UARTIFLS_TXIFLSEL(4U)

#define UART_UARTIMSC_OEIM_Pos            10U
#define UART_UARTIMSC_OEIM_Msk            (1U << UART_UARTIMSC_OEIM_Pos)
#define UART_UARTIMSC_OEIM                UART_UARTIMSC_OEIM_Msk
#define UART_UARTIMSC_BEIM_Pos            9U
#define UART_UARTIMSC_BEIM_Msk            (1U << UART_UARTIMSC_BEIM_Pos)
#define UART_UARTIMSC_BEIM                UART_UARTIMSC_BEIM_Msk
#define UART_UARTIMSC_PEIM_Pos            8U
#define UART_UARTIMSC_PEIM_Msk            (1U << UART_UARTIMSC_PEIM_Pos)
#define UART_UARTIMSC_PEIM                UART_UARTIMSC_PEIM_Msk
#define UART_UARTIMSC_FEIM_Pos            7U
#define UART_UARTIMSC_FEIM_Msk            (1U << UART_UARTIMSC_FEIM_Pos)
#define UART_UARTIMSC_FEIM                UART_UARTIMSC_FEIM_Msk
#define UART_UARTIMSC_RTIM_Pos            6U
#define UART_UARTIMSC_RTIM_Msk            (1U << UART_UARTIMSC_RTIM_Pos)
#define UART_UARTIMSC_RTIM                UART_UARTIMSC_RTIM_Msk
#define UART_UARTIMSC_TXIM_Pos            5U
#define UART_UARTIMSC_TXIM_Msk            (1U << UART_UARTIMSC_TXIM_Pos)
#define UART_UARTIMSC_TXIM                UART_UARTIMSC_TXIM_Msk
#define UART_UARTIMSC_RXIM_Pos            4U
#define UART_UARTIMSC_RXIM_Msk            (1U << UART_UARTIMSC_RXIM_Pos)
#define UART_UARTIMSC_RXIM                UART_UARTIMSC_RXIM_Msk
#define UART_UARTIMSC_DSRMIM_Pos          3U
#define UART_UARTIMSC_DSRMIM_Msk          (1U << UART_UARTIMSC_DSRMIM_Pos)
#define UART_UARTIMSC_DSRMIM              UART_UARTIMSC_DSRMIM_Msk
#define UART_UARTIMSC_DCDMIM_Pos          2U
#define UART_UARTIMSC_DCDMIM_Msk          (1U << UART_UARTIMSC_DCDMIM_Pos)
#define UART_UARTIMSC_DCDMIM              UART_UARTIMSC_DCDMIM_Msk
#define UART_UARTIMSC_CTSMIM_Pos          1U
#define UART_UARTIMSC_CTSMIM_Msk          (1U << UART_UARTIMSC_CTSMIM_Pos)
#define UART_UARTIMSC_CTSMIM              UART_UARTIMSC_CTSMIM_Msk
#define UART_UARTIMSC_RIMIM_Pos           0U
#define UART_UARTIMSC_RIMIM_Msk           (1U << UART_UARTIMSC_RIMIM_Pos)
#define UART_UARTIMSC_RIMIM               UART_UARTIMSC_RIMIM_Msk

#define UART_UARTRIS_OERIS_Pos            10U
#define UART_UARTRIS_OERIS_Msk            (1U << UART_UARTRIS_OERIS_Pos)
#define UART_UARTRIS_OERIS                UART_UARTRIS_OERIS_Msk
#define UART_UARTRIS_BERIS_Pos            9U
#define UART_UARTRIS_BERIS_Msk            (1U << UART_UARTRIS_BERIS_Pos)
#define UART_UARTRIS_BERIS                UART_UARTRIS_BERIS_Msk
#define UART_UARTRIS_PERIS_Pos            8U
#define UART_UARTRIS_PERIS_Msk            (1U << UART_UARTRIS_PERIS_Pos)
#define UART_UARTRIS_PERIS                UART_UARTRIS_PERIS_Msk
#define UART_UARTRIS_FERIS_Pos            7U
#define UART_UARTRIS_FERIS_Msk            (1U << UART_UARTRIS_FERIS_Pos)
#define UART_UARTRIS_FERIS                UART_UARTRIS_FERIS_Msk
#define UART_UARTRIS_RTRIS_Pos            6U
#define UART_UARTRIS_RTRIS_Msk            (1U << UART_UARTRIS_RTRIS_Pos)
#define UART_UARTRIS_RTRIS                UART_UARTRIS_RTRIS_Msk
#define UART_UARTRIS_TXRIS_Pos            5U
#define UART_UARTRIS_TXRIS_Msk            (1U << UART_UARTRIS_TXRIS_Pos)
#define UART_UARTRIS_TXRIS                UART_UARTRIS_TXRIS_Msk
#define UART_UARTRIS_RXRIS_Pos            4U
#define UART_UARTRIS_RXRIS_Msk            (1U << UART_UARTRIS_RXRIS_Pos)
#define UART_UARTRIS_RXRIS                UART_UARTRIS_RXRIS_Msk
#define UART_UARTRIS_DSRRMIS_Pos          3U
#define UART_UARTRIS_DSRRMIS_Msk          (1U << UART_UARTRIS_DSRRMIS_Pos)
#define UART_UARTRIS_DSRRMIS              UART_UARTRIS_DSRRMIS_Msk
#define UART_UARTRIS_DCDRMIS_Pos          2U
#define UART_UARTRIS_DCDRMIS_Msk          (1U << UART_UARTRIS_DCDRMIS_Pos)
#define UART_UARTRIS_DCDRMIS              UART_UARTRIS_DCDRMIS_Msk
#define UART_UARTRIS_CTSRMIS_Pos          1U
#define UART_UARTRIS_CTSRMIS_Msk          (1U << UART_UARTRIS_CTSRMIS_Pos)
#define UART_UARTRIS_CTSRMIS              UART_UARTRIS_CTSRMIS_Msk
#define UART_UARTRIS_RIRMIS_Pos           0U
#define UART_UARTRIS_RIRMIS_Msk           (1U << UART_UARTRIS_RIRMIS_Pos)
#define UART_UARTRIS_RIRMIS               UART_UARTRIS_RIRMIS_Msk

#define UART_UARTMIS_OEMIS_Pos            10U
#define UART_UARTMIS_OEMIS_Msk            (1U << UART_UARTMIS_OEMIS_Pos)
#define UART_UARTMIS_OEMIS                UART_UARTMIS_OEMIS_Msk
#define UART_UARTMIS_BEMIS_Pos            9U
#define UART_UARTMIS_BEMIS_Msk            (1U << UART_UARTMIS_BEMIS_Pos)
#define UART_UARTMIS_BEMIS                UART_UARTMIS_BEMIS_Msk
#define UART_UARTMIS_PEMIS_Pos            8U
#define UART_UARTMIS_PEMIS_Msk            (1U << UART_UARTMIS_PEMIS_Pos)
#define UART_UARTMIS_PEMIS                UART_UARTMIS_PEMIS_Msk
#define UART_UARTMIS_FEMIS_Pos            7U
#define UART_UARTMIS_FEMIS_Msk            (1U << UART_UARTMIS_FEMIS_Pos)
#define UART_UARTMIS_FEMIS                UART_UARTMIS_FEMIS_Msk
#define UART_UARTMIS_RTMIS_Pos            6U
#define UART_UARTMIS_RTMIS_Msk            (1U << UART_UARTMIS_RTMIS_Pos)
#define UART_UARTMIS_RTMIS                UART_UARTMIS_RTMIS_Msk
#define UART_UARTMIS_TXMIS_Pos            5U
#define UART_UARTMIS_TXMIS_Msk            (1U << UART_UARTMIS_TXMIS_Pos)
#define UART_UARTMIS_TXMIS                UART_UARTMIS_TXMIS_Msk
#define UART_UARTMIS_RXMIS_Pos            4U
#define UART_UARTMIS_RXMIS_Msk            (1U << UART_UARTMIS_RXMIS_Pos)
#define UART_UARTMIS_RXMIS                UART_UARTMIS_RXMIS_Msk
#define UART_UARTMIS_DSRMMIS_Pos          3U
#define UART_UARTMIS_DSRMMIS_Msk          (1U << UART_UARTMIS_DSRMMIS_Pos)
#define UART_UARTMIS_DSRRMIS              UART_UARTMIS_DSRMMIS_Msk
#define UART_UARTMIS_DCDMMIS_Pos          2U
#define UART_UARTMIS_DCDMMIS_Msk          (1U << UART_UARTMIS_DCDMMIS_Pos)
#define UART_UARTMIS_DCDMMIS              UART_UARTMIS_DCDMMIS_Msk
#define UART_UARTMIS_CTSMMIS_Pos          1U
#define UART_UARTMIS_CTSMMIS_Msk          (1U << UART_UARTMIS_CTSMMIS_Pos)
#define UART_UARTMIS_CTSMMIS              UART_UARTMIS_CTSMMIS_Msk
#define UART_UARTMIS_RIMMIS_Pos           0U
#define UART_UARTMIS_RIMMIS_Msk           (1U << UART_UARTMIS_RIMMIS_Pos)
#define UART_UARTMIS_RIMMIS               UART_UARTMIS_RIMMIS_Msk

#define UART_UARTICR_OEIC_Pos             10U
#define UART_UARTICR_OEIC_Msk             (1U << UART_UARTICR_OEIC_Pos)
#define UART_UARTICR_OEIC                 UART_UARTICR_OEIC_Msk
#define UART_UARTICR_BEIC_Pos             9U
#define UART_UARTICR_BEIC_Msk             (1U << UART_UARTICR_BEIC_Pos)
#define UART_UARTICR_BEIC                 UART_UARTICR_BEIC_Msk
#define UART_UARTICR_PEIC_Pos             8U
#define UART_UARTICR_PEIC_Msk             (1U << UART_UARTICR_PEIC_Pos)
#define UART_UARTICR_PEIC                 UART_UARTICR_PEIC_Msk
#define UART_UARTICR_FEIC_Pos             7U
#define UART_UARTICR_FEIC_Msk             (1U << UART_UARTICR_FEIC_Pos)
#define UART_UARTICR_FEIC                 UART_UARTICR_FEIC_Msk
#define UART_UARTICR_RTIC_Pos             6U
#define UART_UARTICR_RTIC_Msk             (1U << UART_UARTICR_RTIC_Pos)
#define UART_UARTICR_RTIC                 UART_UARTICR_RTIC_Msk
#define UART_UARTICR_TXIC_Pos             5U
#define UART_UARTICR_TXIC_Msk             (1U << UART_UARTICR_TXIC_Pos)
#define UART_UARTICR_TXIC                 UART_UARTICR_TXIC_Msk
#define UART_UARTICR_RXIC_Pos             4U
#define UART_UARTICR_RXIC_Msk             (1U << UART_UARTICR_RXIC_Pos)
#define UART_UARTICR_RXIC                 UART_UARTICR_RXIC_Msk
#define UART_UARTICR_DSRMIC_Pos           3U
#define UART_UARTICR_DSRMIC_Msk           (1U << UART_UARTICR_DSRMIC_Pos)
#define UART_UARTICR_DSRMIC               UART_UARTICR_DSRMIC_Msk
#define UART_UARTICR_DCDMIC_Pos           2U
#define UART_UARTICR_DCDMIC_Msk           (1U << UART_UARTICR_DCDMIC_Pos)
#define UART_UARTICR_DCDMIC               UART_UARTICR_DCDMIC_Msk
#define UART_UARTICR_CTSMIC_Pos           1U
#define UART_UARTICR_CTSMIC_Msk           (1U << UART_UARTICR_CTSMIC_Pos)
#define UART_UARTICR_CTSMIC               UART_UARTICR_CTSMIC_Msk
#define UART_UARTICR_RIMIC_Pos            0U
#define UART_UARTICR_RIMIC_Msk            (1U << UART_UARTICR_RIMIC_Pos)
#define UART_UARTICR_RIMIC                UART_UARTICR_RIMIC_Msk

#define UART_UARTDMACR_DMAONERR_Pos       2U
#define UART_UARTDMACR_DMAONERR_Msk       (1U << UART_UARTDMACR_DMAONERR_Pos)
#define UART_UARTDMACR_DMAONERR           UART_UARTDMACR_DMAONERR_Msk
#define UART_UARTDMACR_TXDMAE_Pos         1U
#define UART_UARTDMACR_TXDMAE_Msk         (1U << UART_UARTDMACR_TXDMAE_Pos)
#define UART_UARTDMACR_TXDMAE             UART_UARTDMACR_TXDMAE_Msk
#define UART_UARTDMACR_RXDMAE_Pos         0U
#define UART_UARTDMACR_RXDMAE_Msk         (1U << UART_UARTDMACR_RXDMAE_Pos)
#define UART_UARTDMACR_RXDMAE             UART_UARTDMACR_RXDMAE_Msk
/** @} */

/**
 * @name    QMI M0_TIMING / M1_TIMING bits definitions
 * @note    See RP2350 Datasheet 12.14.6 QMI List of Registers
 * @{
 */
#define QMI_TIMING_CLKDIV_Pos             0U
#define QMI_TIMING_CLKDIV_Msk             (0xFFU << QMI_TIMING_CLKDIV_Pos)
#define QMI_TIMING_CLKDIV(n)              ((n) << QMI_TIMING_CLKDIV_Pos)
#define QMI_TIMING_RXDELAY_Pos            8U
#define QMI_TIMING_RXDELAY_Msk            (0x7U << QMI_TIMING_RXDELAY_Pos)
#define QMI_TIMING_RXDELAY(n)             ((n) << QMI_TIMING_RXDELAY_Pos)
#define QMI_TIMING_MIN_DESELECT_Pos       12U
#define QMI_TIMING_MIN_DESELECT_Msk       (0x1FU << QMI_TIMING_MIN_DESELECT_Pos)
#define QMI_TIMING_MIN_DESELECT(n)        ((n) << QMI_TIMING_MIN_DESELECT_Pos)
#define QMI_TIMING_MAX_SELECT_Pos         17U
#define QMI_TIMING_MAX_SELECT_Msk         (0x3FU << QMI_TIMING_MAX_SELECT_Pos)
#define QMI_TIMING_MAX_SELECT(n)          ((n) << QMI_TIMING_MAX_SELECT_Pos)
#define QMI_TIMING_SELECT_HOLD_Pos        23U
#define QMI_TIMING_SELECT_HOLD_Msk        (0x3U << QMI_TIMING_SELECT_HOLD_Pos)
#define QMI_TIMING_SELECT_HOLD(n)         ((n) << QMI_TIMING_SELECT_HOLD_Pos)
#define QMI_TIMING_SELECT_SETUP           (1U << 25)
#define QMI_TIMING_PAGEBREAK_Pos          28U
#define QMI_TIMING_PAGEBREAK_Msk          (0x3U << QMI_TIMING_PAGEBREAK_Pos)
#define QMI_TIMING_PAGEBREAK(n)           ((n) << QMI_TIMING_PAGEBREAK_Pos)
#define QMI_TIMING_COOLDOWN_Pos           30U
#define QMI_TIMING_COOLDOWN_Msk           (0x3U << QMI_TIMING_COOLDOWN_Pos)
#define QMI_TIMING_COOLDOWN(n)            ((n) << QMI_TIMING_COOLDOWN_Pos)

/** PAGEBREAK encoded values. */
#define QMI_TIMING_PAGEBREAK_NONE         0U
#define QMI_TIMING_PAGEBREAK_256          1U
#define QMI_TIMING_PAGEBREAK_1024         2U
#define QMI_TIMING_PAGEBREAK_4096         3U
/** @} */

/**
 * @name    QMI DIRECT_CSR bits definitions
 * @{
 */
#define QMI_DIRECT_CSR_EN                 (1U << 0)
#define QMI_DIRECT_CSR_BUSY               (1U << 1)
#define QMI_DIRECT_CSR_ASSERT_CS0N        (1U << 2)
#define QMI_DIRECT_CSR_ASSERT_CS1N        (1U << 3)
#define QMI_DIRECT_CSR_AUTO_CS0N          (1U << 6)
#define QMI_DIRECT_CSR_AUTO_CS1N          (1U << 7)
#define QMI_DIRECT_CSR_RXDELAY_Pos        8U
#define QMI_DIRECT_CSR_RXDELAY_Msk        (0x7U << QMI_DIRECT_CSR_RXDELAY_Pos)
#define QMI_DIRECT_CSR_RXDELAY(n)         ((n) << QMI_DIRECT_CSR_RXDELAY_Pos)
#define QMI_DIRECT_CSR_CLKDIV_Pos         22U
#define QMI_DIRECT_CSR_CLKDIV_Msk         (0xFFU << QMI_DIRECT_CSR_CLKDIV_Pos)
#define QMI_DIRECT_CSR_CLKDIV(n)          ((n) << QMI_DIRECT_CSR_CLKDIV_Pos)
/** @} */


/**
 * @name    TICKS peripheral indexes
 * @note    See RP2350 Datasheet 8.5.2 TICKS List of Registers
 * @{
 */
#define TICKS_PROC0                       0U
#define TICKS_PROC1                       1U
#define TICKS_TIMER0                      2U
#define TICKS_TIMER1                      3U
#define TICKS_WATCHDOG                    4U
#define TICKS_RISCV                       5U
/** @} */

/**
 * @name    TICKS peripheral bits definitions
 * @{
 */
#define TICKS_CTRL_ENABLE                 (1U << 0)
#define TICKS_CTRL_RUNNING                (1U << 1)
#define TICKS_DIV(n)                      ((n) << 0)
/** @} */

/**
 * @name    WATCHDOG bits definitions
 * @note    See RP2350 Datasheet 12.9.7 Watchdog List of Registers
 * @{
 */
#define WATCHDOG_CTRL_TRIGGER_Pos         31U
#define WATCHDOG_CTRL_TRIGGER_Msk         (1U << WATCHDOG_CTRL_TRIGGER_Pos)
#define WATCHDOG_CTRL_TRIGGER             WATCHDOG_CTRL_TRIGGER_Msk
#define WATCHDOG_CTRL_ENABLE_Pos          30U
#define WATCHDOG_CTRL_ENABLE_Msk          (1U << WATCHDOG_CTRL_ENABLE_Pos)
#define WATCHDOG_CTRL_ENABLE              WATCHDOG_CTRL_ENABLE_Msk
#define WATCHDOG_CTRL_PAUSE_DBG1_Pos      26U
#define WATCHDOG_CTRL_PAUSE_DBG1_Msk      (1U << WATCHDOG_CTRL_PAUSE_DBG1_Pos)
#define WATCHDOG_CTRL_PAUSE_DBG1          WATCHDOG_CTRL_PAUSE_DBG1_Msk
#define WATCHDOG_CTRL_PAUSE_DBG0_Pos      25U
#define WATCHDOG_CTRL_PAUSE_DBG0_Msk      (1U << WATCHDOG_CTRL_PAUSE_DBG0_Pos)
#define WATCHDOG_CTRL_PAUSE_DBG0          WATCHDOG_CTRL_PAUSE_DBG0_Msk
#define WATCHDOG_CTRL_PAUSE_JTAG_Pos      24U
#define WATCHDOG_CTRL_PAUSE_JTAG_Msk      (1U << WATCHDOG_CTRL_PAUSE_JTAG_Pos)
#define WATCHDOG_CTRL_PAUSE_JTAG          WATCHDOG_CTRL_PAUSE_JTAG_Msk
#define WATCHDOG_CTRL_TIME_Pos            0U
#define WATCHDOG_CTRL_TIME_Msk            (0xFFFFFFU << WATCHDOG_CTRL_TIME_Pos)
#define WATCHDOG_CTRL_TIME                WATCHDOG_CTRL_TIME_Msk

#define WATCHDOG_LOAD_Pos                 0U
#define WATCHDOG_LOAD_Msk                 (0xFFFFFFU << WATCHDOG_LOAD_Pos)
#define WATCHDOG_LOAD                     WATCHDOG_LOAD_Msk

#define WATCHDOG_REASON_FORCE_Pos         1U
#define WATCHDOG_REASON_FORCE_Msk         (1U << WATCHDOG_REASON_FORCE_Pos)
#define WATCHDOG_REASON_FORCE             WATCHDOG_REASON_FORCE_Msk
#define WATCHDOG_REASON_TIMER_Pos         0U
#define WATCHDOG_REASON_TIMER_Msk         (1U << WATCHDOG_REASON_TIMER_Pos)
#define WATCHDOG_REASON_TIMER             WATCHDOG_REASON_TIMER_Msk

#define WATCHDOG_TICK_COUNT_Pos           11U
#define WATCHDOG_TICK_COUNT_Msk           (0x1FFU << WATCHDOG_TICK_COUNT_Pos)
#define WATCHDOG_TICK_COUNT               WATCHDOG_TICK_COUNT_Msk
#define WATCHDOG_TICK_RUNNING_Pos         10U
#define WATCHDOG_TICK_RUNNING_Msk         (1U << WATCHDOG_TICK_RUNNING_Pos)
#define WATCHDOG_TICK_RUNNING             WATCHDOG_TICK_RUNNING_Msk
#define WATCHDOG_TICK_ENABLE_Pos          9U
#define WATCHDOG_TICK_ENABLE_Msk          (1U << WATCHDOG_TICK_ENABLE_Pos)
#define WATCHDOG_TICK_ENABLE              WATCHDOG_TICK_ENABLE_Msk
#define WATCHDOG_TICK_CYCLES_Pos          0U
#define WATCHDOG_TICK_CYCLES_Msk          (0x1FFU << WATCHDOG_TICK_CYCLES_Pos)
#define WATCHDOG_TICK_CYCLES              WATCHDOG_TICK_CYCLES_Msk
/** @} */

/**
 * @name    I2C bits definitions
 * @note    See RP2350 Datasheet 12.2.17 I2C List of Registers
 * @{
 */
#define I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE_Pos  10U
#define I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE_Msk  (1U << I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE_Pos)
#define I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE      I2C_IC_CON_STOP_DET_IF_MASTER_ACTIVE_Msk
#define I2C_IC_CON_RX_FIFO_FULL_HLD_CTRL_Pos      9U
#define I2C_IC_CON_RX_FIFO_FULL_HLD_CTRL_Msk      (1U << I2C_IC_CON_RX_FIFO_FULL_HLD_CTRL_Pos)
#define I2C_IC_CON_RX_FIFO_FULL_HLD_CTRL          I2C_IC_CON_RX_FIFO_FULL_HLD_CTRL_Msk
#define I2C_IC_CON_TX_EMPTY_CTRL_Pos              8U
#define I2C_IC_CON_TX_EMPTY_CTRL_Msk              (1U << I2C_IC_CON_TX_EMPTY_CTRL_Pos)
#define I2C_IC_CON_TX_EMPTY_CTRL                  I2C_IC_CON_TX_EMPTY_CTRL_Msk
#define I2C_IC_CON_STOP_DET_IFADDRESSED_Pos       7U
#define I2C_IC_CON_STOP_DET_IFADDRESSED_Msk       (1U << I2C_IC_CON_STOP_DET_IFADDRESSED_Pos)
#define I2C_IC_CON_STOP_DET_IFADDRESSED           I2C_IC_CON_STOP_DET_IFADDRESSED_Msk
#define I2C_IC_CON_IC_SLAVE_DISABLE_Pos           6U
#define I2C_IC_CON_IC_SLAVE_DISABLE_Msk           (1U << I2C_IC_CON_IC_SLAVE_DISABLE_Pos)
#define I2C_IC_CON_IC_SLAVE_DISABLE               I2C_IC_CON_IC_SLAVE_DISABLE_Msk
#define I2C_IC_CON_IC_RESTART_EN_Pos              5U
#define I2C_IC_CON_IC_RESTART_EN_Msk              (1U << I2C_IC_CON_IC_RESTART_EN_Pos)
#define I2C_IC_CON_IC_RESTART_EN                  I2C_IC_CON_IC_RESTART_EN_Msk
#define I2C_IC_CON_IC_10BITADDR_MASTER_Pos        4U
#define I2C_IC_CON_IC_10BITADDR_MASTER_Msk        (1U << I2C_IC_CON_IC_10BITADDR_MASTER_Pos)
#define I2C_IC_CON_IC_10BITADDR_MASTER            I2C_IC_CON_IC_10BITADDR_MASTER_Msk
#define I2C_IC_CON_IC_10BITADDR_SLAVE_Pos         3U
#define I2C_IC_CON_IC_10BITADDR_SLAVE_Msk         (1U << I2C_IC_CON_IC_10BITADDR_SLAVE_Pos)
#define I2C_IC_CON_IC_10BITADDR_SLAVE             I2C_IC_CON_IC_10BITADDR_SLAVE_Msk
#define I2C_IC_CON_SPEED_Pos                      1U
#define I2C_IC_CON_SPEED_Msk                      (3U << I2C_IC_CON_SPEED_Pos)
#define I2C_IC_CON_SPEED                          I2C_IC_CON_SPEED_Msk
#define I2C_IC_CON_MASTER_MODE_Pos                0U
#define I2C_IC_CON_MASTER_MODE_Msk                (1U << I2C_IC_CON_MASTER_MODE_Pos)
#define I2C_IC_CON_MASTER_MODE                    I2C_IC_CON_MASTER_MODE_Msk

#define I2C_IC_TAR_SPECIAL_Pos                    11U
#define I2C_IC_TAR_SPECIAL_Msk                    (1U << I2C_IC_TAR_SPECIAL_Pos)
#define I2C_IC_TAR_SPECIAL                        I2C_IC_TAR_SPECIAL_Msk
#define I2C_IC_TAR_GC_OR_START_Pos                10U
#define I2C_IC_TAR_GC_OR_START_Msk                (1U << I2C_IC_TAR_GC_OR_START_Pos)
#define I2C_IC_TAR_GC_OR_START                    I2C_IC_TAR_GC_OR_START_Msk
#define I2C_IC_TAR_IC_TAR_Pos                     0U
#define I2C_IC_TAR_IC_TAR_Msk                     (0x3FFU << I2C_IC_TAR_IC_TAR_Pos)
#define I2C_IC_TAR_IC_TAR                         I2C_IC_TAR_IC_TAR_Msk

#define I2C_IC_SAR_IC_SAR_Pos                     0U
#define I2C_IC_SAR_IC_SAR_Msk                     (0x3FFU << I2C_IC_SAR_IC_SAR_Pos)
#define I2C_IC_SAR_IC_SAR                         I2C_IC_SAR_IC_SAR_Msk

#define I2C_IC_DATA_CMD_FIRST_DATA_BYTE_Pos       11U
#define I2C_IC_DATA_CMD_FIRST_DATA_BYTE_Msk       (1U << I2C_IC_DATA_CMD_FIRST_DATA_BYTE_Pos)
#define I2C_IC_DATA_CMD_FIRST_DATA_BYTE           I2C_IC_DATA_CMD_FIRST_DATA_BYTE_Msk
#define I2C_IC_DATA_CMD_RESTART_Pos               10U
#define I2C_IC_DATA_CMD_RESTART_Msk               (1U << I2C_IC_DATA_CMD_RESTART_Pos)
#define I2C_IC_DATA_CMD_RESTART                   I2C_IC_DATA_CMD_RESTART_Msk
#define I2C_IC_DATA_CMD_STOP_Pos                  9U
#define I2C_IC_DATA_CMD_STOP_Msk                  (1U << I2C_IC_DATA_CMD_STOP_Pos)
#define I2C_IC_DATA_CMD_STOP                      I2C_IC_DATA_CMD_STOP_Msk
#define I2C_IC_DATA_CMD_CMD_Pos                   8U
#define I2C_IC_DATA_CMD_CMD_Msk                   (1U << I2C_IC_DATA_CMD_CMD_Pos)
#define I2C_IC_DATA_CMD_CMD                       I2C_IC_DATA_CMD_CMD_Msk
#define I2C_IC_DATA_CMD_DAT_Pos                   0U
#define I2C_IC_DATA_CMD_DAT_Msk                   (0xFFU << I2C_IC_DATA_CMD_DAT_Pos)
#define I2C_IC_DATA_CMD_DAT                       I2C_IC_DATA_CMD_DAT_Msk

#define I2C_IC_SS_SCL_HCNT_Pos                    0U
#define I2C_IC_SS_SCL_HCNT_Msk                    (0xFFFFU << I2C_IC_SS_SCL_HCNT_Pos)
#define I2C_IC_SS_SCL_HCNT                        I2C_IC_SS_SCL_HCNT_Msk

#define I2C_IC_SS_SCL_LCNT_Pos                    0U
#define I2C_IC_SS_SCL_LCNT_Msk                    (0xFFFFU << I2C_IC_SS_SCL_LCNT_Pos)
#define I2C_IC_SS_SCL_LCNT                        I2C_IC_SS_SCL_LCNT_Msk

#define I2C_IC_FS_SCL_HCNT_Pos                    0U
#define I2C_IC_FS_SCL_HCNT_Msk                    (0xFFFFU << I2C_IC_FS_SCL_HCNT_Pos)
#define I2C_IC_FS_SCL_HCNT                        I2C_IC_FS_SCL_HCNT_Msk

#define I2C_IC_FS_SCL_LCNT_Pos                    0U
#define I2C_IC_FS_SCL_LCNT_Msk                    (0xFFFFU << I2C_IC_FS_SCL_LCNT_Pos)
#define I2C_IC_FS_SCL_LCNT                        I2C_IC_FS_SCL_LCNT_Msk

#define I2C_IC_INTR_STAT_R_MASTER_ON_HOLD_Pos     13U
#define I2C_IC_INTR_STAT_R_MASTER_ON_HOLD_Msk     (1U << I2C_IC_INTR_STAT_R_MASTER_ON_HOLD_Pos)
#define I2C_IC_INTR_STAT_R_MASTER_ON_HOLD         I2C_IC_INTR_STAT_R_MASTER_ON_HOLD_Msk
#define I2C_IC_INTR_STAT_R_RESTART_DET_Pos        12U
#define I2C_IC_INTR_STAT_R_RESTART_DET_Msk        (1U << I2C_IC_INTR_STAT_R_RESTART_DET_Pos)
#define I2C_IC_INTR_STAT_R_RESTART_DET            I2C_IC_INTR_STAT_R_RESTART_DET_Msk
#define I2C_IC_INTR_STAT_R_GEN_CALL_Pos           11U
#define I2C_IC_INTR_STAT_R_GEN_CALL_Msk           (1U << I2C_IC_INTR_STAT_R_GEN_CALL_Pos)
#define I2C_IC_INTR_STAT_R_GEN_CALL               I2C_IC_INTR_STAT_R_GEN_CALL_Msk
#define I2C_IC_INTR_STAT_R_START_DET_Pos          10U
#define I2C_IC_INTR_STAT_R_START_DET_Msk          (1U << I2C_IC_INTR_STAT_R_START_DET_Pos)
#define I2C_IC_INTR_STAT_R_START_DET              I2C_IC_INTR_STAT_R_START_DET_Msk
#define I2C_IC_INTR_STAT_R_STOP_DET_Pos           9U
#define I2C_IC_INTR_STAT_R_STOP_DET_Msk           (1U << I2C_IC_INTR_STAT_R_STOP_DET_Pos)
#define I2C_IC_INTR_STAT_R_STOP_DET               I2C_IC_INTR_STAT_R_STOP_DET_Msk
#define I2C_IC_INTR_STAT_R_ACTIVITY_Pos           8U
#define I2C_IC_INTR_STAT_R_ACTIVITY_Msk           (1U << I2C_IC_INTR_STAT_R_ACTIVITY_Pos)
#define I2C_IC_INTR_STAT_R_ACTIVITY               I2C_IC_INTR_STAT_R_ACTIVITY_Msk
#define I2C_IC_INTR_STAT_R_RX_DONE_Pos            7U
#define I2C_IC_INTR_STAT_R_RX_DONE_Msk            (1U << I2C_IC_INTR_STAT_R_RX_DONE_Pos)
#define I2C_IC_INTR_STAT_R_RX_DONE                I2C_IC_INTR_STAT_R_RX_DONE_Msk
#define I2C_IC_INTR_STAT_R_TX_ABRT_Pos            6U
#define I2C_IC_INTR_STAT_R_TX_ABRT_Msk            (1U << I2C_IC_INTR_STAT_R_TX_ABRT_Pos)
#define I2C_IC_INTR_STAT_R_TX_ABRT                I2C_IC_INTR_STAT_R_TX_ABRT_Msk
#define I2C_IC_INTR_STAT_R_RD_REQ_Pos             5U
#define I2C_IC_INTR_STAT_R_RD_REQ_Msk             (1U << I2C_IC_INTR_STAT_R_RD_REQ_Pos)
#define I2C_IC_INTR_STAT_R_RD_REQ                 I2C_IC_INTR_STAT_R_RD_REQ_Msk
#define I2C_IC_INTR_STAT_R_TX_EMPTY_Pos           4U
#define I2C_IC_INTR_STAT_R_TX_EMPTY_Msk           (1U << I2C_IC_INTR_STAT_R_TX_EMPTY_Pos)
#define I2C_IC_INTR_STAT_R_TX_EMPTY               I2C_IC_INTR_STAT_R_TX_EMPTY_Msk
#define I2C_IC_INTR_STAT_R_TX_OVER_Pos            3U
#define I2C_IC_INTR_STAT_R_TX_OVER_Msk            (1U << I2C_IC_INTR_STAT_R_TX_OVER_Pos)
#define I2C_IC_INTR_STAT_R_TX_OVER                I2C_IC_INTR_STAT_R_TX_OVER_Msk
#define I2C_IC_INTR_STAT_R_RX_FULL_Pos            2U
#define I2C_IC_INTR_STAT_R_RX_FULL_Msk            (1U << I2C_IC_INTR_STAT_R_RX_FULL_Pos)
#define I2C_IC_INTR_STAT_R_RX_FULL                I2C_IC_INTR_STAT_R_RX_FULL_Msk
#define I2C_IC_INTR_STAT_R_RX_OVER_Pos            1U
#define I2C_IC_INTR_STAT_R_RX_OVER_Msk            (1U << I2C_IC_INTR_STAT_R_RX_OVER_Pos)
#define I2C_IC_INTR_STAT_R_RX_OVER                I2C_IC_INTR_STAT_R_RX_OVER_Msk
#define I2C_IC_INTR_STAT_R_RX_UNDER_Pos           0U
#define I2C_IC_INTR_STAT_R_RX_UNDER_Msk           (1U << I2C_IC_INTR_STAT_R_RX_UNDER_Pos)
#define I2C_IC_INTR_STAT_R_RX_UNDER               I2C_IC_INTR_STAT_R_RX_UNDER_Msk

#define I2C_IC_INTR_MASK_M_MASTER_ON_HOLD_Pos     13U
#define I2C_IC_INTR_MASK_M_MASTER_ON_HOLD_Msk     (1U << I2C_IC_INTR_MASK_M_MASTER_ON_HOLD_Pos)
#define I2C_IC_INTR_MASK_M_MASTER_ON_HOLD         I2C_IC_INTR_MASK_M_MASTER_ON_HOLD_Msk
#define I2C_IC_INTR_MASK_M_RESTART_DET_Pos        12U
#define I2C_IC_INTR_MASK_M_RESTART_DET_Msk        (1U << I2C_IC_INTR_MASK_M_RESTART_DET_Pos)
#define I2C_IC_INTR_MASK_M_RESTART_DET            I2C_IC_INTR_MASK_M_RESTART_DET_Msk
#define I2C_IC_INTR_MASK_M_GEN_CALL_Pos           11U
#define I2C_IC_INTR_MASK_M_GEN_CALL_Msk           (1U << I2C_IC_INTR_MASK_M_GEN_CALL_Pos)
#define I2C_IC_INTR_MASK_M_GEN_CALL               I2C_IC_INTR_MASK_M_GEN_CALL_Msk
#define I2C_IC_INTR_MASK_M_START_DET_Pos          10U
#define I2C_IC_INTR_MASK_M_START_DET_Msk          (1U << I2C_IC_INTR_MASK_M_START_DET_Pos)
#define I2C_IC_INTR_MASK_M_START_DET              I2C_IC_INTR_MASK_M_START_DET_Msk
#define I2C_IC_INTR_MASK_M_STOP_DET_Pos           9U
#define I2C_IC_INTR_MASK_M_STOP_DET_Msk           (1U << I2C_IC_INTR_MASK_M_STOP_DET_Pos)
#define I2C_IC_INTR_MASK_M_STOP_DET               I2C_IC_INTR_MASK_M_STOP_DET_Msk
#define I2C_IC_INTR_MASK_M_ACTIVITY_Pos           8U
#define I2C_IC_INTR_MASK_M_ACTIVITY_Msk           (1U << I2C_IC_INTR_MASK_M_ACTIVITY_Pos)
#define I2C_IC_INTR_MASK_M_ACTIVITY               I2C_IC_INTR_MASK_M_ACTIVITY_Msk
#define I2C_IC_INTR_MASK_M_RX_DONE_Pos            7U
#define I2C_IC_INTR_MASK_M_RX_DONE_Msk            (1U << I2C_IC_INTR_MASK_M_RX_DONE_Pos)
#define I2C_IC_INTR_MASK_M_RX_DONE                I2C_IC_INTR_MASK_M_RX_DONE_Msk
#define I2C_IC_INTR_MASK_M_TX_ABRT_Pos            6U
#define I2C_IC_INTR_MASK_M_TX_ABRT_Msk            (1U << I2C_IC_INTR_MASK_M_TX_ABRT_Pos)
#define I2C_IC_INTR_MASK_M_TX_ABRT                I2C_IC_INTR_MASK_M_TX_ABRT_Msk
#define I2C_IC_INTR_MASK_M_RD_REQ_Pos             5U
#define I2C_IC_INTR_MASK_M_RD_REQ_Msk             (1U << I2C_IC_INTR_MASK_M_RD_REQ_Pos)
#define I2C_IC_INTR_MASK_M_RD_REQ                 I2C_IC_INTR_MASK_M_RD_REQ_Msk
#define I2C_IC_INTR_MASK_M_TX_EMPTY_Pos           4U
#define I2C_IC_INTR_MASK_M_TX_EMPTY_Msk           (1U << I2C_IC_INTR_MASK_M_TX_EMPTY_Pos)
#define I2C_IC_INTR_MASK_M_TX_EMPTY               I2C_IC_INTR_MASK_M_TX_EMPTY_Msk
#define I2C_IC_INTR_MASK_M_TX_OVER_Pos            3U
#define I2C_IC_INTR_MASK_M_TX_OVER_Msk            (1U << I2C_IC_INTR_MASK_M_TX_OVER_Pos)
#define I2C_IC_INTR_MASK_M_TX_OVER                I2C_IC_INTR_MASK_M_TX_OVER_Msk
#define I2C_IC_INTR_MASK_M_RX_FULL_Pos            2U
#define I2C_IC_INTR_MASK_M_RX_FULL_Msk            (1U << I2C_IC_INTR_MASK_M_RX_FULL_Pos)
#define I2C_IC_INTR_MASK_M_RX_FULL                I2C_IC_INTR_MASK_M_RX_FULL_Msk
#define I2C_IC_INTR_MASK_M_RX_OVER_Pos            1U
#define I2C_IC_INTR_MASK_M_RX_OVER_Msk            (1U << I2C_IC_INTR_MASK_M_RX_OVER_Pos)
#define I2C_IC_INTR_MASK_M_RX_OVER                I2C_IC_INTR_MASK_M_RX_OVER_Msk
#define I2C_IC_INTR_MASK_M_RX_UNDER_Pos           0U
#define I2C_IC_INTR_MASK_M_RX_UNDER_Msk           (1U << I2C_IC_INTR_MASK_M_RX_UNDER_Pos)
#define I2C_IC_INTR_MASK_M_RX_UNDER               I2C_IC_INTR_MASK_M_RX_UNDER_Msk

#define I2C_IC_RX_TL_RX_TL_Pos                    0U
#define I2C_IC_RX_TL_RX_TL_Msk                    (0xFFU << I2C_IC_RX_TL_RX_TL_Pos)
#define I2C_IC_RX_TL_RX_TL                        I2C_IC_RX_TL_RX_TL_Msk

#define I2C_IC_TX_TL_TX_TL_Pos                    0U
#define I2C_IC_TX_TL_TX_TL_Msk                    (0xFFU << I2C_IC_TX_TL_TX_TL_Pos)
#define I2C_IC_TX_TL_TX_TL                        I2C_IC_TX_TL_TX_TL_Msk

#define I2C_IC_CLR_INTR_CLR_INTR_Pos              0U
#define I2C_IC_CLR_INTR_CLR_INTR_Msk              (1U << I2C_IC_CLR_INTR_CLR_INTR_Pos)
#define I2C_IC_CLR_INTR_CLR_INTR                  I2C_IC_CLR_INTR_CLR_INTR_Msk

#define I2C_IC_ENABLE_TX_CMD_BLOCK_Pos            2U
#define I2C_IC_ENABLE_TX_CMD_BLOCK_Msk            (1U << I2C_IC_ENABLE_TX_CMD_BLOCK_Pos)
#define I2C_IC_ENABLE_TX_CMD_BLOCK                I2C_IC_ENABLE_TX_CMD_BLOCK_Msk
#define I2C_IC_ENABLE_ABORT_Pos                   1U
#define I2C_IC_ENABLE_ABORT_Msk                   (1U << I2C_IC_ENABLE_ABORT_Pos)
#define I2C_IC_ENABLE_ABORT                       I2C_IC_ENABLE_ABORT_Msk
#define I2C_IC_ENABLE_ENABLE_Pos                  0U
#define I2C_IC_ENABLE_ENABLE_Msk                  (1U << I2C_IC_ENABLE_ENABLE_Pos)
#define I2C_IC_ENABLE_ENABLE                      I2C_IC_ENABLE_ENABLE_Msk

#define I2C_IC_STATUS_SLV_ACTIVITY_Pos            6U
#define I2C_IC_STATUS_SLV_ACTIVITY_Msk            (1U << I2C_IC_STATUS_SLV_ACTIVITY_Pos)
#define I2C_IC_STATUS_SLV_ACTIVITY                I2C_IC_STATUS_SLV_ACTIVITY_Msk
#define I2C_IC_STATUS_MST_ACTIVITY_Pos            5U
#define I2C_IC_STATUS_MST_ACTIVITY_Msk            (1U << I2C_IC_STATUS_MST_ACTIVITY_Pos)
#define I2C_IC_STATUS_MST_ACTIVITY                I2C_IC_STATUS_MST_ACTIVITY_Msk
#define I2C_IC_STATUS_RFF_Pos                     4U
#define I2C_IC_STATUS_RFF_Msk                     (1U << I2C_IC_STATUS_RFF_Pos)
#define I2C_IC_STATUS_RFF                         I2C_IC_STATUS_RFF_Msk
#define I2C_IC_STATUS_RFNE_Pos                    3U
#define I2C_IC_STATUS_RFNE_Msk                    (1U << I2C_IC_STATUS_RFNE_Pos)
#define I2C_IC_STATUS_RFNE                        I2C_IC_STATUS_RFNE_Msk
#define I2C_IC_STATUS_TFE_Pos                     2U
#define I2C_IC_STATUS_TFE_Msk                     (1U << I2C_IC_STATUS_TFE_Pos)
#define I2C_IC_STATUS_TFE                         I2C_IC_STATUS_TFE_Msk
#define I2C_IC_STATUS_TFNF_Pos                    1U
#define I2C_IC_STATUS_TFNF_Msk                    (1U << I2C_IC_STATUS_TFNF_Pos)
#define I2C_IC_STATUS_TFNF                        I2C_IC_STATUS_TFNF_Msk
#define I2C_IC_STATUS_ACTIVITY_Pos                0U
#define I2C_IC_STATUS_ACTIVITY_Msk                (1U << I2C_IC_STATUS_ACTIVITY_Pos)
#define I2C_IC_STATUS_ACTIVITY                    I2C_IC_STATUS_ACTIVITY_Msk

#define I2C_IC_TXFLR_TXFLR_Pos                    0U
#define I2C_IC_TXFLR_TXFLR_Msk                    (0x1FU << I2C_IC_TXFLR_TXFLR_Pos)
#define I2C_IC_TXFLR_TXFLR                        I2C_IC_TXFLR_TXFLR_Msk

#define I2C_IC_RXFLR_RXFLR_Pos                    0U
#define I2C_IC_RXFLR_RXFLR_Msk                    (0x1FU << I2C_IC_RXFLR_RXFLR_Pos)
#define I2C_IC_RXFLR_RXFLR                        I2C_IC_RXFLR_RXFLR_Msk

#define I2C_IC_SDA_HOLD_IC_SDA_RX_HOLD_Pos        16U
#define I2C_IC_SDA_HOLD_IC_SDA_RX_HOLD_Msk        (0xFFU << I2C_IC_SDA_HOLD_IC_SDA_RX_HOLD_Pos)
#define I2C_IC_SDA_HOLD_IC_SDA_RX_HOLD            I2C_IC_SDA_HOLD_IC_SDA_RX_HOLD_Msk
#define I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_Pos        0U
#define I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_Msk        (0xFFFFU << I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_Pos)
#define I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD            I2C_IC_SDA_HOLD_IC_SDA_TX_HOLD_Msk

#define I2C_IC_DMA_CR_TDMAE_Pos                   1U
#define I2C_IC_DMA_CR_TDMAE_Msk                   (1U << I2C_IC_DMA_CR_TDMAE_Pos)
#define I2C_IC_DMA_CR_TDMAE                       I2C_IC_DMA_CR_TDMAE_Msk
#define I2C_IC_DMA_CR_RDMAE_Pos                   0U
#define I2C_IC_DMA_CR_RDMAE_Msk                   (1U << I2C_IC_DMA_CR_RDMAE_Pos)
#define I2C_IC_DMA_CR_RDMAE                       I2C_IC_DMA_CR_RDMAE_Msk

#define I2C_IC_DMA_TDLR_DMATDL_Pos                0U
#define I2C_IC_DMA_TDLR_DMATDL_Msk                (0xFU << I2C_IC_DMA_TDLR_DMATDL_Pos)
#define I2C_IC_DMA_TDLR_DMATDL                    I2C_IC_DMA_TDLR_DMATDL_Msk

#define I2C_IC_DMA_RDLR_DMARDL_Pos                0U
#define I2C_IC_DMA_RDLR_DMARDL_Msk                (0xFU << I2C_IC_DMA_RDLR_DMARDL_Pos)
#define I2C_IC_DMA_RDLR_DMARDL                    I2C_IC_DMA_RDLR_DMARDL_Msk

#define I2C_IC_SDA_SETUP_SDA_SETUP_Pos            0U
#define I2C_IC_SDA_SETUP_SDA_SETUP_Msk            (0xFFU << I2C_IC_SDA_SETUP_SDA_SETUP_Pos)
#define I2C_IC_SDA_SETUP_SDA_SETUP                I2C_IC_SDA_SETUP_SDA_SETUP_Msk

#define I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_Pos  0U
#define I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_Msk  (1U << I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_Pos)
#define I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL      I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_Msk

#define I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos 2U
#define I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk (1U << I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos)
#define I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST     I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk
#define I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos 1U
#define I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk (1U << I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos)
#define I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY     I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk
#define I2C_IC_ENABLE_STATUS_IC_EN_Pos            0U
#define I2C_IC_ENABLE_STATUS_IC_EN_Msk            (1U << I2C_IC_ENABLE_STATUS_IC_EN_Pos)
#define I2C_IC_ENABLE_STATUS_IC_EN                I2C_IC_ENABLE_STATUS_IC_EN_Msk

#define I2C_IC_FS_SPKLEN_IC_FS_SPKLEN_Pos         0U
#define I2C_IC_FS_SPKLEN_IC_FS_SPKLEN_Msk         (0xFFU << I2C_IC_FS_SPKLEN_IC_FS_SPKLEN_Pos)
#define I2C_IC_FS_SPKLEN_IC_FS_SPKLEN             I2C_IC_FS_SPKLEN_IC_FS_SPKLEN_Msk
/** @} */

/**
 * @name    PWM bits definitions
 * @note    See RP2350 Datasheet 12.5.3 PWM List of Registers
 * @{
 */
#define PWM_EN_CH11_Pos                   11U
#define PWM_EN_CH11_Msk                   (1U << PWM_EN_CH11_Pos)
#define PWM_EN_CH11                       PWM_EN_CH11_Msk
#define PWM_EN_CH10_Pos                   10U
#define PWM_EN_CH10_Msk                   (1U << PWM_EN_CH10_Pos)
#define PWM_EN_CH10                       PWM_EN_CH10_Msk
#define PWM_EN_CH9_Pos                    9U
#define PWM_EN_CH9_Msk                    (1U << PWM_EN_CH9_Pos)
#define PWM_EN_CH9                        PWM_EN_CH9_Msk
#define PWM_EN_CH8_Pos                    8U
#define PWM_EN_CH8_Msk                    (1U << PWM_EN_CH8_Pos)
#define PWM_EN_CH8                        PWM_EN_CH8_Msk
#define PWM_EN_CH7_Pos                    7U
#define PWM_EN_CH7_Msk                    (1U << PWM_EN_CH7_Pos)
#define PWM_EN_CH7                        PWM_EN_CH7_Msk
#define PWM_EN_CH6_Pos                    6U
#define PWM_EN_CH6_Msk                    (1U << PWM_EN_CH6_Pos)
#define PWM_EN_CH6                        PWM_EN_CH6_Msk
#define PWM_EN_CH5_Pos                    5U
#define PWM_EN_CH5_Msk                    (1U << PWM_EN_CH5_Pos)
#define PWM_EN_CH5                        PWM_EN_CH5_Msk
#define PWM_EN_CH4_Pos                    4U
#define PWM_EN_CH4_Msk                    (1U << PWM_EN_CH4_Pos)
#define PWM_EN_CH4                        PWM_EN_CH4_Msk
#define PWM_EN_CH3_Pos                    3U
#define PWM_EN_CH3_Msk                    (1U << PWM_EN_CH3_Pos)
#define PWM_EN_CH3                        PWM_EN_CH3_Msk
#define PWM_EN_CH2_Pos                    2U
#define PWM_EN_CH2_Msk                    (1U << PWM_EN_CH2_Pos)
#define PWM_EN_CH2                        PWM_EN_CH2_Msk
#define PWM_EN_CH1_Pos                    1U
#define PWM_EN_CH1_Msk                    (1U << PWM_EN_CH1_Pos)
#define PWM_EN_CH1                        PWM_EN_CH1_Msk
#define PWM_EN_CH0_Pos                    0U
#define PWM_EN_CH0_Msk                    (1U << PWM_EN_CH0_Pos)
#define PWM_EN_CH0                        PWM_EN_CH0_Msk
#define PWM_INTR_CH(n)                    (1U << (n))
#define PWM_INTE_CH(n)                    (1U << (n))
#define PWM_INTF_CH(n)                    (1U << (n))
#define PWM_INTS_CH(n)                    (1U << (n))
/** @} */

/**
 * @name    XOSC bits definitions
 * @note    See RP2350 Datasheet 8.2.8 XOSC List of Registers
 * @{
 */
#define XOSC_CTRL_ENABLE_Pos              12U
#define XOSC_CTRL_ENABLE_Msk              (0xFFFU << XOSC_CTRL_ENABLE_Pos)
#define XOSC_CTRL_ENABLE_DISABLE          (0xD1EU << XOSC_CTRL_ENABLE_Pos)
#define XOSC_CTRL_ENABLE_ENABLE           (0xFABU << XOSC_CTRL_ENABLE_Pos)
#define XOSC_CTRL_FREQ_RANGE_Pos          0U
#define XOSC_CTRL_FREQ_RANGE_Msk          (0xFFFU << XOSC_CTRL_FREQ_RANGE_Pos)
#define XOSC_CTRL_FREQ_RANGE_1_15MHZ      (0xAA0U << XOSC_CTRL_FREQ_RANGE_Pos)

#define XOSC_STATUS_STABLE_Pos            31U
#define XOSC_STATUS_STABLE_Msk            (1U << XOSC_STATUS_STABLE_Pos)
#define XOSC_STATUS_STABLE                XOSC_STATUS_STABLE_Msk
#define XOSC_STATUS_BADWRITE_Pos          24U
#define XOSC_STATUS_BADWRITE_Msk          (1U << XOSC_STATUS_BADWRITE_Pos)
#define XOSC_STATUS_BADWRITE              XOSC_STATUS_BADWRITE_Msk
#define XOSC_STATUS_ENABLED_Pos           12U
#define XOSC_STATUS_ENABLED_Msk           (1U << XOSC_STATUS_ENABLED_Pos)
#define XOSC_STATUS_ENABLED               XOSC_STATUS_ENABLED_Msk

#define XOSC_DORMANT_DORMANT              0x636F6D61U
#define XOSC_DORMANT_WAKE                 0x77616B65U

#define XOSC_STARTUP_X4_Pos               20U
#define XOSC_STARTUP_X4_Msk               (1U << XOSC_STARTUP_X4_Pos)
#define XOSC_STARTUP_X4                   XOSC_STARTUP_X4_Msk
#define XOSC_STARTUP_DELAY_Pos            0U
#define XOSC_STARTUP_DELAY_Msk            (0x3FFFU << XOSC_STARTUP_DELAY_Pos)
#define XOSC_STARTUP_DELAY(n)             ((n) << XOSC_STARTUP_DELAY_Pos)
/** @} */

/**
 * @name    PLL bits definitions
 * @note    See RP2350 Datasheet 8.6.5 PLL List of Registers
 * @{
 */
#define PLL_CS_LOCK_Pos                   31U
#define PLL_CS_LOCK_Msk                   (1U << PLL_CS_LOCK_Pos)
#define PLL_CS_LOCK                       PLL_CS_LOCK_Msk
#define PLL_CS_BYPASS_Pos                 8U
#define PLL_CS_BYPASS_Msk                 (1U << PLL_CS_BYPASS_Pos)
#define PLL_CS_BYPASS                     PLL_CS_BYPASS_Msk
#define PLL_CS_REFDIV_Pos                 0U
#define PLL_CS_REFDIV_Msk                 (0x3FU << PLL_CS_REFDIV_Pos)
#define PLL_CS_REFDIV(n)                  ((n) << PLL_CS_REFDIV_Pos)

#define PLL_PWR_VCOPD_Pos                 5U
#define PLL_PWR_VCOPD_Msk                 (1U << PLL_PWR_VCOPD_Pos)
#define PLL_PWR_VCOPD                     PLL_PWR_VCOPD_Msk
#define PLL_PWR_POSTDIVPD_Pos             3U
#define PLL_PWR_POSTDIVPD_Msk             (1U << PLL_PWR_POSTDIVPD_Pos)
#define PLL_PWR_POSTDIVPD                 PLL_PWR_POSTDIVPD_Msk
#define PLL_PWR_DSMPD_Pos                 2U
#define PLL_PWR_DSMPD_Msk                 (1U << PLL_PWR_DSMPD_Pos)
#define PLL_PWR_DSMPD                     PLL_PWR_DSMPD_Msk
#define PLL_PWR_PD_Pos                    0U
#define PLL_PWR_PD_Msk                    (1U << PLL_PWR_PD_Pos)
#define PLL_PWR_PD                        PLL_PWR_PD_Msk

#define PLL_FBDIV_INT_Pos                 0U
#define PLL_FBDIV_INT_Msk                 (0xFFFU << PLL_FBDIV_INT_Pos)
#define PLL_FBDIV_INT(n)                  ((n) << PLL_FBDIV_INT_Pos)

#define PLL_PRIM_POSTDIV1_Pos             16U
#define PLL_PRIM_POSTDIV1_Msk             (0x7U << PLL_PRIM_POSTDIV1_Pos)
#define PLL_PRIM_POSTDIV1(n)              ((n) << PLL_PRIM_POSTDIV1_Pos)
#define PLL_PRIM_POSTDIV2_Pos             12U
#define PLL_PRIM_POSTDIV2_Msk             (0x7U << PLL_PRIM_POSTDIV2_Pos)
#define PLL_PRIM_POSTDIV2(n)              ((n) << PLL_PRIM_POSTDIV2_Pos)
/** @} */

/**
 * @name    CLOCKS bits definitions
 * @note    See RP2350 Datasheet 8.1.17 Clocks List of Registers
 * @{
 */
/* CLK_REF CTRL */
#define CLOCKS_CLK_REF_CTRL_SRC_Pos       0U
#define CLOCKS_CLK_REF_CTRL_SRC_Msk       (0x3U << CLOCKS_CLK_REF_CTRL_SRC_Pos)
#define CLOCKS_CLK_REF_CTRL_SRC_ROSC      (0x0U << CLOCKS_CLK_REF_CTRL_SRC_Pos)
#define CLOCKS_CLK_REF_CTRL_SRC_AUX       (0x1U << CLOCKS_CLK_REF_CTRL_SRC_Pos)
#define CLOCKS_CLK_REF_CTRL_SRC_XOSC      (0x2U << CLOCKS_CLK_REF_CTRL_SRC_Pos)
#define CLOCKS_CLK_REF_CTRL_AUXSRC_Pos    5U
#define CLOCKS_CLK_REF_CTRL_AUXSRC_Msk    (0x3U << CLOCKS_CLK_REF_CTRL_AUXSRC_Pos)

/* CLK_SYS CTRL */
#define CLOCKS_CLK_SYS_CTRL_SRC_Pos       0U
#define CLOCKS_CLK_SYS_CTRL_SRC_Msk       (0x1U << CLOCKS_CLK_SYS_CTRL_SRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_SRC_REF       (0x0U << CLOCKS_CLK_SYS_CTRL_SRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_SRC_AUX       (0x1U << CLOCKS_CLK_SYS_CTRL_SRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos    5U
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_Msk    (0x7U << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_PLL_SYS  (0x0U << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_PLL_USB  (0x1U << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_ROSC     (0x2U << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_SYS_CTRL_AUXSRC_XOSC     (0x3U << CLOCKS_CLK_SYS_CTRL_AUXSRC_Pos)

/* CLK_PERI CTRL */
#define CLOCKS_CLK_PERI_CTRL_ENABLE_Pos   11U
#define CLOCKS_CLK_PERI_CTRL_ENABLE_Msk   (1U << CLOCKS_CLK_PERI_CTRL_ENABLE_Pos)
#define CLOCKS_CLK_PERI_CTRL_ENABLE       CLOCKS_CLK_PERI_CTRL_ENABLE_Msk
#define CLOCKS_CLK_PERI_CTRL_KILL_Pos     10U
#define CLOCKS_CLK_PERI_CTRL_KILL_Msk     (1U << CLOCKS_CLK_PERI_CTRL_KILL_Pos)
#define CLOCKS_CLK_PERI_CTRL_KILL         CLOCKS_CLK_PERI_CTRL_KILL_Msk
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos   5U
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_Msk   (0x7U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_SYS   (0x0U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_PLL_SYS (0x1U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_PLL_USB (0x2U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_ROSC  (0x3U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_XOSC  (0x4U << CLOCKS_CLK_PERI_CTRL_AUXSRC_Pos)

/* CLK_USB CTRL */
#define CLOCKS_CLK_USB_CTRL_ENABLE_Pos    11U
#define CLOCKS_CLK_USB_CTRL_ENABLE_Msk    (1U << CLOCKS_CLK_USB_CTRL_ENABLE_Pos)
#define CLOCKS_CLK_USB_CTRL_ENABLE        CLOCKS_CLK_USB_CTRL_ENABLE_Msk
#define CLOCKS_CLK_USB_CTRL_KILL_Pos      10U
#define CLOCKS_CLK_USB_CTRL_KILL_Msk      (1U << CLOCKS_CLK_USB_CTRL_KILL_Pos)
#define CLOCKS_CLK_USB_CTRL_KILL          CLOCKS_CLK_USB_CTRL_KILL_Msk
#define CLOCKS_CLK_USB_CTRL_AUXSRC_Pos    5U
#define CLOCKS_CLK_USB_CTRL_AUXSRC_Msk    (0x7U << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_USB_CTRL_AUXSRC_PLL_USB (0x0U << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_USB_CTRL_AUXSRC_PLL_SYS (0x1U << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_USB_CTRL_AUXSRC_ROSC   (0x2U << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_USB_CTRL_AUXSRC_XOSC   (0x3U << CLOCKS_CLK_USB_CTRL_AUXSRC_Pos)

/* CLK_ADC CTRL */
#define CLOCKS_CLK_ADC_CTRL_ENABLE_Pos    11U
#define CLOCKS_CLK_ADC_CTRL_ENABLE_Msk    (1U << CLOCKS_CLK_ADC_CTRL_ENABLE_Pos)
#define CLOCKS_CLK_ADC_CTRL_ENABLE        CLOCKS_CLK_ADC_CTRL_ENABLE_Msk
#define CLOCKS_CLK_ADC_CTRL_KILL_Pos      10U
#define CLOCKS_CLK_ADC_CTRL_KILL_Msk      (1U << CLOCKS_CLK_ADC_CTRL_KILL_Pos)
#define CLOCKS_CLK_ADC_CTRL_KILL          CLOCKS_CLK_ADC_CTRL_KILL_Msk
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos    5U
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_Msk    (0x7U << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_PLL_USB (0x0U << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_PLL_SYS (0x1U << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_ROSC   (0x2U << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_XOSC   (0x3U << CLOCKS_CLK_ADC_CTRL_AUXSRC_Pos)
/** @} */

/**
 * @name    Clock indexes for RP2350
 * @{
 */
#define RP_CLK_GPOUT0                     0U
#define RP_CLK_GPOUT1                     1U
#define RP_CLK_GPOUT2                     2U
#define RP_CLK_GPOUT3                     3U
#define RP_CLK_REF                        4U
#define RP_CLK_SYS                        5U
#define RP_CLK_PERI                       6U
#define RP_CLK_HSTX                       7U
#define RP_CLK_USB                        8U
#define RP_CLK_ADC                        9U
#define RP_CLK_COUNT                      10U
/** @} */

/**
 * @name    Atomic register access aliases
 * @note    RP2350 peripherals support atomic SET/CLR/XOR operations via
 *          register aliases at offsets +0x1000, +0x2000, +0x3000 from base.
 * @{
 */
#define RP_REG_ALIAS_XOR_BITS                0x1000U
#define RP_REG_ALIAS_SET_BITS                0x2000U
#define RP_REG_ALIAS_CLR_BITS                0x3000U

/**
 * @brief   Atomically set bits in a hardware register.
 */
#define rp_set_bits(addr, mask) \
    (*(volatile uint32_t *)((uintptr_t)(addr) | RP_REG_ALIAS_SET_BITS) = (mask))

/**
 * @brief   Atomically clear bits in a hardware register.
 */
#define rp_clear_bits(addr, mask) \
    (*(volatile uint32_t *)((uintptr_t)(addr) | RP_REG_ALIAS_CLR_BITS) = (mask))

/**
 * @brief   Atomically XOR bits in a hardware register.
 */
#define rp_xor_bits(addr, mask) \
    (*(volatile uint32_t *)((uintptr_t)(addr) | RP_REG_ALIAS_XOR_BITS) = (mask))

/**
 * @brief   Atomically write a value with mask using single XOR operation.
 * @note    This is atomic - a single write to the XOR alias register.
 */
#define rp_write_masked(addr, value, mask) \
    rp_xor_bits((addr), (*(volatile uint32_t *)(addr) ^ (value)) & (mask))
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RP2350_H */

/** @} */
