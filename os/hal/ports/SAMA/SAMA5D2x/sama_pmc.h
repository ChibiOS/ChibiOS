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
 * @file    SAMA5D2x/sama_pmc.h
 * @brief   PMC helper driver header.
 *
 * @addtogroup SAMA5D2x_PMC
 * @{
 */

#ifndef _SAMA_PMC_
#define _SAMA_PMC_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Generic PMC operations
 * @{
 */
/**
 * @brief   Enable write protection on PMC registers block.
 *
 * @api
 */
#define pmcEnableWP() {                                                     \
  PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD | PMC_WPMR_WPEN;                    \
}

/**
 * @brief   Disable write protection on PMC registers block.
 *
 * @api
 */
#define pmcDisableWP() {                                                    \
  PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD;                                    \
}

/**
 * @brief   Enables the clock of one or more peripheral having ID from 2 to
 *          31.
 *
 * @param[in] mask      PCER0 peripherals mask
 *
 * @api
 */
#define pmcEnablePidLow(mask) {                                             \
  pmcDisableWP();                                                           \
  PMC->PMC_PCER0 |= (mask);                                                 \
  pmcDisableWP();                                                           \
}

/**
 * @brief   Disables the clock of one or more peripheral having ID from 2 to
 *          31.
 *
 * @param[in] mask      PCDR0 peripherals mask
 *
 * @api
 */
#define pmcDisablePidLow(mask) {                                            \
  pmcDisableWP();                                                           \
  PMC->PMC_PCDR0 |= (mask);                                                 \
  pmcDisableWP();                                                           \
}

/**
 * @brief   Enables the clock of one or more peripheral having ID from 32 to
 *          63.
 *
 * @param[in] mask      PCER1 peripherals mask
 *
 * @api
 */
#define pmcEnablePidHigh(mask) {                                            \
  pmcDisableWP();                                                           \
  PMC->PMC_PCER1 |= (mask);                                                 \
  pmcDisableWP();                                                           \
}

/**
 * @brief   Disables the clock of one or more peripheral having ID from 32 to
 *          63.
 *
 * @param[in] mask      PCDR1 peripherals mask
 *
 * @api
 */
#define pmcDisablePidHigh(mask) {                                           \
  pmcDisableWP();                                                           \
  PMC->PMC_PCDR1 |= (mask);                                                 \
  pmcDisableWP();                                                           \
}
/** @} */

/**
 * @name    ADC peripherals specific PMC operations
 * @{
 */
/**
 * @brief   Enables the PIT peripheral clock.
 *
 * @api
 */
#define pmcEnablePIT() pmcEnablePidLow(SAMA_PID_PIT)

/**
 * @brief   Disables the PIT peripheral clock.
 *
 * @api
 */
#define pmcDisablePIT() pmcDisablePidLow(SAMA_PID_PIT)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* _SAMA_PMC_ */

/** @} */
