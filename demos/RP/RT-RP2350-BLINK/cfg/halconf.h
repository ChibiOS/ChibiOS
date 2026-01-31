/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#ifndef HALCONF_H
#define HALCONF_H

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_9_0_

#include "mcuconf.h"

#define HAL_USE_PAL                         TRUE
#define HAL_USE_ADC                         FALSE
#define HAL_USE_CAN                         FALSE
#define HAL_USE_CRY                         FALSE
#define HAL_USE_DAC                         FALSE
#define HAL_USE_EFL                         FALSE
#define HAL_USE_GPT                         FALSE
#define HAL_USE_I2C                         FALSE
#define HAL_USE_I2S                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MAC                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SERIAL                      FALSE
#define HAL_USE_SERIAL_USB                  FALSE
#define HAL_USE_SIO                         TRUE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_TRNG                        FALSE
#define HAL_USE_UART                        FALSE
#define HAL_USE_USB                         FALSE
#define HAL_USE_WDG                         FALSE
#define HAL_USE_WSPI                        FALSE

#define PAL_USE_CALLBACKS                   FALSE
#define PAL_USE_WAIT                        FALSE

#endif /* HALCONF_H */
