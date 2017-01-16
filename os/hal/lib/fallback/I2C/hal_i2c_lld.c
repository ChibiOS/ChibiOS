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
 * @file    common/I2C/hal_i2c_lld.c
 * @brief   SW I2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */

#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define CHECK_ERROR(msg)                                                    \
  if ((msg) < (msg_t)0) {                                                   \
    return MSG_TIMEOUT;                                                     \
  }

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2C1 driver identifier.*/
#if SW_I2C_USE_I2C1 || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/** @brief I2C2 driver identifier.*/
#if SW_I2C_USE_I2C2 || defined(__DOXYGEN__)
I2CDriver I2CD2;
#endif

/** @brief I2C3 driver identifier.*/
#if SW_I2C_USE_I2C3 || defined(__DOXYGEN__)
I2CDriver I2CD3;
#endif

/** @brief I2C4 driver identifier.*/
#if SW_I2C_USE_I2C4 || defined(__DOXYGEN__)
I2CDriver I2CD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static msg_t i2c_delay(I2CDriver *i2cp) {

  if (!osalOsIsTimeWithinX(osalOsGetSystemTimeX(), i2cp->start, i2cp->end)) {
    return MSG_TIMEOUT;
  }

#if SW_I2C_USE_OSAL_DELAY || defined(__DOXYGEN__)
  osalThreadSleep(i2cp->config->ticks);
#else
  i2cp->config->delay();
#endif
  return MSG_OK;
}

static msg_t i2c_write_start(I2CDriver *i2cp) {

}

static msg_t i2c_write_restart(I2CDriver *i2cp) {

}

static msg_t i2c_write_stop(I2CDriver *i2cp) {

}

static msg_t i2c_writebit(I2CDriver *i2cp, unsigned bit) {

  palWriteLine(i2cp->config->sda, bit);
  CHECK_ERROR(i2c_delay(i2cp));
  palSetLine(i2cp->config->scl);
  CHECK_ERROR(i2c_delay(i2cp));

  /* Clock stretching.*/
  while (palReadLine(i2cp->config->scl) == PAL_LOW) {
    CHECK_ERROR(i2c_delay(i2cp));
  }

  /* Arbitration check.*/
  if ((bit == PAL_HIGH) && (palReadLine(i2cp->config->sda) == PAL_LOW)) {
    i2cp->errors |= I2C_ARBITRATION_LOST;
    return MSG_RESET;
  }

  palClearLine(i2cp->config->scl);

  return MSG_OK;
}

static msg_t i2c_readbit(I2CDriver *i2cp) {
  msg_t bit;

  palSetLine(i2cp->config->sda);
  CHECK_ERROR(i2c_delay(i2cp));
  palSetLine(i2cp->config->scl);

  /* Clock stretching.*/
  while (palReadLine(i2cp->config->scl) == PAL_LOW) {
    CHECK_ERROR(i2c_delay(i2cp));
  }

  CHECK_ERROR(i2c_delay(i2cp));
  bit = palReadLine(i2cp->config->sda);
  palClearLine(i2cp->config->scl);

  return bit;
}

static msg_t i2c_writebyte(I2CDriver *i2cp, uint8_t byte) {
  uint8_t mask;

  for (mask = 0x80U; mask > 0U; mask >>= 1U) {
    CHECK_ERROR(i2c_writebit(i2cp, (byte & mask) != 0));
  }

  return i2c_readbit(i2cp);
}

static msg_t i2c_readbyte(I2CDriver *i2cp, unsigned nack) {
  msg_t byte;
  unsigned i;

  byte = 0U;
  for (i = 0; i < 8; i++) {
    msg_t msg = i2c_readbit(i2cp);
    CHECK_ERROR(msg);
    byte = (byte << 1U) | msg;
  }

  CHECK_ERROR(i2c_writebit(i2cp, PAL_LOW));

  return byte;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if SW_I2C_USE_I2C1
  i2cObjectInit(&I2CD1);
#endif /* SW_I2C_USE_I2C1 */
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {

  /* If in stopped state then enables the I2C and DMA clocks.*/
  if (i2cp->state == I2C_STOP) {

#if SW_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
    }
#endif /* SW_I2C_USE_I2C1 */
  }
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  /* If not in stopped state then disables the I2C clock.*/
  if (i2cp->state != I2C_STOP) {

    /* I2C disable.*/
    //i2c_lld_abort_operation(i2cp);

#if SW_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
    }
#endif
  }
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     systime_t timeout) {
  msg_t msg = MSG_OK;

  (void)i2cp;
  (void)addr;
  (void)rxbuf;
  (void)rxbytes;
  (void)timeout;

  return msg;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 * @details Number of receiving bytes must be 0 or more than 1 on STM32F1x.
 *          This is hardware restriction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      systime_t timeout) {
  msg_t msg = MSG_OK;

  (void)i2cp;
  (void)addr;
  (void)txbuf;
  (void)txbytes;
  (void)rxbuf;
  (void)rxbytes;
  (void)timeout;

  return msg;
}

#endif /* HAL_USE_I2C */

/** @} */
