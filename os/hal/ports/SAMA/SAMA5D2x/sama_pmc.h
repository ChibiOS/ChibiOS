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
 * @notapi
 */
#define pmcEnableWP() {                                                     \
  PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD | PMC_WPMR_WPEN;                    \
}

/**
 * @brief   Disable write protection on PMC registers block.
 *
 * @notapi
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
  pmcEnableWP();                                                            \
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
  pmcEnableWP();                                                            \
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
  pmcEnableWP();                                                            \
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
  pmcEnableWP();                                                            \
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
#define pmcEnablePIT() pmcEnablePidLow(ID_PIT_MSK)

/**
 * @brief   Disables the PIT peripheral clock.
 *
 * @api
 */
#define pmcDisablePIT() pmcDisablePidLow(ID_PIT_MSK)

/**
 * @brief   Enables the XDMAC0 peripheral clock.
 *
 * @api
 */
#define pmcEnableXDMAC0() pmcEnablePidLow(ID_XDMAC0_MSK)

/**
 * @brief   Disables the XDMAC0 peripheral clock.
 *
 * @api
 */
#define pmcDisableXDMAC0() pmcDisablePidLow(ID_XDMAC0_MSK)

/**
 * @brief   Enables the XDMAC1 peripheral clock.
 *
 * @api
 */
#define pmcEnableXDMAC1() pmcEnablePidLow(ID_XDMAC1_MSK)

/**
 * @brief   Disables the XDMAC1 peripheral clock.
 *
 * @api
 */
#define pmcDisableXDMAC1() pmcDisablePidLow(ID_XDMAC1_MSK)

/**
 * @brief   Enables the SPI0 peripheral clock.
 *
 * @api
 */
#define pmcEnableSPI0() pmcEnablePidHigh(ID_SPI0_MSK)

/**
 * @brief   Disables the SPI0 peripheral clock.
 *
 * @api
 */
#define pmcDisableSPI0() pmcDisablePidHigh(ID_SPI0_MSK)

/**
 * @brief   Enables the SPI1 peripheral clock.
 *
 * @api
 */
#define pmcEnableSPI1() pmcEnablePidHigh(ID_SPI1_MSK)

/**
 * @brief   Disables the SPI11 peripheral clock.
 *
 * @api
 */
#define pmcDisableSPI1() pmcDisablePidHigh(ID_SPI1_MSK)

/**
 * @brief   Enables the UART0 peripheral clock.
 *
 * @api
 */
#define pmcEnableUART0() pmcEnablePidLow(ID_UART0_MSK)

/**
 * @brief   Disables the UART0 peripheral clock.
 *
 * @api
 */
#define pmcDisableUART0() pmcDisablePidLow(ID_UART0_MSK)

/**
 * @brief   Enables the UART1 peripheral clock.
 *
 * @api
 */
#define pmcEnableUART1() pmcEnablePidLow(ID_UART1_MSK)

/**
 * @brief   Disables the UART1 peripheral clock.
 *
 * @api
 */
#define pmcDisableUART1() pmcDisablePidLow(ID_UART1_MSK)

/**
 * @brief   Enables the UART2 peripheral clock.
 *
 * @api
 */
#define pmcEnableUART2() pmcEnablePidLow(ID_UART2_MSK)

/**
 * @brief   Disables the UART2 peripheral clock.
 *
 * @api
 */
#define pmcDisableUART2() pmcDisablePidLow(ID_UART2_MSK)

/**
 * @brief   Enables the UART3 peripheral clock.
 *
 * @api
 */
#define pmcEnableUART3() pmcEnablePidLow(ID_UART3_MSK)

/**
 * @brief   Disables the UART3 peripheral clock.
 *
 * @api
 */
#define pmcDisableUART3() pmcDisablePidLow(ID_UART3_MSK)

/**
 * @brief   Enables the UART4 peripheral clock.
 *
 * @api
 */
#define pmcEnableUART4() pmcEnablePidLow(ID_UART4_MSK)

/**
 * @brief   Disables the UART4 peripheral clock.
 *
 * @api
 */
#define pmcDisableUART4() pmcDisablePidLow(ID_UART4_MSK)

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
