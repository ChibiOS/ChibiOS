/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
 * @file    chobjcaches.c
 * @brief   Objects Caches code.
 * @details Objects caches.
 *          <h2>Operation mode</h2>
 *          An object cache allows to retrieve and release objects from a
 *          slow media, for example a disk or flash.<br>
 *          The most recently used objects are kept in a series of RAM
 *          buffers making access faster. Objects are identified by a
 *          pair <group, key> which could be mapped, for example, to a
 *          disk drive identifier and sector identifier.<br>
 *          Read and write operations are performed using externally-supplied
 *          functions, the cache is device-agnostic.<br>
 *          The cache uses internally an hash table, the size of the table
 *          should be dimensioned to minimize the risk of hash collisions,
 *          a factor of two is usually acceptable, it depends on the specific
 *          application requirements.<br>
 *          Operations defined for caches:
 *          - <b>Get Object</b>: Retrieves an object from cache, if not
 *            present then an empty buffer is returned.
 *          - <b>Read Object</b>: Retrieves an object from cache, if not
 *            present a buffer is allocated and the object is read from the
 *            media.
 *          - <b>Release Object</b>: Releases an object to the cache handling
 *            the media update, if required.
 *          .
 * @pre     In order to use the pipes APIs the @p CH_CFG_USE_OBJ_CACHES
 *          option must be enabled in @p chconf.h.
 * @note    Compatible with RT and NIL.
 *
 * @addtogroup oslib_objchaches
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_OBJ_CACHES == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/* Default hash function.*/
#if !defined(OC_HASH_FUNCTION) || defined(__DOXYGEN__)
#define OC_HASH_FUNCTION(ocp, group, key)                                   \
  (((unsigned)(group) + (unsigned)(key)) & ((unsigned)(ocp)->hashn - 1U))
#endif

/* Insertion into an hash slot list.*/
#define HASH_INSERT(ocp, objp, group, key) {                                \
  oc_hash_header_t *hhp;                                                    \
  (hhp) = &(ocp)->hashp[OC_HASH_FUNCTION(ocp, group, key)];                 \
  (objp)->hash_next = (hhp)->hash_next;                                     \
  (objp)->hash_prev = (oc_object_t *)(hhp);                                 \
  (hhp)->hash_next->hash_prev = (objp);                                     \
  (hhp)->hash_next = (objp);                                                \
}

/* Removal of an object from the hash.*/
#define HASH_REMOVE(objp) {                                                 \
  (objp)->hash_prev->hash_next = (objp)->hash_next;                         \
  (objp)->hash_next->hash_prev = (objp)->hash_prev;                         \
}

/* Insertion on LRU list head (newer objects).*/
#define LRU_INSERT_HEAD(ocp, objp) {                                        \
  (objp)->lru_next = (ocp)->lru.lru_next;                                   \
  (objp)->lru_prev = (oc_object_t *)&(ocp)->lru;                            \
  (ocp)->lru.lru_next->lru_prev = (objp);                                   \
  (ocp)->lru.lru_next = (objp);                                             \
}

/* Insertion on LRU list head (newer objects).*/
#define LRU_INSERT_TAIL(ocp, objp) {                                        \
  (objp)->lru_prev = (ocp)->lru.lru_prev;                                   \
  (objp)->lru_next = (oc_object_t *)&(ocp)->lru;                            \
  (ocp)->lru.lru_prev->lru_next = (objp);                                   \
  (ocp)->lru.lru_prev = (objp);                                             \
}

/* Removal of an object from the LRU list.*/
#define LRU_REMOVE(objp) {                                                  \
  (objp)->lru_prev->lru_next = (objp)->lru_next;                            \
  (objp)->lru_next->lru_prev = (objp)->lru_prev;                            \
}

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Returns an object pointer from the cache, if present.
 *
 * @param[out] ocp      pointer to the @p objects_cache_t structure to be
 * @param[in] group     object group identifier
 * @param[in] key       object identifier within the group
 *                      initialized
 * @return              The pointer to the retrieved object.
 * @retval NULL         if the object is not in cache.
 *
 * @notapi
 */
static oc_object_t *hash_get_s(objects_cache_t *ocp,
                               uint32_t group,
                               uint32_t key) {
  oc_hash_header_t *hhp;
  oc_object_t *objp;

  /* Hash slot where to search for an hit.*/
  hhp  = &ocp->hashp[OC_HASH_FUNCTION(ocp, group, key)];
  objp = hhp->hash_next;

  /* Scanning the siblings collision list.*/
  while (objp != (oc_object_t *)hhp) {
    if ((objp->obj_key == key) && (objp->obj_group == group)) {

      /* Cache hit.*/
      return objp;
    }
    objp = objp->hash_next;
  }

  return NULL;
}

/**
 * @brief   Gets the least recently used object buffer from the LRU list.
 *
 * @param[out] ocp      pointer to the @p objects_cache_t structure to be
 * @return              The pointer to the retrieved object.
 *
 * @notapi
 */
static oc_object_t *lru_get_last_s(objects_cache_t *ocp) {
  oc_object_t *objp;

  while (true) {
    /* Waiting for an object buffer to become available in the LRU.*/
    chSemWaitS(&ocp->lru_sem);

    /* Now an object buffer is in the LRU for sure, taking it from the
       LRU tail.*/
    objp = ocp->lru.lru_prev;

    chDbgAssert((objp->obj_flags & OC_FLAG_INLRU) == OC_FLAG_INLRU,
                "not in LRU");
    chDbgAssert(chSemGetCounterI(&objp->obj_sem) == (cnt_t)1,
                "semaphore counter not 1");

    LRU_REMOVE(objp);
    objp->obj_flags &= ~OC_FLAG_INLRU;

    /* Getting the object semaphore, we know there is no wait so
       using the "fast" variant.*/
    chSemFastWaitI(&objp->obj_sem);

    /* If it is a buffer not needing write then it can be used right away.*/
    if ((objp->obj_flags & OC_FLAG_MODIFIED) != 0U) {

      return objp;
    }


    /* Out of critical section.*/
    chSysUnlock();

   /* Invoking the writer asynchronously, it will release the buffer once it
      is written, the operation could be asynchronous. It is responsibility
      of the write function to release the buffer (synchronously or
      asynchronously).*/
    ocp->writef(objp, true);

    /* Critical section enter again.*/
    chSysLock();
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes a @p objects_cache_t object.
 *
 * @param[out] ocp      pointer to the @p objects_cache_t structure to be
 *                      initialized
 * @param[in] hashn     number of elements in the hash table array, must be
 *                      a power of two and not lower than @p objn
 * @param[in] hashp     pointer to the hash table as an array of
 *                      @p oc_hash_header_t
 * @param[in] objn      number of elements in the objects table array
 * @param[in] hashp     pointer to the hash objects as an array of
 *                      @p oc_object_t
 * @param[in] readf     pointer to an object reader function
 * @param[in] writef    pointer to an object writer function
 *
 * Object records states:
 * - Invalid not owned:
 *    (OC_FLAG_INLRU, cnt==1).
 * - Caching an object not owned:
 *    (OC_FLAG_INLRU, OC_FLAG_INHASH, OC_FLAG_CACHEHIT, cnt==1).
 * - Representing an object owned:
 *    (OC_FLAG_INHASH, cnt<=0).
 * - Caching an object owned:
 *    (OC_FLAG_INHASH, OC_FLAG_CACHEHIT, cnt<=0).
 *
 * @init
 */
void chCacheObjectInit(objects_cache_t *ocp,
                       ucnt_t hashn,
                       oc_hash_header_t *hashp,
                       ucnt_t objn,
                       oc_object_t *objp,
                       oc_readf_t readf,
                       oc_writef_t writef) {

  chDbgCheck((ocp != NULL) && (hashp != NULL) && (objp != NULL) &&
             ((hashn & (hashn - 1U)) == 0U) &&
             (objn > (size_t)0) && (hashn >= objn));

  chSemObjectInit(&ocp->cache_sem, (cnt_t)1);
  chSemObjectInit(&ocp->lru_sem, (cnt_t)objn);
  ocp->hashn            = hashn;
  ocp->hashp            = hashp;
  ocp->objn             = objn;
  ocp->objp             = objp;
  ocp->readf            = readf;
  ocp->writef           = writef;
  ocp->lru.hash_next    = NULL;
  ocp->lru.hash_prev    = NULL;
  ocp->lru.lru_next     = (oc_object_t *)&ocp->lru;
  ocp->lru.lru_prev     = (oc_object_t *)&ocp->lru;

  /* Hash headers initialization.*/
  while (hashp < &ocp->hashp[ocp->hashn]) {
    hashp->hash_next = (oc_object_t *)hashp;
    hashp->hash_prev = (oc_object_t *)hashp;
  }

  /* Object headers initialization.*/
  while (objp < &ocp->objp[ocp->objn]) {
    chSemObjectInit(&objp->obj_sem, (cnt_t)1);
    LRU_INSERT_HEAD(ocp, objp);
    objp->obj_group = 0U;
    objp->obj_key   = 0U;
    objp->obj_flags = OC_FLAG_INLRU;
    objp->data      = NULL;
  }
}

/**
 * @brief   Retrieves an object from the cache.
 * @note    If the object is not in cache then the returned object is marked
 *          as @p OC_FLAG_NOTREAD meaning that its data contains garbage and
 *          must be initialized.
 * @note    If the object has been modified by another thread while in cache
 *          (and not written) then it is marked as @p OC_FLAG_MODIFIED.
 *
 * @param[in] ocp       pointer to the @p objects_cache_t structure
 * @param[in] group     object group identifier or @p OC_NO_GROUP if
 *                      requesting a generic object buffer
 * @param[in] key       object identifier within the group
 * @return              The pointer to the retrieved object.
 * @retval NULL         is a reserved value.
 *
 * @api
 */
oc_object_t *chCacheGetObject(objects_cache_t *ocp,
                              uint32_t group,
                              uint32_t key) {
  oc_object_t *objp;

  /* Critical section enter, the hash check operation is fast.*/
  chSysLock();

  /* If the requested object is not a generic one.*/
  if (group == OC_NO_GROUP) {
    /* Any buffer will do.*/
    objp = NULL; /* TODO */
  }
  else {
    /* Checking the cache for a hit.*/
    objp = hash_get_s(ocp, group, key);

    chDbgAssert((objp->obj_flags & OC_FLAG_INHASH) == OC_FLAG_INHASH,
                "not in hash");

    if (objp != NULL) {
      /* Cache hit, checking if the buffer is owned by some
         other thread.*/
      if (chSemGetCounterI(&objp->obj_sem) > (cnt_t)0) {
        /* Not owned case, it is in the LRU list.*/

        chDbgAssert((objp->obj_flags & OC_FLAG_INLRU) == OC_FLAG_INLRU,
                    "not in LRU");

        /* Removing the object from LRU, now it is "owned".*/
        LRU_REMOVE(objp);
        objp->obj_flags &= ~OC_FLAG_INLRU;

        /* Getting the object semaphore, we know there is no wait so
           using the "fast" variant.*/
        chSemFastWaitI(&objp->obj_sem);
      }
      else {
        /* Owned case, some other thread is playing with this object, we
           need to wait.*/

        chDbgAssert((objp->obj_flags & OC_FLAG_INLRU) == 0U, "in LRU");

        /* Waiting on the buffer semaphore.*/
        chSemWaitS(&objp->obj_sem);
      }
    }
    else {
      /* Cache miss, getting an object buffer from the LRU list.*/
      objp = lru_get_last_s(ocp);

      /* Naming this object and publishing it in the hash table.*/
      objp->obj_group  = group;
      objp->obj_key    = key;
      objp->obj_flags |= OC_FLAG_INHASH | OC_FLAG_NOTREAD;
      HASH_INSERT(ocp, objp, group, key);
    }
  }

  /* Out of critical section and returning the object.*/
  chSysUnlock();

  return objp;
}

/**
 * @brief   Releases an object into the cache.
 * @note    This function gives a meaning to the following flags:
 *          - @p OC_FLAG_INLRU must be cleared.
 *          - @p OC_FLAG_INHASH must be set.
 *          - @p OC_FLAG_SHARED must be cleared.
 *          - @p OC_FLAG_ERROR is ignored.
 *          - @p OC_FLAG_NOTREAD invalidates the object and queues it on
 *            the LRU tail.
 *          - @p OC_FLAG_MODIFIED is ignored and kept, a write will occur
 *            when the object is removed from the LRU list (lazy write).
 *          .
 *
 * @param[in] ocp       pointer to the @p objects_cache_t structure
 * @param[in] objp      pointer to the @p oc_object_t structure
 *
 * @iclass
 */
void chCacheReleaseObjectI(objects_cache_t *ocp,
                           oc_object_t *objp) {

  /* Checking initial conditions of the object to be released.*/
  chDbgAssert((objp->obj_flags & (OC_FLAG_INLRU |
                                  OC_FLAG_INHASH |
                                  OC_FLAG_SHARED)) == OC_FLAG_INHASH,
              "invalid object state");
  chDbgAssert(chSemGetCounterI(&objp->obj_sem) <= (cnt_t)0,
              "semaphore counter greater than 0");

  /* If some thread is waiting for this specific buffer then it is
     handed directly without going through the LRU.*/
  if (chSemGetCounterI(&objp->obj_sem) < (cnt_t)0) {
    /* Clearing all flags except those that are still meaningful, note,
       OC_FLAG_NOTREAD and OC_FLAG_MODIFIED are passed, the other thread
       will handle them.*/
    objp->obj_flags &= OC_FLAG_INHASH | OC_FLAG_NOTREAD | OC_FLAG_MODIFIED;
    chSemSignalI(&objp->obj_sem);
    return;
  }

  /* If the object specifies OC_FLAG_NOTREAD then it must be invalidated
     and removed from the hash table.*/
  if ((objp->obj_flags & OC_FLAG_NOTREAD) != 0U) {
    HASH_REMOVE(objp);
    LRU_INSERT_TAIL(ocp, objp);
    objp->obj_group = OC_NO_GROUP;
    objp->obj_key   = 0U;
    objp->obj_flags = OC_FLAG_INLRU;
  }
  else {
    /* LRU insertion point depends on the OC_FLAG_FORGET flag.*/
    if ((objp->obj_flags & OC_FLAG_FORGET) == 0U) {
      /* Placing it on head.*/
      LRU_INSERT_HEAD(ocp, objp);
    }
    else {
      /* Low priority data, placing it on tail.*/
      LRU_INSERT_TAIL(ocp, objp);
    }
    objp->obj_flags &= OC_FLAG_INHASH | OC_FLAG_MODIFIED;
    objp->obj_flags |= OC_FLAG_INLRU;
  }

  /* Increasing the LRU counter semaphore.*/
  chSemSignalI(&ocp->lru_sem);

  /* Releasing the object, we know there are no threads waiting so
     using the "fast" signal variant.*/
  chSemFastSignalI(&objp->obj_sem);
}

/**
 * @brief   Reads object data into the object buffer.
 * @note    In case of read error the behavior is the following:
 *          - Synchronous operation: The @p OC_FLAG_ERROR and
 *            @p OC_FLAG_NOTREAD flags are enforced in the object
 *            buffer.
 *          - Asynchronous operation: The object buffer is discarded, there
 *            is no error notification unless implemented in the physical
 *            read function.
 *          .
 *
 * @param[in] ocp       pointer to the @p objects_cache_t structure
 * @param[in] async     requests an asynchronous operation if supported, the
 *                      function is then responsible for releasing the
 *                      object
 *
 * @api
 */
void chCacheReadObject(objects_cache_t *ocp,
                       oc_object_t *objp,
                       bool async) {

}

void chCacheWriteObject(objects_cache_t *ocp,
                        oc_object_t *objp,
                        bool async);

#endif /* CH_CFG_USE_OBJ_CACHES == TRUE */

/** @} */
