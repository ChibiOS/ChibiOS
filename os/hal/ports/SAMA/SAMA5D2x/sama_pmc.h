/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
  PMC->PMC_PCER0 = (mask);                                                  \
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
  PMC->PMC_PCDR0 = (mask);                                                  \
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
  PMC->PMC_PCER1 = (mask);                                                  \
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
  PMC->PMC_PCDR1 = (mask);                                                  \
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
 * @brief   Enables the H32MX peripheral clock.
 *
 * @api
 */
#define pmcEnableH32MX() pmcEnablePidLow(ID_MATRIX1_MSK)

/**
 * @brief   Disables the H32MX peripheral clock.
 *
 * @api
 */
#define pmcDisableH32MX() pmcDisablePidLow(ID_MATRIX1_MSK)

/**
 * @brief   Enables the H64MX peripheral clock.
 *
 * @api
 */
#define pmcEnableH64MX() pmcEnablePidLow(ID_MATRIX0_MSK)

/**
 * @brief   Disables the H64MX peripheral clock.
 *
 * @api
 */
#define pmcDisableH64MX() pmcDisablePidLow(ID_MATRIX0_MSK)

/**
 * @brief   Enables the PIO peripheral clock.
 *
 * @api
 */
#define pmcEnablePIO() pmcEnablePidLow(ID_PIOA_MSK)

/**
 * @brief   Disables the PIO peripheral clock.
 *
 * @api
 */
#define pmcDisablePIO() pmcDisablePidLow(ID_PIOA_MSK)

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

/**
 * @brief   Enables the FLEXCOM0 peripheral clock.
 *
 * @api
 */
#define pmcEnableFLEXCOM0() pmcEnablePidLow(ID_FLEXCOM0_MSK)

/**
 * @brief   Disables the FLEXCOM0 peripheral clock.
 *
 * @api
 */
#define pmcDisableFLEXCOM0() pmcDisablePidLow(ID_FLEXCOM0_MSK)

/**
 * @brief   Enables the FLEXCOM1 peripheral clock.
 *
 * @api
 */
#define pmcEnableFLEXCOM1() pmcEnablePidLow(ID_FLEXCOM1_MSK)

/**
 * @brief   Disables the FLEXCOM1 peripheral clock.
 *
 * @api
 */
#define pmcDisableFLEXCOM1() pmcDisablePidLow(ID_FLEXCOM1_MSK)

/**
 * @brief   Enables the FLEXCOM2 peripheral clock.
 *
 * @api
 */
#define pmcEnableFLEXCOM2() pmcEnablePidLow(ID_FLEXCOM2_MSK)

/**
 * @brief   Disables the FLEXCOM2 peripheral clock.
 *
 * @api
 */
#define pmcDisableFLEXCOM2() pmcDisablePidLow(ID_FLEXCOM2_MSK)

/**
 * @brief   Enables the FLEXCOM0 peripheral clock.
 *
 * @api
 */
#define pmcEnableFLEXCOM3() pmcEnablePidLow(ID_FLEXCOM3_MSK)

/**
 * @brief   Disables the FLEXCOM3 peripheral clock.
 *
 * @api
 */
#define pmcDisableFLEXCOM3() pmcDisablePidLow(ID_FLEXCOM3_MSK)

/**
 * @brief   Enables the FLEXCOM4 peripheral clock.
 *
 * @api
 */
#define pmcEnableFLEXCOM4() pmcEnablePidLow(ID_FLEXCOM4_MSK)

/**
 * @brief   Disables the FLEXCOM4 peripheral clock.
 *
 * @api
 */
#define pmcDisableFLEXCOM4() pmcDisablePidLow(ID_FLEXCOM4_MSK)

/**
 * @brief   Enables the TC0 peripheral clock.
 *
 * @api
 */
#define pmcEnableTC0() pmcEnablePidHigh(ID_TC0_MSK)

/**
 * @brief   Disables the TC0 peripheral clock.
 *
 * @api
 */
#define pmcDisableTC0() pmcDisablePidHigh(ID_TC0_MSK)

/**
 * @brief   Enables the TC1 peripheral clock.
 *
 * @api
 */
#define pmcEnableTC1() pmcEnablePidHigh(ID_TC1_MSK)

/**
 * @brief   Disables the TC1 peripheral clock.
 *
 * @api
 */
#define pmcDisableTC1() pmcDisablePidHigh(ID_TC1_MSK)


/**
 * @brief   Enables the AES peripheral clock.
 *
 * @api
 */
#define pmcEnableAES() 	pmcEnablePidLow(ID_AES_MSK)

/**
 * @brief   Disables the AES peripheral clock.
 *
 * @api
 */
#define pmcDisableAES() pmcDisablePidLow(ID_AES_MSK)

/**
 * @brief   Enables the TRNG peripheral clock.
 *
 * @api
 */
#define pmcEnableTRNG()	pmcEnablePidHigh(ID_TRNG_MSK)
/**
 * @brief   Disables the TRNG peripheral clock.
 *
 * @api
 */
#define pmcDisableTRNG() pmcDisablePidHigh(ID_TRNG_MSK)

/**
 * @brief   Enables the DES peripheral clock.
 *
 * @api
 */
#define pmcEnableDES()	pmcEnablePidLow(ID_TDES_MSK)
/**
 * @brief   Disables the DES peripheral clock.
 *
 * @api
 */
#define pmcDisableDES() pmcDisablePidLow(ID_TDES_MSK)

/**
 * @brief   Enables the SHA peripheral clock.
 *
 * @api
 */
#define pmcEnableSHA()	pmcEnablePidLow(ID_SHA_MSK)
/**
 * @brief   Disables the SHA peripheral clock.
 *
 * @api
 */
#define pmcDisableSHA() pmcDisablePidLow(ID_SHA_MSK)

/**
 * @brief   Enables the ETH0 peripheral clock.
 *
 * @api
 */
#define pmcEnableETH0() pmcEnablePidLow(ID_GMAC0_MSK)

/**
 * @brief   Disables the ETH0 peripheral clock.
 *
 * @api
 */
#define pmcDisableETH0() pmcDisablePidLow(ID_GMAC0_MSK)

/**
 * @brief   Enables the SECUMOD peripheral clock.
 *
 * @api
 */
#define pmcEnableSEC() pmcEnablePidLow(ID_SECUMOD_MSK)

/**
 * @brief   Disables the SECUMOD peripheral clock.
 *
 * @api
 */
#define pmcDisableSEC() pmcDisablePidLow(ID_SECUMOD_MSK)

/**
 * @brief   Enables the SDMMC0 peripheral clock.
 *
 * @api
 */
#define pmcEnableSDMMC0() 	pmcEnablePidLow(ID_SDMMC0_MSK)

/**
 * @brief   Disables the SDMMC0 peripheral clock.
 *
 * @api
 */
#define pmcDisableSDMMC0() pmcDisablePidLow(ID_SDMMC0_MSK)

/**
 * @brief   Enables the SDMMC1 peripheral clock.
 *
 * @api
 */
#define pmcEnableSDMMC1() 	pmcEnablePidHigh(ID_SDMMC1_MSK)

/**
 * @brief   Disables the SDMMC1 peripheral clock.
 *
 * @api
 */
#define pmcDisableSDMMC1() pmcDisablePidHigh(ID_SDMMC1_MSK)

/**
 * @brief   Enables the TRNG peripheral clock.
 *
 * @api
 */
#define pmcEnableTRNG0()   pmcEnablePidHigh(ID_TRNG_MSK)

/**
 * @brief   Disables the TRNG peripheral clock.
 *
 * @api
 */
#define pmcDisableTRNG0() pmcDisablePidHigh(ID_TRNG_MSK)

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
