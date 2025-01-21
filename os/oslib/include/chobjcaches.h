/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
 * @file    oslib/include/chobjcaches.h
 * @brief   Objects Caches macros and structures.
 *
 * @addtogroup oslib_objchaches
 * @{
 */

#ifndef CHOBJCACHES_H
#define CHOBJCACHES_H

#if (CH_CFG_USE_OBJ_CACHES == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Cached objects flags
 * @{
 */
#define OC_FLAG_INLRU                       0x00000001U
#define OC_FLAG_INHASH                      0x00000002U
#define OC_FLAG_SHARED                      0x00000004U
#define OC_FLAG_NOTSYNC                     0x00000008U
#define OC_FLAG_LAZYWRITE                   0x00000010U
#define OC_FLAG_FORGET                      0x00000020U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_USE_SEMAPHORES == FALSE
#error "CH_CFG_USE_OBJ_CACHES requires CH_CFG_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Flags of cached objects.
 */
typedef uint32_t oc_flags_t;

/**
 * @brief   Type of an hash element header.
 */
typedef struct oc_hash_element oc_hash_element_t;

/**
 * @brief   Type of an object element.
 */
typedef struct oc_lru_element oc_lru_element_t;

/**
 * @brief   Type of a cached object.
 */
typedef struct oc_object oc_object_t;

/**
 * @brief   Type of a cache object.
 */
typedef struct objects_cache objects_cache_t;

/**
 * @brief   Object read function.
 *
 * @param[in] ocp       pointer to the @p objects_cache_t object
 * @param[in] objp      pointer to the @p oc_object_t object
 * @param[in] async     requests an asynchronous operation if supported, the
 *                      function is then responsible for releasing the
 *                      object
 */
typedef bool (*oc_readf_t)(objects_cache_t *ocp,
                           oc_object_t *objp,
                           bool async);

/**
 * @brief   Object write function.
 *
 * @param[in] ocp       pointer to the @p objects_cache_t object
 * @param[in] objp      pointer to the @p oc_object_t object
 * @param[in] async     requests an asynchronous operation if supported, the
 *                      function is then responsible for releasing the
 *                      object
 */
typedef bool (*oc_writef_t)(objects_cache_t *ocp,
                            oc_object_t *objp,
                            bool async);

/**
 * @brief   Structure representing an hash table element.
 */
struct oc_hash_element {
  /**
   * @brief   Next in the collisions list.
   */
  oc_hash_element_t     *next;
  /**
   * @brief   Previous in the collisions list.
   */
  oc_hash_element_t     *prev;
};

/**
 * @brief   Structure representing an LRU list element.
 */
struct oc_lru_element {
  /**
   * @brief   Hash collision list element
   */
  oc_hash_element_t     h;
  /**
   * @brief   Next in the LRU list.
   */
  oc_lru_element_t    *next;
  /**
   * @brief   Previous in the LRU list.
   */
  oc_lru_element_t    *prev;
};

/**
 * @brief   Structure representing a cached object.
 */
struct oc_object {
  /**
   * @brief   Hash and LRU links.
   */
  oc_lru_element_t      list;
  /**
   * @brief   Object pointer to the owner.
   */
  void                  *obj_owner;
  /**
   * @brief   Object key.
   */
  uint32_t              obj_key;
  /**
   * @brief   Semaphore for object access.
   */
  semaphore_t           obj_sem;
  /**
   * @brief   Object flags.
   */
  oc_flags_t            obj_flags;
  /**
   * @brief   User pointer.
   * @note    This pointer can be used to refer to external buffers,
   *          @p chCacheObjectInit() initializes it to @p NULL.
   */
  void                  *dptr;
};

/**
 * @brief   Structure representing a cache object.
 */
struct objects_cache {
  /**
   * @brief   Number of elements in the hash table.
   */
  ucnt_t                hashn;
  /**
   * @brief   Pointer to the hash table.
   */
  oc_hash_element_t  *hashp;
  /**
   * @brief   Number of elements in the objects table.
   */
  ucnt_t                objn;
  /**
   * @brief   Size of elements in the objects table.
   */
  size_t                objsz;
  /**
   * @brief   Pointer to the objects table.
   */
  void                  *objvp;
  /**
   * @brief   LRU list header.
   */
  oc_lru_element_t      list;
  /**
   * @brief   Semaphore for cache access.
   */
  semaphore_t           cache_sem;
  /**
   * @brief   Semaphore for LRU access.
   */
  semaphore_t           lru_sem;
  /**
   * @brief   Reader functions for cached objects.
   */
  oc_readf_t            readf;
  /**
   * @brief   Writer functions for cached objects.
   */
  oc_writef_t           writef;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void chCacheObjectInit(objects_cache_t *ocp,
                         ucnt_t hashn,
                         oc_hash_element_t *hashp,
                         ucnt_t objn,
                         size_t objsz,
                         void *objvp,
                         oc_readf_t readf,
                         oc_writef_t writef);
  oc_object_t *chCacheGetObject(objects_cache_t *ocp,
                                void *owner,
                                uint32_t key);
  void chCacheReleaseObjectI(objects_cache_t *ocp,
                             oc_object_t *objp);
  bool chCacheReadObject(objects_cache_t *ocp,
                         oc_object_t *objp,
                         bool async);
  bool chCacheWriteObject(objects_cache_t *ocp,
                          oc_object_t *objp,
                          bool async);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Releases an object into the cache.
 * @note    This function gives a meaning to the following flags:
 *          - @p OC_FLAG_INLRU must be cleared.
 *          - @p OC_FLAG_INHASH must be set.
 *          - @p OC_FLAG_SHARED must be cleared.
 *          - @p OC_FLAG_NOTSYNC invalidates the object and queues it on
 *            the LRU tail.
 *          - @p OC_FLAG_LAZYWRITE is ignored and kept, a write will occur
 *            when the object is removed from the LRU list (lazy write).
 *
 * @param[in] ocp       pointer to the @p objects_cache_t object
 * @param[in] objp      pointer to the @p oc_object_t structure
 *
 * @api
 */
static inline void chCacheReleaseObject(objects_cache_t *ocp,
                                        oc_object_t *objp) {

  chSysLock();
  chCacheReleaseObjectI(ocp, objp);
  chSchRescheduleS();
  chSysUnlock();
}

#endif /* CH_CFG_USE_OBJ_CACHES == TRUE */

#endif /* CHOBJCACHES_H */

/** @} */
