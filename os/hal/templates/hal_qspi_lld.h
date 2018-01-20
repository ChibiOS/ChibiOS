/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_qspi_lld.h
 * @brief   PLATFORM QSPI subsystem low level driver header.
 *
 * @addtogroup QSPI
 * @{
 */

#ifndef HAL_QSPI_LLD_H
#define HAL_QSPI_LLD_H

#if (HAL_USE_QSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    QSPI capabilities
 * @{
 */
#define QSPI_SUPPORTS_MEMMAP                TRUE
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   QSPID1 driver enable switch.
 * @details If set to @p TRUE the support for QSPID1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(PLATFORM_QSPI_USE_QSPI1) || defined(__DOXYGEN__)
#define PLATFORM_QSPI_USE_QSPI1             FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an QSPI driver.
 */
typedef struct QSPIDriver QSPIDriver;

/**
 * @brief   Type of a QSPI notification callback.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object triggering the
 *                      callback
 */
typedef void (*qspicallback_t)(QSPIDriver *qspip);

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Operation complete callback or @p NULL.
   */
  qspicallback_t             end_cb;
  /* End of the mandatory fields.*/
} QSPIConfig;

/**
 * @brief   Structure representing an QSPI driver.
 */
struct QSPIDriver {
  /**
   * @brief   Driver state.
   */
  qspistate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const QSPIConfig           *config;
#if (QSPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif /* QSPI_USE_WAIT */
#if (QSPI_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* QSPI_USE_MUTUAL_EXCLUSION */
#if defined(QSPI_DRIVER_EXT_FIELDS)
  QSPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (PLATFORM_QSPI_USE_QSPI1 == TRUE) && !defined(__DOXYGEN__)
extern QSPIDriver QSPID1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void qspi_lld_init(void);
  void qspi_lld_start(QSPIDriver *qspip);
  void qspi_lld_stop(QSPIDriver *qspip);
  void qspi_lld_command(QSPIDriver *qspip, const qspi_command_t *cmdp);
  void qspi_lld_send(QSPIDriver *qspip, const qspi_command_t *cmdp,
                     size_t n, const uint8_t *txbuf);
  void qspi_lld_receive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                        size_t n, uint8_t *rxbuf);
#if QSPI_SUPPORTS_MEMMAP == TRUE
  void qspi_lld_map_flash(QSPIDriver *qspip,
                          const qspi_command_t *cmdp,
                          uint8_t **addrp);
  void qspi_lld_unmap_flash(QSPIDriver *qspip);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_QSPI */

#endif /* HAL_QSPI_LLD_H */

/** @} */
