/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#include "hal_sensors.h"

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
 * @brief   BaseAccelerometer specific methods.
 */
#define _base_accelerometer_methods_alone

/**
 * @brief   BaseAccelerometer specific methods with inherited ones.
 */
#define _base_accelerometer_methods                                         \
  _base_sensor_methods                                                      \
  _base_accelerometer_methods_alone

/**
 * @brief   @p BaseAccelerometer virtual methods table.
 */
struct BaseAccelerometerVMT {
  _base_accelerometer_methods
};

/**
 * @brief   @p BaseAccelerometer specific data.
 */
#define _base_accelerometer_data                                            \
  _base_sensor_data
	
/**
 * @brief   Base accelerometer class.
 * @details This class represents a generic a generic accelerometer.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseAccelerometerVMT *vmt_baseaccelerometer;
  _base_sensor_data
} BaseAccelerometer;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions (BaseAccelerometer)
 * @{
 */
/**
 * @brief   Accelerometer get axes number.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @return              The number of axes of the BaseSensor
 *
 * @api
 */
#define accelerometerGetAxesNumber(ip)                                      \
        (ip)->vmt_baseaccelerometer->get_axes_number(ip)

/**
 * @brief   Accelerometer read raw data.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @param[in] dp        pointer to a data array.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define accelerometerReadRaw(ip, dp)                                        \
        (ip)->vmt_baseaccelerometer->read_raw(ip, dp)

/**
 * @brief   Accelerometer read cooked data.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @param[in] dp        pointer to a data array.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define accelerometerReadCooked(ip, dp)                                     \
        (ip)->vmt_baseaccelerometer->read_cooked(ip, dp)
/** @} */

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
