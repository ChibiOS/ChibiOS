/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sandbox/hal_i2c_lld.c
 * @brief   SandBox I2C subsystem low level driver source.
 *
 * @addtogroup HAL_I2C
 * @{
 */

#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2C1 driver identifier.*/
#if VIO_I2C_USE_VI2C1 || defined(__DOXYGEN__)
hal_i2c_driver_c I2CD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_init(uint32_t nvi2c, size_t n, void *p) {

  __syscall3r(230, VIO_CALL(SB_VI2C_INIT, nvi2c), n, p);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_deinit(uint32_t nvi2c) {

  __syscall1r(230, VIO_CALL(SB_VI2C_DEINIT, nvi2c));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_selcfg(uint32_t nvi2c, uint32_t ncfg) {

  __syscall2r(102, VIO_CALL(SB_VI2C_SELCFG, nvi2c), ncfg);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_start_tx(uint32_t nvi2c,
                                           const sb_vi2c_transfer_t *tp) {

  __syscall2r(230, VIO_CALL(SB_VI2C_TX, nvi2c), tp);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_start_rx(uint32_t nvi2c,
                                           const sb_vi2c_transfer_t *tp) {

  __syscall2r(230, VIO_CALL(SB_VI2C_RX, nvi2c), tp);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __i2c_vi2c_stop(uint32_t nvi2c) {

  __syscall1r(230, VIO_CALL(SB_VI2C_STOP, nvi2c));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline i2cflags_t __i2c_vi2c_gcerr(uint32_t nvi2c) {

  __syscall1r(102, VIO_CALL(SB_VI2C_GCERR, nvi2c));
  return (i2cflags_t)r0;
}

/**
 * @brief   Shared I2C service routine.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] nvrq      virtual IRQ number
 */
static void i2c_lld_serve_interrupt(hal_i2c_driver_c *i2cp, uint32_t nvrq) {
  uint32_t sts;

  sts = __sb_vrq_gcsts(nvrq);

  if ((sts & (1U << HAL_DRV_STATE_ERROR)) != 0U) {
    i2cp->errors |= __i2c_vi2c_gcerr(i2cp->nvi2c);
    __i2c_error_isr(i2cp);
    return;
  }

  if ((sts & (1U << HAL_DRV_STATE_COMPLETE)) != 0U) {
    __i2c_complete_isr(i2cp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_I2C_USE_VI2C1 || defined(__DOXYGEN__)
#if !defined(VIO_VI2C1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VI2C1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  i2c_lld_serve_interrupt(&I2CD1, VIO_VI2C1_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

  /* Driver instances initialization.*/
#if VIO_I2C_USE_VI2C1 == TRUE
  i2cObjectInit(&I2CD1);
  I2CD1.nvi2c = 0U;
  __sb_vrq_seten(1U << VIO_VI2C1_IRQ);
#endif
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_start(hal_i2c_driver_c *i2cp) {
  msg_t msg = HAL_RET_SUCCESS;

  /* Enables the peripheral.*/
  if (false) {
  }
#if VIO_I2C_USE_VI2C1 == TRUE
  else if (&I2CD1 == i2cp) {
    msg = (msg_t)__i2c_vi2c_init(i2cp->nvi2c,
                                 sizeof (hal_i2c_config_t),
                                 &i2cp->cfgbuf);
  }
#endif
  else {
    osalDbgAssert(false, "invalid I2C instance");
  }

  if (msg == HAL_RET_SUCCESS) {
    i2cp->config = &i2cp->cfgbuf;
  }

  return msg;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
void i2c_lld_stop(hal_i2c_driver_c *i2cp) {
  msg_t msg = HAL_RET_SUCCESS;

  /* Disables the peripheral.*/
  if (false) {
  }
#if VIO_I2C_USE_VI2C1 == TRUE
  else if (&I2CD1 == i2cp) {
    msg = (msg_t)__i2c_vi2c_deinit(i2cp->nvi2c);
  }
#endif
  else {
    osalDbgAssert(false, "invalid I2C instance");
  }

  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
}

/**
 * @brief   Applies an explicit I2C configuration.
 * @note    The VIO port does not accept arbitrary sandbox-provided
 *          configurations. Only host-approved predefined configurations
 *          selected using @p drvSelectCfgX() are supported.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] config    pointer to the @p hal_i2c_config_t structure
 * @return              Always @p NULL in the VIO port.
 *
 * @notapi
 */
const hal_i2c_config_t *i2c_lld_setcfg(hal_i2c_driver_c *i2cp,
                                       const hal_i2c_config_t *config) {

  (void)i2cp;
  (void)config;

  return NULL;
}

/**
 * @brief       Selects one of the host-defined I2C configurations.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] cfgnum    driver configuration number
 * @return              The configuration pointer.
 *
 * @notapi
 */
const hal_i2c_config_t *i2c_lld_selcfg(hal_i2c_driver_c *i2cp,
                                       unsigned cfgnum) {
  msg_t msg;

  msg = (msg_t)__i2c_vi2c_selcfg(i2cp->nvi2c, cfgnum);
  if (msg != HAL_RET_SUCCESS) {
    return NULL;
  }

  return &i2cp->cfgbuf;
}

/**
 * @brief   Changes the I2C peripheral callback.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] cb        callback function to be associated
 *
 * @notapi
 */
void i2c_lld_set_callback(hal_i2c_driver_c *i2cp, drv_cb_t cb) {

  i2cp->cb = cb;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_start_master_transmit(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                    const uint8_t *txbuf, size_t txbytes,
                                    uint8_t *rxbuf, size_t rxbytes) {
  sb_vi2c_transfer_t transfer;

  transfer.addr    = (uint32_t)addr;
  transfer.txbuf   = txbuf;
  transfer.txbytes = (uint32_t)txbytes;
  transfer.rxbuf   = rxbuf;
  transfer.rxbytes = (uint32_t)rxbytes;

  return (msg_t)__i2c_vi2c_start_tx(i2cp->nvi2c, &transfer);
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_start_master_receive(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                   uint8_t *rxbuf, size_t rxbytes) {
  sb_vi2c_transfer_t transfer;

  transfer.addr    = (uint32_t)addr;
  transfer.txbuf   = NULL;
  transfer.txbytes = 0U;
  transfer.rxbuf   = rxbuf;
  transfer.rxbytes = (uint32_t)rxbytes;

  return (msg_t)__i2c_vi2c_start_rx(i2cp->nvi2c, &transfer);
}

/**
 * @brief   Stops the ongoing I2C transfer.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_stop_transfer(hal_i2c_driver_c *i2cp) {

  return (msg_t)__i2c_vi2c_stop(i2cp->nvi2c);
}

#endif /* HAL_USE_I2C */

/** @} */
