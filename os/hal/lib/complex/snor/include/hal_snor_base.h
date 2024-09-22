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

/**
 * @file        hal_snor_base.h
 * @brief       Generated SNOR Base Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SNOR_BASE
 * @brief       SNOR abstract driver.
 * @details     Base class for SNOR flash devices.
 * @{
 */

#ifndef HAL_SNOR_BASE_H
#define HAL_SNOR_BASE_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       SNOR driver configuration.
 */
struct snor_nocache_t {
  /**
   * @brief       Non-cacheable data buffer.
   */
  uint8_t                   databuf[SNOR_BUFFER_SIZE];
  /**
   * @brief       Non-cacheable WSPI command buffer.
   */
  wspi_command_t            cmdbuf;
};

/**
 * @brief       SNOR driver configuration.
 */
struct snor_config_t {
  /**
   * @brief       WSPI driver to be used for physical communication.
   */
  WSPIDriver                *wspi;
  /**
   * @brief       WSPI driver configuration.
   */
  const WSPIConfig          *wspiconfig;
};

/**
 * @class       hal_snor_base_c
 * @extends     base_object_c.
 * @implements  flash_interface_i
 *
 * @brief       Base class of all SNOR drivers.
 *
 * @name        Class @p hal_snor_base_c structures
 * @{
 */

/**
 * @brief       Type of a SNOR base driver class.
 */
typedef struct hal_snor_base hal_snor_base_c;

/**
 * @brief       Class @p hal_snor_base_c virtual methods table.
 */
struct hal_snor_base_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_snor_base_c.*/
  const flash_descriptor_t * (*get_descriptor)(void *ip);
  flash_error_t (*read)(void *ip, flash_offset_t offset, size_t n, uint8_t *rp);
  flash_error_t (*program)(void *ip, flash_offset_t offset, size_t n, const uint8_t *pp);
  flash_error_t (*start_erase_all)(void *ip);
  flash_error_t (*start_erase_sector)(void *ip, const flash_sector_t *sector);
  flash_error_t (*query_erase)(void *ip, unsigned *msec);
};

/**
 * @brief       Structure representing a SNOR base driver class.
 */
struct hal_snor_base {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_snor_base_vmt *vmt;
  /**
   * @brief       Implemented interface @p flash_interface_i.
   */
  flash_interface_i         fls;
  /**
   * @brief       Driver state.
   */
  flash_state_t             state;
  /**
   * @brief       Flash access mutex.
   */
  mutex_t                   mutex;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_snor_base_c.*/
  void *__snorbase_objinit_impl(void *ip, const void *vmt,
                                snor_nocache_t *nocache);
  void __snorbase_dispose_impl(void *ip);
  /* Regular functions.*/
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_snor_base_c
 * @{
 */
/**
 * @memberof    hal_snor_base_c
 * @public
 *
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline const flash_descriptor_t *snor_get_descriptor(void *ip) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->get_descriptor(ip);
}

/**
 * @memberof    hal_snor_base_c
 * @public
 *
 * @brief       Read operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_ERROR_READ     If the read operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t snor_read(void *ip, flash_offset_t offset,
                                      size_t n, uint8_t *rp) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->read(ip, offset, n, rp);
}

/**
 * @memberof    hal_snor_base_c
 * @public
 *
 * @brief       Program operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_ERROR_PROGRAM  If the program operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t snor_program(void *ip, flash_offset_t offset,
                                         size_t n, const uint8_t *pp) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->program(ip, offset, n, pp);
}

/**
 * @memberof    hal_snor_base_c
 * @public
 *
 * @brief       Starts a whole-device erase operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t snor_start_erase_all(void *ip) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->start_erase_all(ip);
}

/**
 * @memberof    hal_snor_base_c
 * @public
 *
 * @brief       Starts an sector erase operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t snor_start_erase_sector(void *ip,
                                                    const flash_sector_t *sector) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->start_erase_sector(ip, sector);
}

/**
 * @memberof    hal_snor_base_c
 * @public
 *
 * @brief       Queries the driver for erase operation progress.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance.
 * @param[out]    msec          Recommended time, in milliseconds, that should
 *                              be spent before calling this function again,
 *                              can be @p NULL
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t snor_query_erase(void *ip, unsigned *msec) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  return self->vmt->query_erase(ip, msec);
}
/** @} */

#endif /* HAL_SNOR_BASE_H */

/** @} */
