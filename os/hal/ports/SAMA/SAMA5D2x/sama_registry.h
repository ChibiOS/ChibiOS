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
 * @file    SAMA5D2x/sama_registry.h
 * @brief   SAMA5D2x capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef SAMA_REGISTRY_H
#define SAMA_REGISTRY_H

/**
 * @brief   Sub-family identifier.
 */
#if !defined(SAMA5D2X) || defined(__DOXYGEN__)
#define SAMA5D2X
#endif

/*===========================================================================*/
/* Common features.                                                          */
/*===========================================================================*/

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    SAMA5D27 capabilities
 * @{
 */
/*===========================================================================*/
/* SAMA5D27.                                                                 */
/*===========================================================================*/

#if defined(SAMA5D27) || defined(__DOXYGEN__)

/* AIC IDs. */
#define SAMA_AIC_ARM            2U          /**< ARM AIC Identifier.       */
#define SAMA_AIC_PIT            3U          /**< PIT AIC Identifier.       */
#define SAMA_AIC_WDT            4U          /**< WDT AIC Identifier.       */
#define SAMA_AIC_GMAC           5U          /**< GMAC AIC Identifier.      */
#define SAMA_AIC_XDMAC0         6U          /**< XDMAC0 AIC Identifier.    */
#define SAMA_AIC_XDMAC1         7U          /**< XDMAC1 AIC Identifier.    */
#define SAMA_AIC_ICM            8U          /**< ICM AIC Identifier.       */
#define SAMA_AIC_AES            9U          /**< AES AIC Identifier.       */
#define SAMA_AIC_AESB           10U         /**< AESB AIC Identifier.      */
#define SAMA_AIC_TDES           11U         /**< TDES AIC Identifier.      */
#define SAMA_AIC_SHA            12U         /**< SHA AIC Identifier.       */
#define SAMA_AIC_MPDDRC         13U         /**< MPDDRC AIC Identifier.    */
#define SAMA_AIC_H32MX          14U         /**< H32MX AIC Identifier.     */
#define SAMA_AIC_H64MX          15U         /**< H64MX AIC Identifier.     */
#define SAMA_AIC_SECUMOD        16U         /**< SECUMOD AIC Identifier.   */
#define SAMA_AIC_HSMC           17U         /**< HSMC AIC Identifier.      */
#define SAMA_AIC_PIOA           18U         /**< PIOA AIC Identifier.      */
#define SAMA_AIC_FLEXCOM0       19U         /**< FLEXCOM0 AIC Identifier.  */
#define SAMA_AIC_FLEXCOM1       20U         /**< FLEXCOM1 AIC Identifier.  */
#define SAMA_AIC_FLEXCOM2       21U         /**< FLEXCOM2 AIC Identifier.  */
#define SAMA_AIC_FLEXCOM3       22U         /**< FLEXCOM3 AIC Identifier.  */
#define SAMA_AIC_FLEXCOM4       23U         /**< FLEXCOM4 AIC Identifier.  */
#define SAMA_AIC_UART0          24U         /**< UART0 AIC Identifier.     */
#define SAMA_AIC_UART1          25U         /**< UART1 AIC Identifier.     */
#define SAMA_AIC_UART2          26U         /**< UART2 AIC Identifier.     */
#define SAMA_AIC_UART3          27U         /**< UART3 AIC Identifier.     */
#define SAMA_AIC_UART4          28U         /**< UART4 AIC Identifier.     */
#define SAMA_AIC_TWIHS0         29U         /**< TWIHS0 AIC Identifier.    */
#define SAMA_AIC_TWIHS1         30U         /**< TWIHS1 AIC Identifier.    */
#define SAMA_AIC_SDMMC0         31U         /**< SDMMC0 AIC Identifier.    */
#define SAMA_AIC_SDMMC1         32U         /**< SDMMC1 AIC Identifier.    */
#define SAMA_AIC_SPI0           33U         /**< SPI0 AIC Identifier.      */
#define SAMA_AIC_SPI1           34U         /**< SPI1 AIC Identifier.      */
#define SAMA_AIC_TC0            35U         /**< TC0 AIC Identifier.       */
#define SAMA_AIC_TC1            36U         /**< TC1 AIC Identifier.       */
#define SAMA_AIC_PWM            38U         /**< PWM AIC Identifier.       */
#define SAMA_AIC_ADC            40U         /**< ADC AIC Identifier.       */
#define SAMA_AIC_UHPHS          41U         /**< UHPHS AIC Identifier.     */
#define SAMA_AIC_UDPHS          42U         /**< UDPHS AIC Identifier.     */
#define SAMA_AIC_SSC0           43U         /**< SSC0 AIC Identifier.      */
#define SAMA_AIC_SSC1           44U         /**< SSC1 AIC Identifier.      */
#define SAMA_AIC_LCDC           45U         /**< LCDC AIC Identifier.      */
#define SAMA_AIC_ISC            46U         /**< ISC AIC Identifier.       */
#define SAMA_AIC_TRNG           47U         /**< TRNG AIC Identifier.      */
#define SAMA_AIC_PDMIC          48U         /**< PDMIC AIC Identifier.     */
#define SAMA_AIC_AIC0           49U         /**< AIC AIC Identifier.       */
#define SAMA_AIC_SFC            50U         /**< SFC AIC Identifier.       */
#define SAMA_AIC_SECURAM        51U         /**< SECURAM AIC Identifier.   */
#define SAMA_AIC_QSPI0          52U         /**< QSPI0 AIC Identifier.     */
#define SAMA_AIC_QSPI1          53U         /**< QSPI1 AIC Identifier.     */
#define SAMA_AIC_I2SC0          54U         /**< I2SC0 AIC Identifier.     */
#define SAMA_AIC_I2SC1          55U         /**< I2SC1 AIC Identifier.     */
#define SAMA_AIC_MCAN0          56U         /**< MCAN0 AIC Identifier.     */
#define SAMA_AIC_MCAN1          57U         /**< MCAN1 AIC Identifier.     */
#define SAMA_AIC_PTC            58U         /**< PTC AIC Identifier.       */
#define SAMA_AIC_CLASSD         59U         /**< CLASSD AIC Identifier.    */
#define SAMA_AIC_SFR            60U         /**< SFR AIC Identifier.       */
#define SAMA_AIC_SAIC1          61U         /**< SAIC AIC Identifier.      */
#define SAMA_AIC_AIC1           62U         /**< AIC AIC Identifier.       */
#define SAMA_AIC_L2CC           63U         /**< L2CC AIC Identifier.      */

/* PCM Peripheral IDs.*/
#define SAMA_PID_ARM                        (1 << (SAMA_AIC_ARM & 0x1F))
#define SAMA_PID_PIT                        (1 << (SAMA_AIC_PIT & 0x1F))
#define SAMA_PID_WDT                        (1 << (SAMA_AIC_WDT & 0x1F))
#define SAMA_PID_GMAC                       (1 << (SAMA_AIC_GMAC & 0x1F))
#define SAMA_PID_XDMAC0                     (1 << (SAMA_AIC_XDMAC0 & 0x1F))
#define SAMA_PID_XDMAC1                     (1 << (SAMA_AIC_XDMAC1 & 0x1F))
#define SAMA_PID_ICM                        (1 << (SAMA_AIC_ICM & 0x1F))
#define SAMA_PID_AES                        (1 << (SAMA_AIC_AES & 0x1F))
#define SAMA_PID_AESB                       (1 << (SAMA_AIC_AESB & 0x1F))
#define SAMA_PID_TDES                       (1 << (SAMA_AIC_TDES & 0x1F))
#define SAMA_PID_SHA                        (1 << (SAMA_AIC_SHA & 0x1F))
#define SAMA_PID_MPDDRC                     (1 << (SAMA_AIC_MPDDRC & 0x1F))
#define SAMA_PID_H32MX                      (1 << (SAMA_AIC_H32MX & 0x1F))
#define SAMA_PID_H64MX                      (1 << (SAMA_AIC_H64MX & 0x1F))
#define SAMA_PID_SECUMOD                    (1 << (SAMA_AIC_SECUMOD & 0x1F))
#define SAMA_PID_HSMC                       (1 << (SAMA_AIC_HSMC & 0x1F))
#define SAMA_PID_FLEXCOM0                   (1 << (SAMA_AIC_FLEXCOM0 & 0x1F))
#define SAMA_PID_PIOA                       (1 << (SAMA_AIC_PIOA & 0x1F))
#define SAMA_PID_FLEXCOM1                   (1 << (SAMA_AIC_FLEXCOM1 & 0x1F))
#define SAMA_PID_FLEXCOM2                   (1 << (SAMA_AIC_FLEXCOM2 & 0x1F))
#define SAMA_PID_FLEXCOM3                   (1 << (SAMA_AIC_FLEXCOM3 & 0x1F))
#define SAMA_PID_FLEXCOM4                   (1 << (SAMA_AIC_FLEXCOM4 & 0x1F))
#define SAMA_PID_UART0                      (1 << (SAMA_AIC_UART0 & 0x1F))
#define SAMA_PID_UART1                      (1 << (SAMA_AIC_UART1 & 0x1F))
#define SAMA_PID_UART2                      (1 << (SAMA_AIC_UART2 & 0x1F))
#define SAMA_PID_UART3                      (1 << (SAMA_AIC_UART3 & 0x1F))
#define SAMA_PID_UART4                      (1 << (SAMA_AIC_UART4 & 0x1F))
#define SAMA_PID_TWIHS0                     (1 << (SAMA_AIC_TWIHS0 & 0x1F))
#define SAMA_PID_TWIHS1                     (1 << (SAMA_AIC_TWIHS1 & 0x1F))
#define SAMA_PID_SDMMC0                     (1 << (SAMA_AIC_SDMMC0 & 0x1F))
#define SAMA_PID_SDMMC1                     (1 << (SAMA_AIC_SDMMC1 & 0x1F)) 
#define SAMA_PID_SPI0                       (1 << (SAMA_AIC_SPI0 & 0x1F))
#define SAMA_PID_SPI1                       (1 << (SAMA_AIC_SPI1 & 0x1F))
#define SAMA_PID_TC0                        (1 << (SAMA_AIC_TC0 & 0x1F))
#define SAMA_PID_TC1                        (1 << (SAMA_AIC_TC1 & 0x1F))
#define SAMA_PID_PWM                        (1 << (SAMA_AIC_PWM & 0x1F))
#define SAMA_PID_ADC                        (1 << (SAMA_AIC_ADC & 0x1F))
#define SAMA_PID_UHPHS                      (1 << (SAMA_AIC_UHPHS & 0x1F))
#define SAMA_PID_UDPHS                      (1 << (SAMA_AIC_UDPHS & 0x1F))
#define SAMA_PID_SSC0                       (1 << (SAMA_AIC_SSC0 & 0x1F))
#define SAMA_PID_SSC1                       (1 << (SAMA_AIC_SSC1 & 0x1F))
#define SAMA_PID_LCDC                       (1 << (SAMA_AIC_LCDC & 0x1F))
#define SAMA_PID_ISC                        (1 << (SAMA_AIC_ISC & 0x1F))
#define SAMA_PID_TRNG                       (1 << (SAMA_AIC_TRNG & 0x1F))
#define SAMA_PID_PDMIC                      (1 << (SAMA_AIC_PDMIC & 0x1F))
#define SAMA_PID_AIC0                       (1 << (SAMA_AIC_AIC0 & 0x1F))
#define SAMA_PID_SFC                        (1 << (SAMA_AIC_SFC & 0x1F))
#define SAMA_PID_SECURAM                    (1 << (SAMA_AIC_SECURAM & 0x1F))
#define SAMA_PID_QSPI0                      (1 << (SAMA_AIC_QSPI0 & 0x1F))
#define SAMA_PID_QSPI1                      (1 << (SAMA_AIC_QSPI1 & 0x1F))
#define SAMA_PID_I2SC0                      (1 << (SAMA_AIC_I2SC0 & 0x1F))
#define SAMA_PID_I2SC1                      (1 << (SAMA_AIC_I2SC1 & 0x1F))
#define SAMA_PID_MCAN0                      (1 << (SAMA_AIC_MCAN0 & 0x1F))
#define SAMA_PID_MCAN1                      (1 << (SAMA_AIC_MCAN1 & 0x1F))
#define SAMA_PID_PTC                        (1 << (SAMA_AIC_PTC & 0x1F))
#define SAMA_PID_CLASSD                     (1 << (SAMA_AIC_CLASSD & 0x1F))
#define SAMA_PID_SFR                        (1 << (SAMA_AIC_SFR & 0x1F))
#define SAMA_PID_SAIC1                      (1 << (SAMA_AIC_SAIC1 & 0x1F))
#define SAMA_PID_AIC1                       (1 << (SAMA_AIC_AIC1 & 0x1F))
#define SAMA_PID_L2CC                       (1 << (SAMA_AIC_L2CC & 0x1F))
#endif /* defined(SAMA5D27) */

/** @} */

#endif /* SAMA_REGISTRY_H */

/** @} */
