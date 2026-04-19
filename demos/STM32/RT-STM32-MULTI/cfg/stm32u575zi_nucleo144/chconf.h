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

#ifndef STM32U575ZI_NUCLEO144_CHCONF_H
#define STM32U575ZI_NUCLEO144_CHCONF_H

/*
 * Keep the first STM32U5xx integration on the periodic tick path so the demo
 * only depends on the core SysTick support and not yet on the timer-backed
 * free-running time base.
 */
#define CH_CFG_ST_TIMEDELTA                 0

/*
 * Keep the build-driving options explicit in this file because rt.mk/oslib.mk
 * only inspect the local #define lines when USE_SMART_BUILD is enabled.
 */
#define CH_CFG_USE_TM                       TRUE
#define CH_CFG_USE_REGISTRY                 TRUE
#define CH_CFG_USE_SEMAPHORES               TRUE
#define CH_CFG_USE_MUTEXES                  TRUE
#define CH_CFG_USE_CONDVARS                 TRUE
#define CH_CFG_USE_EVENTS                   TRUE
#define CH_CFG_USE_MESSAGES                 TRUE
#define CH_CFG_USE_DYNAMIC                  TRUE
#define CH_CFG_USE_MAILBOXES                TRUE
#define CH_CFG_USE_MEMCORE                  TRUE
#define CH_CFG_USE_HEAP                     TRUE
#define CH_CFG_USE_MEMPOOLS                 TRUE
#define CH_CFG_USE_PIPES                    TRUE
#define CH_CFG_USE_OBJ_CACHES               TRUE
#define CH_CFG_USE_DELEGATES                TRUE
#define CH_CFG_USE_FACTORY                  TRUE

#include "../../../../../os/rt/templates/chconf.h"

#endif /* STM32U575ZI_NUCLEO144_CHCONF_H */
