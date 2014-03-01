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

/*
 * LPC43xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 *
 * IRQ priorities:
 * 7...0        Lowest...highest.
 *
 * LPC base clock sources:
 *  0x00 32 kHz oscillator
 *  0x01 IRC (default)
 *  0x02 ENET_RX_CLK
 *  0x03 ENET_TX_CLK
 *  0x04 GP_CLKIN
 *  0x06 Crystal oscillator
 *  0x07 PLL0USB
 *  0x08 PLL0AUDIO
 *  0x09 PLL1
 *  0x0C IDIVA
 *  0x0D IDIVB
 *  0x0E IDIVC
 *  0x0F IDIVD
 *  0x10 IDIVE
 */

/*
 * HAL driver system settings.
 */
#define LPC_XTAL_ENABLE                 TRUE
#define LPC_PLL1_ENABLE                 TRUE
#define LPC_PLL1_MUL                    17
#define LPC_PLL1_PREDIV                 1
#define LPC_PLL1_POSTDIV_ENABLE         TRUE
#define LPC_PLL1_POSTDIV                2
#define LPC_PLL0USB_ENABLE              FALSE
#define LPC_PLL0AUDIO_ENABLE            FALSE
#define LPC_HAS_FLASH                   FALSE

/* Clock dividers */
#define LPC_IDIVA_ENABLE                FALSE
#define LPC_IDIVA_DIV                   1
#define LPC_IDIVA_SRC                   0x09
#define LPC_IDIVB_ENABLE                TRUE
#define LPC_IDIVB_DIV                   2
#define LPC_IDIVB_SRC                   0x09
#define LPC_IDIVC_ENABLE                TRUE
#define LPC_IDIVC_DIV                   3
#define LPC_IDIVC_SRC                   0x09
#define LPC_IDIVD_ENABLE                FALSE
#define LPC_IDIVD_DIV                   1
#define LPC_IDIVD_SRC                   0x09
#define LPC_IDIVE_ENABLE                FALSE
#define LPC_IDIVE_DIV                   1
#define LPC_IDIVE_SRC                   0x09

/* Base clocks */
#define LPC_BASE_USB0_CLK_ENABLE        FALSE   /* Base clock for USB0. */
#define LPC_BASE_PERIPH_CLK_ENABLE      TRUE   /* Base clock for Cortex-M0SUB subsystem, SGPIO. */
#define LPC_BASE_PERIPH_CLK_SRC         0x09
#define LPC_BASE_USB1_CLK_ENABLE        TRUE    /* Base clock for USB1. */
#define LPC_BASE_USB1_CLK_SRC           0x01
#define LPC_BASE_SPIFI_CLK_ENABLE       TRUE    /* Base clock for SPIFI. */
#define LPC_BASE_SPIFI_CLK_SRC          0x0D
#define LPC_BASE_SPI_CLK_ENABLE         TRUE    /* Base clock for SPI. */
#define LPC_BASE_SPI_CLK_SRC            0x01
#define LPC_BASE_PHY_RX_CLK_ENABLE      TRUE    /* Base clock for Ethernet PHY Receive clock. */
#define LPC_BASE_PHY_RX_CLK_SRC         0x0E
#define LPC_BASE_PHY_TX_CLK_ENABLE      TRUE    /* Base clock for Ethernet PHY Transmit clock. */
#define LPC_BASE_PHY_TX_CLK_SRC         0x0E
#define LPC_BASE_APB1_CLK_ENABLE        TRUE    /* Base clock for APB1: I2C0, I2S, CAN1. */
#define LPC_BASE_APB1_CLK_SRC           0x01
#define LPC_BASE_APB3_CLK_ENABLE        TRUE    /* Base clock for APB3: I2C1, DAC, ADC0, ADC1, CAN0. */
#define LPC_BASE_APB3_CLK_SRC           0x01
#define LPC_BASE_LCD_CLK_ENABLE         FALSE   /* Base clock for LCD. */
#define LPC_BASE_LCD_CLK_SRC            0x01
#define LPC_BASE_SDIO_CLK_ENABLE        FALSE   /* Base clock for SD/MMC. */
#define LPC_BASE_SDIO_CLK_SRC           0x01
#define LPC_BASE_SSP0_CLK_ENABLE        TRUE    /* Base clock for SSP0. */
#define LPC_BASE_SSP0_CLK_SRC           0x01
#define LPC_BASE_SSP1_CLK_ENABLE        TRUE    /* Base clock for SSP1. */
#define LPC_BASE_SSP1_CLK_SRC           0x01
#define LPC_BASE_UART0_CLK_ENABLE       TRUE    /* Base clock for UART0. */
#define LPC_BASE_UART0_CLK_SRC          0x01
#define LPC_BASE_UART1_CLK_ENABLE       TRUE    /* Base clock for UART1. */
#define LPC_BASE_UART1_CLK_SRC          0x01
#define LPC_BASE_UART2_CLK_ENABLE       TRUE    /* Base clock for UART2. */
#define LPC_BASE_UART2_CLK_SRC          0x01
#define LPC_BASE_UART3_CLK_ENABLE       TRUE    /* Base clock for UART3. */
#define LPC_BASE_UART3_CLK_SRC          0x01
#define LPC_BASE_OUT_CLK_ENABLE         FALSE   /* Base clock for UART0. */
#define LPC_BASE_OUT_CLK_SRC            0x01
#define LPC_BASE_APLL_CLK_ENABLE        FALSE   /* Base clock for audio system.  */
#define LPC_BASE_APLL_CLK_SRC           0x01
#define LPC_BASE_CGU_OUT0_CLK_ENABLE    FALSE   /* Base clock for CGU_OUT0 clock output. */
#define LPC_BASE_CGU_OUT0_CLK_SRC       0x01
#define LPC_BASE_CGU_OUT1_CLK_ENABLE    FALSE   /* Base clock for CGU_OUT1 clock output. */
#define LPC_BASE_CGU_OUT1_CLK_SRC       0x01

/*
 * GPT driver system settings.
 */
#define LPC_GPT_USE_TIM0                FALSE
#define LPC_GPT_USE_TIM1                FALSE
#define LPC_GPT_USE_TIM2                FALSE
#define LPC_GPT_USE_TIM3                FALSE
#define LPC_GPT_TIM0_IRQ_PRIORITY       2
#define LPC_GPT_TIM1_IRQ_PRIORITY       2
#define LPC_GPT_TIM2_IRQ_PRIORITY       2
#define LPC_GPT_TIM3_IRQ_PRIORITY       2

/*
 * SERIAL driver system settings.
 */
#define LPC_SERIAL_USE_UART0            FALSE
#define LPC_SERIAL_USE_UART1            FALSE
#define LPC_SERIAL_USE_UART2            FALSE
#define LPC_SERIAL_USE_UART3            TRUE
#define LPC_SERIAL_FIFO_PRELOAD         16
#define LPC_SERIAL_UART0_IRQ_PRIORITY   3
#define LPC_SERIAL_UART1_IRQ_PRIORITY   3
#define LPC_SERIAL_UART2_IRQ_PRIORITY   3
#define LPC_SERIAL_UART3_IRQ_PRIORITY   3

/*
 * I2C driver system settings.
 */
#define LPC_I2C_USE_I2C0                FALSE
#define LPC_I2C_USE_I2C1                FALSE
#define LPC_I2C_I2C0_IRQ_PRIORITY       3
#define LPC_I2C_I2C1_IRQ_PRIORITY       3

/*
 * SPI driver system settings.
 */
#define LPC_SPI_USE_SSP0                FALSE
#define LPC_SPI_USE_SSP1                FALSE
#define LPC_SPI_SSP0_IRQ_PRIORITY       5
#define LPC_SPI_SSP1_IRQ_PRIORITY       5

/*
 * RTC driver system settings.
 */
#define LPC_RTC_IS_CALENDAR             TRUE
#define LPC_RTC_USE_ALARM               TRUE
#define LPC_RTC_IRQ_PRIORITY            3

/*
 * MAC driver system settings.
 */
#define LPC_MAC_TRANSMIT_BUFFERS        2
#define LPC_MAC_RECEIVE_BUFFERS         4
#define LPC_MAC_BUFFERS_SIZE            1522
#define LPC_MAC_PHY_TIMEOUT             100
#define LPC_MAC_ETH1_CHANGE_PHY_STATE   TRUE
#define LPC_MAC_ETH1_IRQ_PRIORITY       13
