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
 * @file    RP2040/rp_registry.h
 * @brief   RP2040 capabilities registry.
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
 * @name    RP2040 capabilities
 * @{
 */

/*===========================================================================*/
/* Common.                                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* RP2040.                                                                   */
/*===========================================================================*/

/* GPIO attributes.*/
#define RP_GPIO_NUM_LINES                   30
#define RP_GPIO_INTR_REGS                   4

/* UART attributes.*/
#define RP_HAS_UART0                        TRUE
#define RP_HAS_UART1                        TRUE

/* PIO attributes.*/
#define RP_HAS_PIO0                         TRUE
#define RP_HAS_PIO1                         TRUE
#define RP_HAS_PIO2                         FALSE

/* TIMER attributes.*/
#define RP_HAS_TIMER0                       TRUE
#define RP_ST_NUM_ALARMS                    4

/* RTC attributes.*/
#define RP_HAS_RTC                          TRUE

/* SPI attributes.*/
#define RP_HAS_SPI0                         TRUE
#define RP_HAS_SPI1                         TRUE

/* I2C attributes.*/
#define RP_HAS_I2C0                         TRUE
#define RP_HAS_I2C1                         TRUE

/* DMA attributes.*/
#define RP_HAS_DMA                          TRUE
#define RP_DMA_NUM_CHANNELS                 12

/* WDG attributes.*/
#define RP_HAS_WDG                          TRUE
#define RP_WDG_STORAGE_SIZE                 32U

/* USB attributes.*/
#define RP_HAS_USB                          TRUE
#define USB_ENDPOINTS_NUMBER                15

/* Flash attributes.*/
#define RP_HAS_FLASH                        TRUE

/** @} */

#endif /* RP_REGISTRY_H */

/** @} */
