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
 * @brief   BaseCompass specific methods.
 */
#define _base_compass_methods_alone

/**
 * @brief   BaseCompass specific methods with inherited ones.
 */
#define _base_compass_methods                                               \
  _base_sensor_methods                                                      \
  _base_compass_methods_alone

/**
 * @brief   @p BaseCompass virtual methods table.
 */
struct BaseCompassVMT {
  _base_compass_methods
};

/**
 * @brief   @p BaseCompass specific data.
 */
#define _base_compass_data                                                  \
  _base_sensor_data
	
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
 * @name    Macro Functions (BaseCompass)
 * @{
 */
/**
 * @brief   Compass get axes number.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @return              The number of axes of the BaseSensor
 *
 * @api
 */
#define compassGetAxesNumber(ip) sensorGetAxesNumber(ip)

/**
 * @brief   Compass read raw data.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @param[in] dp        pointer to a data array.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define compassReadRaw(ip, dp) sensorReadRaw(ip, dp)

/**
 * @brief   Compass read cooked data.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @param[in] dp        pointer to a data array.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define compassReadCooked(ip, dp) sensorReadCooked(ip, dp)

/**
 * @brief   Delete calibration data.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define compassResetCalibration(ip) ((ip)->vmt->reset_calibration(ip))

/**
 * @brief   Compass calibration procedure.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * 
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 *
 * @api
 */
#define compassCalibrate(ip) ((ip)->vmt->calibrate(ip))
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

#endif /* _HAL_COMPASS_H_ */

/** @} */
