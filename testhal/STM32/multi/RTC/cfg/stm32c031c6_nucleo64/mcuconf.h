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

#ifndef TESTHAL_STM32_MULTI_RTC_STM32C031C6_NUCLEO64_MCUCONF_WRAPPER_H
#define TESTHAL_STM32_MULTI_RTC_STM32C031C6_NUCLEO64_MCUCONF_WRAPPER_H

#include "../../../../../../demos/STM32/RT-STM32-MULTI/cfg/stm32c031c6_nucleo64/mcuconf.h"

#undef STM32_LSE_ENABLED
#define STM32_LSE_ENABLED                   FALSE

#undef STM32_RTCSEL
#define STM32_RTCSEL                        RCC_CSR1_RTCSEL_LSI

#define STM32_RTC_PRESA_VALUE               32
#define STM32_RTC_PRESS_VALUE               1024
#define STM32_RTC_CR_INIT                   0

#endif /* TESTHAL_STM32_MULTI_RTC_STM32C031C6_NUCLEO64_MCUCONF_WRAPPER_H */
