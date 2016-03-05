/*
    ChibiOS - Copyright (C) 2016 Rocco Marco Guglielmi

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
 * @file    hal_compass.h
 * @brief   Generic compass interface header.
 *
 * @addtogroup HAL_COMPASS
 * @{
 */

#ifndef _HAL_COMPASS_H_
#define _HAL_COMPASS_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p BaseCompass virtual methods table.
 */

struct BaseCompassVMT {
  _base_compass_methods
};

/**
 * @brief   Base compass class.
 * @details This class represents a generic compass.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseCompassVMT *vmt;
  _base_compass_data

} BaseCompass;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   BaseCompass specific methods.
 */

#define _base_compass_methods                                               \
  _base_sensor_methods                                                      \
  /* Remove the calibration data.*/                                         \
  msg_t (*reset_calibration)(void);                                         \
  /* Invokes the calibration procedure.*/                                   \
  msg_t (*calibrate)(void);

/**
 * @brief   @p BaseCompass specific data.
 * @note    It is empty because @p BaseCompass is only an interface
 *          without implementation.
 */
#define _base_compass_data

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HAL_COMPASS_H_ */

/** @} */
