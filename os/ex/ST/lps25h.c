/*
    ChibiOS - Copyright (C) 2016 Rocco Marco Guglielmi

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 * @file    lps25h.c
 * @brief   LPS25H MEMS interface module code.
 *
 * @addtogroup LPS25H
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "lps25h.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (LPS25H_USE_I2C) || defined(__DOXYGEN__)
/**
 * @brief   Reads registers value using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in]  i2cp      pointer to the I2C interface
 * @param[in]  sad       slave address without R bit
 * @param[in]  reg       first sub-register address
 * @param[out] rxbuf     pointer to an output buffer
 * @param[in]  n         number of consecutive register to read
 * @return               the operation status.
 * @notapi
 */
msg_t lps25hI2CReadRegister(I2CDriver *i2cp, lps25h_sad_t sad, uint8_t reg,
                              uint8_t* rxbuf, size_t n) {
  uint8_t txbuf = reg;  
  if(n > 1)
    txbuf |= LPS25H_SUB_MS;
  
  return i2cMasterTransmitTimeout(i2cp, sad, &txbuf, 1, rxbuf, n,
                                  TIME_INFINITE);
}

/**
 * @brief   Writes a value into a register using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] txbuf      buffer containing sub-address value in first position
 *                       and values to write
 * @param[in] n          size of txbuf less one (not considering the first
 *                       element)
 * @return               the operation status.
 * @notapi
 * @return               the operation status.
 */
msg_t lps25hI2CWriteRegister(I2CDriver *i2cp, lps25h_sad_t sad, uint8_t* txbuf,                             size_t n) {
  if (n > 1)
    (*txbuf) |= LPS25H_SUB_MS;
  return i2cMasterTransmitTimeout(i2cp, sad, txbuf, n + 1, NULL, 0,
                                  TIME_INFINITE);
}
#endif /* LPS25H_USE_I2C */

/*
 * Interface implementation.
 */
static size_t get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LPS25H_NUMBER_OF_AXES;
}

static msg_t read_raw(void *ip, int32_t* axis) {
  uint8_t buff[3];
  msg_t msg = MSG_OK;
  
  *axis = 0;
    
  osalDbgCheck((ip != NULL) && (axis != NULL));
  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY),
              "read_raw(), invalid state");

#if LPS25H_USE_I2C
  osalDbgAssert((((LPS25HDriver *)ip)->config->i2cp->state == I2C_READY),
                "read_raw(), channel not ready");
#if LPS25H_SHARED_I2C
  i2cAcquireBus(((LPS25HDriver *)ip)->config->i2cp);
  i2cStart(((LPS25HDriver *)ip)->config->i2cp,
           ((LPS25HDriver *)ip)->config->i2ccfg);
#endif /* LPS25H_SHARED_I2C */

  msg = lps25hI2CReadRegister(((LPS25HDriver *)ip)->config->i2cp,
                              ((LPS25HDriver *)ip)->config->slaveaddress,
                               LPS25H_AD_PRESS_OUT_XL, buff, 3);

#if LPS25H_SHARED_I2C
  i2cReleaseBus(((LPS25HDriver *)ip)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */
#endif /* LPS25H_USE_I2C */

  if(msg == MSG_OK) {
    *axis = buff[0] + (buff[1] << 8) + (buff[2] << 16);
  }
  return msg;
}
  
static msg_t read_cooked(void *ip, float* axis) {
  int32_t raw;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axis != NULL));

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY),
              "read_cooked(), invalid state");

  msg = read_raw(ip, &raw);

  *axis = raw * ((LPS25HDriver *)ip)->sensitivity;
  *axis -= ((LPS25HDriver *)ip)->bias;
  return msg;
}


static msg_t set_bias(void *ip, float *bp) {
  osalDbgCheck((ip != NULL) && (bp != NULL));

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY) ||
                (((LPS25HDriver *)ip)->state == LPS25H_STOP),
                "set_bias(), invalid state");
  
  ((LPS25HDriver *)ip)->bias = *bp;
  return MSG_OK;
}

static msg_t reset_bias(void *ip) {
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY) ||
                (((LPS25HDriver *)ip)->state == LPS25H_STOP),
                "reset_bias(), invalid state");

  ((LPS25HDriver *)ip)->bias = 0.0f;
  return MSG_OK;
}


static msg_t set_sensivity(void *ip, float *sp) {
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY),
                "set_sensivity(), invalid state");
  
  ((LPS25HDriver *)ip)->sensitivity = *sp;
  return MSG_OK;
}

static msg_t reset_sensivity(void *ip) {

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY),
                "reset_sensivity(), invalid state");

  ((LPS25HDriver *)ip)->sensitivity = LPS25H_SENS;
  return MSG_OK;
}

static const struct BaseSensorVMT vmt_basesensor = {
  get_axes_number, read_raw, read_cooked
};

static const struct BaseBarometerVMT vmt_basebarometer = {
  get_axes_number, read_raw, read_cooked,
  set_bias, reset_bias, set_sensivity, reset_sensivity
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p LPS25HDriver object
 *
 * @init
 */
void lps25hObjectInit(LPS25HDriver *devp) {

  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_basebarometer = &vmt_basebarometer;
  devp->config = NULL;
  devp->bias = 0;
  devp->state = LPS25H_STOP;
}

/**
 * @brief   Configures and activates LPS25H Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LPS25HDriver object
 * @param[in] config    pointer to the @p LPS25HConfig object
 *
 * @api
 */
void lps25hStart(LPS25HDriver *devp, const LPS25HConfig *config) {
  uint8_t cr[2];
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LPS25H_STOP) || (devp->state == LPS25H_READY),
              "lps25hStart(), invalid state");

  devp->config = config;
#if LPS25H_USE_I2C
  /* Control register 1 configuration block.*/
  {
    cr[0] = LPS25H_AD_CTRL_REG1;
    cr[1] = devp->config->outputdatarate | LPS25H_CTRL_REG1_PD;
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] |= devp->config->blockdataupdate;
#endif
  }
  
#if  LPS25H_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
           
  lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress, cr, 1);
                           
#if  LPS25H_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */

  /* Resolution configuration block.*/
  {
    cr[0] = LPS25H_AD_RES_CONF;
    cr[1] = 0x05;
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] = devp->config->respressure | devp->config->restemperature;
#endif
    
  }
#if  LPS25H_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
#endif /* LPS25H_SHARED_I2C */
           
  lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                         cr, 1);
                         
#if  LPS25H_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */  
#endif /* LPS25H_USE_I2C */

  if(devp->config->sensitivity == NULL) {
    devp->sensitivity = LPS25H_SENS;
  }
  else{
    /* Taking Sensitivity from user configurations */
    devp->sensitivity = *devp->config->sensitivity;
  }

  if(devp->config->bias == NULL) {
    devp->bias = 0.0f;
  }
  else{
    /* Taking Bias from user configurations */
    devp->bias = *devp->config->bias;
  }

  /* This is the Barometer transient recovery time */
  osalThreadSleepMilliseconds(5);

  devp->state = LPS25H_READY;
} 

/**
 * @brief   Deactivates the LPS25H Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LPS25HDriver object
 *
 * @api
 */
void lps25hStop(LPS25HDriver *devp) {
  uint8_t cr[2];

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LPS25H_STOP) || (devp->state == LPS25H_READY),
                "lps25hStop(), invalid state");

  if (devp->state == LPS25H_READY) {
#if (LPS25H_USE_I2C)
#if  LPS25H_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LPS25H_SHARED_I2C */

    cr[0] = LPS25H_AD_CTRL_REG1;
    cr[1] = 0;
    lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                           cr, 1);
                           
    i2cStop((devp)->config->i2cp);
#if  LPS25H_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */
#endif /* LPS25H_USE_I2C */
  }
  devp->state = LPS25H_STOP;
}
/** @} */
