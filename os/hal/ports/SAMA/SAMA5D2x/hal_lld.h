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
 * @file    hal_lld.h
 * @brief   PLATFORM HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

/* If the device type is not externally defined, for example from the Makefile,
   then a file named board.h is included. This file must contain a device
   definition compatible with the vendor include file.*/
#if !defined (SAMA5D21) && !defined (SAMA5D22) &&  !defined (SAMA5D23) &&   \
    !defined (SAMA5D24) && !defined (SAMA5D25) &&  !defined (SAMA5D26) &&   \
    !defined (SAMA5D27) && !defined (SAMA5D28)
#include "board.h"
#endif

/* Including the device CMSIS header. Note, we are not using the definitions
   from this header because we need this file to be usable also from
   assembler source files. We verify that the info matches instead.*/
#include "sama5d2x.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification macros
 * @{
 */
#if defined(SAMA5D21) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16-bit DDR, BGA196"

#elif defined(SAMA5D22)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16-bit DDR, CAN, BGA196"

#elif defined(SAMA5D23)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16-bit DDR, CAN, Enhanced Security, BGA196"

#elif defined(SAMA5D24)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16/32-bit DDR, USB HSIC, BGA256"

#elif defined(SAMA5D25)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16/32-bit DDR, BGA289"

#elif defined(SAMA5D26)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16/32-bit DDR, CAN, BGA289"

#elif defined(SAMA5D27)
#define PLATFORM_NAME           "ARM Cortex-A5 processor based embedded MPU, 500Mhz, Neon, L2 cache, TrustZone, 16/32-bit DDR, CAN, Enhanced Security, BGA289"

#else
#error "SAMA5D2x device unsupported or not specified"
#endif
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    PLATFORM configuration options
 * @{
 */
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(SAMA5D2x_MCUCONF)
#error "Using a wrong mcuconf.h file, SAMA5D2x_MCUCONF not defined"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
