/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F746G-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F746G_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics STM32F746G-Discovery"

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                MII_LAN8742A_ID
#define BOARD_PHY_RMII

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                25000000U
#endif

#define STM32_HSE_BYPASS

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F746xx

/*
 * IO pins assignments.
 */
#define GPIOA_ARD_A0                0U
#define GPIOA_RMII_REF_CLK          1U
#define GPIOA_RMII_MDIO             2U
#define GPIOA_ULPI_D0               3U
#define GPIOA_DCMI_HSYNC            4U
#define GPIOA_ULPI_CK               5U
#define GPIOA_DCMI_PIXCK            6U
#define GPIOA_RMII_CRS_DV           7U
#define GPIOA_ARD_D10               8U
#define GPIOA_VCP_TX                9U
#define GPIOA_OTG_FS_ID             10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_ARD_D9                15U

#define GPIOB_ULPI_D1               0U
#define GPIOB_ULPI_D2               1U
#define GPIOB_QSPI_CLK              2U
#define GPIOB_SWO                   3U
#define GPIOB_ARD_D3                4U
#define GPIOB_ULPI_D7               5U
#define GPIOB_QSPI_NCS              6U
#define GPIOB_VCP_RX                7U
#define GPIOB_ARD_D15               8U
#define GPIOB_ARD_D14               9U
#define GPIOB_ULPI_D3               10U
#define GPIOB_ULPI_D4               11U
#define GPIOB_ULPI_D5               12U
#define GPIOB_ULPI_D6               13U
#define GPIOB_ARD_D12               14U
#define GPIOB_ARD_D11               15U

#define GPIOC_ULPI_STP              0U
#define GPIOC_RMII_MDC              1U
#define GPIOC_ULPI_DIR              2U
#define GPIOC_FMC_SDCKE0            3U
#define GPIOC_RMII_RXD0             4U
#define GPIOC_RMII_RXD1             5U
#define GPIOC_ARD_D1                6U
#define GPIOC_ARD_D0                7U
#define GPIOC_SD_D0                 8U
#define GPIOC_SD_D1                 9U
#define GPIOC_SD_D2                 10U
#define GPIOC_SD_D3                 11U
#define GPIOC_SD_CLK                12U
#define GPIOC_SD_DETECT             13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_FMC_D2                0U
#define GPIOD_FMC_D3                1U
#define GPIOD_SD_CMD                2U
#define GPIOD_DCMI_D5               3U
#define GPIOD_OTG_FS_OVER_CURRENT   4U
#define GPIOD_OTG_FS_PWR_SW_ON      5U
#define GPIOD_AUDIO_INT             6U
#define GPIOD_SPDIF_RX0             7U
#define GPIOD_FMC_D13               8U
#define GPIOD_FMC_D14               9U
#define GPIOD_FMC_D15               10U
#define GPIOD_QSPI_D0               11U
#define GPIOD_QSPI_D1               12U
#define GPIOD_QSPI_D3               13U
#define GPIOD_FMC_D0                14U
#define GPIOD_FMC_D1                15U

#define GPIOE_FMC_NBL0              0U
#define GPIOE_FMC_NBL1              1U
#define GPIOE_QSPI_D2               2U
#define GPIOE_OTG_HS_OVER_CURRENT   3U
#define GPIOE_LCD_B0                4U
#define GPIOE_DCMI_D6               5U
#define GPIOE_DCMI_D7               6U
#define GPIOE_FMC_D4                7U
#define GPIOE_FMC_D5                8U
#define GPIOE_FMC_D6                9U
#define GPIOE_FMC_D7                10U
#define GPIOE_FMC_D8                11U
#define GPIOE_FMC_D9                12U
#define GPIOE_FMC_D10               13U
#define GPIOE_FMC_11                14U
#define GPIOE_FMC_D12               15U

#define GPIOF_FMC_A0                0U
#define GPIOF_FMC_A1                1U
#define GPIOF_FMC_A2                2U
#define GPIOF_FMC_A3                3U
#define GPIOF_FMC_A4                4U
#define GPIOF_FMC_A5                5U
#define GPIOF_ARD_A5                6U
#define GPIOF_ARD_A4                7U
#define GPIOF_ARD_A3                8U
#define GPIOF_ARD_A2                9U
#define GPIOF_ARD_A1                10U
#define GPIOF_FMC_SDNRAS            11U
#define GPIOF_FMC_A6                12U
#define GPIOF_FMC_A7                13U
#define GPIOF_FMC_A8                14U
#define GPIOF_FMC_A9                15U

#define GPIOG_FMC_A10               0U
#define GPIOG_FMC_A11               1U
#define GPIOG_RMII_RXER             2U
#define GPIOG_EXT_RST               3U
#define GPIOG_FMC_BA0               4U
#define GPIOG_FMC_BA1               5U
#define GPIOG_ARD_D2                6U
#define GPIOG_ARD_D4                7U
#define GPIOG_FMC_SDCLK             8U
#define GPIOG_DCMI_VSYNC            9U
#define GPIOG_SAI2_SDB              10U
#define GPIOG_RMII_TX_EN            11U
#define GPIOG_LCD_B4                12U
#define GPIOG_RMII_TXD0             13U
#define GPIOG_RMII_TXD1             14U
#define GPIOG_FMC_SDNCAS            15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)            (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)           (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - ARD_A0                    (input pullup).
 * PA1  - RMII_REF_CLK              (alternate 11).
 * PA2  - RMII_MDIO                 (alternate 11).
 * PA3  - ULPI_D0                   (alternate 10).
 * PA4  - DCMI_HSYNC                (input pullup).
 * PA5  - ULPI_CK                   (alternate 10).
 * PA6  - DCMI_PIXCK                (input pullup).
 * PA7  - RMII_CRS_DV               (alternate 11).
 * PA8  - ARD_D10                   (input pullup).
 * PA9  - VCP_TX                    (alternate 7).
 * PA10 - OTG_FS_ID                 (alternate 10).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - ARD_D9                    (input pullup).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_ARD_A0) |         \
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_REF_CLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_MDIO) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_ULPI_D0) |    \
                                     PIN_MODE_INPUT(GPIOA_DCMI_HSYNC) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_ULPI_CK) |    \
                                     PIN_MODE_INPUT(GPIOA_DCMI_PIXCK) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_CRS_DV) |\
                                     PIN_MODE_INPUT(GPIOA_ARD_D10) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_VCP_TX) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_INPUT(GPIOA_ARD_D9))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_ARD_A0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_REF_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_MDIO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ULPI_D0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_DCMI_HSYNC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ULPI_CK) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_DCMI_PIXCK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_CRS_DV) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ARD_D10) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_VCP_TX) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_ARD_D9))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_ARD_A0) |        \
                                     PIN_OSPEED_HIGH(GPIOA_RMII_REF_CLK) |  \
                                     PIN_OSPEED_HIGH(GPIOA_RMII_MDIO) |     \
                                     PIN_OSPEED_HIGH(GPIOA_ULPI_D0) |       \
                                     PIN_OSPEED_HIGH(GPIOA_DCMI_HSYNC) |    \
                                     PIN_OSPEED_HIGH(GPIOA_ULPI_CK) |       \
                                     PIN_OSPEED_HIGH(GPIOA_DCMI_PIXCK) |    \
                                     PIN_OSPEED_VERYLOW(GPIOA_RMII_CRS_DV) |\
                                     PIN_OSPEED_HIGH(GPIOA_ARD_D10) |       \
                                     PIN_OSPEED_HIGH(GPIOA_VCP_TX) |        \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_ID) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) |         \
                                     PIN_OSPEED_HIGH(GPIOA_ARD_D9))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(GPIOA_ARD_A0) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_RMII_REF_CLK) |\
                                     PIN_PUPDR_PULLUP(GPIOA_RMII_MDIO) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_ULPI_D0) |    \
                                     PIN_PUPDR_PULLUP(GPIOA_DCMI_HSYNC) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_ULPI_CK) |    \
                                     PIN_PUPDR_PULLUP(GPIOA_DCMI_PIXCK) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_RMII_CRS_DV) |\
                                     PIN_PUPDR_PULLUP(GPIOA_ARD_D10) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_VCP_TX) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_ID) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_PULLUP(GPIOA_ARD_D9))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_ARD_A0) |           \
                                     PIN_ODR_HIGH(GPIOA_RMII_REF_CLK) |     \
                                     PIN_ODR_HIGH(GPIOA_RMII_MDIO) |        \
                                     PIN_ODR_HIGH(GPIOA_ULPI_D0) |          \
                                     PIN_ODR_HIGH(GPIOA_DCMI_HSYNC) |       \
                                     PIN_ODR_HIGH(GPIOA_ULPI_CK) |          \
                                     PIN_ODR_HIGH(GPIOA_DCMI_PIXCK) |       \
                                     PIN_ODR_HIGH(GPIOA_RMII_CRS_DV) |      \
                                     PIN_ODR_HIGH(GPIOA_ARD_D10) |          \
                                     PIN_ODR_HIGH(GPIOA_VCP_TX) |           \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_ID) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_HIGH(GPIOA_ARD_D9))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_ARD_A0, 0) |         \
                                     PIN_AFIO_AF(GPIOA_RMII_REF_CLK, 11) |  \
                                     PIN_AFIO_AF(GPIOA_RMII_MDIO, 11) |     \
                                     PIN_AFIO_AF(GPIOA_ULPI_D0, 10) |       \
                                     PIN_AFIO_AF(GPIOA_DCMI_HSYNC, 0) |     \
                                     PIN_AFIO_AF(GPIOA_ULPI_CK, 10) |       \
                                     PIN_AFIO_AF(GPIOA_DCMI_PIXCK, 0) |     \
                                     PIN_AFIO_AF(GPIOA_RMII_CRS_DV, 11))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_ARD_D10, 0) |        \
                                     PIN_AFIO_AF(GPIOA_VCP_TX, 7) |         \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |     \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |          \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) |          \
                                     PIN_AFIO_AF(GPIOA_ARD_D9, 0))

/*
 * GPIOB setup:
 *
 * PB0  - ULPI_D1                   (alternate 10).
 * PB1  - ULPI_D2                   (alternate 10).
 * PB2  - QSPI_CLK                  (input pullup).
 * PB3  - SWO                       (alternate 0).
 * PB4  - ARD_D3                    (input pullup).
 * PB5  - ULPI_D7                   (alternate 10).
 * PB6  - QSPI_NCS                  (input pullup).
 * PB7  - VCP_RX                    (alternate 7).
 * PB8  - ARD_D15                   (input pullup).
 * PB9  - ARD_D14                   (input pullup).
 * PB10 - ULPI_D3                   (alternate 10).
 * PB11 - ULPI_D4                   (alternate 10).
 * PB12 - ULPI_D5                   (alternate 10).
 * PB13 - ULPI_D6                   (alternate 10).
 * PB14 - ARD_D12                   (input pullup).
 * PB15 - ARD_D11                   (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(GPIOB_ULPI_D1) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D2) |    \
                                     PIN_MODE_INPUT(GPIOB_QSPI_CLK) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_SWO) |        \
                                     PIN_MODE_INPUT(GPIOB_ARD_D3) |         \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D7) |    \
                                     PIN_MODE_INPUT(GPIOB_QSPI_NCS) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_VCP_RX) |     \
                                     PIN_MODE_INPUT(GPIOB_ARD_D15) |        \
                                     PIN_MODE_INPUT(GPIOB_ARD_D14) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D3) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D4) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D5) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_ULPI_D6) |    \
                                     PIN_MODE_INPUT(GPIOB_ARD_D12) |        \
                                     PIN_MODE_INPUT(GPIOB_ARD_D11))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D2) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_QSPI_CLK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ARD_D3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D7) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_QSPI_NCS) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_VCP_RX) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ARD_D15) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ARD_D14) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D3) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D4) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D5) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ULPI_D6) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ARD_D12) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ARD_D11))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_ULPI_D1) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D2) |       \
                                     PIN_OSPEED_HIGH(GPIOB_QSPI_CLK) |      \
                                     PIN_OSPEED_HIGH(GPIOB_SWO) |           \
                                     PIN_OSPEED_HIGH(GPIOB_ARD_D3) |        \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D7) |       \
                                     PIN_OSPEED_HIGH(GPIOB_QSPI_NCS) |      \
                                     PIN_OSPEED_HIGH(GPIOB_VCP_RX) |        \
                                     PIN_OSPEED_HIGH(GPIOB_ARD_D15) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ARD_D14) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D3) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D4) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D5) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ULPI_D6) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ARD_D12) |       \
                                     PIN_OSPEED_HIGH(GPIOB_ARD_D11))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_ULPI_D1) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_ULPI_D2) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_QSPI_CLK) |     \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_ARD_D3) |       \
                                     PIN_PUPDR_PULLUP(GPIOB_ULPI_D7) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_QSPI_NCS) |     \
                                     PIN_PUPDR_FLOATING(GPIOB_VCP_RX) |     \
                                     PIN_PUPDR_PULLUP(GPIOB_ARD_D15) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_ARD_D14) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_ULPI_D3) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_ULPI_D4) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_ULPI_D5) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_ULPI_D6) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_ARD_D12) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_ARD_D11))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_ULPI_D1) |          \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D2) |          \
                                     PIN_ODR_HIGH(GPIOB_QSPI_CLK) |         \
                                     PIN_ODR_HIGH(GPIOB_SWO) |              \
                                     PIN_ODR_HIGH(GPIOB_ARD_D3) |           \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D7) |          \
                                     PIN_ODR_HIGH(GPIOB_QSPI_NCS) |         \
                                     PIN_ODR_HIGH(GPIOB_VCP_RX) |           \
                                     PIN_ODR_HIGH(GPIOB_ARD_D15) |          \
                                     PIN_ODR_HIGH(GPIOB_ARD_D14) |          \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D3) |          \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D4) |          \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D5) |          \
                                     PIN_ODR_HIGH(GPIOB_ULPI_D6) |          \
                                     PIN_ODR_HIGH(GPIOB_ARD_D12) |          \
                                     PIN_ODR_HIGH(GPIOB_ARD_D11))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_ULPI_D1, 10) |       \
                                     PIN_AFIO_AF(GPIOB_ULPI_D2, 10) |       \
                                     PIN_AFIO_AF(GPIOB_QSPI_CLK, 0) |       \
                                     PIN_AFIO_AF(GPIOB_SWO, 0) |            \
                                     PIN_AFIO_AF(GPIOB_ARD_D3, 0) |         \
                                     PIN_AFIO_AF(GPIOB_ULPI_D7, 10) |       \
                                     PIN_AFIO_AF(GPIOB_QSPI_NCS, 0) |       \
                                     PIN_AFIO_AF(GPIOB_VCP_RX, 7))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_ARD_D15, 0) |        \
                                     PIN_AFIO_AF(GPIOB_ARD_D14, 0) |        \
                                     PIN_AFIO_AF(GPIOB_ULPI_D3, 10) |       \
                                     PIN_AFIO_AF(GPIOB_ULPI_D4, 10) |       \
                                     PIN_AFIO_AF(GPIOB_ULPI_D5, 10) |       \
                                     PIN_AFIO_AF(GPIOB_ULPI_D6, 10) |       \
                                     PIN_AFIO_AF(GPIOB_ARD_D12, 0) |        \
                                     PIN_AFIO_AF(GPIOB_ARD_D11, 0))

/*
 * GPIOC setup:
 *
 * PC0  - ULPI_STP                  (alternate 10).
 * PC1  - RMII_MDC                  (alternate 11).
 * PC2  - ULPI_DIR                  (alternate 10).
 * PC3  - FMC_SDCKE0                (alternate 12).
 * PC4  - RMII_RXD0                 (alternate 11).
 * PC5  - RMII_RXD1                 (alternate 11).
 * PC6  - ARD_D1                    (input pullup).
 * PC7  - ARD_D0                    (input floating).
 * PC8  - SD_D0                     (alternate 12).
 * PC9  - SD_D1                     (alternate 12).
 * PC10 - SD_D2                     (alternate 12).
 * PC11 - SD_D3                     (alternate 12).
 * PC12 - SD_CLK                    (alternate 12).
 * PC13 - SD_DETECT                 (input pullup).
 * PC14 - OSC32_IN                  (input floating).
 * PC15 - OSC32_OUT                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ALTERNATE(GPIOC_ULPI_STP) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_MDC) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_ULPI_DIR) |   \
                                     PIN_MODE_ALTERNATE(GPIOC_FMC_SDCKE0) | \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_RXD0) |  \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_RXD1) |  \
                                     PIN_MODE_INPUT(GPIOC_ARD_D1) |         \
                                     PIN_MODE_INPUT(GPIOC_ARD_D0) |         \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D0) |      \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D1) |      \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D2) |      \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D3) |      \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CLK) |     \
                                     PIN_MODE_INPUT(GPIOC_SD_DETECT) |      \
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN) |       \
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_ULPI_STP) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_MDC) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ULPI_DIR) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_FMC_SDCKE0) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_RXD0) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_RXD1) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ARD_D1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ARD_D0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D0) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D1) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D2) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D3) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CLK) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_DETECT) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_ULPI_STP) |      \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_MDC) |      \
                                     PIN_OSPEED_HIGH(GPIOC_ULPI_DIR) |      \
                                     PIN_OSPEED_HIGH(GPIOC_FMC_SDCKE0) |    \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_RXD0) |     \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_RXD1) |     \
                                     PIN_OSPEED_HIGH(GPIOC_ARD_D1) |        \
                                     PIN_OSPEED_HIGH(GPIOC_ARD_D0) |        \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D0) |         \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D1) |         \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D2) |         \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D3) |         \
                                     PIN_OSPEED_HIGH(GPIOC_SD_CLK) |        \
                                     PIN_OSPEED_HIGH(GPIOC_SD_DETECT) |     \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_IN) |   \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_ULPI_STP) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_MDC) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_ULPI_DIR) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_FMC_SDCKE0) | \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_RXD0) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_RXD1) |  \
                                     PIN_PUPDR_PULLUP(GPIOC_ARD_D1) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_ARD_D0) |     \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D0) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D1) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D2) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D3) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CLK) |     \
                                     PIN_PUPDR_PULLUP(GPIOC_SD_DETECT) |    \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_ULPI_STP) |         \
                                     PIN_ODR_HIGH(GPIOC_RMII_MDC) |         \
                                     PIN_ODR_HIGH(GPIOC_ULPI_DIR) |         \
                                     PIN_ODR_HIGH(GPIOC_FMC_SDCKE0) |       \
                                     PIN_ODR_HIGH(GPIOC_RMII_RXD0) |        \
                                     PIN_ODR_HIGH(GPIOC_RMII_RXD1) |        \
                                     PIN_ODR_HIGH(GPIOC_ARD_D1) |           \
                                     PIN_ODR_HIGH(GPIOC_ARD_D0) |           \
                                     PIN_ODR_HIGH(GPIOC_SD_D0) |            \
                                     PIN_ODR_HIGH(GPIOC_SD_D1) |            \
                                     PIN_ODR_HIGH(GPIOC_SD_D2) |            \
                                     PIN_ODR_HIGH(GPIOC_SD_D3) |            \
                                     PIN_ODR_HIGH(GPIOC_SD_CLK) |           \
                                     PIN_ODR_HIGH(GPIOC_SD_DETECT) |        \
                                     PIN_ODR_HIGH(GPIOC_OSC32_IN) |         \
                                     PIN_ODR_HIGH(GPIOC_OSC32_OUT))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_ULPI_STP, 10) |      \
                                     PIN_AFIO_AF(GPIOC_RMII_MDC, 11) |      \
                                     PIN_AFIO_AF(GPIOC_ULPI_DIR, 10) |      \
                                     PIN_AFIO_AF(GPIOC_FMC_SDCKE0, 12) |    \
                                     PIN_AFIO_AF(GPIOC_RMII_RXD0, 11) |     \
                                     PIN_AFIO_AF(GPIOC_RMII_RXD1, 11) |     \
                                     PIN_AFIO_AF(GPIOC_ARD_D1, 0) |         \
                                     PIN_AFIO_AF(GPIOC_ARD_D0, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_D0, 12) |         \
                                     PIN_AFIO_AF(GPIOC_SD_D1, 12) |         \
                                     PIN_AFIO_AF(GPIOC_SD_D2, 12) |         \
                                     PIN_AFIO_AF(GPIOC_SD_D3, 12) |         \
                                     PIN_AFIO_AF(GPIOC_SD_CLK, 12) |        \
                                     PIN_AFIO_AF(GPIOC_SD_DETECT, 0) |      \
                                     PIN_AFIO_AF(GPIOC_OSC32_IN, 0) |       \
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0))

/*
 * GPIOD setup:
 *
 * PD0  - FMC_D2                    (alternate 12).
 * PD1  - FMC_D3                    (alternate 12).
 * PD2  - SD_CMD                    (alternate 12).
 * PD3  - DCMI_D5                   (input pullup).
 * PD4  - OTG_FS_OVER_CURRENT       (input floating).
 * PD5  - OTG_FS_PWR_SW_ON          (output pushpull maximum).
 * PD6  - AUDIO_INT                 (input pullup).
 * PD7  - SPDIF_RX0                 (input pullup).
 * PD8  - FMC_D13                   (alternate 12).
 * PD9  - FMC_D14                   (alternate 12).
 * PD10 - FMC_D15                   (alternate 12).
 * PD11 - QSPI_D0                   (input pullup).
 * PD12 - QSPI_D1                   (input pullup).
 * PD13 - QSPI_D3                   (input pullup).
 * PD14 - FMC_D0                    (alternate 12).
 * PD15 - FMC_D1                    (alternate 12).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_FMC_D2) |     \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D3) |     \
                                     PIN_MODE_ALTERNATE(GPIOD_SD_CMD) |     \
                                     PIN_MODE_INPUT(GPIOD_DCMI_D5) |        \
                                     PIN_MODE_INPUT(GPIOD_OTG_FS_OVER_CURRENT) |\
                                     PIN_MODE_OUTPUT(GPIOD_OTG_FS_PWR_SW_ON) |\
                                     PIN_MODE_INPUT(GPIOD_AUDIO_INT) |      \
                                     PIN_MODE_INPUT(GPIOD_SPDIF_RX0) |      \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D13) |    \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D14) |    \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D15) |    \
                                     PIN_MODE_INPUT(GPIOD_QSPI_D0) |        \
                                     PIN_MODE_INPUT(GPIOD_QSPI_D1) |        \
                                     PIN_MODE_INPUT(GPIOD_QSPI_D3) |        \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D0) |     \
                                     PIN_MODE_ALTERNATE(GPIOD_FMC_D1))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_FMC_D2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SD_CMD) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_DCMI_D5) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_OTG_FS_OVER_CURRENT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_OTG_FS_PWR_SW_ON) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_AUDIO_INT) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SPDIF_RX0) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D13) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D14) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D15) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_QSPI_D0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_QSPI_D1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_QSPI_D3) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FMC_D1))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_FMC_D2) |        \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D3) |        \
                                     PIN_OSPEED_HIGH(GPIOD_SD_CMD) |        \
                                     PIN_OSPEED_HIGH(GPIOD_DCMI_D5) |       \
                                     PIN_OSPEED_HIGH(GPIOD_OTG_FS_OVER_CURRENT) |\
                                     PIN_OSPEED_HIGH(GPIOD_OTG_FS_PWR_SW_ON) |\
                                     PIN_OSPEED_HIGH(GPIOD_AUDIO_INT) |     \
                                     PIN_OSPEED_HIGH(GPIOD_SPDIF_RX0) |     \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D13) |       \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D14) |       \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D15) |       \
                                     PIN_OSPEED_HIGH(GPIOD_QSPI_D0) |       \
                                     PIN_OSPEED_HIGH(GPIOD_QSPI_D1) |       \
                                     PIN_OSPEED_HIGH(GPIOD_QSPI_D3) |       \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D0) |        \
                                     PIN_OSPEED_HIGH(GPIOD_FMC_D1))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_FMC_D2) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D3) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_SD_CMD) |     \
                                     PIN_PUPDR_PULLUP(GPIOD_DCMI_D5) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_OTG_FS_OVER_CURRENT) |\
                                     PIN_PUPDR_FLOATING(GPIOD_OTG_FS_PWR_SW_ON) |\
                                     PIN_PUPDR_PULLUP(GPIOD_AUDIO_INT) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_SPDIF_RX0) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D13) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D14) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D15) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_QSPI_D0) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_QSPI_D1) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_QSPI_D3) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D0) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_FMC_D1))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_FMC_D2) |           \
                                     PIN_ODR_HIGH(GPIOD_FMC_D3) |           \
                                     PIN_ODR_HIGH(GPIOD_SD_CMD) |           \
                                     PIN_ODR_HIGH(GPIOD_DCMI_D5) |          \
                                     PIN_ODR_HIGH(GPIOD_OTG_FS_OVER_CURRENT) |\
                                     PIN_ODR_HIGH(GPIOD_OTG_FS_PWR_SW_ON) | \
                                     PIN_ODR_HIGH(GPIOD_AUDIO_INT) |        \
                                     PIN_ODR_HIGH(GPIOD_SPDIF_RX0) |        \
                                     PIN_ODR_HIGH(GPIOD_FMC_D13) |          \
                                     PIN_ODR_HIGH(GPIOD_FMC_D14) |          \
                                     PIN_ODR_HIGH(GPIOD_FMC_D15) |          \
                                     PIN_ODR_HIGH(GPIOD_QSPI_D0) |          \
                                     PIN_ODR_LOW(GPIOD_QSPI_D1) |           \
                                     PIN_ODR_LOW(GPIOD_QSPI_D3) |           \
                                     PIN_ODR_LOW(GPIOD_FMC_D0) |            \
                                     PIN_ODR_LOW(GPIOD_FMC_D1))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_FMC_D2, 12) |        \
                                     PIN_AFIO_AF(GPIOD_FMC_D3, 12) |        \
                                     PIN_AFIO_AF(GPIOD_SD_CMD, 12) |        \
                                     PIN_AFIO_AF(GPIOD_DCMI_D5, 0) |        \
                                     PIN_AFIO_AF(GPIOD_OTG_FS_OVER_CURRENT, 0) |\
                                     PIN_AFIO_AF(GPIOD_OTG_FS_PWR_SW_ON, 0) |\
                                     PIN_AFIO_AF(GPIOD_AUDIO_INT, 0) |      \
                                     PIN_AFIO_AF(GPIOD_SPDIF_RX0, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_FMC_D13, 12) |       \
                                     PIN_AFIO_AF(GPIOD_FMC_D14, 12) |       \
                                     PIN_AFIO_AF(GPIOD_FMC_D15, 12) |       \
                                     PIN_AFIO_AF(GPIOD_QSPI_D0, 0) |        \
                                     PIN_AFIO_AF(GPIOD_QSPI_D1, 0) |        \
                                     PIN_AFIO_AF(GPIOD_QSPI_D3, 0) |        \
                                     PIN_AFIO_AF(GPIOD_FMC_D0, 12) |        \
                                     PIN_AFIO_AF(GPIOD_FMC_D1, 12))

/*
 * GPIOE setup:
 *
 * PE0  - FMC_NBL0                  (alternate 12).
 * PE1  - FMC_NBL1                  (alternate 12).
 * PE2  - QSPI_D2                   (input pullup).
 * PE3  - OTG_HS_OVER_CURRENT       (input floating).
 * PE4  - LCD_B0                    (alternate 14).
 * PE5  - DCMI_D6                   (input pullup).
 * PE6  - DCMI_D7                   (input pullup).
 * PE7  - FMC_D4                    (alternate 12).
 * PE8  - FMC_D5                    (alternate 12).
 * PE9  - FMC_D6                    (alternate 12).
 * PE10 - FMC_D7                    (alternate 12).
 * PE11 - FMC_D8                    (alternate 12).
 * PE12 - FMC_D9                    (alternate 12).
 * PE13 - FMC_D10                   (alternate 12).
 * PE14 - FMC_11                    (alternate 12).
 * PE15 - FMC_D12                   (alternate 12).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ALTERNATE(GPIOE_FMC_NBL0) |   \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_NBL1) |   \
                                     PIN_MODE_INPUT(GPIOE_QSPI_D2) |        \
                                     PIN_MODE_INPUT(GPIOE_OTG_HS_OVER_CURRENT) |\
                                     PIN_MODE_ALTERNATE(GPIOE_LCD_B0) |     \
                                     PIN_MODE_INPUT(GPIOE_DCMI_D6) |        \
                                     PIN_MODE_INPUT(GPIOE_DCMI_D7) |        \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D4) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D5) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D6) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D7) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D8) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D9) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D10) |    \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_11) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_FMC_D12))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_FMC_NBL0) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_NBL1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOE_QSPI_D2) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_OTG_HS_OVER_CURRENT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_LCD_B0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D6) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DCMI_D7) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D8) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D9) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D10) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_11) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FMC_D12))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_FMC_NBL0) |      \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_NBL1) |      \
                                     PIN_OSPEED_HIGH(GPIOE_QSPI_D2) |       \
                                     PIN_OSPEED_HIGH(GPIOE_OTG_HS_OVER_CURRENT) |\
                                     PIN_OSPEED_HIGH(GPIOE_LCD_B0) |        \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D6) |       \
                                     PIN_OSPEED_HIGH(GPIOE_DCMI_D7) |       \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D4) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D5) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D6) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D7) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D8) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D9) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D10) |       \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_11) |        \
                                     PIN_OSPEED_HIGH(GPIOE_FMC_D12))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_FLOATING(GPIOE_FMC_NBL0) |   \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_NBL1) |   \
                                     PIN_PUPDR_PULLUP(GPIOE_QSPI_D2) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_OTG_HS_OVER_CURRENT) |\
                                     PIN_PUPDR_FLOATING(GPIOE_LCD_B0) |     \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D6) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_DCMI_D7) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D4) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D5) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D6) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D7) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D8) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D9) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D10) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_11) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_FMC_D12))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_FMC_NBL0) |         \
                                     PIN_ODR_HIGH(GPIOE_FMC_NBL1) |         \
                                     PIN_ODR_HIGH(GPIOE_QSPI_D2) |          \
                                     PIN_ODR_HIGH(GPIOE_OTG_HS_OVER_CURRENT) |\
                                     PIN_ODR_HIGH(GPIOE_LCD_B0) |           \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D6) |          \
                                     PIN_ODR_HIGH(GPIOE_DCMI_D7) |          \
                                     PIN_ODR_HIGH(GPIOE_FMC_D4) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D5) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D6) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D7) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D8) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D9) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D10) |          \
                                     PIN_ODR_HIGH(GPIOE_FMC_11) |           \
                                     PIN_ODR_HIGH(GPIOE_FMC_D12))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_FMC_NBL0, 12) |      \
                                     PIN_AFIO_AF(GPIOE_FMC_NBL1, 12) |      \
                                     PIN_AFIO_AF(GPIOE_QSPI_D2, 0) |        \
                                     PIN_AFIO_AF(GPIOE_OTG_HS_OVER_CURRENT, 0) |\
                                     PIN_AFIO_AF(GPIOE_LCD_B0, 14) |        \
                                     PIN_AFIO_AF(GPIOE_DCMI_D6, 0) |        \
                                     PIN_AFIO_AF(GPIOE_DCMI_D7, 0) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D4, 12))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_FMC_D5, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D6, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D7, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D8, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D9, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D10, 12) |       \
                                     PIN_AFIO_AF(GPIOE_FMC_11, 12) |        \
                                     PIN_AFIO_AF(GPIOE_FMC_D12, 12))

/*
 * GPIOF setup:
 *
 * PF0  - FMC_A0                    (alternate 12).
 * PF1  - FMC_A1                    (alternate 12).
 * PF2  - FMC_A2                    (alternate 12).
 * PF3  - FMC_A3                    (alternate 12).
 * PF4  - FMC_A4                    (alternate 12).
 * PF5  - FMC_A5                    (alternate 12).
 * PF6  - ARD_A5                    (input pullup).
 * PF7  - ARD_A4                    (input pullup).
 * PF8  - ARD_A3                    (input pullup).
 * PF9  - ARD_A2                    (input pullup).
 * PF10 - ARD_A1                    (input pullup).
 * PF11 - FMC_SDNRAS                (alternate 12).
 * PF12 - FMC_A6                    (alternate 12).
 * PF13 - FMC_A7                    (alternate 12).
 * PF14 - FMC_A8                    (alternate 12).
 * PF15 - FMC_A9                    (alternate 12).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_ALTERNATE(GPIOF_FMC_A0) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A1) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A2) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A3) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A4) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A5) |     \
                                     PIN_MODE_INPUT(GPIOF_ARD_A5) |         \
                                     PIN_MODE_INPUT(GPIOF_ARD_A4) |         \
                                     PIN_MODE_INPUT(GPIOF_ARD_A3) |         \
                                     PIN_MODE_INPUT(GPIOF_ARD_A2) |         \
                                     PIN_MODE_INPUT(GPIOF_ARD_A1) |         \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_SDNRAS) | \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A6) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A7) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A8) |     \
                                     PIN_MODE_ALTERNATE(GPIOF_FMC_A9))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_FMC_A0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_ARD_A5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_ARD_A4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_ARD_A3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_ARD_A2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_ARD_A1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_SDNRAS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A8) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_FMC_A9))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_FMC_A0) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A1) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A2) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A3) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A4) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A5) |        \
                                     PIN_OSPEED_HIGH(GPIOF_ARD_A5) |        \
                                     PIN_OSPEED_HIGH(GPIOF_ARD_A4) |        \
                                     PIN_OSPEED_HIGH(GPIOF_ARD_A3) |        \
                                     PIN_OSPEED_HIGH(GPIOF_ARD_A2) |        \
                                     PIN_OSPEED_HIGH(GPIOF_ARD_A1) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_SDNRAS) |    \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A6) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A7) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A8) |        \
                                     PIN_OSPEED_HIGH(GPIOF_FMC_A9))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_FMC_A0) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A1) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A2) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A3) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A4) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A5) |     \
                                     PIN_PUPDR_PULLUP(GPIOF_ARD_A5) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_ARD_A4) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_ARD_A3) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_ARD_A2) |       \
                                     PIN_PUPDR_PULLUP(GPIOF_ARD_A1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_SDNRAS) | \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A6) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A7) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A8) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_FMC_A9))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_FMC_A0) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A1) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A2) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A3) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A4) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A5) |           \
                                     PIN_ODR_HIGH(GPIOF_ARD_A5) |           \
                                     PIN_ODR_HIGH(GPIOF_ARD_A4) |           \
                                     PIN_ODR_HIGH(GPIOF_ARD_A3) |           \
                                     PIN_ODR_HIGH(GPIOF_ARD_A2) |           \
                                     PIN_ODR_HIGH(GPIOF_ARD_A1) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_SDNRAS) |       \
                                     PIN_ODR_HIGH(GPIOF_FMC_A6) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A7) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A8) |           \
                                     PIN_ODR_HIGH(GPIOF_FMC_A9))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_FMC_A0, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A1, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A2, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A3, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A4, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A5, 12) |        \
                                     PIN_AFIO_AF(GPIOF_ARD_A5, 0) |         \
                                     PIN_AFIO_AF(GPIOF_ARD_A4, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_ARD_A3, 0) |         \
                                     PIN_AFIO_AF(GPIOF_ARD_A2, 0) |         \
                                     PIN_AFIO_AF(GPIOF_ARD_A1, 0) |         \
                                     PIN_AFIO_AF(GPIOF_FMC_SDNRAS, 12) |    \
                                     PIN_AFIO_AF(GPIOF_FMC_A6, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A7, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A8, 12) |        \
                                     PIN_AFIO_AF(GPIOF_FMC_A9, 12))

/*
 * GPIOG setup:
 *
 * PG0  - FMC_A10                   (alternate 12).
 * PG1  - FMC_A11                   (alternate 12).
 * PG2  - RMII_RXER                 (input pullup).
 * PG3  - EXT_RST                   (input pullup).
 * PG4  - FMC_BA0                   (alternate 12).
 * PG5  - FMC_BA1                   (alternate 12).
 * PG6  - ARD_D2                    (input pullup).
 * PG7  - ARD_D4                    (input pullup).
 * PG8  - FMC_SDCLK                 (input floating).
 * PG9  - DCMI_VSYNC                (input pullup).
 * PG10 - SAI2_SDB                  (input pullup).
 * PG11 - RMII_TX_EN                (alternate 11).
 * PG12 - LCD_B4                    (alternate 14).
 * PG13 - RMII_TXD0                 (alternate 1).
 * PG14 - RMII_TXD1                 (alternate 11).
 * PG15 - FMC_SDNCAS                (alternate 12).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_ALTERNATE(GPIOG_FMC_A10) |    \
                                     PIN_MODE_ALTERNATE(GPIOG_FMC_A11) |    \
                                     PIN_MODE_INPUT(GPIOG_RMII_RXER) |      \
                                     PIN_MODE_INPUT(GPIOG_EXT_RST) |        \
                                     PIN_MODE_ALTERNATE(GPIOG_FMC_BA0) |    \
                                     PIN_MODE_ALTERNATE(GPIOG_FMC_BA1) |    \
                                     PIN_MODE_INPUT(GPIOG_ARD_D2) |         \
                                     PIN_MODE_INPUT(GPIOG_ARD_D4) |         \
                                     PIN_MODE_INPUT(GPIOG_FMC_SDCLK) |      \
                                     PIN_MODE_INPUT(GPIOG_DCMI_VSYNC) |     \
                                     PIN_MODE_INPUT(GPIOG_SAI2_SDB) |       \
                                     PIN_MODE_ALTERNATE(GPIOG_RMII_TX_EN) | \
                                     PIN_MODE_ALTERNATE(GPIOG_LCD_B4) |     \
                                     PIN_MODE_ALTERNATE(GPIOG_RMII_TXD0) |  \
                                     PIN_MODE_ALTERNATE(GPIOG_RMII_TXD1) |  \
                                     PIN_MODE_ALTERNATE(GPIOG_FMC_SDNCAS))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_FMC_A10) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOG_FMC_A11) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOG_RMII_RXER) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOG_EXT_RST) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOG_FMC_BA0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOG_FMC_BA1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOG_ARD_D2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOG_ARD_D4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOG_FMC_SDCLK) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOG_DCMI_VSYNC) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_SAI2_SDB) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOG_RMII_TX_EN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOG_LCD_B4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOG_RMII_TXD0) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOG_RMII_TXD1) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOG_FMC_SDNCAS))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_HIGH(GPIOG_FMC_A10) |       \
                                     PIN_OSPEED_HIGH(GPIOG_FMC_A11) |       \
                                     PIN_OSPEED_HIGH(GPIOG_RMII_RXER) |     \
                                     PIN_OSPEED_HIGH(GPIOG_EXT_RST) |       \
                                     PIN_OSPEED_HIGH(GPIOG_FMC_BA0) |       \
                                     PIN_OSPEED_HIGH(GPIOG_FMC_BA1) |       \
                                     PIN_OSPEED_HIGH(GPIOG_ARD_D2) |        \
                                     PIN_OSPEED_HIGH(GPIOG_ARD_D4) |        \
                                     PIN_OSPEED_HIGH(GPIOG_FMC_SDCLK) |     \
                                     PIN_OSPEED_HIGH(GPIOG_DCMI_VSYNC) |    \
                                     PIN_OSPEED_HIGH(GPIOG_SAI2_SDB) |      \
                                     PIN_OSPEED_HIGH(GPIOG_RMII_TX_EN) |    \
                                     PIN_OSPEED_HIGH(GPIOG_LCD_B4) |        \
                                     PIN_OSPEED_HIGH(GPIOG_RMII_TXD0) |     \
                                     PIN_OSPEED_HIGH(GPIOG_RMII_TXD1) |     \
                                     PIN_OSPEED_HIGH(GPIOG_FMC_SDNCAS))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_FMC_A10) |    \
                                     PIN_PUPDR_FLOATING(GPIOG_FMC_A11) |    \
                                     PIN_PUPDR_PULLUP(GPIOG_RMII_RXER) |    \
                                     PIN_PUPDR_PULLUP(GPIOG_EXT_RST) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_FMC_BA0) |    \
                                     PIN_PUPDR_FLOATING(GPIOG_FMC_BA1) |    \
                                     PIN_PUPDR_PULLUP(GPIOG_ARD_D2) |       \
                                     PIN_PUPDR_PULLUP(GPIOG_ARD_D4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_FMC_SDCLK) |  \
                                     PIN_PUPDR_PULLUP(GPIOG_DCMI_VSYNC) |   \
                                     PIN_PUPDR_PULLUP(GPIOG_SAI2_SDB) |     \
                                     PIN_PUPDR_FLOATING(GPIOG_RMII_TX_EN) | \
                                     PIN_PUPDR_FLOATING(GPIOG_LCD_B4) |     \
                                     PIN_PUPDR_FLOATING(GPIOG_RMII_TXD0) |  \
                                     PIN_PUPDR_FLOATING(GPIOG_RMII_TXD1) |  \
                                     PIN_PUPDR_FLOATING(GPIOG_FMC_SDNCAS))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_FMC_A10) |          \
                                     PIN_ODR_HIGH(GPIOG_FMC_A11) |          \
                                     PIN_ODR_HIGH(GPIOG_RMII_RXER) |        \
                                     PIN_ODR_HIGH(GPIOG_EXT_RST) |          \
                                     PIN_ODR_HIGH(GPIOG_FMC_BA0) |          \
                                     PIN_ODR_HIGH(GPIOG_FMC_BA1) |          \
                                     PIN_ODR_HIGH(GPIOG_ARD_D2) |           \
                                     PIN_ODR_HIGH(GPIOG_ARD_D4) |           \
                                     PIN_ODR_HIGH(GPIOG_FMC_SDCLK) |        \
                                     PIN_ODR_HIGH(GPIOG_DCMI_VSYNC) |       \
                                     PIN_ODR_HIGH(GPIOG_SAI2_SDB) |         \
                                     PIN_ODR_HIGH(GPIOG_RMII_TX_EN) |       \
                                     PIN_ODR_HIGH(GPIOG_LCD_B4) |           \
                                     PIN_ODR_HIGH(GPIOG_RMII_TXD0) |        \
                                     PIN_ODR_HIGH(GPIOG_RMII_TXD1) |        \
                                     PIN_ODR_HIGH(GPIOG_FMC_SDNCAS))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_FMC_A10, 12) |       \
                                     PIN_AFIO_AF(GPIOG_FMC_A11, 12) |       \
                                     PIN_AFIO_AF(GPIOG_RMII_RXER, 0) |      \
                                     PIN_AFIO_AF(GPIOG_EXT_RST, 0) |        \
                                     PIN_AFIO_AF(GPIOG_FMC_BA0, 12) |       \
                                     PIN_AFIO_AF(GPIOG_FMC_BA1, 12) |       \
                                     PIN_AFIO_AF(GPIOG_ARD_D2, 0) |         \
                                     PIN_AFIO_AF(GPIOG_ARD_D4, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_FMC_SDCLK, 0) |      \
                                     PIN_AFIO_AF(GPIOG_DCMI_VSYNC, 0) |     \
                                     PIN_AFIO_AF(GPIOG_SAI2_SDB, 0) |       \
                                     PIN_AFIO_AF(GPIOG_RMII_TX_EN, 11) |    \
                                     PIN_AFIO_AF(GPIOG_LCD_B4, 14) |        \
                                     PIN_AFIO_AF(GPIOG_RMII_TXD0, 1) |      \
                                     PIN_AFIO_AF(GPIOG_RMII_TXD1, 11) |     \
                                     PIN_AFIO_AF(GPIOG_FMC_SDNCAS, 12))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_HIGH(GPIOH_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input floating).
 * PI1  - PIN1                      (input floating).
 * PI2  - PIN2                      (input floating).
 * PI3  - PIN3                      (input floating).
 * PI4  - PIN4                      (input floating).
 * PI5  - PIN5                      (input floating).
 * PI6  - PIN6                      (input floating).
 * PI7  - PIN7                      (input floating).
 * PI8  - PIN8                      (input floating).
 * PI9  - PIN9                      (input floating).
 * PI10 - PIN10                     (input floating).
 * PI11 - PIN11                     (input floating).
 * PI12 - PIN12                     (input floating).
 * PI13 - PIN13                     (input floating).
 * PI14 - PIN14                     (input floating).
 * PI15 - PIN15                     (input floating).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_HIGH(GPIOI_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))


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
