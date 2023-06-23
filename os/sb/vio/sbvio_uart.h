/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/vhal/sbvio_uart.h
 * @brief   ARM SandBox host Virtual UART macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_UART
 * @{
 */

#ifndef SBVIO_UART_H
#define SBVIO_UART_H

#if (VIO_CFG_ENABLE_UART == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VHAL UART driver.
 */
#define SB_SVC97_HANDLER        sb_fastc_vio_uart

/**
 * @brief   API handler for VHAL UART driver.
 */
#define SB_SVC225_HANDLER       sb_sysc_vio_uart

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO UART unit representation.
 */
typedef struct vio_uart_unit {
  hal_sio_driver_c  *siop;
  sb_class_t        *vrqsb;
  sb_vrqnum_t       vrqn;
} vio_uart_unit_t;

/**
 * @brief   Type of a VIO UART units structure.
 */
typedef struct vio_uart_units {
  uint32_t          n;
  vio_uart_unit_t   units[];
} vio_uart_units_t;

/**
 * @brief   Type of a VIO UART configuration representation.
 */
typedef struct vio_uart_config {
  SIOConfig         *siocfgp;
} vio_uart_config_t;

/**
 * @brief   Type of a VIO UARTs configuration structure.
 */
typedef struct vio_uart_configs {
  uint32_t          n;
  vio_uart_config_t cfgs[];
} vio_uart_configs_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_uart(struct port_extctx *ectxp);
void sb_fastc_vio_uart(struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VIO_CFG_ENABLE_UART == TRUE */

#endif /* SBVIO_UART_H */

/** @} */
