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
 * @file    hal_qspi_lld.c
 * @brief   PLATFORM QSPI subsystem low level driver source.
 *
 * @addtogroup QSPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_QSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief QSPID1 driver identifier.*/
#if (PLATFORM_QSPI_USE_QSPI1 == TRUE) || defined(__DOXYGEN__)
QSPIDriver QSPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level QSPI driver initialization.
 *
 * @notapi
 */
void qspi_lld_init(void) {

#if PLATFORM_QSPI_USE_QSPI1
  qspiObjectInit(&QSPID1);
#endif
}

/**
 * @brief   Configures and activates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_start(QSPIDriver *qspip) {

  /* If in stopped state then full initialization.*/
  if (qspip->state == QSPI_STOP) {
#if PLATFORM_QSPI_USE_QSPI1
    if (&QSPID1 == qspip) {
    }
#endif

    /* Common initializations.*/
  }

  /* QSPI setup and enable.*/
}

/**
 * @brief   Deactivates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_stop(QSPIDriver *qspip) {

  /* If in ready state then disables QSPI.*/
  if (qspip->state == QSPI_READY) {

    /* QSPI disable.*/

    /* Stopping involved clocks.*/
#if PLATFORM_QSPI_USE_QSPI1
    if (&QSPID1 == qspip) {
    }
#endif
  }
}

/**
 * @brief   Sends a command without data phase.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 *
 * @notapi
 */
void qspi_lld_command(QSPIDriver *qspip, const qspi_command_t *cmdp) {

  (void)qspip;
  (void)cmdp;
}

/**
 * @brief   Sends a command with data over the QSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void qspi_lld_send(QSPIDriver *qspip, const qspi_command_t *cmdp,
                   size_t n, const uint8_t *txbuf) {

  (void)qspip;
  (void)cmdp;
  (void)n;
  (void)txbuf;
}

/**
 * @brief   Sends a command then receives data over the QSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void qspi_lld_receive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                      size_t n, uint8_t *rxbuf) {

  (void)qspip;
  (void)cmdp;
  (void)n;
  (void)rxbuf;
}

#if (QSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Maps in memory space a QSPI flash device.
 * @pre     The memory flash device must be initialized appropriately
 *          before mapping it in memory space.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[out] addrp    pointer to the memory start address of the mapped
 *                      flash or @p NULL
 *
 * @notapi
 */
void qspi_lld_map_flash(QSPIDriver *qspip,
                        const qspi_command_t *cmdp,
                        uint8_t **addrp) {

  (void)qspip;
  (void)cmdp;
  (void)addrp;
}

/**
 * @brief   Maps in memory space a QSPI flash device.
 * @post    The memory flash device must be re-initialized for normal
 *          commands exchange.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_unmap_flash(QSPIDriver *qspip) {

  (void)qspip;
}
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */

#endif /* HAL_USE_QSPI */

/** @} */
