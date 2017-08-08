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
/* PCM Peripheral IDs.*/
#define SAMA_PID_SAIC0          (1 << 0)    /**< SAIC0 FIQ PID.            */
#define SAMA_PID_ARM            (1 << 2)    /**< ARM PID.                  */
#define SAMA_PID_PIT            (1 << 3)    /**< PIT PID.                  */
#define SAMA_PID_WDT            (1 << 4)    /**< WDT PID.                  */
#define SAMA_PID_GMAC           (1 << 5)    /**< GMAC PID.                 */
#define SAMA_PID_XDMAC0         (1 << 6)    /**< XDMAC0 PID.               */
#define SAMA_PID_XDMAC1         (1 << 7)    /**< XDMAC1 PID.               */
#define SAMA_PID_ICM            (1 << 8)    /**< ICM PID.                  */
#define SAMA_PID_AES            (1 << 9)    /**< AES PID.                  */
#define SAMA_PID_AESB           (1 << 10)   /**< AESB PID.                 */
#define SAMA_PID_TDES           (1 << 11)   /**< TDES PID.                 */
#define SAMA_PID_SHA            (1 << 12)   /**< SHA PID.                  */
#define SAMA_PID_MPDDRC         (1 << 13)   /**< MPDDRC PID.               */
#define SAMA_PID_H32MX          (1 << 14)   /**< H32MX PID.                */
#define SAMA_PID_H64MX          (1 << 15)   /**< H64MX PID.                */
#define SAMA_PID_SECUMOD        (1 << 16)   /**< SECUMOD PID.              */
#define SAMA_PID_HSMC           (1 << 17)   /**< HSMC PID.                 */
#define SAMA_PID_PIOA           (1 << 18)   /**< PIOA PID.                 */
#define SAMA_PID_FLEXCOM0       (1 << 19)   /**< FLEXCOM0 PID.             */
#define SAMA_PID_FLEXCOM1       (1 << 20)   /**< FLEXCOM1 PID.             */
#define SAMA_PID_FLEXCOM2       (1 << 21)   /**< FLEXCOM2 PID.             */
#define SAMA_PID_FLEXCOM3       (1 << 22)   /**< FLEXCOM3 PID.             */
#define SAMA_PID_FLEXCOM4       (1 << 23)   /**< FLEXCOM4 PID.             */
#define SAMA_PID_UART0          (1 << 24)   /**< UART0 PID.                */
#define SAMA_PID_UART1          (1 << 25)   /**< UART1 PID.                */
#define SAMA_PID_UART2          (1 << 26)   /**< UART2 PID.                */
#define SAMA_PID_UART3          (1 << 27)   /**< UART3 PID.                */
#define SAMA_PID_UART4          (1 << 28)   /**< UART4 PID.                */
#define SAMA_PID_TWIHS0         (1 << 29)   /**< TWIHS0 PID.               */
#define SAMA_PID_TWIHS1         (1 << 30)   /**< TWIHS1 PID.               */
#define SAMA_PID_SDMMC0         (1 << 31)   /**< SDMMC0 PID.               */
#define SAMA_PID_SDMMC1         (1 << 0)    /**< SDMMC1 PID.               */
#define SAMA_PID_SPI0           (1 << 1)    /**< SPI0 PID.                 */
#define SAMA_PID_SPI1           (1 << 2)    /**< SPI1 PID.                 */
#define SAMA_PID_TC0            (1 << 3)    /**< TC0 PID.                  */
#define SAMA_PID_TC1            (1 << 4)    /**< TC1 PID.                  */
#define SAMA_PID_PWM            (1 << 6)    /**< PWM PID.                  */
#define SAMA_PID_ADC            (1 << 8)    /**< ADC PID.                  */
#define SAMA_PID_UHPHS          (1 << 9)    /**< UHPHS PID.                */
#define SAMA_PID_UDPHS          (1 << 10)   /**< UDPHS PID.                */
#define SAMA_PID_SSC0           (1 << 11)   /**< SSC0 PID.                 */
#define SAMA_PID_SSC1           (1 << 12)   /**< SSC1 PID.                 */
#define SAMA_PID_LCDC           (1 << 13)   /**< LCDC PID.                 */
#define SAMA_PID_ISC            (1 << 14)   /**< ISC PID.                  */
#define SAMA_PID_TRNG           (1 << 15)   /**< TRNG PID.                 */
#define SAMA_PID_PDMIC          (1 << 16)   /**< PDMIC PID.                */
#define SAMA_PID_AIC0           (1 << 17)   /**< AIC PID.                  */
#define SAMA_PID_SFC            (1 << 18)   /**< SFC PID.                  */
#define SAMA_PID_SECURAM        (1 << 19)   /**< SECURAM PID.              */
#define SAMA_PID_QSPI0          (1 << 20)   /**< QSPI0 PID.                */
#define SAMA_PID_QSPI1          (1 << 21)   /**< QSPI1 PID.                */
#define SAMA_PID_I2SC0          (1 << 22)   /**< I2SC0 PID.                */
#define SAMA_PID_I2SC1          (1 << 23)   /**< I2SC1 PID.                */
#define SAMA_PID_MCAN0          (1 << 24)   /**< MCAN0 PID.                */
#define SAMA_PID_MCAN1          (1 << 25)   /**< MCAN1 PID.                */
#define SAMA_PID_PTC            (1 << 26)   /**< PTC PID.                  */
#define SAMA_PID_CLASSD         (1 << 27)   /**< CLASSD PID.               */
#define SAMA_PID_SFR            (1 << 28)   /**< SFR PID.                  */
#define SAMA_PID_SAIC1          (1 << 29)   /**< SAIC PID.                 */
#define SAMA_PID_AIC1           (1 << 30)   /**< AIC PID.                  */
#define SAMA_PID_L2CC           (1 << 31)   /**< L2CC PID.                 */
#endif /* defined(SAMA5D27) */

/** @} */

#endif /* SAMA_REGISTRY_H */

/** @} */
