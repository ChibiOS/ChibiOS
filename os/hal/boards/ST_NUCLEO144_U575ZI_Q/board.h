/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32 Nucleo144-U575ZI-Q board.
 *
 * This is a first integration step for the STM32U5xx HAL port. The GPIO
 * mapping follows the existing low-power Nucleo144 layout already used by the
 * L552/L4P5 boards: green LED on PC7, user button on PC13, STLINK VCOM on
 * PG7/PG8 through LPUART1.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_NUCLEO144_U575ZI_Q
#define BOARD_NAME                  "STMicroelectronics STM32 Nucleo144-U575ZI-Q"

/*
 * Board oscillators-related settings.
 * NOTE: HSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                0U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#undef STM32U575xx
#define STM32U575xx

/*
 * IO pins assignments.
 */
#define GPIOC_LED1                  7U
#define GPIOC_LED_GREEN             7U
#define GPIOC_BUTTON                13U

#define GPIOG_LPUART1_TX            7U
#define GPIOG_LPUART1_RX            8U

/*
 * IO lines assignments.
 */
#define LINE_LED1                   PAL_LINE(GPIOC, GPIOC_LED1)
#define LINE_LED_GREEN              PAL_LINE(GPIOC, GPIOC_LED_GREEN)
#define LINE_BUTTON                 PAL_LINE(GPIOC, GPIOC_BUTTON)

#define LINE_LPUART1_TX             PAL_LINE(GPIOG, GPIOG_LPUART1_TX)
#define LINE_LPUART1_RX             PAL_LINE(GPIOG, GPIOG_LPUART1_RX)

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))
#define PIN_LOCKR_DISABLED(n)       (0U << (n))

/*
 * GPIOA setup:
 *
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * Other pins are left in analog mode in this first board support step.
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(0U) |                  \
                                     PIN_MODE_ANALOG(1U) |                  \
                                     PIN_MODE_ANALOG(2U) |                  \
                                     PIN_MODE_ANALOG(3U) |                  \
                                     PIN_MODE_ANALOG(4U) |                  \
                                     PIN_MODE_ANALOG(5U) |                  \
                                     PIN_MODE_ANALOG(6U) |                  \
                                     PIN_MODE_ANALOG(7U) |                  \
                                     PIN_MODE_ANALOG(8U) |                  \
                                     PIN_MODE_ANALOG(9U) |                  \
                                     PIN_MODE_ANALOG(10U) |                 \
                                     PIN_MODE_ANALOG(11U) |                 \
                                     PIN_MODE_ANALOG(12U) |                 \
                                     PIN_MODE_ALTERNATE(13U) |              \
                                     PIN_MODE_ALTERNATE(14U) |              \
                                     PIN_MODE_ANALOG(15U))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(0U) |               \
                                     PIN_OTYPE_PUSHPULL(1U) |               \
                                     PIN_OTYPE_PUSHPULL(2U) |               \
                                     PIN_OTYPE_PUSHPULL(3U) |               \
                                     PIN_OTYPE_PUSHPULL(4U) |               \
                                     PIN_OTYPE_PUSHPULL(5U) |               \
                                     PIN_OTYPE_PUSHPULL(6U) |               \
                                     PIN_OTYPE_PUSHPULL(7U) |               \
                                     PIN_OTYPE_PUSHPULL(8U) |               \
                                     PIN_OTYPE_PUSHPULL(9U) |               \
                                     PIN_OTYPE_PUSHPULL(10U) |              \
                                     PIN_OTYPE_PUSHPULL(11U) |              \
                                     PIN_OTYPE_PUSHPULL(12U) |              \
                                     PIN_OTYPE_PUSHPULL(13U) |              \
                                     PIN_OTYPE_PUSHPULL(14U) |              \
                                     PIN_OTYPE_PUSHPULL(15U))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(0U) |               \
                                     PIN_OSPEED_VERYLOW(1U) |               \
                                     PIN_OSPEED_VERYLOW(2U) |               \
                                     PIN_OSPEED_VERYLOW(3U) |               \
                                     PIN_OSPEED_VERYLOW(4U) |               \
                                     PIN_OSPEED_VERYLOW(5U) |               \
                                     PIN_OSPEED_VERYLOW(6U) |               \
                                     PIN_OSPEED_VERYLOW(7U) |               \
                                     PIN_OSPEED_VERYLOW(8U) |               \
                                     PIN_OSPEED_VERYLOW(9U) |               \
                                     PIN_OSPEED_VERYLOW(10U) |              \
                                     PIN_OSPEED_VERYLOW(11U) |              \
                                     PIN_OSPEED_VERYLOW(12U) |              \
                                     PIN_OSPEED_HIGH(13U) |                 \
                                     PIN_OSPEED_HIGH(14U) |                 \
                                     PIN_OSPEED_VERYLOW(15U))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(0U) |               \
                                     PIN_PUPDR_FLOATING(1U) |               \
                                     PIN_PUPDR_FLOATING(2U) |               \
                                     PIN_PUPDR_FLOATING(3U) |               \
                                     PIN_PUPDR_FLOATING(4U) |               \
                                     PIN_PUPDR_FLOATING(5U) |               \
                                     PIN_PUPDR_FLOATING(6U) |               \
                                     PIN_PUPDR_FLOATING(7U) |               \
                                     PIN_PUPDR_FLOATING(8U) |               \
                                     PIN_PUPDR_FLOATING(9U) |               \
                                     PIN_PUPDR_FLOATING(10U) |              \
                                     PIN_PUPDR_FLOATING(11U) |              \
                                     PIN_PUPDR_FLOATING(12U) |              \
                                     PIN_PUPDR_FLOATING(13U) |              \
                                     PIN_PUPDR_FLOATING(14U) |              \
                                     PIN_PUPDR_FLOATING(15U))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(0U) |                      \
                                     PIN_ODR_LOW(1U) |                      \
                                     PIN_ODR_LOW(2U) |                      \
                                     PIN_ODR_LOW(3U) |                      \
                                     PIN_ODR_LOW(4U) |                      \
                                     PIN_ODR_LOW(5U) |                      \
                                     PIN_ODR_LOW(6U) |                      \
                                     PIN_ODR_LOW(7U) |                      \
                                     PIN_ODR_LOW(8U) |                      \
                                     PIN_ODR_LOW(9U) |                      \
                                     PIN_ODR_LOW(10U) |                     \
                                     PIN_ODR_LOW(11U) |                     \
                                     PIN_ODR_LOW(12U) |                     \
                                     PIN_ODR_LOW(13U) |                     \
                                     PIN_ODR_LOW(14U) |                     \
                                     PIN_ODR_LOW(15U))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(0U, 0U) |                  \
                                     PIN_AFIO_AF(1U, 0U) |                  \
                                     PIN_AFIO_AF(2U, 0U) |                  \
                                     PIN_AFIO_AF(3U, 0U) |                  \
                                     PIN_AFIO_AF(4U, 0U) |                  \
                                     PIN_AFIO_AF(5U, 0U) |                  \
                                     PIN_AFIO_AF(6U, 0U) |                  \
                                     PIN_AFIO_AF(7U, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(8U, 0U) |                  \
                                     PIN_AFIO_AF(9U, 0U) |                  \
                                     PIN_AFIO_AF(10U, 0U) |                 \
                                     PIN_AFIO_AF(11U, 0U) |                 \
                                     PIN_AFIO_AF(12U, 0U) |                 \
                                     PIN_AFIO_AF(13U, 0U) |                 \
                                     PIN_AFIO_AF(14U, 0U) |                 \
                                     PIN_AFIO_AF(15U, 0U))
#define VAL_GPIOA_LOCKR             (PIN_LOCKR_DISABLED(0U) |               \
                                     PIN_LOCKR_DISABLED(1U) |               \
                                     PIN_LOCKR_DISABLED(2U) |               \
                                     PIN_LOCKR_DISABLED(3U) |               \
                                     PIN_LOCKR_DISABLED(4U) |               \
                                     PIN_LOCKR_DISABLED(5U) |               \
                                     PIN_LOCKR_DISABLED(6U) |               \
                                     PIN_LOCKR_DISABLED(7U) |               \
                                     PIN_LOCKR_DISABLED(8U) |               \
                                     PIN_LOCKR_DISABLED(9U) |               \
                                     PIN_LOCKR_DISABLED(10U) |              \
                                     PIN_LOCKR_DISABLED(11U) |              \
                                     PIN_LOCKR_DISABLED(12U) |              \
                                     PIN_LOCKR_DISABLED(13U) |              \
                                     PIN_LOCKR_DISABLED(14U) |              \
                                     PIN_LOCKR_DISABLED(15U))

/*
 * GPIOB setup:
 *
 * PB3  - SWO                       (alternate 0).
 * Other pins are left in analog mode.
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(0U) |                  \
                                     PIN_MODE_ANALOG(1U) |                  \
                                     PIN_MODE_ANALOG(2U) |                  \
                                     PIN_MODE_ALTERNATE(3U) |               \
                                     PIN_MODE_ANALOG(4U) |                  \
                                     PIN_MODE_ANALOG(5U) |                  \
                                     PIN_MODE_ANALOG(6U) |                  \
                                     PIN_MODE_ANALOG(7U) |                  \
                                     PIN_MODE_ANALOG(8U) |                  \
                                     PIN_MODE_ANALOG(9U) |                  \
                                     PIN_MODE_ANALOG(10U) |                 \
                                     PIN_MODE_ANALOG(11U) |                 \
                                     PIN_MODE_ANALOG(12U) |                 \
                                     PIN_MODE_ANALOG(13U) |                 \
                                     PIN_MODE_ANALOG(14U) |                 \
                                     PIN_MODE_ANALOG(15U))
#define VAL_GPIOB_OTYPER            (0x00000000U)
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(0U) |               \
                                     PIN_OSPEED_VERYLOW(1U) |               \
                                     PIN_OSPEED_VERYLOW(2U) |               \
                                     PIN_OSPEED_HIGH(3U) |                  \
                                     PIN_OSPEED_VERYLOW(4U) |               \
                                     PIN_OSPEED_VERYLOW(5U) |               \
                                     PIN_OSPEED_VERYLOW(6U) |               \
                                     PIN_OSPEED_VERYLOW(7U) |               \
                                     PIN_OSPEED_VERYLOW(8U) |               \
                                     PIN_OSPEED_VERYLOW(9U) |               \
                                     PIN_OSPEED_VERYLOW(10U) |              \
                                     PIN_OSPEED_VERYLOW(11U) |              \
                                     PIN_OSPEED_VERYLOW(12U) |              \
                                     PIN_OSPEED_VERYLOW(13U) |              \
                                     PIN_OSPEED_VERYLOW(14U) |              \
                                     PIN_OSPEED_VERYLOW(15U))
#define VAL_GPIOB_PUPDR             (0x00000000U)
#define VAL_GPIOB_ODR               (0x00000000U)
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0U, 0U) |                  \
                                     PIN_AFIO_AF(1U, 0U) |                  \
                                     PIN_AFIO_AF(2U, 0U) |                  \
                                     PIN_AFIO_AF(3U, 0U) |                  \
                                     PIN_AFIO_AF(4U, 0U) |                  \
                                     PIN_AFIO_AF(5U, 0U) |                  \
                                     PIN_AFIO_AF(6U, 0U) |                  \
                                     PIN_AFIO_AF(7U, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(8U, 0U) |                  \
                                     PIN_AFIO_AF(9U, 0U) |                  \
                                     PIN_AFIO_AF(10U, 0U) |                 \
                                     PIN_AFIO_AF(11U, 0U) |                 \
                                     PIN_AFIO_AF(12U, 0U) |                 \
                                     PIN_AFIO_AF(13U, 0U) |                 \
                                     PIN_AFIO_AF(14U, 0U) |                 \
                                     PIN_AFIO_AF(15U, 0U))
#define VAL_GPIOB_LOCKR             (0x00000000U)

/*
 * GPIOC setup:
 *
 * PC2  - VBUS_SENSE                (input floating).
 * PC7  - LED1 LED_GREEN            (output pushpull maximum).
 * PC13 - BUTTON                    (input floating).
 * Other pins are left in analog mode.
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(0U) |                  \
                                     PIN_MODE_ANALOG(1U) |                  \
                                     PIN_MODE_INPUT(2U) |                   \
                                     PIN_MODE_ANALOG(3U) |                  \
                                     PIN_MODE_ANALOG(4U) |                  \
                                     PIN_MODE_ANALOG(5U) |                  \
                                     PIN_MODE_ANALOG(6U) |                  \
                                     PIN_MODE_OUTPUT(GPIOC_LED1) |          \
                                     PIN_MODE_ANALOG(8U) |                  \
                                     PIN_MODE_ANALOG(9U) |                  \
                                     PIN_MODE_ANALOG(10U) |                 \
                                     PIN_MODE_ANALOG(11U) |                 \
                                     PIN_MODE_ANALOG(12U) |                 \
                                     PIN_MODE_INPUT(GPIOC_BUTTON) |         \
                                     PIN_MODE_ANALOG(14U) |                 \
                                     PIN_MODE_ANALOG(15U))
#define VAL_GPIOC_OTYPER            (0x00000000U)
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(0U) |               \
                                     PIN_OSPEED_VERYLOW(1U) |               \
                                     PIN_OSPEED_VERYLOW(2U) |               \
                                     PIN_OSPEED_VERYLOW(3U) |               \
                                     PIN_OSPEED_VERYLOW(4U) |               \
                                     PIN_OSPEED_VERYLOW(5U) |               \
                                     PIN_OSPEED_VERYLOW(6U) |               \
                                     PIN_OSPEED_HIGH(GPIOC_LED1) |          \
                                     PIN_OSPEED_VERYLOW(8U) |               \
                                     PIN_OSPEED_VERYLOW(9U) |               \
                                     PIN_OSPEED_VERYLOW(10U) |              \
                                     PIN_OSPEED_VERYLOW(11U) |              \
                                     PIN_OSPEED_VERYLOW(12U) |              \
                                     PIN_OSPEED_VERYLOW(GPIOC_BUTTON) |     \
                                     PIN_OSPEED_VERYLOW(14U) |              \
                                     PIN_OSPEED_VERYLOW(15U))
#define VAL_GPIOC_PUPDR             (0x00000000U)
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(0U) |                      \
                                     PIN_ODR_LOW(1U) |                      \
                                     PIN_ODR_LOW(2U) |                      \
                                     PIN_ODR_LOW(3U) |                      \
                                     PIN_ODR_LOW(4U) |                      \
                                     PIN_ODR_LOW(5U) |                      \
                                     PIN_ODR_LOW(6U) |                      \
                                     PIN_ODR_LOW(GPIOC_LED1) |              \
                                     PIN_ODR_LOW(8U) |                      \
                                     PIN_ODR_LOW(9U) |                      \
                                     PIN_ODR_LOW(10U) |                     \
                                     PIN_ODR_LOW(11U) |                     \
                                     PIN_ODR_LOW(12U) |                     \
                                     PIN_ODR_LOW(GPIOC_BUTTON) |            \
                                     PIN_ODR_LOW(14U) |                     \
                                     PIN_ODR_LOW(15U))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(0U, 0U) |                  \
                                     PIN_AFIO_AF(1U, 0U) |                  \
                                     PIN_AFIO_AF(2U, 0U) |                  \
                                     PIN_AFIO_AF(3U, 0U) |                  \
                                     PIN_AFIO_AF(4U, 0U) |                  \
                                     PIN_AFIO_AF(5U, 0U) |                  \
                                     PIN_AFIO_AF(6U, 0U) |                  \
                                     PIN_AFIO_AF(GPIOC_LED1, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(8U, 0U) |                  \
                                     PIN_AFIO_AF(9U, 0U) |                  \
                                     PIN_AFIO_AF(10U, 0U) |                 \
                                     PIN_AFIO_AF(11U, 0U) |                 \
                                     PIN_AFIO_AF(12U, 0U) |                 \
                                     PIN_AFIO_AF(GPIOC_BUTTON, 0U) |        \
                                     PIN_AFIO_AF(14U, 0U) |                 \
                                     PIN_AFIO_AF(15U, 0U))
#define VAL_GPIOC_LOCKR             (0x00000000U)

/*
 * GPIOD, GPIOE and GPIOF are left fully in analog mode.
 */
#define VAL_GPIOD_MODER             0xFFFFFFFFU
#define VAL_GPIOD_OTYPER            0x00000000U
#define VAL_GPIOD_OSPEEDR           0x00000000U
#define VAL_GPIOD_PUPDR             0x00000000U
#define VAL_GPIOD_ODR               0x00000000U
#define VAL_GPIOD_AFRL              0x00000000U
#define VAL_GPIOD_AFRH              0x00000000U
#define VAL_GPIOD_LOCKR             0x00000000U

#define VAL_GPIOE_MODER             0xFFFFFFFFU
#define VAL_GPIOE_OTYPER            0x00000000U
#define VAL_GPIOE_OSPEEDR           0x00000000U
#define VAL_GPIOE_PUPDR             0x00000000U
#define VAL_GPIOE_ODR               0x00000000U
#define VAL_GPIOE_AFRL              0x00000000U
#define VAL_GPIOE_AFRH              0x00000000U
#define VAL_GPIOE_LOCKR             0x00000000U

#define VAL_GPIOF_MODER             0xFFFFFFFFU
#define VAL_GPIOF_OTYPER            0x00000000U
#define VAL_GPIOF_OSPEEDR           0x00000000U
#define VAL_GPIOF_PUPDR             0x00000000U
#define VAL_GPIOF_ODR               0x00000000U
#define VAL_GPIOF_AFRL              0x00000000U
#define VAL_GPIOF_AFRH              0x00000000U
#define VAL_GPIOF_LOCKR             0x00000000U

/*
 * GPIOG setup:
 *
 * PG7  - LPUART1_TX                (alternate 8).
 * PG8  - LPUART1_RX                (alternate 8).
 * Other pins are left in analog mode.
 */
#define VAL_GPIOG_MODER             (PIN_MODE_ANALOG(0U) |                  \
                                     PIN_MODE_ANALOG(1U) |                  \
                                     PIN_MODE_ANALOG(2U) |                  \
                                     PIN_MODE_ANALOG(3U) |                  \
                                     PIN_MODE_ANALOG(4U) |                  \
                                     PIN_MODE_ANALOG(5U) |                  \
                                     PIN_MODE_ANALOG(6U) |                  \
                                     PIN_MODE_ALTERNATE(GPIOG_LPUART1_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOG_LPUART1_RX) | \
                                     PIN_MODE_ANALOG(9U) |                  \
                                     PIN_MODE_ANALOG(10U) |                 \
                                     PIN_MODE_ANALOG(11U) |                 \
                                     PIN_MODE_ANALOG(12U) |                 \
                                     PIN_MODE_ANALOG(13U) |                 \
                                     PIN_MODE_ANALOG(14U) |                 \
                                     PIN_MODE_ANALOG(15U))
#define VAL_GPIOG_OTYPER            (0x00000000U)
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_VERYLOW(0U) |               \
                                     PIN_OSPEED_VERYLOW(1U) |               \
                                     PIN_OSPEED_VERYLOW(2U) |               \
                                     PIN_OSPEED_VERYLOW(3U) |               \
                                     PIN_OSPEED_VERYLOW(4U) |               \
                                     PIN_OSPEED_VERYLOW(5U) |               \
                                     PIN_OSPEED_VERYLOW(6U) |               \
                                     PIN_OSPEED_HIGH(GPIOG_LPUART1_TX) |    \
                                     PIN_OSPEED_HIGH(GPIOG_LPUART1_RX) |    \
                                     PIN_OSPEED_VERYLOW(9U) |               \
                                     PIN_OSPEED_VERYLOW(10U) |              \
                                     PIN_OSPEED_VERYLOW(11U) |              \
                                     PIN_OSPEED_VERYLOW(12U) |              \
                                     PIN_OSPEED_VERYLOW(13U) |              \
                                     PIN_OSPEED_VERYLOW(14U) |              \
                                     PIN_OSPEED_VERYLOW(15U))
#define VAL_GPIOG_PUPDR             (0x00000000U)
#define VAL_GPIOG_ODR               (0x00000000U)
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(0U, 0U) |                  \
                                     PIN_AFIO_AF(1U, 0U) |                  \
                                     PIN_AFIO_AF(2U, 0U) |                  \
                                     PIN_AFIO_AF(3U, 0U) |                  \
                                     PIN_AFIO_AF(4U, 0U) |                  \
                                     PIN_AFIO_AF(5U, 0U) |                  \
                                     PIN_AFIO_AF(6U, 0U) |                  \
                                     PIN_AFIO_AF(GPIOG_LPUART1_TX, 8U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_LPUART1_RX, 8U) |    \
                                     PIN_AFIO_AF(9U, 0U) |                  \
                                     PIN_AFIO_AF(10U, 0U) |                 \
                                     PIN_AFIO_AF(11U, 0U) |                 \
                                     PIN_AFIO_AF(12U, 0U) |                 \
                                     PIN_AFIO_AF(13U, 0U) |                 \
                                     PIN_AFIO_AF(14U, 0U) |                 \
                                     PIN_AFIO_AF(15U, 0U))
#define VAL_GPIOG_LOCKR             (0x00000000U)

/*
 * GPIOH is left fully in analog mode.
 */
#define VAL_GPIOH_MODER             0xFFFFFFFFU
#define VAL_GPIOH_OTYPER            0x00000000U
#define VAL_GPIOH_OSPEEDR           0x00000000U
#define VAL_GPIOH_PUPDR             0x00000000U
#define VAL_GPIOH_ODR               0x00000000U
#define VAL_GPIOH_AFRL              0x00000000U
#define VAL_GPIOH_AFRH              0x00000000U
#define VAL_GPIOH_LOCKR             0x00000000U

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void __early_init(void);
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
