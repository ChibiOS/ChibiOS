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
 * @file    chfactory.c
 * @brief   ChibiOS objects factory and registry code.
 *
 * @addtogroup objects_factory
 * @details The object factory is a subsystem that allows to:
 *          - Register static objects by name.
 *          - Dynamically create objects and assign them a name.
 *          - Retrieve existing objects by name.
 *          - Free objects by reference.
 *          .
 *          Allocated OS objects are handled using a reference counter, only
 *          when all references have been released then the object memory is
 *          freed in a pool.<br>
 * @pre     This subsystem requires the @p CH_CFG_USE_MEMCORE and
 *          @p CH_CFG_USE_MEMPOOLS options to be set to @p TRUE. The
 *          option @p CH_CFG_USE_HEAP is also required if the support
 *          for variable length objects is enabled.
 * @note    Compatible with RT and NIL.
 * @{
 */

#include <string.h>

#include "ch.h"
#include "chfactory.h"

#if (CH_CFG_USE_FACTORY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Factory object static instance.
 * @note    It is a global object because it could be accessed through
 *          a specific debugger plugin.
 */
objects_factory_t ch_factory;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static inline void dyn_list_init(dyn_list_t *dlp) {

  dlp->next = (dyn_element_t *)dlp;
}

static dyn_element_t *dyn_list_find(const char *name, dyn_list_t *dlp) {
  dyn_element_t *p = dlp->next;

  while (p != (dyn_element_t *)dlp) {
    if (strncmp(p->name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT) == 0) {
      return p;
    }
    p = p->next;
  }

  chDbgAssert(true, "invalid reference passed");

  return NULL;
}

static dyn_element_t *dyn_list_unlink(dyn_element_t *element,
                                      dyn_list_t *dlp) {
  dyn_element_t *prev = (dyn_element_t *)dlp;

  /* Scanning the list.*/
  while (prev->next != (dyn_element_t *)dlp) {
    if (prev->next == element) {
      /* Found.*/
      prev->next = element->next;
      return element;
    }

    /* Next element in the list.*/
    prev = prev->next;
  }

  return NULL;
}

#if (CH_FACTORY_REQUIRES_HEAP == TRUE) || defined(__DOXYGEN__)
static dyn_element_t *dyn_create_object_heap(const char *name,
                                             dyn_list_t *dlp,
                                             size_t size) {
  dyn_element_t *dep;

  chDbgCheck(name != NULL);

  /* Checking if an object with this name has already been created.*/
  dep = dyn_list_find(name, dlp);
  if (dep != NULL) {
    return NULL;
  }

  /* Allocating space for the new buffer object.*/
  dep = (dyn_element_t *)chHeapAlloc(NULL, size);
  if (dep) {
    return NULL;
  }

  /* Initializing object list element.*/
  strncpy(dep->name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT);
  dep->refs = 1U;
  dep->next = dlp->next;

  /* Updating factory list.*/
  dlp->next = dep;

  return dep;
}

static void dyn_release_object_heap(dyn_element_t *dep,
                                    dyn_list_t *dlp) {

  chDbgCheck(dep != NULL);
  chDbgAssert(dep->refs > 0U, "invalid references number");

  dep = dyn_list_unlink(dep, dlp);

  dep->refs--;
  if (dep->refs == 0U) {
    chHeapFree((void *)dep);
  }
}
#endif /* CH_FACTORY_REQUIRES_HEAP == TRUE */

#if (CH_FACTORY_REQUIRES_POOLS == TRUE) || defined(__DOXYGEN__)
static dyn_element_t *dyn_create_object_pool(const char *name,
                                             dyn_list_t *dlp,
                                             memory_pool_t *mp) {
  dyn_element_t *dep;

  chDbgCheck(name != NULL);

  /* Checking if an object object with this name has already been created.*/
  dep = dyn_list_find(name, dlp);
  if (dep != NULL) {
    return NULL;
  }

  /* Allocating space for the new object.*/
  dep = (dyn_element_t *)chPoolAlloc(mp);
  if (dep == NULL) {
    return NULL;
  }

  /* Initializing object list element.*/
  strncpy(dep->name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT);
  dep->refs = 1U;
  dep->next = ch_factory.sem_list.next;

  /* Updating factory list.*/
  dlp->next = (dyn_element_t *)dep;

  return dep;
}

static void dyn_release_object_pool(dyn_element_t *dep,
                                    dyn_list_t *dlp,
                                    memory_pool_t *mp) {

  chDbgCheck(dep != NULL);
  chDbgAssert(dep->refs > 0U, "invalid references number");

  dep = dyn_list_unlink(dep, dlp);

  dep->refs--;
  if (dep->refs == 0U) {
    chPoolFree(mp, (void *)dep);
  }
}
#endif /* CH_FACTORY_REQUIRES_POOLS == TRUE */

static dyn_element_t *dyn_find_object(const char *name, dyn_list_t *dlp) {
  dyn_element_t *dep;

  chDbgCheck(name != NULL);

  /* Checking if an object with this name has already been created.*/
  dep = dyn_list_find(name, dlp);
  if (dep != NULL) {
    /* Increasing references counter.*/
    dep->refs++;
  }

  return dep;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the objects factory.
 *
 * @init
 */
void _factory_init(void) {

#if CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE
  dyn_list_init(&ch_factory.obj_list);
  chPoolObjectInit(&ch_factory.obj_pool,
                   sizeof (registered_object_t),
                   chCoreAllocAlignedI);
#endif
#if CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE
  dyn_list_init(&ch_factory.buf_list);
#endif
#if CH_CFG_FACTORY_SEMAPHORES == TRUE
  dyn_list_init(&ch_factory.sem_list);
  chPoolObjectInit(&ch_factory.sem_pool,
                   sizeof (dyn_semaphore_t),
                   chCoreAllocAlignedI);
#endif
#if CH_CFG_FACTORY_MAILBOXES == TRUE
  dyn_list_init(&ch_factory.mbx_list);
#endif
}

#if (CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Registers a generic object.
 * @post    A reference to the registered object is returned and the
 *          reference counter is initialized to one.
 *
 * @param[in] name      name to be assigned to the registered object
 * @param[in] objp      pointer to the object to be registered
 *
 * @api
 */
registered_object_t *chFactoryRegisterObject(const char *name,
                                             void *objp) {
  registered_object_t *rop;

  chSysLock();

  rop = (registered_object_t *)dyn_create_object_pool(name,
                                                      &ch_factory.obj_list,
                                                      &ch_factory.obj_pool);
  if (rop != NULL) {
    /* Initializing registered object data.*/
    rop->objp = objp;
  }

  chSysUnlock();

  return rop;
}

/**
 * @brief   Retrieves a registered object.
 * @post    A reference to the registered object is returned with the
 *          reference counter increased by one.
 *
 * @param[in] name      name of the registered object
 *
 * @return              The reference to the found registered object.
 * @retval NULL         if a registered object with the specified name
 *                      does not exist.
 *
 * @api
 */
registered_object_t *chFactoryFindObject(const char *name) {
  registered_object_t *rop;

  chSysLock();

  rop = (registered_object_t *)dyn_find_object(name, &ch_factory.obj_list);

  chSysUnlock();

  return rop;
}

/**
 * @brief   Releases a registered object.
 * @details The reference counter of the registered object is decreased
 *          by one, if reaches zero then the registered object memory
 *          is freed.
 * @note    The object itself is not freed, it could be static, only the
 *          allocated list element is freed.
 *
 * @param[in] rop       registered object reference
 *
 * @api
 */
void chFactoryReleaseObject(registered_object_t *rop){

  chSysLock();

  dyn_release_object_pool(&rop->element,
                          &ch_factory.obj_list,
                          &ch_factory.obj_pool);

  chSysUnlock();
}
#endif /* CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE */

#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a generic dynamic buffer object.
 * @post    A reference to the dynamic buffer object is returned and the
 *          reference counter is initialized to one.
 * @post    The dynamic buffer object is filled with zeros.
 *
 * @param[in] name      name to be assigned to the new dynamic buffer object
 * @param[in] size      payload size of the dynamic buffer object to be created
 *
 * @return              The reference to the created dynamic buffer object.
 * @retval NULL         if the dynamic buffer object cannot be allocated or
 *                      a dynamic buffer object with the same name exists.
 *
 * @api
 */
dyn_buffer_t *chFactoryCreateBuffer(const char *name, size_t size) {
  dyn_buffer_t *dbp;

  chSysLock();

  dbp = (dyn_buffer_t *)dyn_create_object_heap(name,
                                               &ch_factory.buf_list,
                                               size);
  if (dbp != NULL) {
    /* Initializing buffer object data.*/
    memset((void *)dbp->buffer, 0, size);
  }

  chSysUnlock();

  return dbp;
}

/**
 * @brief   Retrieves a dynamic buffer object.
 * @post    A reference to the dynamic buffer object is returned with the
 *          reference counter increased by one.
 *
 * @param[in] name      name of the dynamic buffer object
 *
 * @return              The reference to the found dynamic buffer object.
 * @retval NULL         if a dynamic buffer object with the specified name
 *                      does not exist.
 *
 * @api
 */
dyn_buffer_t *chFactoryFindBuffer(const char *name) {
  dyn_buffer_t *dbp;

  chSysLock();

  dbp = (dyn_buffer_t *)dyn_find_object(name, &ch_factory.buf_list);

  chSysUnlock();

  return dbp;
}

/**
 * @brief   Releases a dynamic buffer object.
 * @details The reference counter of the dynamic buffer object is decreased
 *          by one, if reaches zero then the dynamic buffer object memory
 *          is freed.
 *
 * @param[in] dbp       dynamic buffer object reference
 *
 * @api
 */
void chFactoryReleaseBuffer(dyn_buffer_t *dbp) {

  chSysLock();

  dyn_release_object_heap(&dbp->element, &ch_factory.buf_list);

  chSysUnlock();
}
#endif /* CH_CFG_FACTORY_GENERIC_BUFFERS = TRUE */

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic semaphore object.
 * @post    A reference to the dynamic semaphore object is returned and the
 *          reference counter is initialized to one.
 * @post    The dynamic semaphore object is initialized and ready to use.
 *
 * @param[in] name      name to be assigned to the new dynamic semaphore object
 * @param[in] n         dynamic semaphore object counter initialization value
 *
 * @return              The reference to the created dynamic semaphore object.
 * @retval NULL         if the dynamic semaphore object cannot be allocated or
 *                      a dynamic semaphore with the same name exists.
 *
 * @api
 */
dyn_semaphore_t *chFactoryCreateSemaphore(const char *name, cnt_t n) {
  dyn_semaphore_t *dsp;

  chSysLock();

  dsp = (dyn_semaphore_t *)dyn_create_object_pool(name,
                                                  &ch_factory.sem_list,
                                                  &ch_factory.sem_pool);
  if (dsp != NULL) {
    /* Initializing semaphore object dataa.*/
    chSemObjectInit(&dsp->sem, n);
  }

  chSysUnlock();

  return dsp;
}

/**
 * @brief   Retrieves a dynamic semaphore object.
 * @post    A reference to the dynamic semaphore object is returned with the
 *          reference counter increased by one.
 *
 * @param[in] name      name of the dynamic semaphore object
 *
 * @return              The reference to the found dynamic semaphore object.
 * @retval NULL         if a dynamic semaphore object with the specified name
 *                      does not exist.
 *
 * @api
 */
dyn_semaphore_t *chFactoryFindSemaphore(const char *name) {
  dyn_semaphore_t *dsp;

  chSysLock();

  dsp = (dyn_semaphore_t *)dyn_find_object(name, &ch_factory.sem_list);

  chSysUnlock();

  return dsp;
}

/**
 * @brief   Releases a dynamic semaphore object.
 * @details The reference counter of the dynamic semaphore object is decreased
 *          by one, if reaches zero then the dynamic semaphore object memory
 *          is freed.
 *
 * @param[in] dsp       dynamic semaphore object reference
 *
 * @api
 */
void chFactoryReleaseSemaphore(dyn_semaphore_t *dsp) {

  chSysLock();

  dyn_release_object_pool(&dsp->element,
                          &ch_factory.sem_list,
                          &ch_factory.sem_pool);

  chSysUnlock();
}
#endif /* CH_CFG_FACTORY_SEMAPHORES = TRUE */

#if (CH_CFG_FACTORY_MAILBOXES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic mailbox object.
 * @post    A reference to the dynamic mailbox object is returned and the
 *          reference counter is initialized to one.
 * @post    The dynamic buffer object is filled with zeros.
 *
 * @param[in] name      name to be assigned to the new dynamic mailbox object
 * @param[in] n         mailbox buffer size as number of messages
 *
 * @return              The reference to the created dynamic mailbox object.
 * @retval NULL         if the dynamic mailbox object cannot be allocated or
 *                      a dynamic mailbox object with the same name exists.
 *
 * @api
 */
dyn_mailbox_t *chFactoryCreateMailbox(const char *name, cnt_t n) {
  dyn_mailbox_t *dmp;

  chSysLock();

  dmp = (dyn_mailbox_t *)dyn_create_object_heap(name,
                                                &ch_factory.mbx_list,
                                                sizeof (dyn_mailbox_t) +
                                                ((size_t)n * sizeof (msg_t)));
  if (dmp != NULL) {
    /* Initializing mailbox object data.*/
    chMBObjectInit(&dmp->mbx, dmp->buffer, n);
  }

  chSysUnlock();

  return dmp;
}

/**
 * @brief   Retrieves a dynamic mailbox object.
 * @post    A reference to the dynamic mailbox object is returned with the
 *          reference counter increased by one.
 *
 * @param[in] name      name of the dynamic mailbox object
 *
 * @return              The reference to the found dynamic mailbox object.
 * @retval NULL         if a dynamic mailbox object with the specified name
 *                      does not exist.
 *
 * @api
 */
dyn_mailbox_t *chFactoryFindMailbox(const char *name) {
  dyn_mailbox_t *dmp;

  chSysLock();

  dmp = (dyn_mailbox_t *)dyn_find_object(name, &ch_factory.mbx_list);

  chSysUnlock();

  return dmp;
}

/**
 * @brief   Releases a dynamic mailbox object.
 * @details The reference counter of the dynamic mailbox object is decreased
 *          by one, if reaches zero then the dynamic mailbox object memory
 *          is freed.
 *
 * @param[in] dbp       dynamic mailbox object reference
 *
 * @api
 */
void chFactoryReleaseMailbox(dyn_mailbox_t *dmp) {

  chSysLock();

  dyn_release_object_heap(&dmp->element, &ch_factory.mbx_list);

  chSysUnlock();
}
#endif /* CH_CFG_FACTORY_MAILBOXES = TRUE */

#endif /* CH_CFG_USE_FACTORY == TRUE */

/** @} */
