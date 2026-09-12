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

/**
 * @file    RP2350/rp_registry.h
 * @brief   RP2350 capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef RP_REGISTRY_H
#define RP_REGISTRY_H

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    RP2350 capabilities
 * @{
 */

/*===========================================================================*/
/* Common.                                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* RP2350                                                                   */
/*===========================================================================*/

/* GPIO attributes.*/
#define RP_GPIO_NUM_LINES                   48
#define RP_GPIO_INTR_REGS                   6

/* UART attributes.*/
#define RP_HAS_UART0                        TRUE
#define RP_HAS_UART1                        TRUE

/* PIO attributes.*/
#define RP_HAS_PIO0                         TRUE
#define RP_HAS_PIO1                         TRUE
#define RP_HAS_PIO2                         TRUE
#define RP_PIO_NUM_BLOCKS                   3

/* TIMER attributes.*/
#define RP_HAS_TIMER0                       TRUE
#define RP_HAS_TIMER1                       TRUE
#define RP_ST_NUM_ALARMS                    4

/* RP2350 does NOT have an RTC */
#define RP_HAS_RTC                          FALSE

/* SPI attributes.*/
#define RP_HAS_SPI0                         TRUE
#define RP_HAS_SPI1                         TRUE

/* I2C attributes.*/
#define RP_HAS_I2C0                         TRUE
#define RP_HAS_I2C1                         TRUE

/* DMA attributes.*/
#define RP_HAS_DMA                          TRUE
#define RP_DMA_NUM_CHANNELS                 16

/* WDG attributes.*/
#define RP_HAS_WDG                          TRUE
#define RP_WDG_STORAGE_SIZE                 32U
#define RP_PSM_WDSEL_ALL_BITS               0x1FFFFFFU
#define RP_WDG_HAS_E1_ERRATA                FALSE

/* USB attributes.*/
#define RP_HAS_USB                          TRUE
#define USB_ENDPOINTS_NUMBER                15

/* Flash attributes.*/
#define RP_HAS_FLASH                        TRUE

#define RP_HAS_HSTX                         TRUE
#define RP_HAS_TRNG                         TRUE
#define RP_HAS_SHA256                       TRUE
#define RP_HAS_OTP                          TRUE

/* PWM attributes.*/
#define RP_HAS_PWM                          TRUE
#define RP_PWM_NUM_SLICES                   12
#define RP_HAS_PWM0                         TRUE
#define RP_HAS_PWM1                         TRUE
#define RP_HAS_PWM2                         TRUE
#define RP_HAS_PWM3                         TRUE
#define RP_HAS_PWM4                         TRUE
#define RP_HAS_PWM5                         TRUE
#define RP_HAS_PWM6                         TRUE
#define RP_HAS_PWM7                         TRUE
#define RP_HAS_PWM8                         TRUE
#define RP_HAS_PWM9                         TRUE
#define RP_HAS_PWM10                        TRUE
#define RP_HAS_PWM11                        TRUE

/* ADC attributes.
   RP2350A (QFN-60): 4 external inputs on GPIO26-29 (AINSEL 0-3), temp on AINSEL 4.
   RP2350B (QFN-80): 8 external inputs on GPIO40-47 (AINSEL 0-7), temp on AINSEL 8.
   Select QFN-80 variant by defining RP2350B_QFN80=1 in the board build flags. */
#define RP_HAS_ADC                          TRUE
#if defined(RP2350B_QFN80)
/* RP2350B QFN-80 (Laurel carrier board): GPIO40-47 = AINSEL 0-7, temp on AINSEL 8. */
#define RP_ADC_NUM_CHANNELS                 9U
#define RP_ADC_TEMPERATURE_CHANNEL          8U
#define RP_ADC_BASE_PIN                     40U
#else
/* RP2350A QFN-60 (Pico 2): GPIO26-29 = AINSEL 0-3, temp on AINSEL 4. */
#define RP_ADC_NUM_CHANNELS                 5U
#define RP_ADC_TEMPERATURE_CHANNEL          4U
#define RP_ADC_BASE_PIN                     26U
#endif
#define RP_ADC_HAS_TEMPERATURE_SENSOR       TRUE
#define RP_ADC_FIFO_DEPTH                   8U
#define RP_ADC_DREQ                         48U
#define RP_ADC_AINSEL_BITS                  4U
#define RP_ADC_RROBIN_MASK                  0x01FF0000U

/** @} */

#endif /* RP_REGISTRY_H */

/** @} */
