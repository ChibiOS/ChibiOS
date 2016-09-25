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
 * @addtogroup lps25h
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
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] reg        first sub-register address
 * @return               the read value.
 */
uint8_t lps25hI2CReadRegister(I2CDriver *i2cp, lps25h_sad_t sad, uint8_t reg,
                               msg_t* msgp) {
  msg_t msg;
  uint8_t rxbuf;
  msg = i2cMasterTransmitTimeout(i2cp, sad, &reg, 1, &rxbuf, 1,
                                 TIME_INFINITE);
  if(msgp != NULL){
    *msgp = msg;
  }
  return rxbuf;
}

/**
 * @brief   Writes a value into a register using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] sub        sub-register address
 * @param[in] value      the value to be written
 * @return               the operation status.
 */
msg_t lps25hI2CWriteRegister(I2CDriver *i2cp, lps25h_sad_t sad, uint8_t reg,
                              uint8_t value) {
  uint8_t txbuf[2];
  txbuf[0] = reg;
  txbuf[1] = value;
  return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, NULL, 0, TIME_INFINITE);
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
  int32_t tmp;
  msg_t msg = MSG_OK;
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
	tmp = lps25hI2CReadRegister(((LPS25HDriver *)ip)->config->i2cp,
								((LPS25HDriver *)ip)->config->slaveaddress,
								  LPS25H_AD_PRESS_OUT_XL, NULL);
	if (msg != MSG_OK)
		return msg;
	tmp +=  lps25hI2CReadRegister(((LPS25HDriver *)ip)->config->i2cp,
	                                ((LPS25HDriver *)ip)->config->slaveaddress,
	                                  LPS25H_AD_PRESS_OUT_L, NULL) << 8;
    if (msg != MSG_OK)
        return msg;
    tmp +=  lps25hI2CReadRegister(((LPS25HDriver *)ip)->config->i2cp,
                                    ((LPS25HDriver *)ip)->config->slaveaddress,
                                      LPS25H_AD_PRESS_OUT_H, NULL) << 16;
	if (msg != MSG_OK)
		return msg;
	*axis = (int32_t)tmp;

	
#if LPS25H_SHARED_I2C
  i2cReleaseBus(((LPS25HDriver *)ip)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */
#endif /* LPS25H_USE_I2C */
  return MSG_OK;
}
	
static msg_t read_cooked(void *ip, float* axis) {
  int32_t raw;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axis != NULL));

  osalDbgAssert((((LPS25HDriver *)ip)->state == LPS25H_READY),
              "read_cooked(), invalid state");

  msg = read_raw(ip, &raw);

  *axis = raw / ((LPS25HDriver *)ip)->sensitivity;
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

  ((LPS25HDriver *)ip)->bias = 0;
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
  devp->state  = LPS25H_STOP;
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
  uint8_t cr;
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LPS25H_STOP) || (devp->state == LPS25H_READY),
              "lps25hStart(), invalid state");			  

  devp->config = config;

#if LPS25H_USE_I2C
#if	LPS25H_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);

  /* Control register 1 configuration block.*/
  {
    cr = devp->config->outputdatarate | LPS25H_CTRL_REG1_PD;
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
    cr |= devp->config->blockdataupdate;

#endif
    lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                           LPS25H_AD_CTRL_REG1, cr);
  }

  /* Control register 1 configuration block.*/
  {
    cr = 0x05;
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
    cr = devp->config->respressure | devp->config->restemperature;

#endif
    lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                           LPS25H_AD_RES_CONF, cr);
  }

#if	LPS25H_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */  
#endif /* LPS25H_USE_I2C */
  /* Storing sensitivity information according to full scale value */
  devp->sensitivity = LPS25H_SENS;
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

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LPS25H_STOP) || (devp->state == LPS25H_READY),
                "lps25hStop(), invalid state");

#if (LPS25H_USE_I2C)
  if (devp->state == LPS25H_STOP) {
#if	LPS25H_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LPS25H_SHARED_I2C */
    lps25hI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                           LPS25H_AD_CTRL_REG1, 0);
    i2cStop((devp)->config->i2cp);
#if	LPS25H_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LPS25H_SHARED_I2C */    
  }			  
#endif /* LPS25H_USE_I2C */
  devp->state = LPS25H_STOP;
}
/** @} */
