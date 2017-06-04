/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    generic_cm0.h
 * @brief   CMSIS header for a generic Cortex-M0 platform.
 *
 * @addtogroup GENERIC_CMSIS_DEVICE
 * @{
 */

#ifndef __GENERIC_CM0_H
#define __GENERIC_CM0_H

#define __CM0_REV                   0
#define __MPU_PRESENT               0
#if !defined(__NVIC_PRIO_BITS) || defined(__DOXYGEN__)
#define __NVIC_PRIO_BITS            2
#endif
#if !defined(__Vendor_SysTickConfig) || defined(__DOXYGEN__)
#define __Vendor_SysTickConfig      0
#endif

typedef enum {
  NonMaskableInt_IRQn         = -14,
  HardFault_IRQn              = -13,
  SVC_IRQn                    = -5,
  PendSV_IRQn                 = -2,
  SysTick_IRQn                = -1,
  Vector40_IRQn               = 0,
  Vector44_IRQn               = 1,
  Vector48_IRQn               = 2,
  Vector4C_IRQn               = 3,
  Vector50_IRQn               = 4,
  Vector54_IRQn               = 5,
  Vector58_IRQn               = 6,
  Vector5C_IRQn               = 7,
  Vector60_IRQn               = 8,
  Vector64_IRQn               = 9,
  Vector68_IRQn               = 10,
  Vector6C_IRQn               = 11,
  Vector70_IRQn               = 12,
  Vector74_IRQn               = 13,
  Vector78_IRQn               = 14,
  Vector7C_IRQn               = 15,
  Vector80_IRQn               = 16,
  Vector84_IRQn               = 17,
  Vector88_IRQn               = 18,
  Vector8C_IRQn               = 19,
  Vector90_IRQn               = 20,
  Vector94_IRQn               = 21,
  Vector98_IRQn               = 22,
  Vector9C_IRQn               = 23,
  VectorA0_IRQn               = 24,
  VectorA4_IRQn               = 25,
  VectorA8_IRQn               = 26,
  VectorAC_IRQn               = 27,
  VectorB0_IRQn               = 28,
  VectorB4_IRQn               = 29,
  VectorB8_IRQn               = 30,
  VectorBC_IRQn               = 31
} IRQn_Type;


#include "core_cm0.h"
#include "system_generic_cm0.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GENERIC_CM0_H */

/** @} */
