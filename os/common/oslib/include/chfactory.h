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

#if !defined(CH_CFG_USE_FACTORY)
#define CH_CFG_USE_FACTORY                  FALSE
#endif

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
 * @brief   Enables the registry of generic objects.
 */
#if !defined(CH_CFG_FACTORY_OBJECTS_REGISTRY) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_OBJECTS_REGISTRY     TRUE
#endif

/**
 * @brief   Enables factory for generic buffers.
 */
#if !defined(CH_CFG_FACTORY_GENERIC_BUFFERS) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_GENERIC_BUFFERS      TRUE
#endif

/**
 * @brief   Enables factory for semaphores.
 */
#if !defined(CH_CFG_FACTORY_SEMAPHORES) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_SEMAPHORES           TRUE
#endif

/**
 * @brief   Enables factory for mailboxes.
 */
#if !defined(CH_CFG_FACTORY_MAILBOXES) || defined(__DOXYGEN__)
#define CH_CFG_FACTORY_MAILBOXES            TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#define CH_FACTORY_REQUIRES_POOLS                                           \
  ((CH_CFG_FACTORY_SEMAPHORES == TRUE))

#define CH_FACTORY_REQUIRES_HEAP                                            \
  ((CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) ||                              \
   (CH_CFG_FACTORY_MAILBOXES == TRUE))

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

#if (CH_CFG_FACTORY_MAILBOXES == TRUE) && (CH_CFG_USE_MAILBOXES == FALSE)
#error "CH_CFG_FACTORY_MAILBOXES requires CH_CFG_USE_MAILBOXES"
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

#if (CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a registered object.
 */
typedef struct ch_registered_static_object {
  /**
   * @brief   List element of the registered object.
   */
  dyn_element_t         element;
  /**
   * @brief   Pointer to the object.
   * @note    The type of the object is not stored in anyway.
   */
  void                  *objp;
} registered_object_t;
#endif

#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a dynamic buffer object.
 */
typedef struct ch_dyn_object {
  /**
   * @brief   List element of the dynamic buffer object.
   */
  dyn_element_t         element;
  /**
   * @brief   The buffer.
   * @note    This requires C99.
   */
  uint8_t               buffer[];
} dyn_buffer_t;
#endif

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a dynamic semaphore.
 */
typedef struct ch_dyn_semaphore {
  /**
   * @brief   List element of the dynamic semaphore.
   */
  dyn_element_t         element;
  /**
   * @brief   The semaphore.
   */
  semaphore_t           sem;
} dyn_semaphore_t;
#endif

#if (CH_CFG_FACTORY_MAILBOXES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Type of a dynamic buffer object.
 */
typedef struct ch_dyn_mailbox {
  /**
   * @brief   List element of the dynamic buffer object.
   */
  dyn_element_t         element;
  /**
   * @brief   The mailbox.
   */
  mailbox_t             mbx;
  /**
   * @brief   Mailbox buffer.
   * @note    This requires C99.
   */
  msg_t                 buffer[];
} dyn_mailbox_t;
#endif

/**
 * @brief   Type of the factory main object.
 */
typedef struct ch_objects_factory {
  /**
   * @brief   List of the registered objects.
   */
  dyn_list_t            obj_list;
  /**
   * @brief   Pool of the available registered objects.
   */
  memory_pool_t         obj_pool;
#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXIGEN__)
  /**
   * @brief   List of the allocated buffer objects.
   */
  dyn_list_t            buf_list;
#endif /* CH_CFG_FACTORY_GENERIC_BUFFERS = TRUE */
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
#if (CH_CFG_FACTORY_MAILBOXES == TRUE) || defined(__DOXIGEN__)
  /**
   * @brief   List of the allocated buffer objects.
   */
  dyn_list_t            mbx_list;
#endif /* CH_CFG_FACTORY_MAILBOXES = TRUE */
} objects_factory_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern objects_factory_t ch_factory;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _factory_init(void);
#if (CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE) || defined(__DOXIGEN__)
  registered_object_t *chFactoryRegisterObject(const char *name,
                                                            void *objp);
  registered_object_t *chFactoryFindObject(const char *name);
  void chFactoryReleaseObject(registered_object_t *rop);
#endif
#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXIGEN__)
  dyn_buffer_t *chFactoryCreateBuffer(const char *name, size_t size);
  dyn_buffer_t *chFactoryFindBuffer(const char *name);
  void chFactoryReleaseBuffer(dyn_buffer_t *dbp);
#endif
#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
  dyn_semaphore_t *chFactoryCreateSemaphore(const char *name, cnt_t n);
  dyn_semaphore_t *chFactoryFindSemaphore(const char *name);
  void chFactoryReleaseSemaphore(dyn_semaphore_t *dsp);
#endif
#if (CH_CFG_FACTORY_MAILBOXES == TRUE) || defined(__DOXIGEN__)
  dyn_mailbox_t *chFactoryCreateMailbox(const char *name, size_t n);
  dyn_mailbox_t *chFactoryFindMailbox(const char *name);
  void chFactoryReleaseMailbox(dyn_mailbox_t *dmp);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Duplicates an object reference.
 * @note    This function can be used on any kind of dynamic object.
 *
 * @param[in] dep       pointer to the element field of the object
 *
 * @iclass
 */
static inline dyn_element_t *chFactoryDuplicateReferenceI(dyn_element_t *dep) {

  chDbgCheckClassI();

  dep->refs++;

  return dep;
}

#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Returns the size of a generic dynamic buffer object.
 *
 * @return              The size of the buffer object in bytes.
 *
 * @api
 */
static inline size_t chFactoryGetBufferSize(dyn_buffer_t *dbp) {

  return chHeapGetSize(dbp) - sizeof (dyn_element_t);
}
#endif

#endif /* CH_CFG_USE_FACTORY == TRUE */

#endif /* CHFACTORY_H */

/** @} */
