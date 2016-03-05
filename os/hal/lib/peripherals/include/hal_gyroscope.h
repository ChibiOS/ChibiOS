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
 * @file    hal_gyroscope.h
 * @brief   Generic gyroscope interface header.
 *
 * @addtogroup HAL_GYROSCOPE
 * @{
 */

#ifndef _HAL_GYROSCOPE_H_
#define _HAL_GYROSCOPE_H_

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
 * @brief   @p BaseGyroscope virtual methods table.
 */
struct BaseGyroscopeVMT {
  _base_gyroscope_methods
};

/**
 * @brief   Base gyroscope class.
 * @details This class represents a generic gyroscope MEMS.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseGyroscopeVMT *vmt;
  _base_gyroscope_data
} BaseGyroscope;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   BaseGyroscope specific methods.
 */
#define _base_gyroscope_methods                                             \
  _base_sensor_methods                                                      \
  /* Remove the calibration data.*/                                         \
  msg_t (*reset_calibration)(void);                                         \
  /* Invokes the calibration procedure.*/                                   \
  msg_t (*calibrate)(void);

/**
 * @brief   @p BaseGyroscope specific data.
 * @note    It is empty because @p BaseGyroscope is only an interface
 *          without implementation.
 */
#define _base_gyroscope_data

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HAL_GYROSCOPE_H_ */

/** @} */
