/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC43xx HAL driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC43xx/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "LPC43xx.h"
#include "nvic.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS TRUE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME           "LPC43xx"

#define IRCOSCCLK               12000000UL	    /**< High speed internal clock. */

#define CLK_SEL_32KHZ           0x00000000
#define CLK_SEL_IRC             0x00000001
#define CLK_SEL_ENET_RX_CLK     0x00000002
#define CLK_SEL_ENET_TX_CLK     0x00000003
#define CLK_SEL_GP_CLKIN        0x00000004
#define CLK_SEL_XTAL            0x00000006
#define CLK_SEL_PLL0USB         0x00000007
#define CLK_SEL_PLL0AUDIO       0x00000008
#define CLK_SEL_PLL1            0x00000009
#define CLK_SEL_IDIVA           0x0000000C
#define CLK_SEL_IDIVB           0x0000000D
#define CLK_SEL_IDIVC           0x0000000E
#define CLK_SEL_IDIVD           0x0000000F
#define CLK_SEL_IDIVE           0x00000010

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Crystal oscillator enable.
 */
#if !defined(LPC_XTAL_ENABLE) || defined(__DOXYGEN__)
#define LPC_XTAL_ENABLE                 TRUE
#endif

/**
 * @brief   PLL1 enable.
 */
#if !defined(LPC_PLL1_ENABLE) || defined(__DOXYGEN__)
#define LPC_PLL1_ENABLE                 TRUE
#endif

/**
 * @brief   PLL1 multiplier.
 * @note    Final frequency must not exceed the CCO ratings.
 */
#if !defined(LPC_PLL1_MUL) || defined(__DOXYGEN__)
#define LPC_PLL1_MUL                    17
#endif

/**
 * @brief   PLL1 pre-divider.
 * @note    The value must be in the 1..4 range and the final frequency
 *          must not exceed the CCO ratings.
 */
#if !defined(LPC_PLL1_PREDIV) || defined(__DOXYGEN__)
#define LPC_PLL1_PREDIV                 1
#endif

/**
 * @brief   PLL1 post-divider enable.
 */
#if !defined(LPC_PLL1_POSTDIV_ENABLE) || defined(__DOXYGEN__)
#define LPC_PLL1_POSTDIV_ENABLE         FALSE
#endif

/**
 * @brief   PLL1 post-divider.
 * @note    The value must be 2, 4, 8 or 16.
 */
#if !defined(LPC_PLL1_POSTDIV) || defined(__DOXYGEN__)
#define LPC_PLL1_POSTDIV                2
#endif

/**
 * @brief   USB0 PLL0 enable.
 */
#if !defined(LPC_PLL0USB_ENABLE) || defined(__DOXYGEN__)
#define LPC_PLL0USB_ENABLE              FALSE
#endif

/**
 * @brief   Audio PLL0 enable.
 */
#if !defined(LPC_PLL0AUDIO_ENABLE) || defined(__DOXYGEN__)
#define LPC_PLL0AUDIO_ENABLE            FALSE
#endif

/**
 * @brief   IDIVA clock divider enable.
 */
#if !defined(LPC_IDIVA_ENABLE) || defined(__DOXYGEN__)
#define LPC_IDIVA_ENABLE                FALSE
#endif

/**
 * @brief   IDIVA clock divider source.
 */
#if !defined(LPC_IDIVA_SRC) || defined(__DOXYGEN__)
#define LPC_IDIVA_SRC                   CLK_SEL_IRC
#endif

/**
 * @brief   IDIVA clock divider.
 */
#if !defined(LPC_IDIVA_DIV) || defined(__DOXYGEN__)
#define LPC_IDIVA_DIV                   1
#endif

/**
 * @brief   IDIVB clock divider enable.
 */
#if !defined(LPC_IDIVB_ENABLE) || defined(__DOXYGEN__)
#define LPC_IDIVB_ENABLE                FALSE
#endif

/**
 * @brief   IDIVB clock divider source.
 */
#if !defined(LPC_IDIVB_SRC) || defined(__DOXYGEN__)
#define LPC_IDIVB_SRC                   CLK_SEL_IRC
#endif

/**
 * @brief   IDIVB clock divider.
 */
#if !defined(LPC_IDIVB_DIV) || defined(__DOXYGEN__)
#define LPC_IDIVB_DIV                   1
#endif

/**
 * @brief   IDIVC clock divider enable.
 */
#if !defined(LPC_IDIVC_ENABLE) || defined(__DOXYGEN__)
#define LPC_IDIVC_ENABLE                TRUE
#endif

/**
 * @brief   IDIVC clock divider source.
 */
#if !defined(LPC_IDIVC_SRC) || defined(__DOXYGEN__)
#define LPC_IDIVC_SRC                   CLK_SEL_PLL1
#endif

/**
 * @brief   IDIVC clock divider.
 */
#if !defined(LPC_IDIVC_DIV) || defined(__DOXYGEN__)
#define LPC_IDIVC_DIV                   3
#endif

/**
 * @brief   IDIVD clock divider enable.
 */
#if !defined(LPC_IDIVD_ENABLE) || defined(__DOXYGEN__)
#define LPC_IDIVD_ENABLE                FALSE
#endif

/**
 * @brief   IDIVD clock divider source.
 */
#if !defined(LPC_IDIVD_SRC) || defined(__DOXYGEN__)
#define LPC_IDIVD_SRC                   CLK_SEL_IRC
#endif

/**
 * @brief   IDIVD clock divider.
 */
#if !defined(LPC_IDIVD_DIV) || defined(__DOXYGEN__)
#define LPC_IDIVD_DIV                   1
#endif

/**
 * @brief   IDIVE clock divider enable.
 */
#if !defined(LPC_IDIVE_ENABLE) || defined(__DOXYGEN__)
#define LPC_IDIVE_ENABLE                FALSE
#endif

/**
 * @brief   IDIVE clock divider source.
 */
#if !defined(LPC_IDIVE_SRC) || defined(__DOXYGEN__)
#define LPC_IDIVE_SRC                   CLK_SEL_IRC
#endif

/**
 * @brief   IDIVE clock divider.
 */
#if !defined(LPC_IDIVE_DIV) || defined(__DOXYGEN__)
#define LPC_IDIVE_DIV                   1
#endif

/**
 * @brief   Base M4 clock enable.
 * @note    Must be always enabled.
 */
#if !defined(LPC_BASE_M4_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_M4_CLK_ENABLE          TRUE
#endif

/**
 * @brief   Base USB0 clock enable.
 */
#if !defined(LPC_BASE_USB0_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_USB0_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base peripheral clock enable.
 * @note    Base clock for Cortex-M0SUB subsystem, SGPIO.
 */
#if !defined(LPC_BASE_PERIPH_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_PERIPH_CLK_ENABLE      TRUE
#endif

/**
 * @brief   Base peripheral clock source.
 */
#if !defined(LPC_BASE_PERIPH_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_PERIPH_CLK_SRC         CLK_SEL_PLL1
#endif

/**
 * @brief   Base USB1 clock enable.
 */
#if !defined(LPC_BASE_USB1_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_USB1_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base USB1 clock source.
 */
#if !defined(LPC_BASE_USB1_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_USB1_CLK_SRC           CLK_SEL_PLL1
#endif

/**
 * @brief   Base SPIFI clock enable.
 */
#if !defined(LPC_BASE_SPIFI_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_SPIFI_CLK_ENABLE       TRUE
#endif

/**
 * @brief   Base SPIFI clock source.
 */
#if !defined(LPC_BASE_SPIFI_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_SPIFI_CLK_SRC          CLK_SEL_PLL1
#endif

/**
 * @brief   Base SPI clock enable.
 */
#if !defined(LPC_BASE_SPI_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_SPI_CLK_ENABLE         TRUE
#endif

/**
 * @brief   Base SPI clock source.
 */
#if !defined(LPC_BASE_SPI_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_SPI_CLK_SRC            CLK_SEL_IRC
#endif

/**
 * @brief   Base Ethernet PHY Receive clock enable.
 */
#if !defined(LPC_BASE_PHY_RX_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_PHY_RX_CLK_ENABLE      TRUE
#endif

/**
 * @brief   Base Ethernet PHY Receive clock source.
 */
#if !defined(LPC_BASE_PHY_RX_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_PHY_RX_CLK_SRC         CLK_SEL_IDIVC
#endif

/**
 * @brief   Base Ethernet PHY Transmit clock enable.
 */
#if !defined(LPC_BASE_PHY_TX_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_PHY_TX_CLK_ENABLE      TRUE
#endif

/**
 * @brief   Base Ethernet PHY Transmit clock source.
 */
#if !defined(LPC_BASE_PHY_TX_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_PHY_TX_CLK_SRC         CLK_SEL_IDIVC
#endif

/**
 * @brief   Base APB1 clock enable.
 * @note    Base clock for I2C0, I2S, CAN1.
 */
#if !defined(LPC_BASE_APB1_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_APB1_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base APB1 clock source.
 */
#if !defined(LPC_BASE_APB1_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_APB1_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base APB3 clock enable.
 * @note    Base clock for I2C1, DAC, ADC0, ADC1, CAN0.
 */
#if !defined(LPC_BASE_APB3_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_APB3_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base APB3 clock source.
 */
#if !defined(LPC_BASE_APB3_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_APB3_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base LCD clock enable.
 */
#if !defined(LPC_BASE_LCD_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_LCD_CLK_ENABLE         FALSE
#endif

/**
 * @brief   Base LCD clock source.
 */
#if !defined(LPC_BASE_LCD_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_LCD_CLK_SRC            CLK_SEL_IRC
#endif

/**
 * @brief   Base SDIO clock enable.
 */
#if !defined(LPC_BASE_SDIO_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_SDIO_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base SDIO clock source.
 */
#if !defined(LPC_BASE_SDIO_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_SDIO_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base SSP0 clock enable.
 */
#if !defined(LPC_BASE_SSP0_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_SSP0_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base SSP0 clock source.
 */
#if !defined(LPC_BASE_SSP0_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_SSP0_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base SSP1 clock enable.
 */
#if !defined(LPC_BASE_SSP1_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_SSP1_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base SSP1 clock source.
 */
#if !defined(LPC_BASE_SSP1_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_SSP1_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base UART0 clock enable.
 */
#if !defined(LPC_BASE_UART0_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_UART0_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base UART0 clock source.
 */
#if !defined(LPC_BASE_UART0_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_UART0_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base UART1 clock enable.
 */
#if !defined(LPC_BASE_UART1_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_UART1_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base UART1 clock source.
 */
#if !defined(LPC_BASE_UART1_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_UART1_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base UART2 clock enable.
 */
#if !defined(LPC_BASE_UART2_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_UART2_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base UART2 clock source.
 */
#if !defined(LPC_BASE_UART2_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_UART2_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base UART3 clock enable.
 */
#if !defined(LPC_BASE_UART3_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_UART3_CLK_ENABLE        TRUE
#endif

/**
 * @brief   Base UART3 clock source.
 */
#if !defined(LPC_BASE_UART3_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_UART3_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base OUT clock enable.
 */
#if !defined(LPC_BASE_OUT_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_OUT_CLK_ENABLE         FALSE
#endif

/**
 * @brief   Base OUT clock source.
 */
#if !defined(LPC_BASE_OUT_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_OUT_CLK_SRC            CLK_SEL_IRC
#endif

/**
 * @brief   Base APLL clock enable.
 */
#if !defined(LPC_BASE_APLL_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_APLL_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base APLL clock source.
 */
#if !defined(LPC_BASE_APLL_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_APLL_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base CGU OUT0 clock enable.
 */
#if !defined(LPC_BASE_CGU_OUT0_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_CGU_OUT0_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base CGU OUT0 clock source.
 */
#if !defined(LPC_BASE_CGU_OUT0_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_CGU_OUT0_CLK_SRC           CLK_SEL_IRC
#endif

/**
 * @brief   Base CGU OUT1 clock enable.
 */
#if !defined(LPC_BASE_CGU_OUT1_CLK_ENABLE) || defined(__DOXYGEN__)
#define LPC_BASE_CGU_OUT1_CLK_ENABLE        FALSE
#endif

/**
 * @brief   Base CGU OUT1 clock source.
 */
#if !defined(LPC_BASE_CGU_OUT1_CLK_SRC) || defined(__DOXYGEN__)
#define LPC_BASE_CGU_OUT1_CLK_SRC           CLK_SEL_IRC
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Calculated OSCRANGE setting.
 */
#if (XTAL < 18000000) || defined(__DOXYGEN__)
#define LPC_OSCRANGE        0
#else
#define LPC_OSCRANGE        1
#endif

/**
 * @brief   PLL1 FCLKIN frequency select.
 */
#if LPC_XTAL_ENABLE
#define LPC_PLL1_FCLKIN     XTAL
#else
#define LPC_PLL1_FCLKIN     IRCOSCCLK
#endif

/**
 * @brief   MSEL mask in PLL1_CTRL register.
 */
#if (((LPC_PLL1_MUL) >= 1) && ((LPC_PLL1_MUL) <= 256))|| defined(__DOXYGEN__)
#define LPC_PLL1_CTRL_MSEL ((LPC_PLL1_MUL) - 1)
#else
#error "Invalid LPC_PLL1_MULL value."
#endif

/**
 * @brief   NSEL mask in PLL1_CTRL register.
 */
#if (((LPC_PLL1_PREDIV) >= 1) && ((LPC_PLL1_PREDIV) <= 4)) || defined(__DOXYGEN__)
#define LPC_PLL1_CTRL_NSEL ((LPC_PLL1_PREDIV) - 1)
#else
#error "Invalid LPC_PLL1_PREDIV value (1 to 4 accepted)."
#endif

/**
 * @brief   PSEL mask in PLL1_CTRL register.
 */
#if ((LPC_PLL1_POSTDIV) == 2) || ((LPC_PLL1_POSTDIV) == 4) || ((LPC_PLL1_POSTDIV) == 8) ||\
    ((LPC_PLL1_POSTDIV) == 16) || defined(__DOXYGEN__)
#define LPC_PLL1_CTRL_PSEL (((LPC_PLL1_POSTDIV) / 2) - 1)
#else
#error "Invalid LPC_PLL1_POSTDIV  value (2, 4, 6, 8 accepted)."
#endif

/**
 * @brief   CCO frequency.
 */
#define  LPC_PLL1_CCO   (((LPC_PLL1_MUL) *    \
                              LPC_PLL1_FCLKIN)/(LPC_PLL1_PREDIV))

#if (LPC_PLL1_CCO < 156000000) || (LPC_PLL1_CCO > 320000000)
#error "CCO frequency out of the acceptable range (156...320)."
#endif

/**
 * @brief   PLL1 clock frequency.
 */
#if LPC_PLL1_ENABLE && LPC_PLL1_POSTDIV_ENABLE
#define  LPC_PLL1_CLK     (LPC_PLL1_CCO / (LPC_PLL1_POSTDIV))
#elif LPC_PLL1_ENABLE
#define  LPC_PLL1_CLK     LPC_PLL1_CCO
#endif

/**
 * @brief   CPU clock frequency.
 */
#if LPC_PLL1_ENABLE
#define  LPC_BASE_M4_CLK        LPC_PLL1_CLK
#define  LPC_BASE_M4_CLK_SRC    CLK_SEL_PLL1
#elif LPC_XTAL_ENABLE
#define  LPC_BASE_M4_CLK        XTAL
#define  LPC_BASE_M4_CLK_SRC    CLK_SEL_XTAL
#else
#define  LPC_BASE_M4_CLK        IRCOSCCLK
#define  LPC_BASE_M4_CLK_SRC    CLK_SEL_IRC
#endif

/**
 * @brief   CPU clock frequency range check.
 * @note 	Max clock for LPC43xx is 204 MHz.
 */
#if (LPC_BASE_M4_CLK  > 204000000) || defined(__DOXYGEN__)
#error "CPU Clock out of range."
#endif

#if (!LPC_FLASHLESS) || defined(__DOXYGEN__)
/**
 * @brief   Flash wait states.
 */
#if (LPC_BASE_M4_CLK <= 21000000) || defined(__DOXYGEN__)
#define LPC_FLASHCFG_FLASHTIM   0UL
#elif LPC_BASE_M4_CLK <= 43000000
#define LPC_FLASHCFG_FLASHTIM   1UL
#elif LPC_BASE_M4_CLK <= 64000000
#define LPC_FLASHCFG_FLASHTIM   2UL
#elif LPC_BASE_M4_CLK <= 86000000
#define LPC_FLASHCFG_FLASHTIM   3UL
#elif LPC_BASE_M4_CLK <= 107000000
#define LPC_FLASHCFG_FLASHTIM   4UL
#elif LPC_BASE_M4_CLK <= 129000000
#define LPC_FLASHCFG_FLASHTIM   5UL
#elif LPC_BASE_M4_CLK <= 150000000
#define LPC_FLASHCFG_FLASHTIM   6UL
#elif LPC_BASE_M4_CLK <= 172000000
#define LPC_FLASHCFG_FLASHTIM   7UL
#elif LPC_BASE_M4_CLK <= 193000000
#define LPC_FLASHCFG_FLASHTIM   8UL
#else
#define LPC_FLASHCFG_FLASHTIM   9UL
#endif
#endif

/**
 * @brief   CPU clock frequency.
 */
#if LPC_IDIVA_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_IDIVA_CLK     RTC_XTAL/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IRC
#define  LPC_IDIVA_CLK     IRCOSCCLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_IDIVA_CLK     ENET_RX_CLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_IDIVA_CLK     ENET_TX_CLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_IDIVA_CLK     GP_CLKIN_CLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_XTAL
#define  LPC_IDIVA_CLK     XTAL/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_IDIVA_CLK source can't be PLL0USB."
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_IDIVA_CLK     LPC_PLL0AUDIO_CLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_PLL1
#define  LPC_IDIVA_CLK     LPC_PLL1_CLK/LPC_IDIVA_DIV
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IDIVA
#error   "LPC_IDIVA_CLK source can't be IDIVA_CLK."
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IDIVB
#error   "LPC_IDIVA_CLK source can't be IDIVB_CLK."
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IDIVC
#error   "LPC_IDIVA_CLK source can't be IDIVC_CLK."
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IDIVD
#error   "LPC_IDIVA_CLK source can't be IDIVD_CLK."
#elif LPC_IDIVA_CLK_SRC == CLK_SEL_IDIVE
#error   "LPC_IDIVA_CLK source can't be IDIVE_CLK."
#else
#error  "LPC_IDIVA_CLK wrong clock source."
#endif

#if LPC_IDIVB_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_IDIVB_CLK     RTC_XTAL/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IRC
#define  LPC_IDIVB_CLK     IRCOSCCLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_IDIVB_CLK     ENET_RX_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_IDIVB_CLK     ENET_TX_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_IDIVB_CLK     GP_CLKIN_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_XTAL
#define  LPC_IDIVB_CLK     XTAL/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_IDIVB_CLK source can't be PLL0USB."
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_IDIVB_CLK     LPC_PLL0AUDIO_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_PLL1
#define  LPC_IDIVB_CLK     LPC_PLL1_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_IDIVB_CLK     LPC_IDIVA_CLK/LPC_IDIVB_DIV
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IDIVB
#error   "LPC_IDIVB_CLK source can't be IDIVB_CLK."
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IDIVC
#error   "LPC_IDIVB_CLK source can't be IDIVC_CLK."
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IDIVD
#error   "LPC_IDIVB_CLK source can't be IDIVD_CLK."
#elif LPC_IDIVB_CLK_SRC == CLK_SEL_IDIVE
#error   "LPC_IDIVB_CLK source can't be IDIVE_CLK."
#else
#error  "LPC_IDIVB_CLK wrong clock source."
#endif

#if LPC_IDIVC_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_IDIVC_CLK     RTC_XTAL/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IRC
#define  LPC_IDIVC_CLK     IRCOSCCLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_IDIVC_CLK     ENET_RX_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_IDIVC_CLK     ENET_TX_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_IDIVC_CLK     GP_CLKIN_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_XTAL
#define  LPC_IDIVC_CLK     XTAL/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_IDIVC_CLK source can't be PLL0USB."
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_IDIVC_CLK     LPC_PLL0AUDIO_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_PLL1
#define  LPC_IDIVC_CLK     LPC_PLL1_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_IDIVC_CLK     LPC_IDIVA_CLK/LPC_IDIVC_DIV
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IDIVB
#error   "LPC_IDIVC_CLK source can't be IDIVB_CLK."
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IDIVC
#error   "LPC_IDIVC_CLK source can't be IDIVC_CLK."
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IDIVD
#error   "LPC_IDIVC_CLK source can't be IDIVD_CLK."
#elif LPC_IDIVC_CLK_SRC == CLK_SEL_IDIVE
#error   "LPC_IDIVC_CLK source can't be IDIVE_CLK."
#else
#error  "LPC_IDIVC_CLK wrong clock source."
#endif

#if LPC_IDIVD_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_IDIVD_CLK     RTC_XTAL/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IRC
#define  LPC_IDIVD_CLK     IRCOSCCLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_IDIVD_CLK     ENET_RX_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_IDIVD_CLK     ENET_TX_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_IDIVD_CLK     GP_CLKIN_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_XTAL
#define  LPC_IDIVD_CLK     XTAL/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_IDIVD_CLK source can't be PLL0USB."
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_IDIVD_CLK     LPC_PLL0AUDIO_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_PLL1
#define  LPC_IDIVD_CLK     LPC_PLL1_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_IDIVD_CLK     LPC_IDIVA_CLK/LPC_IDIVD_DIV
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IDIVB
#error   "LPC_IDIVD_CLK source can't be IDIVB_CLK."
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IDIVC
#error   "LPC_IDIVD_CLK source can't be IDIVC_CLK."
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IDIVD
#error   "LPC_IDIVD_CLK source can't be IDIVD_CLK."
#elif LPC_IDIVD_CLK_SRC == CLK_SEL_IDIVE
#error   "LPC_IDIVD_CLK source can't be IDIVE_CLK."
#else
#error  "LPC_IDIVD_CLK wrong clock source."
#endif

#if LPC_IDIVE_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_IDIVE_CLK     RTC_XTAL/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IRC
#define  LPC_IDIVE_CLK     IRCOSCCLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_IDIVE_CLK     ENET_RX_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_IDIVE_CLK     ENET_TX_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_IDIVE_CLK     GP_CLKIN_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_XTAL
#define  LPC_IDIVE_CLK     XTAL/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_IDIVE_CLK source can't be PLL0USB."
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_IDIVE_CLK     LPC_PLL0AUDIO_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_PLL1
#define  LPC_IDIVE_CLK     LPC_PLL1_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_IDIVE_CLK     LPC_IDIVA_CLK/LPC_IDIVE_DIV
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IDIVB
#error   "LPC_IDIVE_CLK source can't be IDIVB_CLK."
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IDIVC
#error   "LPC_IDIVE_CLK source can't be IDIVC_CLK."
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IDIVD
#error   "LPC_IDIVE_CLK source can't be IDIVD_CLK."
#elif LPC_IDIVE_CLK_SRC == CLK_SEL_IDIVE
#error   "LPC_IDIVE_CLK source can't be IDIVE_CLK."
#else
#error  "LPC_IDIVE_CLK wrong clock source."
#endif

/**
 * @brief   Peripheral clocks frequency.
 */
#if LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_PERIPH_CLK     RTC_XTAL
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_PERIPH_CLK     IRCOSCCLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_PERIPH_CLK     ENET_RX_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_PERIPH_CLK     ENET_TX_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_PERIPH_CLK     GP_CLKIN_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_PERIPH_CLK     XTAL
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_PERIPH_CLK source can't be PLL0USB."
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_PERIPH_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_PERIPH_CLK     LPC_PLL1_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_PERIPH_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_PERIPH_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_PERIPH_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_PERIPH_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_PERIPH_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_PERIPH_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_PERIPH_CLK wrong clock source."
#endif

#if LPC_BASE_USB1_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_USB1_CLK     RTC_XTAL
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_USB1_CLK     IRCOSCCLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_USB1_CLK     ENET_RX_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_USB1_CLK     ENET_TX_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_USB1_CLK     GP_CLKIN_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_USB1_CLK     XTAL
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_PLL0USB
#define  LPC_BASE_USB1_CLK     LPC_PLL0USB_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_USB1_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_USB1_CLK     LPC_PLL1_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_USB1_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_USB1_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_USB1_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_USB1_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_USB1_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_USB1_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_USB1_CLK wrong clock source."
#endif

#if LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_SPIFI_CLK     RTC_XTAL
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_SPIFI_CLK     IRCOSCCLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_SPIFI_CLK     ENET_RX_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_SPIFI_CLK     ENET_TX_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_SPIFI_CLK     GP_CLKIN_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_SPIFI_CLK     XTAL
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_SPIFI_CLK source can't be PLL0USB."
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_SPIFI_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_SPIFI_CLK     LPC_PLL1_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_SPIFI_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_SPIFI_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_SPIFI_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_SPIFI_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_SPIFI_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_SPIFI_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_SPIFI_CLK wrong clock source."
#endif

#if LPC_BASE_SPI_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_SPI_CLK     RTC_XTAL
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_SPI_CLK     IRCOSCCLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_SPI_CLK     ENET_RX_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_SPI_CLK     ENET_TX_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_SPI_CLK     GP_CLKIN_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_SPI_CLK     XTAL
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_SPI_CLK source can't be PLL0USB."
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_SPI_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_SPI_CLK     LPC_PLL1_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_SPI_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_SPI_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_SPI_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_SPI_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_SPI_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_SPI_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_SPI_CLK wrong clock source."
#endif

#if LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_PHY_RX_CLK     RTC_XTAL
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_PHY_RX_CLK     IRCOSCCLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_PHY_RX_CLK     ENET_RX_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_PHY_RX_CLK     ENET_TX_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_PHY_RX_CLK     GP_CLKIN_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_PHY_RX_CLK     XTAL
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_PHY_RX_CLK source can't be PLL0USB."
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_PHY_RX_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_PHY_RX_CLK     LPC_PLL1_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_PHY_RX_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_PHY_RX_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_PHY_RX_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_PHY_RX_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_PHY_RX_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_PHY_RX_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_PHY_RX_CLK wrong clock source."
#endif

#if LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_PHY_TX_CLK     RTC_XTAL
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_PHY_TX_CLK     IRCOSCCLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_PHY_TX_CLK     ENET_RX_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_PHY_TX_CLK     ENET_TX_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_PHY_TX_CLK     GP_CLKIN_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_PHY_TX_CLK     XTAL
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_PHY_TX_CLK source can't be PLL0USB."
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_PHY_TX_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_PHY_TX_CLK     LPC_PLL1_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_PHY_TX_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_PHY_TX_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_PHY_TX_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_PHY_TX_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_PHY_TX_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_PHY_TX_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_PHY_TX_CLK wrong clock source."
#endif

#if LPC_BASE_APB1_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_APB1_CLK     RTC_XTAL
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_APB1_CLK     IRCOSCCLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_APB1_CLK     ENET_RX_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_APB1_CLK     ENET_TX_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_APB1_CLK     GP_CLKIN_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_APB1_CLK     XTAL
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_APB1_CLK source can't be PLL0USB."
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_APB1_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_APB1_CLK     LPC_PLL1_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_APB1_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_APB1_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_APB1_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_APB1_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_APB1_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_APB1_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_APB1_CLK wrong clock source."
#endif

#if LPC_BASE_APB3_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_APB3_CLK     RTC_XTAL
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_APB3_CLK     IRCOSCCLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_APB3_CLK     ENET_RX_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_APB3_CLK     ENET_TX_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_APB3_CLK     GP_CLKIN_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_APB3_CLK     XTAL
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_APB3_CLK source can't be PLL0USB."
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_APB3_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_APB3_CLK     LPC_PLL1_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_APB3_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_APB3_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_APB3_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_APB3_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_APB3_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_APB3_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_APB3_CLK wrong clock source."
#endif

#if LPC_BASE_LCD_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_LCD_CLK     RTC_XTAL
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_LCD_CLK     IRCOSCCLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_LCD_CLK     ENET_RX_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_LCD_CLK     ENET_TX_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_LCD_CLK     GP_CLKIN_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_LCD_CLK     XTAL
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_LCD_CLK source can't be PLL0USB."
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_LCD_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_LCD_CLK     LPC_PLL1_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_LCD_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_LCD_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_LCD_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_LCD_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_LCD_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_LCD_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_LCD_CLK wrong clock source."
#endif

#if LPC_BASE_SDIO_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_SDIO_CLK     RTC_XTAL
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_SDIO_CLK     IRCOSCCLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_SDIO_CLK     ENET_RX_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_SDIO_CLK     ENET_TX_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_SDIO_CLK     GP_CLKIN_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_SDIO_CLK     XTAL
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_SDIO_CLK source can't be PLL0USB."
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_SDIO_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_SDIO_CLK     LPC_PLL1_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_SDIO_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_SDIO_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_SDIO_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_SDIO_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_SDIO_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_SDIO_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_SDIO_CLK wrong clock source."
#endif

#if LPC_BASE_SSP0_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_SSP0_CLK     RTC_XTAL
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_SSP0_CLK     IRCOSCCLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_SSP0_CLK     ENET_RX_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_SSP0_CLK     ENET_TX_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_SSP0_CLK     GP_CLKIN_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_SSP0_CLK     XTAL
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_SSP0_CLK source can't be PLL0USB."
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_SSP0_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_SSP0_CLK     LPC_PLL1_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_SSP0_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_SSP0_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_SSP0_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_SSP0_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_SSP0_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_SSP0_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_SSP0_CLK wrong clock source."
#endif

#if LPC_BASE_SSP1_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_SSP1_CLK     RTC_XTAL
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_SSP1_CLK     IRCOSCCLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_SSP1_CLK     ENET_RX_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_SSP1_CLK     ENET_TX_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_SSP1_CLK     GP_CLKIN_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_SSP1_CLK     XTAL
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_SSP1_CLK source can't be PLL0USB."
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_SSP1_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_SSP1_CLK     LPC_PLL1_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_SSP1_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_SSP1_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_SSP1_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_SSP1_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_SSP1_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_SSP1_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_SSP1_CLK wrong clock source."
#endif

#if LPC_BASE_UART0_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_UART0_CLK     RTC_XTAL
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_UART0_CLK     IRCOSCCLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_UART0_CLK     ENET_RX_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_UART0_CLK     ENET_TX_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_UART0_CLK     GP_CLKIN_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_UART0_CLK     XTAL
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_UART0_CLK source can't be PLL0USB."
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_UART0_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_UART0_CLK     LPC_PLL1_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_UART0_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_UART0_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_UART0_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_UART0_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_UART0_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_UART0_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_UART0_CLK wrong clock source."
#endif

#if LPC_BASE_UART1_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_UART1_CLK     RTC_XTAL
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_UART1_CLK     IRCOSCCLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_UART1_CLK     ENET_RX_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_UART1_CLK     ENET_TX_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_UART1_CLK     GP_CLKIN_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_UART1_CLK     XTAL
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_UART1_CLK source can't be PLL0USB."
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_UART1_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_UART1_CLK     LPC_PLL1_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_UART1_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_UART1_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_UART1_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_UART1_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_UART1_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_UART1_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_UART1_CLK wrong clock source."
#endif

#if LPC_BASE_UART2_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_UART2_CLK     RTC_XTAL
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_UART2_CLK     IRCOSCCLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_UART2_CLK     ENET_RX_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_UART2_CLK     ENET_TX_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_UART2_CLK     GP_CLKIN_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_UART2_CLK     XTAL
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_UART2_CLK source can't be PLL0USB."
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_UART2_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_UART2_CLK     LPC_PLL1_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_UART2_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_UART2_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_UART2_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_UART2_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_UART2_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_UART2_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_UART2_CLK wrong clock source."
#endif

#if LPC_BASE_UART3_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_UART3_CLK     RTC_XTAL
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_UART3_CLK     IRCOSCCLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_UART3_CLK     ENET_RX_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_UART3_CLK     ENET_TX_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_UART3_CLK     GP_CLKIN_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_UART3_CLK     XTAL
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_UART3_CLK source can't be PLL0USB."
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_UART3_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_UART3_CLK     LPC_PLL1_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_UART3_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_UART3_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_UART3_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_UART3_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_UART3_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_UART3_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_UART3_CLK wrong clock source."
#endif

#if LPC_BASE_OUT_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_OUT_CLK     RTC_XTAL
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_OUT_CLK     IRCOSCCLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_OUT_CLK     ENET_RX_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_OUT_CLK     ENET_TX_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_OUT_CLK     GP_CLKIN_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_OUT_CLK     XTAL
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_OUT_CLK source can't be PLL0USB."
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_OUT_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_OUT_CLK     LPC_PLL1_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_OUT_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_OUT_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_OUT_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_OUT_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_OUT_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_OUT_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_OUT_CLK wrong clock source."
#endif

#if LPC_BASE_APLL_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_APLL_CLK     RTC_XTAL
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_APLL_CLK     IRCOSCCLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_APLL_CLK     ENET_RX_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_APLL_CLK     ENET_TX_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_APLL_CLK     GP_CLKIN_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_APLL_CLK     XTAL
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_APLL_CLK source can't be PLL0USB."
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_APLL_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_APLL_CLK     LPC_PLL1_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_APLL_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_APLL_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_APLL_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_APLL_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_APLL_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_APLL_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_APLL_CLK wrong clock source."
#endif

#if LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_CGU_OUT0_CLK     RTC_XTAL
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_CGU_OUT0_CLK     IRCOSCCLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_CGU_OUT0_CLK     ENET_RX_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_CGU_OUT0_CLK     ENET_TX_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_CGU_OUT0_CLK     GP_CLKIN_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_CGU_OUT0_CLK     XTAL
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_CGU_OUT0_CLK source can't be PLL0USB."
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_CGU_OUT0_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_CGU_OUT0_CLK     LPC_PLL1_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_CGU_OUT0_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_CGU_OUT0_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_CGU_OUT0_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_CGU_OUT0_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_CGU_OUT0_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_CGU_OUT0_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_CGU_OUT0_CLK wrong clock source."
#endif

#if LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_32KHZ
#define  LPC_BASE_CGU_OUT1_CLK     RTC_XTAL
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IRC
#define  LPC_BASE_CGU_OUT1_CLK     IRCOSCCLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_ENET_RX_CLK
#define  LPC_BASE_CGU_OUT1_CLK     ENET_RX_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_ENET_TX_CLK
#define  LPC_BASE_CGU_OUT1_CLK     ENET_TX_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_GP_CLKIN
#define  LPC_BASE_CGU_OUT1_CLK     GP_CLKIN_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_XTAL
#define  LPC_BASE_CGU_OUT1_CLK     XTAL
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_PLL0USB
#error   "LPC_BASE_CGU_OUT1_CLK source can't be PLL0USB."
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_PLL0AUDIO
#define  LPC_BASE_CGU_OUT1_CLK     LPC_PLL0AUDIO_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_PLL1
#define  LPC_BASE_CGU_OUT1_CLK     LPC_PLL1_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IDIVA
#define  LPC_BASE_CGU_OUT1_CLK     LPC_IDIVA_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IDIVB
#define  LPC_BASE_CGU_OUT1_CLK     LPC_IDIVB_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IDIVC
#define  LPC_BASE_CGU_OUT1_CLK     LPC_IDIVC_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IDIVD
#define  LPC_BASE_CGU_OUT1_CLK     LPC_IDIVD_CLK
#elif LPC_BASE_CGU_OUT1_CLK_SRC == CLK_SEL_IDIVE
#define  LPC_BASE_CGU_OUT1_CLK     LPC_IDIVE_CLK
#else
#error  "LPC_BASE_CGU_OUT1_CLK wrong clock source."
#endif

#define  LPC_CLK_M4_ETHERNET    LPC_BASE_M4_CLK
/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of the realtime free counter value.
 */
typedef uint32_t halrtcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This service is implemented by returning the content of the
 *          DWT_CYCCNT register.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_value()         DWT_CYCCNT

/**
 * @brief   Realtime counter frequency.
 * @note    The DWT_CYCCNT register is incremented directly by the cpu
 *          clock so this function returns LPC_BASE_M4_CLK.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_frequency()     LPC_BASE_M4_CLK

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#include "lpc43xx_dma.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void lpc_clock_init(void);
  void lpc_deep_sleep_enter(uint32_t core_type);
  void lpc_deep_sleep_exit(void);
  void lpc_deep_power_down_enter(uint32_t core_type);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
