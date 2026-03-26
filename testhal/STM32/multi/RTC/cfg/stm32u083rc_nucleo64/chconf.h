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

#ifndef TESTHAL_STM32_MULTI_RTC_STM32U083RC_NUCLEO64_CHCONF_WRAPPER_H
#define TESTHAL_STM32_MULTI_RTC_STM32U083RC_NUCLEO64_CHCONF_WRAPPER_H

#define CH_CFG_ST_RESOLUTION                16
#define CH_CFG_ST_FREQUENCY                 1000
#define CH_CFG_USE_TM                       FALSE
#define CH_DBG_SYSTEM_STATE_CHECK           FALSE
#define CH_DBG_ENABLE_CHECKS                FALSE
#define CH_DBG_ENABLE_ASSERTS               FALSE
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#define CH_DBG_ENABLE_STACK_CHECK           FALSE
#define CH_DBG_FILL_THREADS                 FALSE

#include "../../../../../../os/rt/templates/chconf.h"

#endif /* TESTHAL_STM32_MULTI_RTC_STM32U083RC_NUCLEO64_CHCONF_WRAPPER_H */
