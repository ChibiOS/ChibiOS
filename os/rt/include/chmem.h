/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    rt/include/chmem.h
 * @brief   Memory related macros and structures.
 *
 * @addtogroup memory
 * @details This module implements architecture-specific memory classes
 *          modifiers. This is required in NUMA multi-master systems to place
 *          variables and data in the most appropriate memory.
 *          Several classes are defined:
 *          - GLOBAL, used for variables shared among all bus masters without
 *            coherence requirement.
 *          - GLOBAL_COHERENT, global variables that require cache coherence.
 *          - LOCAL, used for variables shared among all bus masters that are
 *            accessed mainly by a specific core without coherence requirement.
 *          - LOCAL_COHERENT, local variables that require cache coherence.
 *          - PRIVATE, variables private to a specific core.
 *          - PRIVATE_COHERENT, variables private to a specific core that
 *            require cache coherence.
 *          .
 *          Memory classes are defined in the port layer and their full
 *          implementation is not mandatory, some could be unimplemented on
 *          specific architectures. If a memory class is not defined in the
 *          port layer then the system defaults it to the next less restrictive
 *          memory class:
 *          - Missing PRIVATE is defaulted to LOCAL.
 *          - Missing LOCAL is defaulted to GLOBAL.
 *          - Missing GLOBAL is defaulted to standard BSS segment.
 *          - Missing PRIVATE_COHERENT is defaulted to LOCAL_COHERENT.
 *          - Missing LOCAL_COHERENT is defaulted to GLOBAL_COHERENT.
 *          - Missing GLOBAL_COHERENT is defaulted to standard BSS segment.
 *          .
 *          Note, depending on the architecture, cache coherence can be
 *          implemented by placing variables in a non-cacheable area. This
 *          can have a performance impact so it is better to not place
 *          variables in coherent classes unless strictly required.
 * @{
 */

#ifndef CHMEM_H
#define CHMEM_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Global RAM affinity macro.
 * @details Global RAM, the memory is optimally accessed by all cores.
 * @note    This is equivalent to a normal C non-initialized variable, the
 *          port layer has the option to enforce some kind of attribute when
 *          this modifier is used.
 * @note    Only uninitialized variables can be tagged with this attribute.
 */
#if defined(PORT_MEM_GLOBAL_BSS) || defined(__DOXYGEN__)
#define CH_MEM_GLOBAL_BSS               PORT_MEM_GLOBAL_BSS
#else
#define CH_MEM_GLOBAL_BSS               /* Default BSS.*/
#endif

/**
 * @brief   Local RAM affinity macro.
 * @details Local RAM, the memory is optimally accessed only by the specified
 *          core, if the system does not support this memory class then the
 *          variable is created as @p CH_MEM_GLOBAL_BSS.
 * @note    Cache Coherence is not implied in this class of memories.
 * @note    Only uninitialized variables can be tagged with this attribute.
 *
 * @param[in] c         core number as a numeric literal
 */
#if defined(PORT_MEM_LOCAL_BSS) || defined(__DOXYGEN__)
#define CH_MEM_LOCAL_BSS(c)             PORT_MEM_LOCAL_BSS##c
#else
#define CH_MEM_LOCAL_BSS(c)             CH_MEM_GLOBAL_BSS /* Fallback.*/
#endif

/**
 * @brief   Private RAM affinity macro.
 * @details Private RAM, the memory is reachable only by the specified core,
 *          if the system does not support this memory class then the
 *          variable is created as @p CH_MEM_LOCAL_BSS.
 * @note    The memory is meant to be reachable only by the specified core.
 * @note    This class of memory is optimal for thread-private data, thread
 *          stacks and, in general, any variable that is exclusively accessed
 *          by a known core.
 * @note    Only uninitialized variables can be tagged with this attribute.
 *
 * @param[in] c         core number as a numeric literal
 */
#if defined(PORT_MEM_PRIVATE_BSS) || defined(__DOXYGEN__)
#define CH_MEM_PRIVATE_BSS(c)           PORT_MEM_PRIVATE_BSS##c
#else
#define CH_MEM_PRIVATE_BSS(c)           CH_MEM_LOCAL_BSS(c) /* Fallback.*/
#endif

/**
 * @brief   Global and coherent RAM affinity macro.
 * @details Global coherent RAM, the memory is optimally accessed by all
 *          cores and is guaranteed to be cache-coherent, if the system does
 *          not support this memory class then the variable is created as a
 *          normal BSS variable.
 * @note    Coherence method is architecture-dependent, it could involve
 *          disabling data cache entirely over variables declared using this
 *          modifier. If coherence is not strictly required then declare a
 *          normal variable (no modifier).
 * @note    On multicore systems all OS synchronization objects (semaphores,
 *          mutexes, queues, etc) must be declared as coherent.
 * @note    Only uninitialized variables can be tagged with this attribute.
 */
#if defined(PORT_MEM_GLOBAL_COHERENT_BSS) || defined(__DOXYGEN__)
#define CH_MEM_GLOBAL_COHERENT_BSS      PORT_MEM_GLOBAL_COHERENT_BSS
#else
#define CH_MEM_GLOBAL_COHERENT_BSS      /* Default BSS.*/
#endif

/**
 * @brief   Local and coherent RAM affinity macro.
 * @details Local coherent RAM, the memory is optimally accessed only by the
 *          specified core and is guaranteed to be cache-coherent, if the
 *          system does not support this memory class then the variable is
 *          created as as @p CH_MEM_GLOBAL_COHERENT_BSS.
 * @note    Coherence method is architecture-dependent, it could involve
 *          disabling data cache entirely over variables declared using this
 *          modifier. If coherence is not strictly required then use
 *          @p CH_MEM_LOCAL_BSS() instead.
 * @note    On multicore systems all OS synchronization objects (semaphores,
 *          mutexes, queues, etc) must be declared as shared or the
 *          inter-core synchronization will fail.
 * @note    Only uninitialized variables can be tagged with this attribute.
 *
 * @param[in] c         core number as a numeric literal
 */
#if defined(PORT_MEM_LOCAL_COHERENT_BSS) || defined(__DOXYGEN__)
#define CH_MEM_LOCAL_COHERENT_BSS(c)    PORT_MEM_LOCAL_COHERENT_BSS##c
#else
#define CH_MEM_LOCAL_COHERENT_BSS(c)    CH_MEM_GLOBAL_COHERENT_BSS /* Fallback.*/
#endif

/**
 * @brief   Private and coherent RAM affinity macro.
 * @details Private coherent RAM, the memory is reachable only by the
 *          specified core, if the system does not support this memory class
 *          then the variable is created as @p CH_MEM_LOCAL_COHERENT_BSS.
 * @note    The memory is meant to be reachable only by the specified core.
 * @note    This class of memory is optimal for thread-private data, thread
 *          stacks and, in general, any variable that is exclusively accessed
 *          by a known core.
 * @note    Only uninitialized variables can be tagged with this attribute.
 *
 * @param[in] c         core number as a numeric literal
 */
#if defined(PORT_MEM_PRIVATE_COHERENT_BSS) || defined(__DOXYGEN__)
#define CH_MEM_PRIVATE_COHERENT_BSS(c)  PORT_MEM_PRIVATE_COHERENT_BSS##c
#else
#define CH_MEM_PRIVATE_COHERENT_BSS(c)  CH_MEM_LOCAL_COHERENT_BSS(c) /* Fallback.*/
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHMEM_H */

/** @} */
