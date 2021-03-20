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

typedef struct
{
  __IO uint32_t xxx;
} XXX_TypeDef;

#define XIP_BASE                        0x10000000U

#define PERIPH_BASE                     0x40000000U

/**
 * @name    Peripherals
 * @{
 */
#define XXX                 ((XXX_TypeDef *)(PERIPH_BASE + 0x00000000U))
/** @{ */

/**
 * @name    Peripheral bits defitions
 */
#define XXX_REG_NAME_Pos                0U
#define XXX_REG_NAME_Msk                (3U << XXX_REG_NAME_Pos)
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RP2040_H */

/** @} */
