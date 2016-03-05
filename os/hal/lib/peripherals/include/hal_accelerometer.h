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
 * @file    hal_accelerometer.h
 * @brief   Generic accelerometer interface header.
 *
 * @addtogroup HAL_ACCELEROMETER
 * @{
 */

#ifndef _HAL_ACCELEROMETER_H_
#define _HAL_ACCELEROMETER_H_

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
 * @brief   @p BaseAccelerometer virtual methods table.
 */
struct BaseAccelerometerVMT {
  _base_accelerometer_methods
};

/**
 * @brief   Base accelerometer class.
 * @details This class represents a generic a generic accelerometer.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseAccelerometerVMT *vmt;
  _base_sensor_data
} BaseAccelerometer;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   BaseAccelerometer specific methods.
 */
#define _base_accelerometer_methods                                         \
  _base_sensor_methods                                                      \
  /* Remove the calibration data.*/                                            \
  msg_t (*reset_calibration)(void);                                         \
  /* Invokes the calibration procedure.*/                                   \
  msg_t (*calibrate)(void);

/**
 * @brief   @p BaseAccelerometer specific data.
 * @note    It is empty because @p BaseAccelerometer is only an interface
 *          without implementation.
 */
#define _base_accelerometer_data

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HAL_ACCELEROMETER_H_ */

/** @} */
