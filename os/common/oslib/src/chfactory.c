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
 * @brief   Allocates a generic dynamic object.
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
 */
dyn_object_t *chFactoryCreateObject(const char *name, size_t size) {
  dyn_object_t *dop;

  chSysLock();

  /* Checking if an object with this name has already been created.*/
  dop = (dyn_object_t *)dyn_list_find(&ch_factory.obj_list, name);
  if (dop != NULL) {
    /* Object exists, error.*/
    chSysUnlock();
    return NULL;
  }

  dop = chHeapAlloc(NULL, size);
  if (dop == NULL) {
    chSysUnlock();
    return NULL;
  }

  /* Initializing object data and metadata.*/
  strncpy(dop->list.name, name, CH_CFG_FACTORY_MAX_NAMES_LENGHT);
  dop->list.refs = 1;
  dop->list.next = ch_factory.obj_list.next;
  memset((void *)dop->obj, 0, size);

  /* Updating factory list.*/
  ch_factory.obj_list.next = (dyn_element_t *)dop;

  chSysUnlock();

  return dop;
}

dyn_object_t *chFactoryFindObject(const char *name) {

}

void chFactoryReleaseObject(dyn_object_t *dop) {

}

size_t chFactoryGetObjectSize(dyn_object_t *dop) {

  return chHeapGetSize((void *)dop);
}
#endif /* CH_CFG_FACTORY_GENERIC = TRUE */

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXIGEN__)
dyn_semaphore_t *chFactoryCreateSemaphore(const char *name, cnt_t n) {

}

dyn_semaphore_t *chFactoryFindSemaphore(const char *name) {

}

void chFactoryReleaseSemaphore(dyn_semaphore_t *dsp) {

}
#endif /* CH_CFG_FACTORY_SEMAPHORES = TRUE */

#endif /* CH_CFG_USE_FACTORY == TRUE */

/** @} */
