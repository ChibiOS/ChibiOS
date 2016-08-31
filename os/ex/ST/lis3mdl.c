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
 * @file    lis3mdl.c
 * @brief   LIS3MDL MEMS interface module code.
 *
 * @addtogroup lis3mdl
 * @{
 */

#include "hal.h"
#include "lis3mdl.h"

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

#if (LIS3MDL_USE_I2C) || defined(__DOXYGEN__)
/**
 * @brief   Reads registers value using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] reg        first sub-register address
 * @return               the read value.
 */
uint8_t lis3mdlI2CReadRegister(I2CDriver *i2cp, lis3mdl_sad_t sad, uint8_t reg,
                               msg_t* msgp) {
  msg_t msg;
#if defined(STM32F103_MCUCONF)
  uint8_t rxbuf[2];
  msg = i2cMasterTransmitTimeout(i2cp, sad, &txbuf, 1, rxbuf, 2,
                                   TIME_INFINITE);
  if(msgp != NULL){
    *msgp = msg;
  }
  return rxbuf[0];
#else
  uint8_t txbuf, rxbuf;
  txbuf = reg;
  msg = i2cMasterTransmitTimeout(i2cp, sad, &txbuf, 1, &rxbuf, 1,
                                        TIME_INFINITE);
  if(msgp != NULL){
    *msgp = msg;
  }
  return rxbuf;
#endif
}

/**
 * @brief   Writes a value into a register using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] reg        sub-register address
 * @param[in] value      the value to be written
 * @return               the operation status.
 */
msg_t lis3mdlI2CWriteRegister(I2CDriver *i2cp, lis3mdl_sad_t sad, uint8_t reg,
                              uint8_t value) {
  uint8_t txbuf[2];
  txbuf[0] = reg;
  txbuf[1] = value;
  return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, NULL, 0, TIME_INFINITE);
}
#endif /* LIS3MDL_USE_I2C */

/*
 * Interface implementation.
 */
static size_t get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LIS3MDL_NUMBER_OF_AXES;
}

static msg_t read_raw(void *ip, int32_t axes[LIS3MDL_NUMBER_OF_AXES]) {
  int16_t tmp;
  msg_t msg = MSG_OK;
  osalDbgCheck((ip != NULL) && (axes != NULL));
  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
                "read_raw(), invalid state");

  osalDbgAssert((((LIS3MDLDriver *)ip)->config->i2cp->state == I2C_READY),
                "read_raw(), channel not ready");
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus(((LIS3MDLDriver *)ip)->config->i2cp);
  i2cStart(((LIS3MDLDriver *)ip)->config->i2cp,
           ((LIS3MDLDriver *)ip)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */
  tmp = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                               ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                 LIS3MDL_AD_OUT_X_L, &msg);
  if (msg != MSG_OK)
    return msg;
  tmp += lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                  LIS3MDL_AD_OUT_X_H, &msg) << 8;
  if (msg != MSG_OK)
    return msg;
  axes[0] = (int32_t)tmp - ((LIS3MDLDriver *)ip)->bias[0];

  tmp = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                               ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                 LIS3MDL_AD_OUT_Y_L, &msg);
  if (msg != MSG_OK)
    return msg;
  tmp += lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                  LIS3MDL_AD_OUT_Y_H, &msg) << 8;
  if (msg != MSG_OK)
    return msg;
  axes[1] = (int32_t)tmp - ((LIS3MDLDriver *)ip)->bias[1];

  tmp = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                               ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                 LIS3MDL_AD_OUT_Z_L, &msg);
  if (msg != MSG_OK)
    return msg;
  tmp += lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                  LIS3MDL_AD_OUT_Z_H, &msg) << 8;
  if (msg != MSG_OK)
    return msg;
  axes[2] = (int32_t)tmp - ((LIS3MDLDriver *)ip)->bias[2];
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
  return MSG_OK;
}

static msg_t read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LIS3MDL_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
               "read_cooked(), invalid state");

  msg = read_raw(ip, raw);
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] / ((LIS3MDLDriver *)ip)->sensitivity[i];
    axes[i] -= ((LIS3MDLDriver *)ip)->bias[i];
  }
  return msg;
}

static msg_t set_bias(void *ip, int32_t *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY) ||
                (((LIS3MDLDriver *)ip)->state == LIS3MDL_STOP),
                "set_bias(), invalid state");
  
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
    ((LIS3MDLDriver *)ip)->bias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY) ||
                (((LIS3MDLDriver *)ip)->state == LIS3MDL_STOP),
              "reset_bias(), invalid state");

  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
    ((LIS3MDLDriver *)ip)->bias[i] = 0;
  return MSG_OK;
}

static msg_t set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
                "set_sensivity(), invalid state");
  
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
    ((LIS3MDLDriver *)ip)->sensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
                "reset_sensivity(), invalid state");

  if(((LIS3MDLDriver *)ip)->config->fullscale == LIS3MDL_FS_4GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      ((LIS3MDLDriver *)ip)->sensitivity[i] = LIS3MDL_SENS_4GA;
  else if(((LIS3MDLDriver *)ip)->config->fullscale == LIS3MDL_FS_8GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      ((LIS3MDLDriver *)ip)->sensitivity[i] = LIS3MDL_SENS_8GA;
  else if(((LIS3MDLDriver *)ip)->config->fullscale == LIS3MDL_FS_12GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      ((LIS3MDLDriver *)ip)->sensitivity[i] = LIS3MDL_SENS_12GA;
  else if(((LIS3MDLDriver *)ip)->config->fullscale == LIS3MDL_FS_16GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      ((LIS3MDLDriver *)ip)->sensitivity[i] = LIS3MDL_SENS_16GA;
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), compass full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t set_full_scale(void *ip, lis3mdl_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr;
  msg_t msg;

  if(fs == LIS3MDL_FS_4GA) {
    newfs = LIS3MDL_4GA;
  }
  else if(fs == LIS3MDL_FS_8GA) {
    newfs = LIS3MDL_8GA;
  }
  else if(fs == LIS3MDL_FS_12GA) {
    newfs = LIS3MDL_12GA;
  }
  else if(fs == LIS3MDL_FS_16GA) {
    newfs = LIS3MDL_16GA;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((LIS3MDLDriver *)ip)->fullscale) {
    scale = newfs / ((LIS3MDLDriver *)ip)->fullscale;
    ((LIS3MDLDriver *)ip)->fullscale = newfs;
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus(((LIS3MDLDriver *)ip)->config->i2cp);
  i2cStart(((LIS3MDLDriver *)ip)->config->i2cp,
           ((LIS3MDLDriver *)ip)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */

    /* Updating register.*/
    cr = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                  LIS3MDL_AD_CTRL_REG2, &msg);
    if(msg != MSG_OK)
      return msg;

    cr &= ~(LIS3MDL_CTRL_REG2_FS_MASK);
    cr |= fs;
    msg = lis3mdlI2CWriteRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                  ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                    LIS3MDL_AD_CTRL_REG2, cr);
    if(msg != MSG_OK)
      return msg;
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */

    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
      ((LIS3MDLDriver *)ip)->sensitivity[i] *= scale;
      ((LIS3MDLDriver *)ip)->bias[i] *= scale;
    }
  }
  return MSG_OK;
}

static const struct BaseSensorVMT vmt_basesensor = {
  get_axes_number, read_raw, read_cooked
};

static const struct BaseCompassVMT vmt_basecompass = {
  get_axes_number, read_raw, read_cooked,
  set_bias, reset_bias, set_sensivity, reset_sensivity
};

static const struct LIS3MDLVMT vmt_lis3mdl = {
  get_axes_number, read_raw, read_cooked,
  set_bias, reset_bias, set_sensivity, reset_sensivity,
  set_full_scale
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p LIS3MDLDriver object
 *
 * @init
 */
void lis3mdlObjectInit(LIS3MDLDriver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_basecompass = &vmt_basecompass;
  devp->vmt_lis3mdl = &vmt_lis3mdl;
  devp->config = NULL;
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
    devp->bias[i] = 0;
  devp->state  = LIS3MDL_STOP;
}

/**
 * @brief   Configures and activates LIS3MDL Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LIS3MDLDriver object
 * @param[in] config    pointer to the @p LIS3MDLConfig object
 *
 * @api
 */
void lis3mdlStart(LIS3MDLDriver *devp, const LIS3MDLConfig *config) {
  uint32_t i;
  uint8_t cr;
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LIS3MDL_STOP) || (devp->state == LIS3MDL_READY),
              "lis3mdlStart(), invalid state");

  devp->config = config;

#if LIS3MDL_USE_I2C
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
           
  /* Control register 1 configuration block.*/
  {
    cr = devp->config->outputdatarate;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr |= devp->config->operationmodexy;
#else
    cr |= LIS3MDL_CTRL_REG1_OM0 | LIS3MDL_CTRL_REG1_OM1;
#endif
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG1, cr);
  }

  /* Control register 2 configuration block.*/
  {
    cr = devp->config->fullscale;
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG2, cr);
  }

  /* Control register 3 configuration block.*/
  {
    cr = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr = devp->config->conversionmode;
#endif
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG3, cr);
  }

  /* Control register 4 configuration block.*/
  {
    cr = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr = devp->config->operationmodez | devp->config->endianness;
#endif
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG4, cr);
  }

  /* Control register 5 configuration block.*/
  {
    cr = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr = devp->config->blockdataupdate;
#endif
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG5, cr);
  }

                              
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == LIS3MDL_FS_4GA) {
    devp->fullscale = LIS3MDL_4GA;
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_4GA;
  }
  else if(devp->config->fullscale == LIS3MDL_FS_8GA) {
    devp->fullscale = LIS3MDL_8GA;
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_8GA;
  }
  else if(devp->config->fullscale == LIS3MDL_FS_12GA) {
    devp->fullscale = LIS3MDL_12GA;
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_12GA;
  }
  else if(devp->config->fullscale == LIS3MDL_FS_16GA) {
    devp->fullscale = LIS3MDL_16GA;
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_16GA;
  }
  else
    osalDbgAssert(FALSE, "lis3mdlStart(), compass full scale issue");
  devp->state = LIS3MDL_READY;
} 

/**
 * @brief   Deactivates the LIS3MDL Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LIS3MDLDriver object
 *
 * @api
 */
void lis3mdlStop(LIS3MDLDriver *devp) {
  uint8_t cr;
  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS3MDL_STOP) || (devp->state == LIS3MDL_READY),
                "lis3mdlStop(), invalid state");

#if (LIS3MDL_USE_I2C)
  if (devp->state == LIS3MDL_STOP) {
#if LIS3MDL_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */
    cr = LIS3MDL_CTRL_REG3_MD0 | LIS3MDL_CTRL_REG3_MD1;
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG3, cr);
    i2cStop((devp)->config->i2cp);
#if LIS3MDL_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
  }
#endif /* LIS3MDL_USE_I2C */
  devp->state = LIS3MDL_STOP;
}
/** @} */
