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
   * @brief       Driver state.
   */
  flash_state_t             state;
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

#endif /* HAL_SNOR_BASE_H */

/** @} */
