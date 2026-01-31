[#ftl]
[#--
    Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="xmcuconf.h" /]
/*
[@license.EmitLicenseAsText /]
*/

#ifndef XMCUCONF_H
#define XMCUCONF_H

/*
 * STM32H5xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define STM32H5xx_XMCUCONF
#define STM32H562_XMCUCONF
#define STM32H563_XMCUCONF
#define STM32H573_XMCUCONF

/*
 * HAL driver general settings.
 */
#define STM32_NO_INIT                       ${doc.STM32_NO_INIT!"FALSE"}
#define STM32_CLOCK_DYNAMIC                 ${doc.STM32_CLOCK_DYNAMIC!"FALSE"}

/*
 * ICache settings.
 */
#define STM32_ICACHE_CR                     ${doc.STM32_ICACHE_CR!"(ICACHE_CR_EN)"}
#define STM32_ICACHE_CRR0                   ${doc.STM32_ICACHE_CRR0!"(0U)"}
#define STM32_ICACHE_CRR1                   ${doc.STM32_ICACHE_CRR1!"(0U)"}
#define STM32_ICACHE_CRR2                   ${doc.STM32_ICACHE_CRR2!"(0U)"}
#define STM32_ICACHE_CRR3                   ${doc.STM32_ICACHE_CRR3!"(0U)"}

/*
 * PWR settings.
 */
#define STM32_PWR_VOSCR                     ${doc.STM32_PWR_VOSCR!"PWR_VOSCR_VOS_RANGE0"}
#define STM32_PWR_BDCR                      ${doc.STM32_PWR_BDCR!"(0U)"}
#define STM32_PWR_UCPDR                     ${doc.STM32_PWR_UCPDR!"(0U)"}
#define STM32_PWR_SCCR                      ${doc.STM32_PWR_SCCR!"(0U)"}
#define STM32_PWR_VMCR                      ${doc.STM32_PWR_VMCR!"(0U)"}
#define STM32_PWR_USBSCR                    ${doc.STM32_PWR_USBSCR!"(0U)"}
#define STM32_PWR_WUCR                      ${doc.STM32_PWR_WUCR!"(0U)"}
#define STM32_PWR_IORETR                    ${doc.STM32_PWR_IORETR!"(0U)"}
#define STM32_PWR_SECCFGR                   ${doc.STM32_PWR_SECCFGR!"(0U)"}
#define STM32_PWR_PRIVCFGR                  ${doc.STM32_PWR_PRIVCFGR!"(0U)"}

/*
 * Clock settings.
 */
#define STM32_HSI_ENABLED                   ${doc.STM32_HSI_ENABLED!"TRUE"}
#define STM32_HSIDIV_VALUE                  ${doc.STM32_HSIDIV_VALUE!"2"}
#define STM32_HSI48_ENABLED                 ${doc.STM32_HSI48_ENABLED!"FALSE"}
#define STM32_CSI_ENABLED                   ${doc.STM32_CSI_ENABLED!"FALSE"}
#define STM32_HSE_ENABLED                   ${doc.STM32_HSE_ENABLED!"FALSE"}
#define STM32_LSI_ENABLED                   ${doc.STM32_LSI_ENABLED!"FALSE"}
#define STM32_LSE_ENABLED                   ${doc.STM32_LSE_ENABLED!"FALSE"}
#define STM32_SW                            ${doc.STM32_SW!"RCC_CFGR1_SW_PLL1P"}
#define STM32_PLL1SRC                       ${doc.STM32_PLL1SRC!"RCC_PLL1CFGR_PLL1SRC_HSI"}
#define STM32_PLL1M_VALUE                   ${doc.STM32_PLL1M_VALUE!"16"}
#define STM32_PLL1N_VALUE                   ${doc.STM32_PLL1N_VALUE!"250"}
#define STM32_PLL1P_VALUE                   ${doc.STM32_PLL1P_VALUE!"2"}
#define STM32_PLL1Q_VALUE                   ${doc.STM32_PLL1Q_VALUE!"4"}
#define STM32_PLL1R_VALUE                   ${doc.STM32_PLL1R_VALUE!"2"}
#define STM32_PLL2SRC                       ${doc.STM32_PLL2SRC!"RCC_PLL2CFGR_PLL2SRC_HSI"}
#define STM32_PLL2M_VALUE                   ${doc.STM32_PLL2M_VALUE!"16"}
#define STM32_PLL2N_VALUE                   ${doc.STM32_PLL2N_VALUE!"250"}
#define STM32_PLL2P_VALUE                   ${doc.STM32_PLL2P_VALUE!"2"}
#define STM32_PLL2Q_VALUE                   ${doc.STM32_PLL2Q_VALUE!"2"}
#define STM32_PLL2R_VALUE                   ${doc.STM32_PLL2R_VALUE!"2"}
#define STM32_PLL3SRC                       ${doc.STM32_PLL3SRC!"RCC_PLL3CFGR_PLL3SRC_HSI"}
#define STM32_PLL3M_VALUE                   ${doc.STM32_PLL3M_VALUE!"16"}
#define STM32_PLL3N_VALUE                   ${doc.STM32_PLL3N_VALUE!"250"}
#define STM32_PLL3P_VALUE                   ${doc.STM32_PLL3P_VALUE!"2"}
#define STM32_PLL3Q_VALUE                   ${doc.STM32_PLL3Q_VALUE!"2"}
#define STM32_PLL3R_VALUE                   ${doc.STM32_PLL3R_VALUE!"2"}
#define STM32_HPRE                          ${doc.STM32_HPRE!"RCC_CFGR2_HPRE_DIV1"}
#define STM32_PPRE1                         ${doc.STM32_PPRE1!"RCC_CFGR2_PPRE1_DIV1"}
#define STM32_PPRE2                         ${doc.STM32_PPRE2!"RCC_CFGR2_PPRE2_DIV1"}
#define STM32_PPRE3                         ${doc.STM32_PPRE3!"RCC_CFGR2_PPRE3_DIV1"}
#define STM32_STOPWUCK                      ${doc.STM32_STOPWUCK!"RCC_CFGR1_STOPWUCK_HSI"}
#define STM32_STOPKERWUCK                   ${doc.STM32_STOPKERWUCK!"RCC_CFGR1_STOPKERWUCK_HSI"}
#define STM32_TIMPRE                        ${doc.STM32_TIMPRE!"RCC_CFGR1_TIMPRE_LOW"}
#define STM32_RTCPRE_VALUE                  ${doc.STM32_RTCPRE_VALUE!"RCC_CFGR1_RTCPRE_NOCLOCK"}
#define STM32_MCO1SEL                       ${doc.STM32_MCO1SEL!"RCC_CFGR1_MCO1SEL_HSI"}
#define STM32_MCO1PRE_VALUE                 ${doc.STM32_MCO1PRE_VALUE!"RCC_CFGR1_MCO1PRE_NOCLOCK"}
#define STM32_MCO2SEL                       ${doc.STM32_MCO2SEL!"RCC_CFGR1_MCO2SEL_SYSCLK"}
#define STM32_MCO2PRE_VALUE                 ${doc.STM32_MCO2PRE_VALUE!"RCC_CFGR1_MCO2PRE_NOCLOCK"}
#define STM32_LSCOSEL                       ${doc.STM32_LSCOSEL!"RCC_BDCR_LSCOSEL_NOCLOCK"}

/*
 * Peripherals clock sources.
 */
#define STM32_USART1SEL                     ${doc.STM32_USART1SEL!"RCC_CCIPR1_USART1SEL_PCLK2"}
#define STM32_USART2SEL                     ${doc.STM32_USART2SEL!"RCC_CCIPR1_USART2SEL_PCLK1"}
#define STM32_USART3SEL                     ${doc.STM32_USART3SEL!"RCC_CCIPR1_USART3SEL_PCLK1"}
#define STM32_UART4SEL                      ${doc.STM32_UART4SEL!"RCC_CCIPR1_UART4SEL_PCLK1"}
#define STM32_UART5SEL                      ${doc.STM32_UART5SEL!"RCC_CCIPR1_UART5SEL_PCLK1"}
#define STM32_USART6SEL                     ${doc.STM32_USART6SEL!"RCC_CCIPR1_USART6SEL_PCLK1"}
#define STM32_UART7SEL                      ${doc.STM32_UART7SEL!"RCC_CCIPR1_UART7SEL_PCLK1"}
#define STM32_UART8SEL                      ${doc.STM32_UART8SEL!"RCC_CCIPR1_UART8SEL_PCLK1"}
#define STM32_UART9SEL                      ${doc.STM32_UART9SEL!"RCC_CCIPR1_UART9SEL_PCLK1"}
#define STM32_USART10SEL                    ${doc.STM32_USART10SEL!"RCC_CCIPR1_USART10SEL_PCLK1"}
#define STM32_USART11SEL                    ${doc.STM32_USART11SEL!"RCC_CCIPR2_USART11SEL_PCLK1"}
#define STM32_UART12SEL                     ${doc.STM32_UART12SEL!"RCC_CCIPR2_UART12SEL_PCLK1"}
#define STM32_LPUART1SEL                    ${doc.STM32_LPUART1SEL!"RCC_CCIPR3_LPUART1SEL_PCLK3"}
#define STM32_TIMICSEL                      ${doc.STM32_TIMICSEL!"RCC_CCIPR1_TIMICSEL_NOCLK"}
#define STM32_LPTIM1SEL                     ${doc.STM32_LPTIM1SEL!"RCC_CCIPR2_LPTIM1SEL_PCLK3"}
#define STM32_LPTIM2SEL                     ${doc.STM32_LPTIM2SEL!"RCC_CCIPR2_LPTIM2SEL_PCLK1"}
#define STM32_LPTIM3SEL                     ${doc.STM32_LPTIM3SEL!"RCC_CCIPR2_LPTIM3SEL_PCLK3"}
#define STM32_LPTIM4SEL                     ${doc.STM32_LPTIM4SEL!"RCC_CCIPR2_LPTIM4SEL_PCLK3"}
#define STM32_LPTIM5SEL                     ${doc.STM32_LPTIM5SEL!"RCC_CCIPR2_LPTIM5SEL_PCLK3"}
#define STM32_LPTIM6SEL                     ${doc.STM32_LPTIM6SEL!"RCC_CCIPR2_LPTIM6SEL_PCLK3"}
#define STM32_SPI1SEL                       ${doc.STM32_SPI1SEL!"RCC_CCIPR3_SPI1SEL_PLL1Q"}
#define STM32_SPI2SEL                       ${doc.STM32_SPI2SEL!"RCC_CCIPR3_SPI2SEL_PLL1Q"}
#define STM32_SPI3SEL                       ${doc.STM32_SPI3SEL!"RCC_CCIPR3_SPI3SEL_PLL1Q"}
#define STM32_SPI4SEL                       ${doc.STM32_SPI4SEL!"RCC_CCIPR3_SPI4SEL_PCLK2"}
#define STM32_SPI5SEL                       ${doc.STM32_SPI5SEL!"RCC_CCIPR3_SPI5SEL_PCLK3"}
#define STM32_SPI6SEL                       ${doc.STM32_SPI6SEL!"RCC_CCIPR3_SPI6SEL_PCLK2"}
#define STM32_OSPISEL                       ${doc.STM32_OSPISEL!"RCC_CCIPR4_OSPISEL_HCLK4"}
#define STM32_SYSTICKSEL                    ${doc.STM32_SYSTICKSEL!"RCC_CCIPR4_SYSTICKSEL_HCLKDIV8"}
#define STM32_USBSEL                        ${doc.STM32_USBSEL!"RCC_CCIPR4_USBSEL_NOCLOCK"}
#define STM32_SDMMC1SEL                     ${doc.STM32_SDMMC1SEL!"RCC_CCIPR4_SDMMC1SEL_PLL1Q"}
#define STM32_SDMMC2SEL                     ${doc.STM32_SDMMC2SEL!"RCC_CCIPR4_SDMMC2SEL_PLL1Q"}
#define STM32_I2C1SEL                       ${doc.STM32_I2C1SEL!"RCC_CCIPR4_I2C1SEL_PCLK1"}
#define STM32_I2C2SEL                       ${doc.STM32_I2C2SEL!"RCC_CCIPR4_I2C2SEL_PCLK1"}
#define STM32_I2C3SEL                       ${doc.STM32_I2C3SEL!"RCC_CCIPR4_I2C3SEL_PCLK3"}
#define STM32_I2C4SEL                       ${doc.STM32_I2C4SEL!"RCC_CCIPR4_I2C4SEL_PCLK3"}
#define STM32_I3C1SEL                       ${doc.STM32_I3C1SEL!"RCC_CCIPR4_I3C1SEL_PCLK1"}
#define STM32_ADCDACSEL                     ${doc.STM32_ADCDACSEL!"RCC_CCIPR5_ADCDACSEL_HCLK"}
#define STM32_DACSEL                        ${doc.STM32_DACSEL!"RCC_CCIPR5_DACSEL_IGNORE"}
#define STM32_RNGSEL                        ${doc.STM32_RNGSEL!"RCC_CCIPR5_RNGSEL_IGNORE"}
#define STM32_CECSEL                        ${doc.STM32_CECSEL!"RCC_CCIPR5_CECSEL_IGNORE"}
#define STM32_FDCANSEL                      ${doc.STM32_FDCANSEL!"RCC_CCIPR5_FDCANSEL_IGNORE"}
#define STM32_SAI1SEL                       ${doc.STM32_SAI1SEL!"RCC_CCIPR5_SAI1SEL_PLL1Q"}
#define STM32_SAI2SEL                       ${doc.STM32_SAI2SEL!"RCC_CCIPR5_SAI2SEL_PLL1Q"}
#define STM32_CKPERSEL                      ${doc.STM32_CKPERSEL!"RCC_CCIPR5_CKPERSEL_HSI"}
#define STM32_RTCSEL                        ${doc.STM32_RTCSEL!"RCC_BDCR_RTCSEL_NOCLOCK"}

/*
 * IRQ system settings.
 */
#define STM32_IRQ_DAC1_PRIORITY             9

#define STM32_IRQ_EXTI0_PRIORITY            ${doc.STM32_IRQ_EXTI0_PRIORITY!"6"}
#define STM32_IRQ_EXTI1_PRIORITY            ${doc.STM32_IRQ_EXTI1_PRIORITY!"6"}
#define STM32_IRQ_EXTI2_PRIORITY            ${doc.STM32_IRQ_EXTI2_PRIORITY!"6"}
#define STM32_IRQ_EXTI3_PRIORITY            ${doc.STM32_IRQ_EXTI3_PRIORITY!"6"}
#define STM32_IRQ_EXTI4_PRIORITY            ${doc.STM32_IRQ_EXTI4_PRIORITY!"6"}
#define STM32_IRQ_EXTI5_PRIORITY            ${doc.STM32_IRQ_EXTI5_PRIORITY!"6"}
#define STM32_IRQ_EXTI6_PRIORITY            ${doc.STM32_IRQ_EXTI6_PRIORITY!"6"}
#define STM32_IRQ_EXTI7_PRIORITY            ${doc.STM32_IRQ_EXTI7_PRIORITY!"6"}
#define STM32_IRQ_EXTI8_PRIORITY            ${doc.STM32_IRQ_EXTI8_PRIORITY!"6"}
#define STM32_IRQ_EXTI9_PRIORITY            ${doc.STM32_IRQ_EXTI9_PRIORITY!"6"}
#define STM32_IRQ_EXTI10_PRIORITY           ${doc.STM32_IRQ_EXTI10_PRIORITY!"6"}
#define STM32_IRQ_EXTI11_PRIORITY           ${doc.STM32_IRQ_EXTI11_PRIORITY!"6"}
#define STM32_IRQ_EXTI12_PRIORITY           ${doc.STM32_IRQ_EXTI12_PRIORITY!"6"}
#define STM32_IRQ_EXTI13_PRIORITY           ${doc.STM32_IRQ_EXTI13_PRIORITY!"6"}
#define STM32_IRQ_EXTI14_PRIORITY           ${doc.STM32_IRQ_EXTI14_PRIORITY!"6"}
#define STM32_IRQ_EXTI15_PRIORITY           ${doc.STM32_IRQ_EXTI15_PRIORITY!"6"}
#define STM32_IRQ_EXTI17_PRIORITY           ${doc.STM32_IRQ_EXTI17_PRIORITY!"6"}
#define STM32_IRQ_EXTI19_PRIORITY           ${doc.STM32_IRQ_EXTI19_PRIORITY!"6"}

#define STM32_IRQ_I2C1_PRIORITY             ${doc.STM32_IRQ_I2C1_PRIORITY!"5"}
#define STM32_IRQ_I2C2_PRIORITY             ${doc.STM32_IRQ_I2C2_PRIORITY!"5"}
#define STM32_IRQ_I2C3_PRIORITY             ${doc.STM32_IRQ_I2C3_PRIORITY!"5"}
#define STM32_IRQ_I2C4_PRIORITY             ${doc.STM32_IRQ_I2C4_PRIORITY!"5"}

#define STM32_IRQ_SPI1_PRIORITY             ${doc.STM32_IRQ_SPI1_PRIORITY!"10"}
#define STM32_IRQ_SPI2_PRIORITY             ${doc.STM32_IRQ_SPI2_PRIORITY!"10"}
#define STM32_IRQ_SPI3_PRIORITY             ${doc.STM32_IRQ_SPI3_PRIORITY!"10"}
#define STM32_IRQ_SPI4_PRIORITY             ${doc.STM32_IRQ_SPI4_PRIORITY!"10"}
#define STM32_IRQ_SPI5_PRIORITY             ${doc.STM32_IRQ_SPI5_PRIORITY!"10"}
#define STM32_IRQ_SPI6_PRIORITY             ${doc.STM32_IRQ_SPI6_PRIORITY!"10"}

#define STM32_IRQ_TIM1_BRK_PRIORITY         ${doc.STM32_IRQ_TIM1_BRK_PRIORITY!"7"}
#define STM32_IRQ_TIM1_UP_PRIORITY          ${doc.STM32_IRQ_TIM1_UP_PRIORITY!"7"}
#define STM32_IRQ_TIM1_TRGCO_PRIORITY       ${doc.STM32_IRQ_TIM1_TRGCO_PRIORITY!"7"}
#define STM32_IRQ_TIM1_CC_PRIORITY          ${doc.STM32_IRQ_TIM1_CC_PRIORITY!"7"}
#define STM32_IRQ_TIM2_PRIORITY             ${doc.STM32_IRQ_TIM2_PRIORITY!"7"}
#define STM32_IRQ_TIM3_PRIORITY             ${doc.STM32_IRQ_TIM3_PRIORITY!"7"}
#define STM32_IRQ_TIM4_PRIORITY             ${doc.STM32_IRQ_TIM4_PRIORITY!"7"}
#define STM32_IRQ_TIM5_PRIORITY             ${doc.STM32_IRQ_TIM5_PRIORITY!"7"}
#define STM32_IRQ_TIM6_PRIORITY             ${doc.STM32_IRQ_TIM6_PRIORITY!"7"}
#define STM32_IRQ_TIM7_PRIORITY             ${doc.STM32_IRQ_TIM7_PRIORITY!"7"}
#define STM32_IRQ_TIM8_BRK_PRIORITY         ${doc.STM32_IRQ_TIM8_BRK_PRIORITY!"7"}
#define STM32_IRQ_TIM8_UP_PRIORITY          ${doc.STM32_IRQ_TIM8_UP_PRIORITY!"7"}
#define STM32_IRQ_TIM8_CC_PRIORITY          ${doc.STM32_IRQ_TIM8_CC_PRIORITY!"7"}
#define STM32_IRQ_TIM8_CC_PRIORITY          ${doc.STM32_IRQ_TIM8_CC_PRIORITY!"7"}
#define STM32_IRQ_TIM12_PRIORITY            ${doc.STM32_IRQ_TIM12_PRIORITY!"7"}
#define STM32_IRQ_TIM13_PRIORITY            ${doc.STM32_IRQ_TIM13_PRIORITY!"7"}
#define STM32_IRQ_TIM14_PRIORITY            ${doc.STM32_IRQ_TIM14_PRIORITY!"7"}
#define STM32_IRQ_TIM15_PRIORITY            ${doc.STM32_IRQ_TIM15_PRIORITY!"7"}
#define STM32_IRQ_TIM16_PRIORITY            ${doc.STM32_IRQ_TIM16_PRIORITY!"7"}
#define STM32_IRQ_TIM17_PRIORITY            ${doc.STM32_IRQ_TIM17_PRIORITY!"7"}

#define STM32_IRQ_USART1_PRIORITY           ${doc.STM32_IRQ_USART1_PRIORITY!"12"}
#define STM32_IRQ_USART2_PRIORITY           ${doc.STM32_IRQ_USART2_PRIORITY!"12"}
#define STM32_IRQ_USART3_PRIORITY           ${doc.STM32_IRQ_USART3_PRIORITY!"12"}
#define STM32_IRQ_UART4_PRIORITY            ${doc.STM32_IRQ_UART4_PRIORITY!"12"}
#define STM32_IRQ_UART5_PRIORITY            ${doc.STM32_IRQ_UART5_PRIORITY!"12"}
#define STM32_IRQ_USART6_PRIORITY           ${doc.STM32_IRQ_USART6_PRIORITY!"12"}
#define STM32_IRQ_UART7_PRIORITY            ${doc.STM32_IRQ_UART7_PRIORITY!"12"}
#define STM32_IRQ_UART8_PRIORITY            ${doc.STM32_IRQ_UART8_PRIORITY!"12"}
#define STM32_IRQ_UART9_PRIORITY            ${doc.STM32_IRQ_UART9_PRIORITY!"12"}
#define STM32_IRQ_USART10_PRIORITY          ${doc.STM32_IRQ_USART10_PRIORITY!"12"}
#define STM32_IRQ_USART11_PRIORITY          ${doc.STM32_IRQ_USART11_PRIORITY!"12"}
#define STM32_IRQ_UART12_PRIORITY           ${doc.STM32_IRQ_UART12_PRIORITY!"12"}
#define STM32_IRQ_LPUART1_PRIORITY          ${doc.STM32_IRQ_LPUART1_PRIORITY!"12"}

#define STM32_IRQ_USB1_PRIORITY             ${doc.STM32_IRQ_USB1_PRIORITY!"13"}

/*
 * SIO driver system settings.
 */
#define STM32_SIO_USE_USART1                ${doc.STM32_SIO_USE_USART1!"FALSE"}
#define STM32_SIO_USE_USART2                ${doc.STM32_SIO_USE_USART2!"FALSE"}
#define STM32_SIO_USE_USART3                ${doc.STM32_SIO_USE_USART3!"FALSE"}
#define STM32_SIO_USE_UART4                 ${doc.STM32_SIO_USE_UART4!"FALSE"}
#define STM32_SIO_USE_UART5                 ${doc.STM32_SIO_USE_UART5!"FALSE"}
#define STM32_SIO_USE_USART6                ${doc.STM32_SIO_USE_USART6!"FALSE"}
#define STM32_SIO_USE_UART7                 ${doc.STM32_SIO_USE_UART7!"FALSE"}
#define STM32_SIO_USE_UART8                 ${doc.STM32_SIO_USE_UART8!"FALSE"}
#define STM32_SIO_USE_UART9                 ${doc.STM32_SIO_USE_UART9!"FALSE"}
#define STM32_SIO_USE_USART10               ${doc.STM32_SIO_USE_USART10!"FALSE"}
#define STM32_SIO_USE_USART11               ${doc.STM32_SIO_USE_USART11!"FALSE"}
#define STM32_SIO_USE_UART12                ${doc.STM32_SIO_USE_UART12!"FALSE"}
#define STM32_SIO_USE_LPUART1               ${doc.STM32_SIO_USE_LPUART1!"FALSE"}

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  ${doc.STM32_SPI_USE_SPI1!"FALSE"}
#define STM32_SPI_USE_SPI2                  ${doc.STM32_SPI_USE_SPI2!"FALSE"}
#define STM32_SPI_USE_SPI3                  ${doc.STM32_SPI_USE_SPI3!"FALSE"}
#define STM32_SPI_USE_SPI4                  ${doc.STM32_SPI_USE_SPI4!"FALSE"}
#define STM32_SPI_USE_SPI5                  ${doc.STM32_SPI_USE_SPI5!"FALSE"}
#define STM32_SPI_USE_SPI6                  ${doc.STM32_SPI_USE_SPI6!"FALSE"}
#define STM32_SPI_SPI1_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI1_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI1_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI1_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI2_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI2_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI2_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI2_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI3_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI3_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI3_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI3_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI4_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI4_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI4_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI4_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI5_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI5_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI5_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI5_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI6_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI6_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI6_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI6_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI1_DMA_PRIORITY         ${doc.STM32_SPI_SPI1_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI2_DMA_PRIORITY         ${doc.STM32_SPI_SPI2_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI3_DMA_PRIORITY         ${doc.STM32_SPI_SPI3_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI4_DMA_PRIORITY         ${doc.STM32_SPI_SPI4_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI5_DMA_PRIORITY         ${doc.STM32_SPI_SPI5_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI6_DMA_PRIORITY         ${doc.STM32_SPI_SPI6_DMA_PRIORITY!"1"}
#define STM32_SPI_DMA_ERROR_HOOK(spip)      ${doc.STM32_SPI_DMA_ERROR_HOOK!"osalSysHalt(\"DMA failure\")"}

/*
 * ST driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               ${doc.STM32_ST_IRQ_PRIORITY!"8"}
#define STM32_ST_USE_TIMER                  ${doc.STM32_ST_USE_TIMER!"2"}

#endif /* XMCUCONF_H */
