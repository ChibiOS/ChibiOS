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

#include "ch.h"
#include "hal.h"

#include "lis3mdl.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  LIS3MDL_SENS_4GA                    ((float)6842.0f)
#define  LIS3MDL_SENS_8GA                    ((float)3421.0f)
#define  LIS3MDL_SENS_12GA                   ((float)2281.0f)
#define  LIS3MDL_SENS_16GA                   ((float)1711.0f)

#define  LIS3MDL_TEMP_SENS                   ((float)8.0f)

#define  LIS3MDL_DI                          ((uint8_t)0xFF)
#define  LIS3MDL_DI_0                        ((uint8_t)0x01)
#define  LIS3MDL_DI_1                        ((uint8_t)0x02)
#define  LIS3MDL_DI_2                        ((uint8_t)0x04)
#define  LIS3MDL_DI_3                        ((uint8_t)0x08)
#define  LIS3MDL_DI_4                        ((uint8_t)0x10)
#define  LIS3MDL_DI_5                        ((uint8_t)0x20)
#define  LIS3MDL_DI_6                        ((uint8_t)0x40)
#define  LIS3MDL_DI_7                        ((uint8_t)0x80)

#define  LIS3MDL_AD                          ((uint8_t)0x3F)
#define  LIS3MDL_AD_0                        ((uint8_t)0x01)
#define  LIS3MDL_AD_1                        ((uint8_t)0x02)
#define  LIS3MDL_AD_2                        ((uint8_t)0x04)
#define  LIS3MDL_AD_3                        ((uint8_t)0x08)
#define  LIS3MDL_AD_4                        ((uint8_t)0x10)
#define  LIS3MDL_AD_5                        ((uint8_t)0x20)
#define  LIS3MDL_AD_6                        ((uint8_t)0x40)

#define  LIS3MDL_RW                          ((uint8_t)0x80)

#define  LIS3MDL_AD_WHO_AM_I                 ((uint8_t)0x0F)
#define  LIS3MDL_AD_CTRL_REG1                ((uint8_t)0x20)
#define  LIS3MDL_AD_CTRL_REG2                ((uint8_t)0x21)
#define  LIS3MDL_AD_CTRL_REG3                ((uint8_t)0x22)
#define  LIS3MDL_AD_CTRL_REG4                ((uint8_t)0x23)
#define  LIS3MDL_AD_CTRL_REG5                ((uint8_t)0x24)
#define  LIS3MDL_AD_STATUS_REG               ((uint8_t)0x27)
#define  LIS3MDL_AD_OUT_X_L                  ((uint8_t)0x28)
#define  LIS3MDL_AD_OUT_X_H                  ((uint8_t)0x29)
#define  LIS3MDL_AD_OUT_Y_L                  ((uint8_t)0x2A)
#define  LIS3MDL_AD_OUT_Y_H                  ((uint8_t)0x2B)
#define  LIS3MDL_AD_OUT_Z_L                  ((uint8_t)0x2C)
#define  LIS3MDL_AD_OUT_Z_H                  ((uint8_t)0x2D)
#define  LIS3MDL_AD_TEMP_OUT_L               ((uint8_t)0x2E)
#define  LIS3MDL_AD_TEMP_OUT_H               ((uint8_t)0x2F)
#define  LIS3MDL_AD_INT_CFG                  ((uint8_t)0x30)
#define  LIS3MDL_AD_INT_SOURCE               ((uint8_t)0x31)
#define  LIS3MDL_AD_INT_THS_L                ((uint8_t)0x32)
#define  LIS3MDL_AD_INT_THS_H                ((uint8_t)0x33)

#define  LIS3MDL_CTRL_REG2_FS_MASK           ((uint8_t)0x60)

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
 * @param[in] sub        sub-register address
 * @param[in] value      the value to be written
 * @return               the operation status.
 */
msg_t lis3mdlI2CWriteRegister(I2CDriver *i2cp, lis3mdl_sad_t sad, uint8_t reg,
                              uint8_t value) {
  uint8_t rxbuf;
  uint8_t txbuf[2];
  switch (reg) {
  default:
    /* Reserved register must not be written, according to the datasheet
     * this could permanently damage the device.
     */
    chDbgAssert(FALSE, "lis3mdlI2CWriteRegister(), reserved register");
  case LIS3MDL_AD_WHO_AM_I:
  case LIS3MDL_AD_STATUS_REG:
  case LIS3MDL_AD_OUT_X_L:
  case LIS3MDL_AD_OUT_X_H:
  case LIS3MDL_AD_OUT_Y_L:
  case LIS3MDL_AD_OUT_Y_H:
  case LIS3MDL_AD_OUT_Z_L:
  case LIS3MDL_AD_OUT_Z_H:
  case LIS3MDL_AD_TEMP_OUT_L:
  case LIS3MDL_AD_TEMP_OUT_H:
  case LIS3MDL_AD_INT_SOURCE:
  case LIS3MDL_AD_INT_THS_L:
  case LIS3MDL_AD_INT_THS_H:
  /* Read only registers cannot be written, the command is ignored.*/
    return MSG_RESET;
  case LIS3MDL_AD_CTRL_REG1:
  case LIS3MDL_AD_CTRL_REG2:
  case LIS3MDL_AD_CTRL_REG3:
  case LIS3MDL_AD_CTRL_REG4:
  case LIS3MDL_AD_CTRL_REG5:
  case LIS3MDL_AD_INT_CFG:
    txbuf[0] = reg;
    txbuf[1] = value;
    return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, &rxbuf, 0, TIME_INFINITE);
    break;
  }
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

  osalDbgCheck((ip != NULL) && (axes != NULL));
  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
              "read_raw(), invalid state");

#if LIS3MDL_USE_I2C
  osalDbgAssert((((LIS3MDLDriver *)ip)->config->i2cp->state == I2C_READY),
                "read_raw(), channel not ready");
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus(((LIS3MDLDriver *)ip)->config->i2cp);
  i2cStart(((LIS3MDLDriver *)ip)->config->i2cp,
           ((LIS3MDLDriver *)ip)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */

  axes[0] = (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                             ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                               LIS3MDL_AD_OUT_X_L, NULL));
  axes[0] += (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                              ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                                LIS3MDL_AD_OUT_X_H, NULL) << 8);
  axes[0] -= ((LIS3MDLDriver *)ip)->bias[0];

  axes[1] = (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                             ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                             LIS3MDL_AD_OUT_Y_L, NULL));
  axes[1] += (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                              ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                              LIS3MDL_AD_OUT_Y_H, NULL) << 8);
  axes[1] -= ((LIS3MDLDriver *)ip)->bias[1];

  axes[2] = (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                             ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                               LIS3MDL_AD_OUT_Z_L, NULL));
  axes[2] += (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                              ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                                LIS3MDL_AD_OUT_Z_H, NULL) << 8);
  axes[2] -= ((LIS3MDLDriver *)ip)->bias[2];

#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
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
  else
    osalDbgAssert(FALSE, "reset_sensivity(), compass full scale issue");
  return MSG_OK;
}

static msg_t get_temperature(void *ip, float* tempp) {
  int16_t temp;
#if LIS3MDL_USE_I2C
  osalDbgAssert((((LIS3MDLDriver *)ip)->config->i2cp->state == I2C_READY),
                "gyro_read_raw(), channel not ready");
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus(((LIS3MDLDriver *)ip)->config->i2cp);
  i2cStart(((LIS3MDLDriver *)ip)->config->i2cp,
           ((LIS3MDLDriver *)ip)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */
  temp = (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                          ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                            LIS3MDL_AD_TEMP_OUT_L, NULL));
  temp += (int16_t)(lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                           ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                             LIS3MDL_AD_TEMP_OUT_H, NULL) << 8);
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
  *tempp = (float)temp / LIS3MDL_TEMP_SENS;
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
  get_temperature
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
  lis3mdlI2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LIS3MDL_AD_CTRL_REG1,
                          devp->config->temperature |
                          devp->config->outputdatarate |
                          devp->config->operationmodexy);
  lis3mdlI2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LIS3MDL_AD_CTRL_REG2,
                          devp->config->fullscale);
  lis3mdlI2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LIS3MDL_AD_CTRL_REG3,
                          devp->config->conversionmode);
  lis3mdlI2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LIS3MDL_AD_CTRL_REG4,
                          devp->config->operationmodez |
                          devp->config->endianness);
  lis3mdlI2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LIS3MDL_AD_CTRL_REG5,
                          devp->config->blockdataupdate);
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == LIS3MDL_FS_4GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_4GA;
  else if(devp->config->fullscale == LIS3MDL_FS_8GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_8GA;
  else if(devp->config->fullscale == LIS3MDL_FS_12GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_12GA;
  else if(devp->config->fullscale == LIS3MDL_FS_16GA)
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3MDL_SENS_16GA;
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
    lis3mdlI2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LIS3MDL_AD_CTRL_REG3,
                            LIS3MDL_MD_POWER_DOWN);
    i2cStop((devp)->config->i2cp);
#if LIS3MDL_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
  }
#endif /* LIS3MDL_USE_I2C */
  devp->state = LIS3MDL_STOP;
}
/** @} */
