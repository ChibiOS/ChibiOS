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
 * @details Byte pipes.
 *          <h2>Operation mode</h2>
 *          A pipe is an asynchronous communication mechanism.<br>
 *          Operations defined for mailboxes:
 *          - <b>Write</b>: Writes a buffer of data in the pipe in FIFO order.
 *          - <b>Read</b>: A buffer of data is read from the read and removed.
 *          - <b>Reset</b>: The pipe is emptied and all the stored data
 *            is lost.
 *          .
 * @pre     In order to use the pipes APIs the @p CH_CFG_USE_PIPES
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

/* Insertion on LRU head (newer objects).*/
#define LRU_INSERT_HEAD(ocp, objp) {                                        \
  (objp)->lru_next = (ocp)->lru.lru_next;                                   \
  (objp)->lru_prev = (oc_object_t *)&(ocp)->lru;                            \
  (ocp)->lru.lru_next->lru_prev = (objp);                                   \
  (ocp)->lru.lru_next = (objp);                                             \
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

static oc_object_t *oc_hash_get(objects_cache_t *ocp,
                                uint32_t group,
                                uint32_t key) {

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
 * @init
 */
void chCacheObjectInit(objects_cache_t *ocp,
                       ucnt_t hashn,
                       oc_hash_header_t *hashp,
                       ucnt_t objn,
                       oc_object_t *objp,
                       oc_readf_t readf,
                       oc_writef_t writef) {
  unsigned i;

  chDbgCheck((ocp != NULL) && (hashp != NULL) && (objp != NULL) &&
             (hashn & (hashn - 1U) == 0U) &&
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
    HASH_INSERT(ocp, objp, OC_NO_GROUP, (uint32_t)i);
    LRU_INSERT_HEAD(ocp, objp);
    objp->obj_group = OC_NO_GROUP;
    objp->obj_key   = 0U;
    objp->obj_flags = OC_FLAG_INVALID;
    objp->data      = NULL;
  }
}

/**
 * @brief   Retrieves an object from the cache.
 * @note    If the object is not in cache then the returned object does
 *          not specify the flag @p OC_FLAG_CACHEHIT meaning its data contains
 *          garbage and must be initialized.
 *
 * @param[in] ocp       pointer to the @p objects_cache_t structure
 * @param[in] group     object group identifier
 * @param[in] key       object identifier within the group
 * @return              The pointer to the retrieved object.
 * @retval NULL         is a reserved value.
 */
oc_object_t *chCacheGetObject(objects_cache_t *ocp,
                              uint32_t group,
                              uint32_t key) {

}

/**
 * @brief   Releases an object into the cache.
 *
 * @param[in] ocp       pointer to the @p objects_cache_t structure
 * @param[in] objp      pointer to the @p oc_object_t structure
 */
void chCacheReleaseObject(objects_cache_t *ocp,
                          oc_object_t *objp) {

}

#endif /* CH_CFG_USE_OBJ_CACHES == TRUE */

/** @} */
