/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    templates/chlibconf.h
 * @brief   Configuration file template.
 * @details A copy of this file must be placed in each project directory, it
 *          contains the application specific kernel settings.
 *
 * @addtogroup oslib_config
 * @details OS Library related settings.
 * @{
 */

#ifndef CHLIBCONF_H
#define CHLIBCONF_H

#define _CHIBIOS_LIB_CONF_
#define _CHIBIOS_LIB_CONF_VER_1_0_

/**
 * @brief   Mailboxes APIs.
 * @details If enabled then the asynchronous messages (mailboxes) APIs are
 *          included in the kernel.
 *
 * @note    The default is @p TRUE.
 * @note    Requires @p CH_CFG_USE_SEMAPHORES in chconf.h.
 */
#define CH_CFG_USE_MAILBOXES                TRUE

/**
 * @brief   Core Memory Manager APIs.
 * @details If enabled then the core memory manager APIs are included
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 */
#define CH_CFG_USE_MEMCORE                  TRUE

/**
 * @brief   Heap Allocator APIs.
 * @details If enabled then the memory heap allocator APIs are included
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 * @note    Requires @p CH_CFG_USE_MEMCORE and either @p CH_CFG_USE_MUTEXES or
 *          @p CH_CFG_USE_SEMAPHORES in chconf.h.
 * @note    Mutexes are recommended.
 */
#define CH_CFG_USE_HEAP                     TRUE

/**
 * @brief   Memory Pools Allocator APIs.
 * @details If enabled then the memory pools allocator APIs are included
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 */
#define CH_CFG_USE_MEMPOOLS                 TRUE

/**
 * @brief  Objects FIFOs APIs.
 * @details If enabled then the objects FIFOs APIs are included
 *          in the kernel.
 *
 * @note    The default is @p TRUE.
 */
#define CH_CFG_USE_OBJ_FIFOS                TRUE

/**
 * @brief   Objects Factory APIs.
 * @details If enabled then the objects factory APIs are included in the
 *          kernel.
 *
 * @note    The default is @p FALSE.
 */
#define CH_CFG_USE_FACTORY                  TRUE

/**
 * @brief   Maximum length for object names.
 * @details If the specified length is zero then the name is stored by
 *          pointer but this could have unintended side effects.
 */
#define CH_CFG_FACTORY_MAX_NAMES_LENGTH     8

/**
 * @brief   Enables the registry of generic objects.
 */
#define CH_CFG_FACTORY_OBJECTS_REGISTRY     TRUE

/**
 * @brief   Enables factory for generic buffers.
 */
#define CH_CFG_FACTORY_GENERIC_BUFFERS      TRUE

/**
 * @brief   Enables factory for semaphores.
 */
#define CH_CFG_FACTORY_SEMAPHORES           TRUE

/**
 * @brief   Enables factory for mailboxes.
 */
#define CH_CFG_FACTORY_MAILBOXES            TRUE

/**
 * @brief   Enables factory for objects FIFOs.
 */
#define CH_CFG_FACTORY_OBJ_FIFOS            TRUE

#endif  /* CHLIBCONF_H */

/** @} */
