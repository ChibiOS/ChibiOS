/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    SAMA5D2x/sama_registry.h
 * @brief   SAMA5D2x capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef SAMA_REGISTRY_H
#define SAMA_REGISTRY_H

/**
 * @brief   Sub-family identifier.
 */
#if !defined(SAMA5D2X) || defined(__DOXYGEN__)
#define SAMA5D2X
#endif

/*===========================================================================*/
/* Common features.                                                          */
/*===========================================================================*/

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    SAMA5D27 capabilities
 * @{
 */
/*===========================================================================*/
/* SAMA5D27.                                                                 */
/*===========================================================================*/

#if defined(SAMA5D27) || defined(__DOXYGEN__)

#endif /* defined(SAMA5D27) */

/* PIO attributes.*/
#define SAMA_HAS_PIOA                       TRUE
#define SAMA_HAS_PIOB                       TRUE
#define SAMA_HAS_PIOC                       TRUE
#define SAMA_HAS_PIOD                       FALSE

/** @} */

#endif /* SAMA_REGISTRY_H */

/** @} */
