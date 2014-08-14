/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_NAME                  "NAND flash test board. Codename PNC"

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F40_41xxx

/*
 * IO pins assignments.
 */
#define GPIOA_CTL_27V_P             0
#define GPIOA_CTL_27V_N             1
#define GPIOA_CTL_6V_P              2
#define GPIOA_CTL_6V_N              3
#define GPIOA_SPI1_NSS              4
#define GPIOA_SPI1_SCK              5
#define GPIOA_SPI1_MISO             6
#define GPIOA_SPI1_MOSI             7
#define GPIOA_TP1                   8
#define GPIOA_TX_MCU                9
#define GPIOA_RX_MCU                10
#define GPIOA_OTG_FS_DM             11
#define GPIOA_OTG_FS_DP             12
#define GPIOA_JTMS                  13
#define GPIOA_JTCK                  14
#define GPIOA_JTDI                  15

#define GPIOB_DATA_SENT_OUT         0
#define GPIOB_SPI1_RST              1
#define GPIOB_PIN2                  2
#define GPIOB_JTDO                  3
#define GPIOB_JTRST                 4
#define GPIOB_NAND_WP               5
#define GPIOB_I2C1_SCL              6
#define GPIOB_I2C1_SDA              7
#define GPIOB_USB_DISCOVERY         8
#define GPIOB_USB_PRESENT           9
#define GPIOB_I2C2_SCL              10
#define GPIOB_I2C2_SDA              11
#define GPIOB_PIN12                 12
#define GPIOB_SPI2_CLK              13
#define GPIOB_SPI2_MISO             14
#define GPIOB_XSENS_EXTTRIG         15

#define GPIOC_PIN0                  0
#define GPIOC_SPI3_CS1              1
#define GPIOC_SPI3_CS2              2
#define GPIOC_FLASH_CS              3
#define GPIOC_SPI2_CS1              4
#define GPIOC_SPI2_CS2              5
#define GPIOC_XSENS_TX              6
#define GPIOC_XSENS_RX              7
#define GPIOC_MPU6050_CLK           8
#define GPIOC_TP2                   9
#define GPIOC_FLASH_CLK             10
#define GPIOC_FLASH_MISO            11
#define GPIOC_FLASH_MOSI            12
#define GPIOC_PPS_OUT               13
#define GPIOC_PIN14                 14
#define GPIOC_PIN15                 15

#define GPIOD_NAND_IO2              0
#define GPIOD_NAND_IO3              1
#define GPIOD_PIN2                  2
#define GPIOD_PIN3                  3
#define GPIOD_NAND_RE               4
#define GPIOD_NAND_WE               5
#define GPIOD_NAND_RB_NWAIT         6
#define GPIOD_NAND_CE               7
#define GPIOD_PIN8                  8
#define GPIOD_PIN9                  9
#define GPIOD_PIN10                 10
#define GPIOD_NAND_CLE              11
#define GPIOD_NAND_ALE              12
#define GPIOD_PIN13                 13
#define GPIOD_NAND_IO0              14
#define GPIOD_NAND_IO1              15

#define GPIOE_MPU6050_INT1          0
#define GPIOE_MPU6050_INT2          1
#define GPIOE_PIN2                  2
#define GPIOE_SPI1_INT              3
#define GPIOE_PPS_IN                4
#define GPIOE_DATA_SENT_IN          5
#define GPIOE_NAND_RB_INT           6
#define GPIOE_NAND_IO4              7
#define GPIOE_NAND_IO5              8
#define GPIOE_NAND_IO6              9
#define GPIOE_NAND_IO7              10
#define GPIOE_LED_G                 11
#define GPIOE_LED_R                 12
#define GPIOE_FLASH_WP              13
#define GPIOE_FLASH_RST             14
#define GPIOE_PIN15                 15

#define GPIOF_PIN0                  0
#define GPIOF_PIN1                  1
#define GPIOF_PIN2                  2
#define GPIOF_PIN3                  3
#define GPIOF_PIN4                  4
#define GPIOF_PIN5                  5
#define GPIOF_PIN6                  6
#define GPIOF_PIN7                  7
#define GPIOF_PIN8                  8
#define GPIOF_PIN9                  9
#define GPIOF_PIN10                 10
#define GPIOF_PIN11                 11
#define GPIOF_PIN12                 12
#define GPIOF_PIN13                 13
#define GPIOF_PIN14                 14
#define GPIOF_PIN15                 15

#define GPIOG_PIN0                  0
#define GPIOG_PIN1                  1
#define GPIOG_PIN2                  2
#define GPIOG_PIN3                  3
#define GPIOG_PIN4                  4
#define GPIOG_PIN5                  5
#define GPIOG_PIN6                  6
#define GPIOG_PIN7                  7
#define GPIOG_PIN8                  8
#define GPIOG_PIN9                  9
#define GPIOG_PIN10                 10
#define GPIOG_PIN11                 11
#define GPIOG_PIN12                 12
#define GPIOG_PIN13                 13
#define GPIOG_PIN14                 14
#define GPIOG_PIN15                 15

#define GPIOH_OSC_IN                0
#define GPIOH_OSC_OUT               1
#define GPIOH_PIN2                  2
#define GPIOH_PIN3                  3
#define GPIOH_PIN4                  4
#define GPIOH_PIN5                  5
#define GPIOH_PIN6                  6
#define GPIOH_PIN7                  7
#define GPIOH_PIN8                  8
#define GPIOH_PIN9                  9
#define GPIOH_PIN10                 10
#define GPIOH_PIN11                 11
#define GPIOH_PIN12                 12
#define GPIOH_PIN13                 13
#define GPIOH_PIN14                 14
#define GPIOH_PIN15                 15

#define GPIOI_PIN0                  0
#define GPIOI_PIN1                  1
#define GPIOI_PIN2                  2
#define GPIOI_PIN3                  3
#define GPIOI_PIN4                  4
#define GPIOI_PIN5                  5
#define GPIOI_PIN6                  6
#define GPIOI_PIN7                  7
#define GPIOI_PIN8                  8
#define GPIOI_PIN9                  9
#define GPIOI_PIN10                 10
#define GPIOI_PIN11                 11
#define GPIOI_PIN12                 12
#define GPIOI_PIN13                 13
#define GPIOI_PIN14                 14
#define GPIOI_PIN15                 15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_CTL_27V_P) |         \
                                     PIN_MODE_ANALOG(GPIOA_CTL_27V_N) |           \
                                     PIN_MODE_ANALOG(GPIOA_CTL_6V_P) |           \
                                     PIN_MODE_ANALOG(GPIOA_CTL_6V_N) |           \
                                     PIN_MODE_OUTPUT(GPIOA_SPI1_NSS) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_SCK) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_MISO) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI1_MOSI) |        \
                                     PIN_MODE_OUTPUT(GPIOA_TP1) |           \
                                     PIN_MODE_ALTERNATE(GPIOA_TX_MCU) |        \
                                     PIN_MODE_ALTERNATE(GPIOA_RX_MCU) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_JTMS) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_JTCK) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_JTDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_CTL_27V_P) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CTL_27V_N) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CTL_6V_P) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CTL_6V_N) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_NSS) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_SCK) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_MISO) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_MOSI) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TP1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TX_MCU) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RX_MCU) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTMS) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTCK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIOA_CTL_27V_P) |        \
                                     PIN_OSPEED_2M(GPIOA_CTL_27V_N) |          \
                                     PIN_OSPEED_2M(GPIOA_CTL_6V_P) |          \
                                     PIN_OSPEED_2M(GPIOA_CTL_6V_N) |          \
                                     PIN_OSPEED_50M(GPIOA_SPI1_NSS) |          \
                                     PIN_OSPEED_100M(GPIOA_SPI1_SCK) |            \
                                     PIN_OSPEED_100M(GPIOA_SPI1_MISO) |            \
                                     PIN_OSPEED_100M(GPIOA_SPI1_MOSI) |            \
                                     PIN_OSPEED_100M(GPIOA_TP1) |          \
                                     PIN_OSPEED_100M(GPIOA_TX_MCU) |       \
                                     PIN_OSPEED_100M(GPIOA_RX_MCU) |     \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_100M(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_100M(GPIOA_JTMS) |         \
                                     PIN_OSPEED_100M(GPIOA_JTCK) |         \
                                     PIN_OSPEED_100M(GPIOA_JTDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_CTL_27V_P) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_CTL_27V_N) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_CTL_6V_P) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_CTL_6V_N) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_SPI1_NSS) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_SPI1_SCK) |        \
                                     PIN_PUPDR_PULLUP(GPIOA_SPI1_MISO) |        \
                                     PIN_PUPDR_PULLUP(GPIOA_SPI1_MOSI) |        \
                                     PIN_PUPDR_PULLUP(GPIOA_TP1) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_TX_MCU) |    \
                                     PIN_PUPDR_PULLUP(GPIOA_RX_MCU) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_JTMS) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_JTCK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_JTDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_CTL_27V_P) |           \
                                     PIN_ODR_HIGH(GPIOA_CTL_27V_N) |             \
                                     PIN_ODR_HIGH(GPIOA_CTL_6V_P) |             \
                                     PIN_ODR_HIGH(GPIOA_CTL_6V_N) |             \
                                     PIN_ODR_HIGH(GPIOA_SPI1_NSS) |             \
                                     PIN_ODR_HIGH(GPIOA_SPI1_SCK) |              \
                                     PIN_ODR_HIGH(GPIOA_SPI1_MISO) |              \
                                     PIN_ODR_HIGH(GPIOA_SPI1_MOSI) |              \
                                     PIN_ODR_HIGH(GPIOA_TP1) |             \
                                     PIN_ODR_HIGH(GPIOA_TX_MCU) |          \
                                     PIN_ODR_HIGH(GPIOA_RX_MCU) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_JTMS) |            \
                                     PIN_ODR_HIGH(GPIOA_JTCK) |            \
                                     PIN_ODR_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_CTL_27V_P, 0) |         \
                                     PIN_AFIO_AF(GPIOA_CTL_27V_N, 0) |           \
                                     PIN_AFIO_AF(GPIOA_CTL_6V_P, 0) |           \
                                     PIN_AFIO_AF(GPIOA_CTL_6V_N, 0) |           \
                                     PIN_AFIO_AF(GPIOA_SPI1_NSS, 0) |           \
                                     PIN_AFIO_AF(GPIOA_SPI1_SCK, 5) |            \
                                     PIN_AFIO_AF(GPIOA_SPI1_MISO, 5) |            \
                                     PIN_AFIO_AF(GPIOA_SPI1_MOSI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_TP1, 0) |           \
                                     PIN_AFIO_AF(GPIOA_TX_MCU, 7) |        \
                                     PIN_AFIO_AF(GPIOA_RX_MCU, 7) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |     \
                                     PIN_AFIO_AF(GPIOA_JTMS, 0) |          \
                                     PIN_AFIO_AF(GPIOA_JTCK, 0) |          \
                                     PIN_AFIO_AF(GPIOA_JTDI, 0))

/*
 * GPIOB setup:
 */
#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_DATA_SENT_OUT) |           \
                                     PIN_MODE_OUTPUT(GPIOB_SPI1_RST) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN2) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_JTDO) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_JTRST) |           \
                                     PIN_MODE_OUTPUT(GPIOB_NAND_WP) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SCL) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C1_SDA) |           \
                                     PIN_MODE_OUTPUT(GPIOB_USB_DISCOVERY) |           \
                                     PIN_MODE_INPUT(GPIOB_USB_PRESENT) |        \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C2_SCL) |         \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C2_SDA) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |          \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_CLK) |          \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_MISO) |          \
                                     PIN_MODE_INPUT(GPIOB_XSENS_EXTTRIG))

#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_DATA_SENT_OUT) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_RST) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTDO) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTRST) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_NAND_WP) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SCL) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C1_SDA) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USB_DISCOVERY) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USB_PRESENT) |       \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C2_SCL) |     \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C2_SDA) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_CLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_MISO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_XSENS_EXTTRIG))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_2M(GPIOB_DATA_SENT_OUT) |          \
                                     PIN_OSPEED_2M(GPIOB_SPI1_RST) |          \
                                     PIN_OSPEED_100M(GPIOB_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOB_JTDO) |           \
                                     PIN_OSPEED_100M(GPIOB_JTRST) |          \
                                     PIN_OSPEED_2M(GPIOB_NAND_WP) |          \
                                     PIN_OSPEED_2M(GPIOB_I2C1_SCL) |           \
                                     PIN_OSPEED_2M(GPIOB_I2C1_SDA) |          \
                                     PIN_OSPEED_2M(GPIOB_USB_DISCOVERY) |          \
                                     PIN_OSPEED_2M(GPIOB_USB_PRESENT) |           \
                                     PIN_OSPEED_2M(GPIOB_I2C2_SCL) |        \
                                     PIN_OSPEED_2M(GPIOB_I2C2_SDA) |         \
                                     PIN_OSPEED_100M(GPIOB_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOB_SPI2_CLK) |         \
                                     PIN_OSPEED_100M(GPIOB_SPI2_MISO) |         \
                                     PIN_OSPEED_100M(GPIOB_XSENS_EXTTRIG))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_DATA_SENT_OUT) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOB_SPI1_RST) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_JTDO) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_JTRST) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOB_NAND_WP) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_USB_DISCOVERY) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_USB_PRESENT) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SCL) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SDA) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_CLK) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_MISO) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_XSENS_EXTTRIG))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_DATA_SENT_OUT) |             \
                                     PIN_ODR_LOW(GPIOB_SPI1_RST) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOB_JTDO) |              \
                                     PIN_ODR_HIGH(GPIOB_JTRST) |             \
                                     PIN_ODR_LOW(GPIOB_NAND_WP) |             \
                                     PIN_ODR_LOW(GPIOB_I2C1_SCL) |              \
                                     PIN_ODR_LOW(GPIOB_I2C1_SDA) |             \
                                     PIN_ODR_HIGH(GPIOB_USB_DISCOVERY) |             \
                                     PIN_ODR_HIGH(GPIOB_USB_PRESENT) |              \
                                     PIN_ODR_LOW(GPIOB_I2C2_SCL) |           \
                                     PIN_ODR_LOW(GPIOB_I2C2_SDA) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOB_SPI2_CLK) |            \
                                     PIN_ODR_HIGH(GPIOB_SPI2_MISO) |            \
                                     PIN_ODR_HIGH(GPIOB_XSENS_EXTTRIG))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_DATA_SENT_OUT, 0) |           \
                                     PIN_AFIO_AF(GPIOB_SPI1_RST, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOB_JTDO, 0) |            \
                                     PIN_AFIO_AF(GPIOB_JTRST, 0) |           \
                                     PIN_AFIO_AF(GPIOB_NAND_WP, 0) |           \
                                     PIN_AFIO_AF(GPIOB_I2C1_SCL, 4) |            \
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA, 4))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_USB_DISCOVERY, 0) |           \
                                     PIN_AFIO_AF(GPIOB_USB_PRESENT, 0) |            \
                                     PIN_AFIO_AF(GPIOB_I2C2_SCL, 4) |         \
                                     PIN_AFIO_AF(GPIOB_I2C2_SDA, 4) |          \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOB_SPI2_CLK, 5) |          \
                                     PIN_AFIO_AF(GPIOB_SPI2_MISO, 5) |          \
                                     PIN_AFIO_AF(GPIOB_XSENS_EXTTRIG, 0))

/*
 * GPIOC setup:
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(GPIOC_PIN0) |\
                                     PIN_MODE_OUTPUT(GPIOC_SPI3_CS1) |           \
                                     PIN_MODE_OUTPUT(GPIOC_SPI3_CS2) |           \
                                     PIN_MODE_OUTPUT(GPIOC_FLASH_CS) |        \
                                     PIN_MODE_OUTPUT(GPIOC_SPI2_CS1) |           \
                                     PIN_MODE_OUTPUT(GPIOC_SPI2_CS2) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_XSENS_TX) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_XSENS_RX) |       \
                                     PIN_MODE_ALTERNATE(GPIOC_MPU6050_CLK) |           \
                                     PIN_MODE_OUTPUT(GPIOC_TP2) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_FLASH_CLK) |       \
                                     PIN_MODE_ALTERNATE(GPIOC_FLASH_MISO) |          \
                                     PIN_MODE_ALTERNATE(GPIOC_FLASH_MOSI) |       \
                                     PIN_MODE_OUTPUT(GPIOC_PPS_OUT) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_CS1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_CS2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_FLASH_CS) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI2_CS1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI2_CS2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_XSENS_TX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_XSENS_RX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_MPU6050_CLK) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_TP2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_FLASH_CLK) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_FLASH_MISO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_FLASH_MOSI) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PPS_OUT) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_2M(GPIOC_PIN0) |\
                                     PIN_OSPEED_2M(GPIOC_SPI3_CS1) |          \
                                     PIN_OSPEED_2M(GPIOC_SPI3_CS2) |          \
                                     PIN_OSPEED_2M(GPIOC_FLASH_CS) |       \
                                     PIN_OSPEED_2M(GPIOC_SPI2_CS1) |          \
                                     PIN_OSPEED_2M(GPIOC_SPI2_CS2) |          \
                                     PIN_OSPEED_2M(GPIOC_XSENS_TX) |          \
                                     PIN_OSPEED_2M(GPIOC_XSENS_RX) |          \
                                     PIN_OSPEED_100M(GPIOC_MPU6050_CLK) |          \
                                     PIN_OSPEED_100M(GPIOC_TP2) |          \
                                     PIN_OSPEED_100M(GPIOC_FLASH_CLK) |          \
                                     PIN_OSPEED_100M(GPIOC_FLASH_MISO) |         \
                                     PIN_OSPEED_100M(GPIOC_FLASH_MOSI) |          \
                                     PIN_OSPEED_100M(GPIOC_PPS_OUT) |         \
                                     PIN_OSPEED_100M(GPIOC_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0) |\
                                     PIN_PUPDR_PULLUP(GPIOC_SPI3_CS1) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_SPI3_CS2) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_FLASH_CS) |      \
                                     PIN_PUPDR_PULLUP(GPIOC_SPI2_CS1) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_SPI2_CS2) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_XSENS_TX) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_XSENS_RX) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_MPU6050_CLK) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_TP2) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_FLASH_CLK) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_FLASH_MISO) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_FLASH_MOSI) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_PPS_OUT) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |  \
                                     PIN_ODR_HIGH(GPIOC_SPI3_CS1) |             \
                                     PIN_ODR_HIGH(GPIOC_SPI3_CS2) |             \
                                     PIN_ODR_HIGH(GPIOC_FLASH_CS) |          \
                                     PIN_ODR_HIGH(GPIOC_SPI2_CS1) |             \
                                     PIN_ODR_HIGH(GPIOC_SPI2_CS2) |             \
                                     PIN_ODR_HIGH(GPIOC_XSENS_TX) |             \
                                     PIN_ODR_HIGH(GPIOC_XSENS_RX) |             \
                                     PIN_ODR_HIGH(GPIOC_MPU6050_CLK) |             \
                                     PIN_ODR_HIGH(GPIOC_TP2) |             \
                                     PIN_ODR_HIGH(GPIOC_FLASH_CLK) |             \
                                     PIN_ODR_HIGH(GPIOC_FLASH_MISO) |            \
                                     PIN_ODR_HIGH(GPIOC_FLASH_MOSI) |             \
                                     PIN_ODR_HIGH(GPIOC_PPS_OUT) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0) |\
                                     PIN_AFIO_AF(GPIOC_SPI3_CS1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_SPI3_CS2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_FLASH_CS, 0) |        \
                                     PIN_AFIO_AF(GPIOC_SPI2_CS1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_SPI2_CS2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_XSENS_TX, 8) |           \
                                     PIN_AFIO_AF(GPIOC_XSENS_RX, 8))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_MPU6050_CLK, 2) |           \
                                     PIN_AFIO_AF(GPIOC_TP2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_FLASH_CLK, 6) |           \
                                     PIN_AFIO_AF(GPIOC_FLASH_MISO, 6) |          \
                                     PIN_AFIO_AF(GPIOC_FLASH_MOSI, 6) |           \
                                     PIN_AFIO_AF(GPIOC_PPS_OUT, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0))

/*
 * GPIOD setup:
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_NAND_IO2) |           \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_IO3) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN3) |           \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_RE) |         \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_WE) |   \
                                     PIN_MODE_INPUT(GPIOD_NAND_RB_NWAIT) |           \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_CE) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_CLE) |          \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_ALE) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN13) |          \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_IO0) |          \
                                     PIN_MODE_ALTERNATE(GPIOD_NAND_IO1))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_NAND_IO2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_IO3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_RE) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_WE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_RB_NWAIT) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_CE) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_CLE) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_ALE) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_IO0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_NAND_IO1))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_100M(GPIOD_NAND_IO2) |          \
                                     PIN_OSPEED_100M(GPIOD_NAND_IO3) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOD_NAND_RE) |         \
                                     PIN_OSPEED_100M(GPIOD_NAND_WE) |  \
                                     PIN_OSPEED_100M(GPIOD_NAND_RB_NWAIT) |          \
                                     PIN_OSPEED_100M(GPIOD_NAND_CE) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOD_NAND_CLE) |         \
                                     PIN_OSPEED_100M(GPIOD_NAND_ALE) |          \
                                     PIN_OSPEED_100M(GPIOD_PIN13) |          \
                                     PIN_OSPEED_100M(GPIOD_NAND_IO0) |          \
                                     PIN_OSPEED_100M(GPIOD_NAND_IO1))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_NAND_IO2) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_IO3) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_RE) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_WE) |\
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_RB_NWAIT) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_CE) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_CLE) |      \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_ALE) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN13) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_IO0) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_NAND_IO1))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_NAND_IO2) |             \
                                     PIN_ODR_HIGH(GPIOD_NAND_IO3) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOD_NAND_RE) |            \
                                     PIN_ODR_HIGH(GPIOD_NAND_WE) |     \
                                     PIN_ODR_HIGH(GPIOD_NAND_RB_NWAIT) |             \
                                     PIN_ODR_HIGH(GPIOD_NAND_CE) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOD_NAND_CLE) |            \
                                     PIN_ODR_HIGH(GPIOD_NAND_ALE) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN13) |              \
                                     PIN_ODR_HIGH(GPIOD_NAND_IO0) |              \
                                     PIN_ODR_HIGH(GPIOD_NAND_IO1))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_NAND_IO2, 12) |           \
                                     PIN_AFIO_AF(GPIOD_NAND_IO3, 12) |           \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOD_NAND_RE, 12) |          \
                                     PIN_AFIO_AF(GPIOD_NAND_WE, 12) |   \
                                     PIN_AFIO_AF(GPIOD_NAND_RB_NWAIT, 0) |           \
                                     PIN_AFIO_AF(GPIOD_NAND_CE, 12))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOD_NAND_CLE, 12) |          \
                                     PIN_AFIO_AF(GPIOD_NAND_ALE, 12) |           \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0) |           \
                                     PIN_AFIO_AF(GPIOD_NAND_IO0, 12) |           \
                                     PIN_AFIO_AF(GPIOD_NAND_IO1, 12))

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_MPU6050_INT1) |           \
                                     PIN_MODE_INPUT(GPIOE_MPU6050_INT2) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOE_SPI1_INT) |        \
                                     PIN_MODE_INPUT(GPIOE_PPS_IN) |           \
                                     PIN_MODE_INPUT(GPIOE_DATA_SENT_IN) |           \
                                     PIN_MODE_INPUT(GPIOE_NAND_RB_INT) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_NAND_IO4) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_NAND_IO5) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_NAND_IO6) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_NAND_IO7) |          \
                                     PIN_MODE_OUTPUT(GPIOE_LED_R) |          \
                                     PIN_MODE_OUTPUT(GPIOE_LED_G) |          \
                                     PIN_MODE_ALTERNATE(GPIOE_FLASH_WP) |          \
                                     PIN_MODE_ALTERNATE(GPIOE_FLASH_RST) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_MPU6050_INT1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_MPU6050_INT2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SPI1_INT) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PPS_IN) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_DATA_SENT_IN) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_NAND_RB_INT) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_NAND_IO4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_NAND_IO5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_NAND_IO6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_NAND_IO7) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_LED_R) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_LED_G) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FLASH_WP) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_FLASH_RST) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_100M(GPIOE_MPU6050_INT1) |          \
                                     PIN_OSPEED_100M(GPIOE_MPU6050_INT2) |          \
                                     PIN_OSPEED_100M(GPIOE_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOE_SPI1_INT) |        \
                                     PIN_OSPEED_100M(GPIOE_PPS_IN) |          \
                                     PIN_OSPEED_100M(GPIOE_DATA_SENT_IN) |          \
                                     PIN_OSPEED_100M(GPIOE_NAND_RB_INT) |          \
                                     PIN_OSPEED_100M(GPIOE_NAND_IO4) |          \
                                     PIN_OSPEED_100M(GPIOE_NAND_IO5) |          \
                                     PIN_OSPEED_100M(GPIOE_NAND_IO6) |          \
                                     PIN_OSPEED_100M(GPIOE_NAND_IO7) |         \
                                     PIN_OSPEED_2M(GPIOE_LED_R) |         \
                                     PIN_OSPEED_2M(GPIOE_LED_G) |         \
                                     PIN_OSPEED_2M(GPIOE_FLASH_WP) |         \
                                     PIN_OSPEED_2M(GPIOE_FLASH_RST) |         \
                                     PIN_OSPEED_100M(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOE_MPU6050_INT1) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOE_MPU6050_INT2) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN2) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_SPI1_INT) |     \
                                     PIN_PUPDR_PULLUP(GPIOE_PPS_IN) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_DATA_SENT_IN) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_NAND_RB_INT) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_NAND_IO4) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_NAND_IO5) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_NAND_IO6) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_NAND_IO7) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOE_LED_R) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOE_LED_G) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_FLASH_WP) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_FLASH_RST) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_MPU6050_INT1) |             \
                                     PIN_ODR_HIGH(GPIOE_MPU6050_INT2) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOE_SPI1_INT) |           \
                                     PIN_ODR_HIGH(GPIOE_PPS_IN) |             \
                                     PIN_ODR_HIGH(GPIOE_DATA_SENT_IN) |             \
                                     PIN_ODR_HIGH(GPIOE_NAND_RB_INT) |             \
                                     PIN_ODR_HIGH(GPIOE_NAND_IO4) |             \
                                     PIN_ODR_HIGH(GPIOE_NAND_IO5) |             \
                                     PIN_ODR_HIGH(GPIOE_NAND_IO6) |             \
                                     PIN_ODR_HIGH(GPIOE_NAND_IO7) |            \
                                     PIN_ODR_LOW(GPIOE_LED_R) |            \
                                     PIN_ODR_LOW(GPIOE_LED_G) |            \
                                     PIN_ODR_HIGH(GPIOE_FLASH_WP) |            \
                                     PIN_ODR_HIGH(GPIOE_FLASH_RST) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_MPU6050_INT1, 0) |           \
                                     PIN_AFIO_AF(GPIOE_MPU6050_INT2, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOE_SPI1_INT, 0) |         \
                                     PIN_AFIO_AF(GPIOE_PPS_IN, 0) |           \
                                     PIN_AFIO_AF(GPIOE_DATA_SENT_IN, 0) |           \
                                     PIN_AFIO_AF(GPIOE_NAND_RB_INT, 0) |           \
                                     PIN_AFIO_AF(GPIOE_NAND_IO4, 12))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_NAND_IO5, 12) |           \
                                     PIN_AFIO_AF(GPIOE_NAND_IO6, 12) |           \
                                     PIN_AFIO_AF(GPIOE_NAND_IO7, 12) |          \
                                     PIN_AFIO_AF(GPIOE_LED_R, 0) |          \
                                     PIN_AFIO_AF(GPIOE_LED_G, 0) |          \
                                     PIN_AFIO_AF(GPIOE_FLASH_WP, 0) |          \
                                     PIN_AFIO_AF(GPIOE_FLASH_RST, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_100M(GPIOF_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_100M(GPIOG_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0))

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
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_100M(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_100M(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_100M(GPIOH_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN15))
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
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_100M(GPIOI_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN15))
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

#define nand_wp_assert()    palClearPad(GPIOB, GPIOB_NAND_WP)
#define nand_wp_release()   palSetPad(GPIOB, GPIOB_NAND_WP)
#define red_led_on()        palSetPad(GPIOE, GPIOE_LED_R)
#define red_led_off()       palClearPad(GPIOE, GPIOE_LED_R)

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
