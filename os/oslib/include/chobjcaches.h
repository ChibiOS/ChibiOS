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
 * @file    chobjcaches.h
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
#define OC_FLAG_CACHEHIT                    0x00000004U
#define OC_FLAG_ERROR                       0x00000008U
#define OC_FLAG_MODIFIED                    0x00000010U
/** @} */

/**
 * @brief   Identifier for an invalid group.
 */
#define OC_NO_GROUP                         0xFFFFFFFFU

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
typedef struct ch_oc_hash_header oc_hash_header_t;

/**
 * @brief   Type of an LRU element header.
 */
typedef struct ch_oc_lru_header oc_lru_header_t;

/**
 * @brief   Type of a cached object.
 */
typedef struct ch_oc_object oc_object_t;

/**
 * @brief   Object read function.
 */
typedef msg_t (*oc_readf_t)(oc_object_t *cop);

/**
 * @brief   Object write function.
 */
typedef msg_t (*oc_writef_t)(oc_object_t *cop);

/**
 * @brief   Structure representing an hash table element.
 */
struct ch_oc_hash_header {
  /**
   * @brief   Next in the collisions list.
   */
  oc_object_t           *hash_next;
  /**
   * @brief   Previous in the collisions list.
   */
  oc_object_t           *hash_prev;
};

/**
 * @brief   Structure representing an hash table element.
 */
struct ch_oc_lru_header {
  /**
   * @brief   Next in the collisions list.
   */
  oc_object_t           *hash_next;
  /**
   * @brief   Previous in the collisions list.
   */
  oc_object_t           *hash_prev;
  /**
   * @brief   Next in the LRU list.
   */
  oc_object_t           *lru_next;
  /**
   * @brief   Previous in the LRU list.
   */
  oc_object_t           *lru_prev;
};

/**
 * @brief   Structure representing a cached object.
 */
struct ch_oc_object {
  /**
   * @brief   Next in the collisions list.
   */
  oc_object_t           *hash_next;
  /**
   * @brief   Previous in the collisions list.
   */
  oc_object_t           *hash_prev;
  /**
   * @brief   Next in the LRU list.
   */
  oc_object_t           *lru_next;
  /**
   * @brief   Previous in the LRU list.
   */
  oc_object_t           *lru_prev;
  /**
   * @brief   Object group.
   */
  uint32_t              obj_group;
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
   * @brief   Pointer to the data part of the object.
   * @note    This pointer must be initialized outside this module after
   *          calling @p chCacheObjectInit() which sets it to @p NULL.
   */
  void                  *data;
};

/**
 * @brief   Type of a cache object.
 */
typedef struct {
  /**
   * @brief   Number of elements in the hash table.
   */
  size_t                hashn;
  /**
   * @brief   Pointer to the hash table.
   */
  oc_hash_header_t      *hashp;
  /**
   * @brief   Number of elements in the objects table.
   */
  size_t                objn;
  /**
   * @brief   Pointer to the objects table.
   */
  oc_object_t           *objp;
  /**
   * @brief   LRU list header.
   */
  oc_lru_header_t       lru;
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
} objects_cache_t;

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
                         oc_hash_header_t *hashp,
                         ucnt_t objn,
                         oc_object_t *objp,
                         oc_readf_t readf,
                         oc_writef_t writef);
  oc_object_t *chCacheGetObject(objects_cache_t *ocp,
                                uint32_t group,
                                uint32_t key);
  void chCacheReleaseObject(objects_cache_t *ocp,
                            oc_object_t *objp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CH_CFG_USE_OBJ_CACHES == TRUE */

#endif /* CHOBJCACHES_H */

/** @} */
