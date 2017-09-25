/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chfactory.h
 * @brief   ChibiOS objects factory structures and macros.
 *
 * @addtogroup objects_factory
 * @{
 */

#ifndef CHFACTORY_H
#define CHFACTORY_H

#if (CH_CFG_USE_FACTORY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Maximum length for object names.
 * @details If the specified length is zero then the name is stored by
 *          pointer but this could have unintended side effects.
 */
#if !defined(CH_CFG_FACTORY_MAX_NAMES_LENGHT) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_MAX_NAMES_LENGHT     8
#endif

/**
 * @brief   Enables factory for generic objects.
 * @note    Generic objects require the heap allocator.
 */
#if !defined(CH_CFG_FACTORY_GENERIC) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_GENERIC              TRUE
#endif

/**
 * @brief   Enables factory for semaphores.
 */
#if !defined(CH_CFG_FACTORY_SEMAPHORES) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_SEMAPHORES           TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#define CH_FACTORY_REQUIRES_POOLS                                           \
  ((CH_CFG_FACTORY_SEMAPHORES == TRUE))

#define CH_FACTORY_REQUIRES_HEAP                                            \
  ((CH_CFG_FACTORY_GENERIC == TRUE))

#if (CH_CFG_FACTORY_MAX_NAMES_LENGHT < 0) ||                                \
    (CH_CFG_FACTORY_MAX_NAMES_LENGHT > 32)
#error "invalid CH_CFG_FACTORY_MAX_NAMES_LENGHT value"
#endif

#if CH_CFG_USE_MEMCORE == FALSE
#error "CH_CFG_USE_FACTORY requires CH_CFG_USE_MEMCORE"
#endif

#if (CH_FACTORY_REQUIRES_POOLS == TRUE) && (CH_CFG_USE_MEMPOOLS == FALSE)
#error "CH_CFG_USE_MEMPOOLS is required"
#endif

#if (CH_FACTORY_REQUIRES_HEAP == TRUE) && (CH_CFG_USE_HEAP == FALSE)
#error "CH_CFG_USE_HEAP is required"
#endif

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) && (CH_CFG_USE_SEMAPHORES == FALSE)
#error "CH_CFG_FACTORY_SEMAPHORES requires CH_CFG_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a dynamic object list element.
 */
typedef struct ch_dyn_element {
  /**
   * @brief   Next dynamic object in the list.
   */
  struct ch_dyn_element *next;
  /**
   * @brief   Number of references to this object.
   */
  ucnt_t                refs;
#if (CH_CFG_FACTORY_MAX_NAMES_LENGHT > 0) || defined(__DOXIGEN__)
  char                  name[CH_CFG_FACTORY_MAX_NAMES_LENGHT];
#else
  const char            *name;
#endif
} dyn_element_t;

/**
 * @brief   Type of a dynamic object list.
 */
typedef struct ch_dyn_list {
    dyn_element_t       *next;
} dyn_list_t;

#if (CH_CFG_FACTORY_GENERIC == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a dynamic semaphore.
 */
typedef struct ch_dyn_object {
  /**
   * @brief   List element of the dynamic object.
   */
  dyn_element_t         list;
  /**
   * @brief   Physical objects.
   * @note    This requires C99.
   */
  uint8_t               obj[];
} dyn_object_t;
#endif

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a dynamic semaphore.
 */
typedef struct ch_dyn_semaphore {
  /**
   * @brief   List element of the dynamic semaphore.
   */
  dyn_element_t         list;
  /**
   * @brief   Physical semaphore.
   */
  semaphore_t           sem;
} dyn_semaphore_t;
#endif

/**
 * @brief   Type of the factory main object.
 */
typedef struct ch_objects_factory {
#if (CH_CFG_FACTORY_GENERIC == TRUE) || defined(__DOXIGEN__)
  /**
   * @brief   List of the allocated objects.
   */
  dyn_list_t            obj_list;
#endif /* CH_CFG_FACTORY_GENERIC = TRUE */
#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
  /**
   * @brief   List of the allocated semaphores.
   */
  dyn_list_t            sem_list;
  /**
   * @brief   Pool of the available semaphores.
   */
  memory_pool_t         sem_pool;
#endif /* CH_CFG_FACTORY_SEMAPHORES = TRUE */
} objects_factory_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
objects_factory_t ch_factory;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _factory_init(void);
#if (CH_CFG_FACTORY_GENERIC == TRUE) || defined(__DOXIGEN__)
  dyn_object_t *chFactoryCreateObject(const char *name, size_t size);
  dyn_object_t *chFactoryFindObject(const char *name);
  void chFactoryReleaseObject(dyn_object_t *dop);
  size_t chFactoryGetObjectSize(dyn_object_t *dop);
#endif
#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
  dyn_semaphore_t *chFactoryCreateSemaphore(const char *name, cnt_t n);
  dyn_semaphore_t *chFactoryFindSemaphore(const char *name);
  void chFactoryReleaseSemaphore(dyn_semaphore_t *dsp);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CH_CFG_USE_FACTORY == TRUE */

#endif /* CHFACTORY_H */

/** @} */
