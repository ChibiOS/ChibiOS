[#ftl]
[#--
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
[@pp.changeOutputFile name="mcuconf.h" /]
/*
[@license.EmitLicenseAsText /]
*/

#ifndef MCUCONF_H
#define MCUCONF_H

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

#define STM32H5xx_MCUCONF
#define STM32H503_MCUCONF

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
#define STM32_PWR_VOSCR                     ${doc.STM32_PWR_VOSCR!"STM32_VOS_RANGE0"}
#define STM32_PWR_BDCR                      ${doc.STM32_PWR_BDCR!"(0U)"}
#define STM32_PWR_SCCR                      ${doc.STM32_PWR_SCCR!"(0U)"}
#define STM32_PWR_VMCR                      ${doc.STM32_PWR_VMCR!"(0U)"}
#define STM32_PWR_WUCR                      ${doc.STM32_PWR_WUCR!"(0U)"}
#define STM32_PWR_IORETR                    ${doc.STM32_PWR_IORETR!"(0U)"}
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
#define STM32_SW                            ${doc.STM32_SW!"STM32_SW_PLL1P"}
#define STM32_PLL1SRC                       ${doc.STM32_PLL1SRC!"STM32_PLL1SRC_HSI"}
#define STM32_PLL1M_VALUE                   ${doc.STM32_PLL1M_VALUE!"16"}
#define STM32_PLL1N_VALUE                   ${doc.STM32_PLL1N_VALUE!"250"}
#define STM32_PLL1P_VALUE                   ${doc.STM32_PLL1P_VALUE!"2"}
#define STM32_PLL1Q_VALUE                   ${doc.STM32_PLL1Q_VALUE!"4"}
#define STM32_PLL1R_VALUE                   ${doc.STM32_PLL1R_VALUE!"2"}
#define STM32_PLL2SRC                       ${doc.STM32_PLL2SRC!"STM32_PLL2SRC_HSI"}
#define STM32_PLL2M_VALUE                   ${doc.STM32_PLL2M_VALUE!"16"}
#define STM32_PLL2N_VALUE                   ${doc.STM32_PLL2N_VALUE!"250"}
#define STM32_PLL2P_VALUE                   ${doc.STM32_PLL2P_VALUE!"2"}
#define STM32_PLL2Q_VALUE                   ${doc.STM32_PLL2Q_VALUE!"2"}
#define STM32_PLL2R_VALUE                   ${doc.STM32_PLL2R_VALUE!"2"}
#define STM32_HPRE                          ${doc.STM32_HPRE!"STM32_HPRE_DIV1"}
#define STM32_PPRE1                         ${doc.STM32_PPRE1!"STM32_PPRE1_DIV1"}
#define STM32_PPRE2                         ${doc.STM32_PPRE2!"STM32_PPRE2_DIV1"}
#define STM32_PPRE3                         ${doc.STM32_PPRE3!"STM32_PPRE3_DIV1"}
#define STM32_STOPWUCK                      ${doc.STM32_STOPWUCK!"STM32_STOPWUCK_HSI"}
#define STM32_STOPKERWUCK                   ${doc.STM32_STOPKERWUCK!"STM32_STOPKERWUCK_HSI"}
#define STM32_RTCPRE_VALUE                  ${doc.STM32_RTCPRE_VALUE!"STM32_RTCPRE_NOCLOCK"}
#define STM32_MCO1SEL                       ${doc.STM32_MCO1SEL!"STM32_MCO1SEL_HSI"}
#define STM32_MCO1PRE_VALUE                 ${doc.STM32_MCO1PRE_VALUE!"STM32_MCO1PRE_NOCLOCK"}
#define STM32_MCO2SEL                       ${doc.STM32_MCO2SEL!"STM32_MCO2SEL_SYSCLK"}
#define STM32_MCO2PRE_VALUE                 ${doc.STM32_MCO2PRE_VALUE!"STM32_MCO2PRE_NOCLOCK"}
#define STM32_LSCOSEL                       ${doc.STM32_LSCOSEL!"STM32_LSCOSEL_NOCLOCK"}

/*
 * Peripherals clock sources.
 */
#define STM32_USART1SEL                     ${doc.STM32_USART1SEL!"STM32_USART1SEL_PCLK2"}
#define STM32_USART2SEL                     ${doc.STM32_USART2SEL!"STM32_USART2SEL_PCLK1"}
#define STM32_USART3SEL                     ${doc.STM32_USART3SEL!"STM32_USART3SEL_PCLK1"}
#define STM32_LPUART1SEL                    ${doc.STM32_LPUART1SEL!"STM32_LPUART1SEL_PCLK3"}
#define STM32_TIMICSEL                      ${doc.STM32_TIMICSEL!"STM32_TIMICSEL_NOCLK"}
#define STM32_LPTIM1SEL                     ${doc.STM32_LPTIM1SEL!"STM32_LPTIM1SEL_PCLK3"}
#define STM32_LPTIM2SEL                     ${doc.STM32_LPTIM2SEL!"STM32_LPTIM2SEL_PCLK1"}
#define STM32_SPI1SEL                       ${doc.STM32_SPI1SEL!"STM32_SPI1SEL_PLL1Q"}
#define STM32_SPI2SEL                       ${doc.STM32_SPI2SEL!"STM32_SPI2SEL_PLL1Q"}
#define STM32_SPI3SEL                       ${doc.STM32_SPI3SEL!"STM32_SPI3SEL_PLL1Q"}
#define STM32_SYSTICKSEL                    ${doc.STM32_SYSTICKSEL!"STM32_SYSTICKSEL_HCLKDIV8"}
#define STM32_USBSEL                        ${doc.STM32_USBSEL!"STM32_USBSEL_NOCLOCK"}
#define STM32_I2C1SEL                       ${doc.STM32_I2C1SEL!"STM32_I2C1SEL_PCLK1"}
#define STM32_I2C2SEL                       ${doc.STM32_I2C2SEL!"STM32_I2C2SEL_PCLK1"}
#define STM32_I3C1SEL                       ${doc.STM32_I3C1SEL!"STM32_I3C1SEL_PCLK1"}
#define STM32_ADCDACSEL                     ${doc.STM32_ADCDACSEL!"STM32_ADCDACSEL_HCLK"}
#define STM32_DACSEL                        ${doc.STM32_DACSEL!"STM32_DACSEL_IGNORE"}
#define STM32_RNGSEL                        ${doc.STM32_RNGSEL!"STM32_RNGSEL_IGNORE"}
#define STM32_FDCANSEL                      ${doc.STM32_FDCANSEL!"STM32_FDCANSEL_IGNORE"}
#define STM32_CKPERSEL                      ${doc.STM32_CKPERSEL!"STM32_CKPERSEL_HSI"}
#define STM32_RTCSEL                        ${doc.STM32_RTCSEL!"STM32_RTCSEL_NOCLOCK"}

/*
 * IRQ system settings.
 */
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

#define STM32_IRQ_SPI1_PRIORITY             ${doc.STM32_IRQ_SPI1_PRIORITY!"10"}
#define STM32_IRQ_SPI2_PRIORITY             ${doc.STM32_IRQ_SPI2_PRIORITY!"10"}
#define STM32_IRQ_SPI3_PRIORITY             ${doc.STM32_IRQ_SPI3_PRIORITY!"10"}

#define STM32_IRQ_TIM1_BRK_PRIORITY         ${doc.STM32_IRQ_TIM1_BRK_PRIORITY!"7"}
#define STM32_IRQ_TIM1_UP_PRIORITY          ${doc.STM32_IRQ_TIM1_UP_PRIORITY!"7"}
#define STM32_IRQ_TIM1_TRGCO_PRIORITY       ${doc.STM32_IRQ_TIM1_TRGCO_PRIORITY!"7"}
#define STM32_IRQ_TIM1_CC_PRIORITY          ${doc.STM32_IRQ_TIM1_CC_PRIORITY!"7"}
#define STM32_IRQ_TIM2_PRIORITY             ${doc.STM32_IRQ_TIM2_PRIORITY!"7"}
#define STM32_IRQ_TIM3_PRIORITY             ${doc.STM32_IRQ_TIM3_PRIORITY!"7"}
#define STM32_IRQ_TIM6_PRIORITY             ${doc.STM32_IRQ_TIM6_PRIORITY!"7"}
#define STM32_IRQ_TIM7_PRIORITY             ${doc.STM32_IRQ_TIM7_PRIORITY!"7"}

#define STM32_IRQ_USART1_PRIORITY           ${doc.STM32_IRQ_USART1_PRIORITY!"12"}
#define STM32_IRQ_USART2_PRIORITY           ${doc.STM32_IRQ_USART2_PRIORITY!"12"}
#define STM32_IRQ_USART3_PRIORITY           ${doc.STM32_IRQ_USART3_PRIORITY!"12"}
#define STM32_IRQ_LPUART1_PRIORITY          ${doc.STM32_IRQ_LPUART1_PRIORITY!"12"}

#define STM32_IRQ_USB1_PRIORITY             ${doc.STM32_IRQ_USB1_PRIORITY!"13"}

/*
 * ADC driver system settings.
 */
#define STM32_ADC_USE_ADC1                  ${doc.STM32_ADC_USE_ADC1!"FALSE"}
#define STM32_ADC_COMPACT_SAMPLES           ${doc.STM32_ADC_COMPACT_SAMPLES!"FALSE"}
#define STM32_ADC_ADC1_DMA3_CHANNEL         ${doc.STM32_ADC_ADC1_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_ADC_ADC1_DMA_PRIORITY         ${doc.STM32_ADC_ADC1_DMA_PRIORITY!"2"}
#define STM32_ADC_ADC1_IRQ_PRIORITY         ${doc.STM32_ADC_ADC1_IRQ_PRIORITY!"5"}
#define STM32_ADC_ADC1_DMA_IRQ_PRIORITY     ${doc.STM32_ADC_ADC1_DMA_IRQ_PRIORITY!"5"}
#define STM32_ADC_ADC12_CLOCK_MODE          ${doc.STM32_ADC_ADC12_CLOCK_MODE!"ADC_CCR_CKMODE_AHB_DIV4"}
#define STM32_ADC_ADC12_PRESC               ${doc.STM32_ADC_ADC12_PRESC!"ADC_CCR_PRESC_DIV2"}

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_FDCAN1                ${doc.STM32_CAN_USE_FDCAN1!"FALSE"}

/*
 * DAC driver system settings.
 */
#define STM32_DAC_DUAL_MODE                 ${doc.STM32_DAC_DUAL_MODE!"FALSE"}
#define STM32_DAC_USE_DAC1_CH1              ${doc.STM32_DAC_USE_DAC1_CH1!"FALSE"}
#define STM32_DAC_USE_DAC1_CH2              ${doc.STM32_DAC_USE_DAC1_CH2!"FALSE"}
#define STM32_DAC_DAC1_CH1_IRQ_PRIORITY     ${doc.STM32_DAC_DAC1_CH1_IRQ_PRIORITY!"10"}
#define STM32_DAC_DAC1_CH2_IRQ_PRIORITY     ${doc.STM32_DAC_DAC1_CH2_IRQ_PRIORITY!"10"}
#define STM32_DAC_DAC1_CH1_DMA_PRIORITY     ${doc.STM32_DAC_DAC1_CH1_DMA_PRIORITY!"2"}
#define STM32_DAC_DAC1_CH2_DMA_PRIORITY     ${doc.STM32_DAC_DAC1_CH2_DMA_PRIORITY!"2"}
#define STM32_DAC_DAC1_CH1_DMA3_CHANNEL     ${doc.STM32_DAC_DAC1_CH1_DMA_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_DAC_DAC1_CH2_DMA3_CHANNEL     ${doc.STM32_DAC_DAC1_CH2_DMA_CHANNEL!"STM32_DMA3_MASK_FIFO2"}

/*
 * GPT driver system settings.
 */
#define STM32_GPT_USE_TIM1                  ${doc.STM32_GPT_USE_TIM1!"FALSE"}
#define STM32_GPT_USE_TIM2                  ${doc.STM32_GPT_USE_TIM2!"FALSE"}
#define STM32_GPT_USE_TIM3                  ${doc.STM32_GPT_USE_TIM3!"FALSE"}
#define STM32_GPT_USE_TIM6                  ${doc.STM32_GPT_USE_TIM6!"FALSE"}
#define STM32_GPT_USE_TIM7                  ${doc.STM32_GPT_USE_TIM7!"FALSE"}

/*
 * I2C driver system settings.
 */
#define STM32_I2C_USE_I2C1                  ${doc.STM32_I2C_USE_I2C1!"FALSE"}
#define STM32_I2C_USE_I2C2                  ${doc.STM32_I2C_USE_I2C2!"FALSE"}
#define STM32_I2C_BUSY_TIMEOUT              ${doc.STM32_I2C_BUSY_TIMEOUT!"50"}
#define STM32_I2C_I2C1_DMA_PRIORITY         ${doc.STM32_I2C_I2C1_DMA_PRIORITY!"3"}
#define STM32_I2C_I2C2_DMA_PRIORITY         ${doc.STM32_I2C_I2C2_DMA_PRIORITY!"3"}
#define STM32_I2C_I2C1_DMA3_CHANNEL         ${doc.STM32_I2C_I2C1_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_I2C_I2C2_DMA3_CHANNEL         ${doc.STM32_I2C_I2C2_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_I2C_DMA_ERROR_HOOK(i2cp)      ${doc.STM32_I2C_DMA_ERROR_HOOK!"osalSysHalt(\"DMA failure\")"}

/*
 * ICU driver system settings.
 */
#define STM32_ICU_USE_TIM1                  ${doc.STM32_ICU_USE_TIM1!"FALSE"}
#define STM32_ICU_USE_TIM2                  ${doc.STM32_ICU_USE_TIM2!"FALSE"}
#define STM32_ICU_USE_TIM3                  ${doc.STM32_ICU_USE_TIM3!"FALSE"}

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_TIM1                  ${doc.STM32_PWM_USE_TIM1!"FALSE"}
#define STM32_PWM_USE_TIM2                  ${doc.STM32_PWM_USE_TIM2!"FALSE"}
#define STM32_PWM_USE_TIM3                  ${doc.STM32_PWM_USE_TIM3!"FALSE"}

/*
 * RTC driver system settings.
 */
#define STM32_RTC_PRESA_VALUE               ${doc.STM32_RTC_PRESA_VALUE!"32"}
#define STM32_RTC_PRESS_VALUE               ${doc.STM32_RTC_PRESS_VALUE!"1024"}
#define STM32_RTC_CR_INIT                   ${doc.STM32_RTC_CR_INIT!"0"}
#define STM32_TAMP_CR1_INIT                 ${doc.STM32_TAMP_CR1_INIT!"0"}
#define STM32_TAMP_CR2_INIT                 ${doc.STM32_TAMP_CR2_INIT!"0"}
#define STM32_TAMP_FLTCR_INIT               ${doc.STM32_TAMP_FLTCR_INIT!"0"}
#define STM32_TAMP_IER_INIT                 ${doc.STM32_TAMP_IER_INIT!"0"}

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             ${doc.STM32_SERIAL_USE_USART1!"FALSE"}
#define STM32_SERIAL_USE_USART2             ${doc.STM32_SERIAL_USE_USART2!"FALSE"}
#define STM32_SERIAL_USE_USART3             ${doc.STM32_SERIAL_USE_USART3!"FALSE"}
#define STM32_SERIAL_USE_LPUART1            ${doc.STM32_SERIAL_USE_LPUART1!"FALSE"}

/*
 * SIO driver system settings.
 */
#define STM32_SIO_USE_USART1                ${doc.STM32_SIO_USE_USART1!"FALSE"}
#define STM32_SIO_USE_USART2                ${doc.STM32_SIO_USE_USART2!"FALSE"}
#define STM32_SIO_USE_USART3                ${doc.STM32_SIO_USE_USART3!"FALSE"}
#define STM32_SIO_USE_LPUART1               ${doc.STM32_SIO_USE_LPUART1!"FALSE"}

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  ${doc.STM32_SPI_USE_SPI1!"FALSE"}
#define STM32_SPI_USE_SPI2                  ${doc.STM32_SPI_USE_SPI2!"FALSE"}
#define STM32_SPI_USE_SPI3                  ${doc.STM32_SPI_USE_SPI3!"FALSE"}
#define STM32_SPI_SPI1_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI1_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI1_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI1_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI2_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI2_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI2_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI2_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI3_RX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI3_RX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI3_TX_DMA3_CHANNEL      ${doc.STM32_SPI_SPI3_TX_DMA3_CHANNEL!"STM32_DMA3_MASK_FIFO2"}
#define STM32_SPI_SPI1_DMA_PRIORITY         ${doc.STM32_SPI_SPI1_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI2_DMA_PRIORITY         ${doc.STM32_SPI_SPI2_DMA_PRIORITY!"1"}
#define STM32_SPI_SPI3_DMA_PRIORITY         ${doc.STM32_SPI_SPI3_DMA_PRIORITY!"1"}
#define STM32_SPI_DMA_ERROR_HOOK(spip)      ${doc.STM32_SPI_DMA_ERROR_HOOK!"osalSysHalt(\"DMA failure\")"}

/*
 * ST driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               ${doc.STM32_ST_IRQ_PRIORITY!"8"}
#define STM32_ST_USE_TIMER                  ${doc.STM32_ST_USE_TIMER!"2"}

/*
 * TRNG driver system settings.
 */
#define STM32_TRNG_USE_RNG1                 ${doc.STM32_TRNG_USE_RNG1!"FALSE"}

/*
 * UART driver system settings.
 */

/*
 * USB driver system settings.
 */
#define STM32_USB_USE_USB1                  ${doc.STM32_USB_USE_USB1!" FALSE"}
#define STM32_USB_USE_ISOCHRONOUS           ${doc.STM32_USB_USE_ISOCHRONOUS!"FALSE"}
#define STM32_USB_USE_FAST_COPY             ${doc.STM32_USB_USE_FAST_COPY!"FALSE"}
#define STM32_USB_HOST_WAKEUP_DURATION      ${doc.STM32_USB_HOST_WAKEUP_DURATION!"2"}
#define STM32_USB_48MHZ_DELTA               ${doc.STM32_USB_48MHZ_DELTA!"120000"}

/*
 * WDG driver system settings.
 */
#define STM32_WDG_USE_IWDG                  ${doc.STM32_WDG_USE_IWDG!"FALSE"}

/*
 * WSPI driver system settings.
 */

#endif /* MCUCONF_H */
