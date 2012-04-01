/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Olimex STM32-P407 board.
 * NOTE: Part of JTAG signals are used for other functions, this board can be
 *       used using SWD only.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_STM32_P407
#define BOARD_NAME              "Olimex STM32-P407"

/*
 * Board frequencies.
 * NOTE: The LSE crystal is not fitted by default on the board.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            25000000

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD               330

/*
 * MCU type as defined in the ST header file stm32f4xx.h.
 */
#define STM32F4XX

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_WKUP       0
#define GPIOA_ETH_RMII_REF_CLK  1
#define GPIOA_ETH_RMII_MDIO     2
#define GPIOA_ETH_RMII_MDINT    3
#define GPIOA_DCMI_HSYNC        4
#define GPIOA_USELESS5          5
#define GPIOA_DCMI_PIXCLK       6
#define GPIOA_ETH_RMII_CRS_DV   7
#define GPIOA_MCO1              8
#define GPIOA_OTG_FS_VBUS       9
#define GPIOA_DCMI_D1           10
#define GPIOA_OTG_FS_DM         11
#define GPIOA_OTG_FS_DP         12
#define GPIOA_SWDIO             13
#define GPIOA_SWCLK             14
#define GPIOA_I2S3_WS           15

#define GPIOB_LCD_BL            0
#define GPIOB_BUZ               1
#define GPIOB_CAM_ENB           2
#define GPIOB_I2S3_CK           3
#define GPIOB_USELESS4          4
#define GPIOB_I2S3_SD           5
#define GPIOB_DCMI_D5           6
#define GPIOB_DCMI_VSYNC        7
#define GPIOB_CAN1_RX           8
#define GPIOB_CAN1_TX           9
#define GPIOB_USB_FS_FAULT      10
#define GPIOB_ETH_RMII_TX_EN    11
#define GPIOB_OTG_HS_ID         12
#define GPIOB_OTG_HS_VBUS       13
#define GPIOB_OTG_HS_DM         14
#define GPIOB_OTG_HS_DP         15

#define GPIOC_TRIM              0
#define GPIOC_ETH_RMII_MDC      1
#define GPIOC_USB_FS_VBUSON     2
#define GPIOC_USELESS3          3
#define GPIOC_ETH_RMII_RXD0     4
#define GPIOC_ETH_RMII_RXD1     5
#define GPIOC_USELESS6          6
#define GPIOC_I2S3_MCK          7
#define GPIOC_DCMI_D2           8
#define GPIOC_DCMI_D3           9
#define GPIOC_SPI3_SCK          10
#define GPIOC_SPI3_MISO         11
#define GPIOC_SPI3_MOSI         12
#define GPIOC_BUTTON_TAMPER     13
#define GPIOC_OSC32_IN          14
#define GPIOC_OSC32_OUT         15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * Port A setup.
 * All input with pull-up except:
 * PA0  - GPIOA_BUTTON_WKUP     (input floating).
 * PA1  - GPIOA_ETH_RMII_REF_CLK(alternate 11).
 * PA2  - GPIOA_ETH_RMII_MDIO   (alternate 11).
 * PA3  - GPIOA_ETH_RMII_MDINT  (input floating).
 * PA4  - GPIOA_DCMI_HSYNC      (input pull-up).
 * PA5  - GPIOA_USELESS5        (input pull-up).
 * PA6  - GPIOA_DCMI_PIXCLK     (input pull-up).
 * PA7  - GPIOA_ETH_RMII_CRS_DV (alternate 11).
 * PA8  - GPIOA_MCO1            (alternate 0).
 * PA9  - GPIOA_OTG_FS_VBUS     (input pull-up).
 * PA10 - GPIOA_DCMI_D1         (input pull-up).
 * PA11 - GPIOA_OTG_FS_DM       (alternate 10).
 * PA12 - GPIOA_OTG_FS_DP       (alternate 10).
 * PA13 - GPIOA_SWDIO           (alternate 0).
 * PA14 - GPIOA_SWCLK           (alternate 0, pull-down).
 * PA15 - GPIOA_I2S3_WS         (alternate 6).
 */
#define VAL_GPIOA_MODER     (PIN_MODE_INPUT(GPIOA_BUTTON_WKUP) |            \
                             PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_REF_CLK) |   \
                             PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_MDIO) |      \
                             PIN_MODE_INPUT(GPIOA_ETH_RMII_MDINT) |         \
                             PIN_MODE_INPUT(GPIOA_DCMI_HSYNC) |             \
                             PIN_MODE_INPUT(GPIOA_USELESS5) |               \
                             PIN_MODE_INPUT(GPIOA_DCMI_PIXCLK) |            \
                             PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_CRS_DV) |    \
                             PIN_MODE_ALTERNATE(GPIOA_MCO1) |               \
                             PIN_MODE_INPUT(GPIOA_OTG_FS_VBUS) |            \
                             PIN_MODE_INPUT(GPIOA_DCMI_D1) |                \
                             PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |          \
                             PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |          \
                             PIN_MODE_ALTERNATE(GPIOA_SWDIO) |              \
                             PIN_MODE_ALTERNATE(GPIOA_SWCLK) |              \
                             PIN_MODE_ALTERNATE(GPIOA_I2S3_WS))
#define VAL_GPIOA_OTYPER    0x00000000
#define VAL_GPIOA_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOA_PUPDR     (PIN_PUDR_PULLUP(GPIOA_DCMI_HSYNC) |            \
                             PIN_PUDR_PULLUP(GPIOA_USELESS5) |              \
                             PIN_PUDR_PULLUP(GPIOA_DCMI_PIXCLK) |           \
                             PIN_PUDR_PULLUP(GPIOA_OTG_FS_VBUS) |           \
                             PIN_PUDR_PULLUP(GPIOA_DCMI_D1) |               \
                             PIN_PUDR_PULLDOWN(GPIOA_SWCLK))
#define VAL_GPIOA_ODR       0xFFFFFFFF
#define VAL_GPIOA_AFRL      (PIN_AFIO_AF(GPIOA_ETH_RMII_REF_CLK, 11) |      \
                             PIN_AFIO_AF(GPIOA_ETH_RMII_MDIO, 11) |         \
                             PIN_AFIO_AF(GPIOA_ETH_RMII_CRS_DV, 11))
#define VAL_GPIOA_AFRH      (PIN_AFIO_AF(GPIOA_MCO1, 0) |                   \
                             PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |             \
                             PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |             \
                             PIN_AFIO_AF(GPIOA_SWDIO, 0) |                  \
                             PIN_AFIO_AF(GPIOA_SWCLK, 0) |                  \
                             PIN_AFIO_AF(GPIOA_I2S3_WS, 6))

/*
 * Port B setup.
 * All input with pull-up except:
 * PB0  - GPIOB_LCD_BL          (output push-pull).
 * PB1  - GPIOB_BUZ             (output push-pull).
 * PB2  - GPIOB_CAM_ENB         (input floating).
 * PB3  - GPIOB_I2S3_CK         (alternate 6).
 * PB4  - GPIOB_USELESS4        (input pull-up).
 * PB5  - GPIOB_I2S3_SD         (alternate 6).
 * PB6  - GPIOB_DCMI_D5         (input pull-up).
 * PB7  - GPIOB_DCMI_VSYNC      (input pull-up).
 * PB8  - GPIOB_CAN1_RX         (alternate 9).
 * PB9  - GPIOB_CAN1_TX         (alternate 9).
 * PB10 - GPIOB_USB_FS_FAULT    (input floating).
 * PB11 - GPIOB_ETH_RMII_TX_EN  (alternate 11).
 * PB12 - GPIOB_OTG_HS_ID       (alternate 12).
 * PB13 - GPIOB_OTG_HS_VBUS     (input pull-up).
 * PB14 - GPIOB_OTG_HS_DM       (alternate 12).
 * PB15 - GPIOB_OTG_HS_DP       (alternate 12).
 */
#define VAL_GPIOB_MODER     (PIN_MODE_OUTPUT(GPIOB_LCD_BL) |                \
                             PIN_MODE_OUTPUT(GPIOB_BUZ) |                   \
                             PIN_MODE_INPUT(GPIOB_CAM_ENB) |                \
                             PIN_MODE_ALTERNATE(GPIOB_I2S3_CK) |            \
                             PIN_MODE_INPUT(GPIOB_USELESS4) |               \
                             PIN_MODE_ALTERNATE(GPIOB_I2S3_SD) |            \
                             PIN_MODE_INPUT(GPIOB_DCMI_D5) |                \
                             PIN_MODE_INPUT(GPIOB_DCMI_VSYNC) |             \
                             PIN_MODE_ALTERNATE(GPIOB_CAN1_RX) |            \
                             PIN_MODE_ALTERNATE(GPIOB_CAN1_TX) |            \
                             PIN_MODE_INPUT(GPIOB_USB_FS_FAULT) |           \
                             PIN_MODE_ALTERNATE(GPIOB_ETH_RMII_TX_EN) |     \
                             PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ID) |          \
                             PIN_MODE_INPUT(GPIOB_OTG_HS_VBUS) |            \
                             PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DM) |          \
                             PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OTYPER    0x00000000
#define VAL_GPIOB_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOB_PUPDR     (PIN_PUDR_PULLUP(GPIOB_USELESS4) |              \
                             PIN_PUDR_PULLUP(GPIOB_DCMI_D5) |               \
                             PIN_PUDR_PULLUP(GPIOB_DCMI_VSYNC) |            \
                             PIN_PUDR_PULLUP(GPIOB_OTG_HS_VBUS))
#define VAL_GPIOB_ODR       0xFFFFFFFF
#define VAL_GPIOB_AFRL      (PIN_AFIO_AF(GPIOB_I2S3_CK, 6) |                \
                             PIN_AFIO_AF(GPIOB_I2S3_SD, 6))
#define VAL_GPIOB_AFRH      (PIN_AFIO_AF(GPIOB_CAN1_RX, 9) |                \
                             PIN_AFIO_AF(GPIOB_CAN1_TX, 9) |                \
                             PIN_AFIO_AF(GPIOB_ETH_RMII_TX_EN, 11) |        \
                             PIN_AFIO_AF(GPIOB_OTG_HS_ID, 12) |             \
                             PIN_AFIO_AF(GPIOB_OTG_HS_DM, 12) |             \
                             PIN_AFIO_AF(GPIOB_OTG_HS_DP, 12))

/*
 * Port C setup.
 * All input with pull-up except:
 * PC0  - GPIOC_OTG_FS_POWER_ON (output push-pull).
 * PC7  - GPIOC_MCLK            (alternate 6).
 * PC10 - GPIOC_SCLK            (alternate 6).
 * PC12 - GPIOC_SDIN            (alternate 6).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(GPIOC_OTG_FS_POWER_ON) |\
                                     PIN_MODE_INPUT(1) |                    \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_INPUT(3) |                    \
                                     PIN_MODE_INPUT(4) |                    \
                                     PIN_MODE_INPUT(5) |                    \
                                     PIN_MODE_INPUT(6) |                    \
                                     PIN_MODE_ALTERNATE(GPIOC_MCLK) |       \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(9) |                    \
                                     PIN_MODE_ALTERNATE(GPIOC_SCLK) |       \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_ALTERNATE(GPIOC_SDIN) |       \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER            0x00000000
#define VAL_GPIOC_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOC_PUPDR             (PIN_PUDR_FLOATING(GPIOC_OTG_FS_POWER_ON) |\
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_FLOATING(GPIOC_MCLK)  |       \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_FLOATING(GPIOC_SCLK)  |       \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_FLOATING(GPIOC_SDIN)  |       \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOC_ODR               0xFFFFFFFF
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_MCLK, 6))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SCLK, 6) |           \
                                     PIN_AFIO_AF(GPIOC_SDIN, 6))

/*
 * Port D setup.
 * All input with pull-up except:
 * PD4  - GPIOD_RESET           (output push-pull).
 * PD5  - GPIOD_OVER_CURRENT    (input floating).
 * PD12 - GPIOD_LED4            (output push-pull).
 * PD13 - GPIOD_LED3            (output push-pull).
 * PD14 - GPIOD_LED5            (output push-pull).
 * PD15 - GPIOD_LED6            (output push-pull).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(0) |                    \
                                     PIN_MODE_INPUT(1) |                    \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_INPUT(3) |                    \
                                     PIN_MODE_OUTPUT(GPIOD_RESET) |         \
                                     PIN_MODE_INPUT(GPIOD_OVER_CURRENT) |   \
                                     PIN_MODE_INPUT(6) |                    \
                                     PIN_MODE_INPUT(7) |                    \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(9) |                    \
                                     PIN_MODE_INPUT(10) |                   \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_OUTPUT(GPIOD_LED4) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED3) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED5) |          \
                                     PIN_MODE_OUTPUT(GPIOD_LED6))
#define VAL_GPIOD_OTYPER            0x00000000
#define VAL_GPIOD_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOD_PUPDR             (PIN_PUDR_PULLUP(0) |                   \
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_FLOATING(GPIOD_RESET) |       \
                                     PIN_PUDR_FLOATING(GPIOD_OVER_CURRENT) |\
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_FLOATING(GPIOD_LED4) |        \
                                     PIN_PUDR_FLOATING(GPIOD_LED3) |        \
                                     PIN_PUDR_FLOATING(GPIOD_LED5) |        \
                                     PIN_PUDR_FLOATING(GPIOD_LED6))
#define VAL_GPIOD_ODR               0x00000FCF
#define VAL_GPIOD_AFRL              0x00000000
#define VAL_GPIOD_AFRH              0x00000000

/*
 * Port E setup.
 * All input with pull-up except:
 * PE0  - GPIOE_INT1                (input floating).
 * PE1  - GPIOE_INT2                (input floating).
 * PE3  - GPIOE_CS_SPI              (output push-pull).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_INT1) |           \
                                     PIN_MODE_INPUT(GPIOE_INT2) |           \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_OUTPUT(GPIOE_CS_SPI) |        \
                                     PIN_MODE_INPUT(4) |                    \
                                     PIN_MODE_INPUT(5) |                    \
                                     PIN_MODE_INPUT(6) |                    \
                                     PIN_MODE_INPUT(7) |                    \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(9) |                    \
                                     PIN_MODE_INPUT(10) |                   \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_INPUT(12) |                   \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOE_OTYPER            0x00000000
#define VAL_GPIOE_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOE_PUPDR             (PIN_PUDR_FLOATING(GPIOE_INT1) |        \
                                     PIN_PUDR_FLOATING(GPIOE_INT2) |        \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_FLOATING(GPIOE_CS_SPI) |      \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOE_ODR               0xFFFFFFFF
#define VAL_GPIOE_AFRL              0x00000000
#define VAL_GPIOE_AFRH              0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER             0x00000000
#define VAL_GPIOF_OTYPER            0x00000000
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             0xFFFFFFFF
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL              0x00000000
#define VAL_GPIOF_AFRH              0x00000000

/*
 * Port G setup.
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER             0x00000000
#define VAL_GPIOG_OTYPER            0x00000000
#define VAL_GPIOG_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOG_PUPDR             0xFFFFFFFF
#define VAL_GPIOG_ODR               0xFFFFFFFF
#define VAL_GPIOG_AFRL              0x00000000
#define VAL_GPIOG_AFRH              0x00000000

/*
 * Port H setup.
 * All input with pull-up except:
 * PH0  - GPIOH_OSC_IN          (input floating).
 * PH1  - GPIOH_OSC_OUT         (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(2) |                    \
                                     PIN_MODE_INPUT(3) |                    \
                                     PIN_MODE_INPUT(4) |                    \
                                     PIN_MODE_INPUT(5) |                    \
                                     PIN_MODE_INPUT(6) |                    \
                                     PIN_MODE_INPUT(7) |                    \
                                     PIN_MODE_INPUT(8) |                    \
                                     PIN_MODE_INPUT(9) |                    \
                                     PIN_MODE_INPUT(10) |                   \
                                     PIN_MODE_INPUT(11) |                   \
                                     PIN_MODE_INPUT(12) |                   \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOH_OTYPER            0x00000000
#define VAL_GPIOH_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOH_PUPDR             (PIN_PUDR_FLOATING(GPIOH_OSC_IN) |      \
                                     PIN_PUDR_FLOATING(GPIOH_OSC_OUT) |     \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOH_ODR               0xFFFFFFFF
#define VAL_GPIOH_AFRL              0x00000000
#define VAL_GPIOH_AFRH              0x00000000

/*
 * Port I setup.
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER             0x00000000
#define VAL_GPIOI_OTYPER            0x00000000
#define VAL_GPIOI_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOI_PUPDR             0xFFFFFFFF
#define VAL_GPIOI_ODR               0xFFFFFFFF
#define VAL_GPIOI_AFRL              0x00000000
#define VAL_GPIOI_AFRH              0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
