/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    oop_referenced_object.h
 * @brief   Base class for objects with a reference counter.
 * @details This header defines a base class for classes requiring a
 *          a reference counter and a disposing mechanism.
 *
 * @addtogroup OOP_REFERENCED_OBJECT
 * @details Base class for objects that implement a reference counter and
 *          are disposed when the number of references reaches zero.
 * @{
 */

#ifndef OOP_REFERENCED_OBJECT_H
#define OOP_REFERENCED_OBJECT_H

#include "oop_object.h"

/**
 * @brief   Type of a referenced object class.
 */
typedef struct referenced_object referenced_object_c;

/**
 * @brief   @p referenced_object_c specific methods.
 * @note    This object defines no methods.
 */
#define __referenced_object_methods                                         \
  __base_object_methods

/**
 * @brief   @p referenced_object_c specific data.
 */
#define __referenced_object_data                                            \
  __base_object_data                                                        \
  unsigned                                  references;


/**
 * @brief   @p referenced_object_c virtual methods table.
 */
struct __referenced_object_vmt {                                            \
  __referenced_object_methods                                               \
  unsigned (*release)(referenced_object_c *ip);
};

/**
 * @brief   Structure representing a referenced object class.
 */
typedef struct referenced_object {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct __referenced_object_vmt      *vmt;
  __referenced_object_data
} referenced_object_c;

/**
 * @name    Methods implementations
 * @{
 */
/**
 * @brief   Object creation implementation.
 *
 * @param[in] ip        Pointer to a @p referenced_object_c structure to be
 *                      initialized.
 * @return              A new reference to the object.
 */
CC_FORCE_INLINE
static inline referenced_object_c *__referenced_object_objinit_impl(referenced_object_c *ip,
                                                                    const void *vmt) {

  __base_object_objinit_impl((base_object_c *)ip, vmt);
  ip->references = 1U;

  return ip;
}

/**
 * @brief   Object finalization implementation.
 *
 * @param[in] ip        Pointer to a @p referenced_object_c structure to be
 *                      disposed.
 */
CC_FORCE_INLINE
static inline void __referenced_object_dispose_impl(referenced_object_c *ip) {

  __base_object_dispose_impl((base_object_c *)ip);
  /* Nothing.*/
}

/**
 * @brief   Reference creation implementation.
 *
 * @param[in] ip        An existing reference to the object.
 * @return              A new reference to the object.
 */
CC_FORCE_INLINE
static inline referenced_object_c *__referenced_object_addref_impl(referenced_object_c *ip) {

  ip->references++;

  return ip;
}

/**
 * @brief   References count implementation.
 */
CC_FORCE_INLINE
static inline unsigned __referenced_object_getcount_impl(referenced_object_c *ip) {

  return ip->references;
}

/**
 * @brief   Reference release implementation.
 *
 * @param[in] ip        A reference to the object.
 * @return              Remaining references.
 */
CC_FORCE_INLINE
static inline unsigned __referenced_object_release_impl(referenced_object_c *ip) {

  osalDbgAssert(ip->references > 0U, "zero references");

  if (--ip->references == 0U) {
    __referenced_object_dispose_impl(ip);
  }

  return ip->references;
}
/** @} */

#endif /* OOP_REFERENCED_OBJECT_H */

/** @} */
