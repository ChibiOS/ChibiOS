/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021,2022,2023 Giovanni Di Sirio.

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
 * @file    oslib/src/chfactory.c
 * @brief   ChibiOS objects factory and registry code.
 *
 * @addtogroup oslib_objects_factory
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

#if (CH_CFG_USE_FACTORY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*
 * Defaults on the best synchronization mechanism available.
 */
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
#define FACTORY_LOCK()      chMtxLock(&ch_factory.mtx)
#define FACTORY_UNLOCK()    chMtxUnlock(&ch_factory.mtx)
#else
#define FACTORY_LOCK()      (void) chSemWait(&ch_factory.sem)
#define FACTORY_UNLOCK()    chSemSignal(&ch_factory.sem)
#endif

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

static void copy_name(const char *sp, char *dp) {
  unsigned i;
  char c;

  i = CH_CFG_FACTORY_MAX_NAMES_LENGTH;
  do {
    c = *sp++;
    *dp++ = c;
    i--;
  } while ((c != (char)0) && (i > 0U));
}

static inline void dyn_list_init(dyn_list_t *dlp) {

  dlp->next = (dyn_element_t *)dlp;
}

static dyn_element_t *dyn_list_find(const char *name, dyn_list_t *dlp) {
  dyn_element_t *p = dlp->next;

  while (p != (dyn_element_t *)dlp) {
    if (strncmp(p->name, name, CH_CFG_FACTORY_MAX_NAMES_LENGTH) == 0) {
      return p;
    }
    p = p->next;
  }

  return NULL;
}

static dyn_element_t *dyn_list_find_prev(dyn_element_t *element,
                                      dyn_list_t *dlp) {
  dyn_element_t *prev = (dyn_element_t *)dlp;

  /* Scanning the list.*/
  while (prev->next != (dyn_element_t *)dlp) {
    if (prev->next == element) {
      return prev;
    }

    /* Next element in the list.*/
    prev = prev->next;
  }

  return NULL;
}

static dyn_element_t *dyn_list_unlink(dyn_element_t *prev) {
  dyn_element_t *element = prev->next;

  prev->next = element->next;

  return element;
}

#if CH_FACTORY_REQUIRES_HEAP || defined(__DOXYGEN__)
static dyn_element_t *dyn_create_object_heap(const char *name,
                                             dyn_list_t *dlp,
                                             size_t size,
                                             unsigned align) {
  dyn_element_t *dep;

  chDbgCheck(name != NULL);

  /* Checking if an object with this name has already been created.*/
  dep = dyn_list_find(name, dlp);
  if (dep != NULL) {
    return NULL;
  }

  /* Allocating space for the new buffer object.*/
  dep = (dyn_element_t *)chHeapAllocAligned(NULL, size, align);
  if (dep == NULL) {
    return NULL;
  }

  /* Initializing object list element.*/
  copy_name(name, dep->name);
  dep->refs = (ucnt_t)1;
  dep->next = dlp->next;

  /* Updating factory list.*/
  dlp->next = dep;

  return dep;
}

static ucnt_t dyn_release_object_heap(dyn_element_t *dep,
                                      dyn_list_t *dlp) {
  dyn_element_t *prev;
  ucnt_t refs;

  chDbgCheck(dep != NULL);

  /* Checking 1st if the object is in the list.*/
  prev = dyn_list_find_prev(dep, dlp);
  if (prev != NULL) {

    chDbgAssert(dep->refs > (ucnt_t)0, "invalid references number");

    refs = --dep->refs;
    if (refs == (ucnt_t)0) {
      chHeapFree((void *)dyn_list_unlink(prev));
    }
  }
  else {

    chDbgAssert(false, "unknown object");

    refs = (ucnt_t)0;
  }

  return refs;
}
#endif /* CH_FACTORY_REQUIRES_HEAP */

#if CH_FACTORY_REQUIRES_POOLS || defined(__DOXYGEN__)
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
  copy_name(name, dep->name);
  dep->refs = (ucnt_t)1;
  dep->next = dlp->next;

  /* Updating factory list.*/
  dlp->next = (dyn_element_t *)dep;

  return dep;
}

static ucnt_t dyn_release_object_pool(dyn_element_t *dep,
                                      dyn_list_t *dlp,
                                      memory_pool_t *mp) {
  dyn_element_t *prev;
  ucnt_t refs;

  chDbgCheck(dep != NULL);

  /* Checking 1st if the object is in the list.*/
  prev = dyn_list_find_prev(dep, dlp);
  if (prev != NULL) {

    chDbgAssert(dep->refs > (ucnt_t)0, "invalid references number");

    refs = --dep->refs;
    if (refs == (ucnt_t)0) {
      chPoolFree(mp, (void *)dyn_list_unlink(prev));
    }
  }
  else {

    chDbgAssert(false, "unknown object");

    refs = (ucnt_t)0;
  }

  return refs;
}
#endif /* CH_FACTORY_REQUIRES_POOLS */

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
void __factory_init(void) {

#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  chMtxObjectInit(&ch_factory.mtx);
#else
  chSemObjectInit(&ch_factory.sem, (cnt_t)1);
#endif

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
#if CH_CFG_FACTORY_OBJ_FIFOS == TRUE
  dyn_list_init(&ch_factory.fifo_list);
#endif
#if CH_CFG_FACTORY_PIPES == TRUE
  dyn_list_init(&ch_factory.pipe_list);
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
 * @return              The reference to the registered object.
 * @retval NULL         if the object to be registered cannot be allocated or
 *                      a registered object with the same name exists.
 *
 * @api
 */
registered_object_t *chFactoryRegisterObject(const char *name,
                                             void *objp) {
  registered_object_t *rop;

  FACTORY_LOCK();

  rop = (registered_object_t *)dyn_create_object_pool(name,
                                                      &ch_factory.obj_list,
                                                      &ch_factory.obj_pool);
  if (rop != NULL) {
    /* Initializing registered object data.*/
    rop->objp = objp;
  }

  FACTORY_UNLOCK();

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

  FACTORY_LOCK();

  rop = (registered_object_t *)dyn_find_object(name, &ch_factory.obj_list);

  FACTORY_UNLOCK();

  return rop;
}

/**
 * @brief   Retrieves a registered object by pointer.
 * @post    A reference to the registered object is returned with the
 *          reference counter increased by one.
 *
 * @param[in] objp      pointer to the object to be retrieved
 *
 * @return              The reference to the found registered object.
 * @retval NULL         if a registered object with the specified pointer
 *                      does not exist.
 *
 * @api
 */
registered_object_t *chFactoryFindObjectByPointer(void *objp) {
  registered_object_t *rop = (registered_object_t *)ch_factory.obj_list.next;

  FACTORY_LOCK();

  while ((void *)rop != (void *)&ch_factory.obj_list) {
    if (rop->objp == objp) {
      rop->element.refs++;

      FACTORY_UNLOCK();

      return rop;
    }
    rop = (registered_object_t *)rop->element.next;
  }

  FACTORY_UNLOCK();

  return NULL;
}

/**
 * @brief   Releases a registered object and report subsequent reference count.
 * @details The reference counter of the registered object is decreased
 *          by one. If the count reaches zero then the containing list element
 *          is returned to the free pool. The reference count is returned so
 *          that caller can take action based on references becoming zero.
 * @note    The registered object itself is not freed since it could be static.
 *          Only the containing list element is freed.
 *
 * @param[in] rop       registered object reference
 * @return 		        The reference count of registered object subsequent to
 *                      release.
 * @retval 0		    if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleaseObject(registered_object_t *rop) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_pool(&rop->element,
                                 &ch_factory.obj_list,
                                 &ch_factory.obj_pool);

  FACTORY_UNLOCK();

  return refs;
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

  FACTORY_LOCK();

  dbp = (dyn_buffer_t *)dyn_create_object_heap(name,
                                               &ch_factory.buf_list,
                                               sizeof (dyn_buffer_t) + size,
                                               CH_HEAP_ALIGNMENT);
  if (dbp != NULL) {
    /* Initializing buffer object data.*/
    memset((void *)(dbp + 1), 0, size);
  }

  FACTORY_UNLOCK();

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

  FACTORY_LOCK();

  dbp = (dyn_buffer_t *)dyn_find_object(name, &ch_factory.buf_list);

  FACTORY_UNLOCK();

  return dbp;
}

/**
 * @brief   Releases a dynamic buffer object.
 * @details The reference counter of the dynamic buffer object is decreased
 *          by one, if reaches zero then the dynamic buffer object memory
 *          is freed.
 *
 * @param[in] dbp       dynamic buffer object reference
 * @return              The reference count of registered object subsequent to
 *                      release.
 * @retval 0            if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleaseBuffer(dyn_buffer_t *dbp) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_heap(&dbp->element, &ch_factory.buf_list);

  FACTORY_UNLOCK();

  return refs;
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

  FACTORY_LOCK();

  dsp = (dyn_semaphore_t *)dyn_create_object_pool(name,
                                                  &ch_factory.sem_list,
                                                  &ch_factory.sem_pool);
  if (dsp != NULL) {
    /* Initializing semaphore object dataa.*/
    chSemObjectInit(&dsp->sem, n);
  }

  FACTORY_UNLOCK();

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

  FACTORY_LOCK();

  dsp = (dyn_semaphore_t *)dyn_find_object(name, &ch_factory.sem_list);

  FACTORY_UNLOCK();

  return dsp;
}

/**
 * @brief   Releases a dynamic semaphore object.
 * @details The reference counter of the dynamic semaphore object is decreased
 *          by one, if reaches zero then the dynamic semaphore object memory
 *          is freed.
 *
 * @param[in] dsp       dynamic semaphore object reference
 * @return              The reference count of registered object subsequent to
 *                      release.
 * @retval 0            if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleaseSemaphore(dyn_semaphore_t *dsp) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_pool(&dsp->element,
                                 &ch_factory.sem_list,
                                 &ch_factory.sem_pool);

  FACTORY_UNLOCK();

  return refs;
}
#endif /* CH_CFG_FACTORY_SEMAPHORES = TRUE */

#if (CH_CFG_FACTORY_MAILBOXES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic mailbox object.
 * @post    A reference to the dynamic mailbox object is returned and the
 *          reference counter is initialized to one.
 * @post    The dynamic mailbox object is initialized and ready to use.
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
dyn_mailbox_t *chFactoryCreateMailbox(const char *name, size_t n) {
  dyn_mailbox_t *dmp;

  FACTORY_LOCK();

  dmp = (dyn_mailbox_t *)dyn_create_object_heap(name,
                                                &ch_factory.mbx_list,
                                                sizeof (dyn_mailbox_t) +
                                                (n * sizeof (msg_t)),
                                                CH_HEAP_ALIGNMENT);
  if (dmp != NULL) {
    /* Initializing mailbox object data.*/
    chMBObjectInit(&dmp->mbx, (msg_t *)(dmp + 1), n);
  }

  FACTORY_UNLOCK();

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

  FACTORY_LOCK();

  dmp = (dyn_mailbox_t *)dyn_find_object(name, &ch_factory.mbx_list);

  FACTORY_UNLOCK();

  return dmp;
}

/**
 * @brief   Releases a dynamic mailbox object.
 * @details The reference counter of the dynamic mailbox object is decreased
 *          by one, if reaches zero then the dynamic mailbox object memory
 *          is freed.
 *
 * @param[in] dmp       dynamic mailbox object reference
 * @return              The reference count of registered object subsequent to
 *                      release.
 * @retval 0            if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleaseMailbox(dyn_mailbox_t *dmp) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_heap(&dmp->element, &ch_factory.mbx_list);

  FACTORY_UNLOCK();

  return refs;
}
#endif /* CH_CFG_FACTORY_MAILBOXES = TRUE */

#if (CH_CFG_FACTORY_OBJ_FIFOS == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic "objects FIFO" object.
 * @post    A reference to the dynamic "objects FIFO" object is returned and
 *          the reference counter is initialized to one.
 * @post    The dynamic "objects FIFO" object is initialized and ready to use.
 *
 * @param[in] name      name to be assigned to the new dynamic "objects FIFO"
 *                      object
 * @param[in] objsize   size of objects
 * @param[in] objn      number of objects available
 * @param[in] objalign  required objects alignment
 * @return              The reference to the created dynamic "objects FIFO"
 *                      object.
 * @retval NULL         if the dynamic "objects FIFO" object cannot be
 *                      allocated or a dynamic "objects FIFO" object with
 *                      the same name exists.
 *
 * @api
 */
dyn_objects_fifo_t *chFactoryCreateObjectsFIFO(const char *name,
                                               size_t objsize,
                                               size_t objn,
                                               unsigned objalign) {
  size_t size1, size2;
  dyn_objects_fifo_t *dofp;

  FACTORY_LOCK();

  /* Enforcing alignment for the objects array.*/
  objsize = MEM_ALIGN_NEXT(objsize, objalign);
  size1   = MEM_ALIGN_NEXT(sizeof (dyn_objects_fifo_t) + (objn * sizeof (msg_t)),
                           objalign);
  size2   = objn * objsize;

  /* Allocating the FIFO object with messages buffer and objects buffer.*/
  dofp = (dyn_objects_fifo_t *)dyn_create_object_heap(name,
                                                      &ch_factory.fifo_list,
                                                      size1 + size2,
                                                      objalign);
  if (dofp != NULL) {
    msg_t *msgbuf = (msg_t *)(dofp + 1);
    uint8_t *objbuf = (uint8_t *)dofp + size1;

    /* Initializing mailbox object data.*/
    chFifoObjectInitAligned(&dofp->fifo, objsize, objn, objalign,
                            (void *)objbuf, msgbuf);
  }

  FACTORY_UNLOCK();

  return dofp;
}

/**
 * @brief   Retrieves a dynamic "objects FIFO" object.
 * @post    A reference to the dynamic "objects FIFO" object is returned with
 *          the reference counter increased by one.
 *
 * @param[in] name      name of the dynamic "objects FIFO" object
 *
 * @return              The reference to the found dynamic "objects FIFO"
 *                      object.
 * @retval NULL         if a dynamic "objects FIFO" object with the specified
 *                      name does not exist.
 *
 * @api
 */
dyn_objects_fifo_t *chFactoryFindObjectsFIFO(const char *name) {
  dyn_objects_fifo_t *dofp;

  FACTORY_LOCK();

  dofp = (dyn_objects_fifo_t *)dyn_find_object(name, &ch_factory.fifo_list);

  FACTORY_UNLOCK();

  return dofp;
}

/**
 * @brief   Releases a dynamic "objects FIFO" object.
 * @details The reference counter of the dynamic "objects FIFO" object is
 *          decreased by one, if reaches zero then the dynamic "objects FIFO"
 *          object memory is freed.
 *
 * @param[in] dofp      dynamic "objects FIFO" object reference
 * @return              The reference count of registered object subsequent to
 *                      release.
 * @retval 0            if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleaseObjectsFIFO(dyn_objects_fifo_t *dofp) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_heap(&dofp->element, &ch_factory.fifo_list);

  FACTORY_UNLOCK();

  return refs;
}
#endif /* CH_CFG_FACTORY_OBJ_FIFOS = TRUE */

#if (CH_CFG_FACTORY_PIPES == TRUE) || defined(__DOXIGEN__)
/**
 * @brief   Creates a dynamic pipe object.
 * @post    A reference to the dynamic pipe object is returned and
 *          the reference counter is initialized to one.
 * @post    The dynamic pipe object is initialized and ready to use.
 *
 * @param[in] name      name to be assigned to the new dynamic pipe
 *                      object
 * @param[in] size      pipe buffer size
 * @return              The reference to the created dynamic pipe
 *                      object.
 * @retval NULL         if the dynamic pipe object cannot be
 *                      allocated or a dynamic pipe object with
 *                      the same name exists.
 *
 * @api
 */
dyn_pipe_t *chFactoryCreatePipe(const char *name, size_t size) {
  dyn_pipe_t *dpp;

  FACTORY_LOCK();

  dpp = (dyn_pipe_t *)dyn_create_object_heap(name,
                                             &ch_factory.pipe_list,
                                             sizeof (dyn_pipe_t) + size,
                                             CH_HEAP_ALIGNMENT);
  if (dpp != NULL) {
    /* Initializing mailbox object data.*/
    chPipeObjectInit(&dpp->pipe, (uint8_t *)(dpp + 1), size);
  }

  FACTORY_UNLOCK();

  return dpp;
}

/**
 * @brief   Retrieves a dynamic pipe object.
 * @post    A reference to the dynamic pipe object is returned with
 *          the reference counter increased by one.
 *
 * @param[in] name      name of the pipe object
 *
 * @return              The reference to the found dynamic pipe
 *                      object.
 * @retval NULL         if a dynamic pipe object with the specified
 *                      name does not exist.
 *
 * @api
 */
dyn_pipe_t *chFactoryFindPipe(const char *name) {
  dyn_pipe_t *dpp;

  FACTORY_LOCK();

  dpp = (dyn_pipe_t *)dyn_find_object(name, &ch_factory.pipe_list);

  FACTORY_UNLOCK();

  return dpp;
}

/**
 * @brief   Releases a dynamic pipe object.
 * @details The reference counter of the dynamic pipe object is
 *          decreased by one, if reaches zero then the dynamic pipe
 *          object memory is freed.
 *
 * @param[in] dpp       dynamic pipe object reference
 * @return              The reference count of registered object subsequent to
 *                      release.
 * @retval 0            if the object has been released.
 *
 * @api
 */
ucnt_t chFactoryReleasePipe(dyn_pipe_t *dpp) {
  ucnt_t refs;

  FACTORY_LOCK();

  refs = dyn_release_object_heap(&dpp->element, &ch_factory.pipe_list);

  FACTORY_UNLOCK();

  return refs;
}
#endif /* CH_CFG_FACTORY_PIPES = TRUE */

#endif /* CH_CFG_USE_FACTORY == TRUE */

/** @} */
