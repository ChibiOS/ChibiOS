/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "hal_snor_base_c" methods.                                   */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_snor_base_c
 * @{
 */
/**
 * @memberof    hal_snor_base_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_snor_base_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     nocache       Pointer to a non-cacheable buffer.
 * @return                      A new reference to the object.
 */
void *__snorbase_objinit_impl(void *ip, const void *vmt,
                              snor_nocache_t *nocache) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __bo_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->nocache = nocache;

  return self;
}

/**
 * @memberof    hal_snor_base_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance to be
 *                              disposed.
 */
void __snorbase_dispose_impl(void *ip) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  /* Finalization code.*/
  ;

  /* Finalization of the ancestors-defined parts.*/
  __bo_dispose_impl(self);
}
/** @} */

