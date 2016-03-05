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
 * @file    hal_sensors.h
 * @brief   Generic sensors interface header.
 *
 * @addtogroup HAL_SENSORS
 * @{
 */

#ifndef _HAL_SENSORS_H_
#define _HAL_SENSORS_H_

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
 * @brief   @p BaseSensor virtual methods table.
 */
struct BaseSensorVMT {
  _base_sensor_methods
};

/**
 * @brief   Base stream class.
 * @details This class represents a generic blocking unbuffered sequential
 *          data stream.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseSensorVMT *vmt;
  _base_sensor_data
} BaseSensor;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   BaseSensor specific methods.
 */
#define _base_sensor_methods                                                \
  /* Get number of axes.*/                                                  \
  size_t (*get_axes_number)(void);                                          \
  /* Reads the sensor raw data.*/                                           \
  msg_t (*read_raw)(uint32_t axes[]);                                       \
  /* Reads the sensor returning normalized data.*/                          \
  msg_t (*read_cooked)(float axes[]);

/**
 * @brief   @p BaseSensor specific data.
 * @note    It is empty because @p BaseSensor is only an interface
 *          without implementation.
 */
#define _base_sensor_data

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HAL_SENSORS_H_ */

/** @} */
