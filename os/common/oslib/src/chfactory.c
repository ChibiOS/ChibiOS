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
 * @brief   ChibiOS objects factory code.
 *
 * @addtogroup objects_factory
 * @details The object factory is a subsystem that allows to:
 *          - Create objects and assign them a name.
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

static dyn_element_t *dyn_list_find(dyn_list_t *dlp, const char *name) {
  dyn_element_t *p = dlp->next;

  while (p != (dyn_element_t *)dlp) {
    if (strncmp(p->name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT) == 0) {
      return p;
    }
    p = p->next;
  }

  return NULL;
}

static dyn_element_t *dyn_list_unlink(dyn_list_t *dlp, dyn_element_t *element) {
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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the objects factory.
 *
 * @init
 */
void _factory_init(void) {

#if CH_CFG_FACTORY_GENERIC == TRUE
  dyn_list_init(&ch_factory.obj_list);
#endif
#if CH_CFG_FACTORY_SEMAPHORES == TRUE
  dyn_list_init(&ch_factory.sem_list);
  chPoolObjectInit(&ch_factory.sem_pool,
                   sizeof (dyn_semaphore_t),
                   chCoreAllocAlignedI);
#endif
}

#if (CH_CFG_FACTORY_GENERIC == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a generic dynamic object.
 * @post    A reference to the object is returned and the reference counter
 *          is initialized to one.
 * @post    The object is zero filled.
 *
 * @param[in] name      name to be assigned to the new object
 * @param[in] size      payload size of the object to be created
 *
 * @return              The reference to the created object.
 * @retval NULL         if the object cannot be allocated or an object with
 *                      the same name exists.
 *
 * @api
 */
dyn_object_t *chFactoryCreateObject(const char *name, size_t size) {
  dyn_object_t *dop;

  chDbgCheck(name != NULL);

  chSysLock();

  /* Checking if an object with this name has already been created.*/
  dop = (dyn_object_t *)dyn_list_find(&ch_factory.obj_list, name);
  if (dop != NULL) {
    /* Object exists, error.*/
    chSysUnlock();
    return NULL;
  }

  /* Allocating space for the new object.*/
  dop = chHeapAlloc(NULL, size);
  if (dop == NULL) {
    chSysUnlock();
    return NULL;
  }

  /* Initializing object data and metadata.*/
  strncpy(dop->element.name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT);
  dop->element.refs = 1;
  dop->element.next = ch_factory.obj_list.next;
  memset((void *)dop->obj, 0, size);

  /* Updating factory list.*/
  ch_factory.obj_list.next = (dyn_element_t *)dop;

  chSysUnlock();

  return dop;
}

/**
 * @brief   Retrieves a generic dynamic object.
 * @post    A reference to the object is returned with the reference counter
 *          increased by one.
 *
 * @param[in] name      name to be assigned to the new object
 *
 * @return              The reference to the found object.
 * @retval NULL         if an object with the specified name name does
 *                      not exist.
 *
 * @api
 */
dyn_object_t *chFactoryFindObject(const char *name) {
  dyn_object_t *dop;

  chDbgCheck(name != NULL);

  chSysLock();

  /* Checking if an object with this name has already been created.*/
  dop = (dyn_object_t *)dyn_list_find(&ch_factory.obj_list, name);
  if (dop == NULL) {
    /* Object does not exists, error.*/
    chSysUnlock();
    return NULL;
  }

  /* Increasing references counter.*/
  dop->element.refs += 1;

  chSysUnlock();

  return dop;
}

/**
 * @brief   Releases a generic dynamic object.
 * @details The reference counter of the object is decreased by one, if
 *          reaches zero then the object memory is freed.
 *
 * @param[in] dop       generic object reference
 *
 * @api
 */
void chFactoryReleaseObject(dyn_object_t *dop) {

  chDbgCheck(dop != NULL);

  chSysLock();

  chDbgAssert(dop->element.refs > 0U, "invalid references number");

  dop = (dyn_object_t *)dyn_list_unlink(&ch_factory.obj_list,
                                        &dop->element);

  chDbgAssert(dop != NULL, "invalid reference passed");

  dop->element.refs--;
  if (dop->element.refs == 0) {
    chHeapFree((void *)dop);
  }

  chSysUnlock();
}

size_t chFactoryGetObjectSize(dyn_object_t *dop) {

  return chHeapGetSize((void *)dop);
}
#endif /* CH_CFG_FACTORY_GENERIC = TRUE */

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic semaphore object.
 * @post    A reference to the semaphore is returned and the reference counter
 *          is initialized to one.
 * @post    The semaphore object is initialized and ready to use.
 *
 * @param[in] name      name to be assigned to the new semaphore object
 * @param[in] n         semaphore counter initialization value
 *
 * @return              The reference to the created semaphore object.
 * @retval NULL         if the semaphore cannot be allocated or an semaphore
 *                      with the same name exists.
 *
 * @api
 */
dyn_semaphore_t *chFactoryCreateSemaphore(const char *name, cnt_t n) {
  dyn_semaphore_t *dsp;

  chDbgCheck(name != NULL);

  chSysLock();

  /* Checking if an object with this name has already been created.*/
  dsp = (dyn_semaphore_t *)dyn_list_find(&ch_factory.sem_list, name);
  if (dsp != NULL) {
    /* Semaphore exists, error.*/
    chSysUnlock();
    return NULL;
  }

  /* Allocating space for the new object.*/
  dsp = chCoreAlloc(sizeof (dyn_semaphore_t));
  if (dsp == NULL) {
    chSysUnlock();
    return NULL;
  }

  /* Initializing object data and metadata.*/
  strncpy(dsp->element.name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT);
  dsp->element.refs = 1;
  dsp->element.next = ch_factory.obj_list.next;
  chSemObjectInit(&dsp->sem, n);

  /* Updating factory list.*/
  ch_factory.obj_list.next = (dyn_element_t *)dsp;

  chSysUnlock();

  return dsp;
}

/**
 * @brief   Retrieves a generic dynamic semaphore object.
 * @post    A reference to the semaphore is returned with the reference counter
 *          increased by one.
 *
 * @param[in] name      name to be assigned to the new semaphore object
 *
 * @return              The reference to the found semaphore object.
 * @retval NULL         if a semaphore with the specified name name does
 *                      not exist.
 *
 * @api
 */
dyn_semaphore_t *chFactoryFindSemaphore(const char *name) {
  dyn_semaphore_t *dsp;

  chDbgCheck(name != NULL);

  chSysLock();

  /* Checking if an object with this name has already been created.*/
  dsp = (dyn_semaphore_t *)dyn_list_find(&ch_factory.obj_list, name);
  if (dsp == NULL) {
    /* Object does not exists, error.*/
    chSysUnlock();
    return NULL;
  }

  /* Increasing references counter.*/
  dsp->element.refs += 1;

  chSysUnlock();

  return dsp;
}

/**
 * @brief   Releases a semaphore dynamic object.
 * @details The reference counter of the semaphore is decreased by one, if
 *          reaches zero then the semaphore memory is freed.
 *
 * @param[in] dsp       semaphore object reference
 *
 * @api
 */
void chFactoryReleaseSemaphore(dyn_semaphore_t *dsp) {

  chDbgCheck(dsp != NULL);

  chSysLock();

  chDbgAssert(dsp->element.refs > 0U, "invalid references number");

  dsp = (dyn_semaphore_t *)dyn_list_unlink(&ch_factory.sem_list,
                                           &dsp->element);

  chDbgAssert(dsp != NULL, "invalid reference passed");

  dsp->element.refs--;
  if (dsp->element.refs == 0) {
    chPoolFree(&ch_factory.sem_pool, (void *)dsp);
  }

  chSysUnlock();
}
#endif /* CH_CFG_FACTORY_SEMAPHORES = TRUE */

#endif /* CH_CFG_USE_FACTORY == TRUE */

/** @} */
