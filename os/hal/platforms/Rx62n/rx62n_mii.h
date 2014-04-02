/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    RX62N/rx62n_mii.h
 * @brief   RX62N low level MII driver header.
 *
 * @addtogroup RX62N_MII
 * @{
 */

#ifndef _RX62N_MII_H_
#define _RX62N_MII_H_

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * PIR bit definitions.
 */
#define PIR_MDC                (1<<0)
#define PIR_MMD                (1<<1)
#define PIR_MDO                (1<<2)
#define PIR_MDI                (1<<3)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a PHY register value.
 */
typedef uint16_t phyreg_t;

/**
 * @brief   Type of a PHY register address.
 */
typedef uint8_t phyaddr_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void miiInit(void);
  void miiReset(MACDriver *macp);
  phyreg_t miiGet(MACDriver *macp, phyaddr_t addr);
  void miiPut(MACDriver *macp, phyaddr_t addr, phyreg_t value);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MAC */

#endif /* _RX62N_MII_H_ */

/** @} */
