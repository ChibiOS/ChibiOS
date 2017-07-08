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
 * @file    lsm6ds0.c
 * @brief   LSM6DS0 MEMS interface module code.
 *
 * @addtogroup LSM6DS0
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "lsm6ds0.h"

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

#if (LSM6DS0_USE_I2C) || defined(__DOXYGEN__)
/**
 * @brief   Reads registers value using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 * @note    IF_ADD_INC bit must be 1 in CTRL_REG8
 *
 * @param[in]  i2cp      pointer to the I2C interface
 * @param[in]  sad       slave address without R bit
 * @param[in]  reg       first sub-register address
 * @param[out] rxbuf     pointer to an output buffer
 * @param[in]  n         number of consecutive register to read
 * @return               the operation status.
 * @notapi
 */
msg_t lsm6ds0I2CReadRegister(I2CDriver *i2cp, lsm6ds0_sad_t sad, uint8_t reg,
                             uint8_t* rxbuf, size_t n) {

  return i2cMasterTransmitTimeout(i2cp, sad, &reg, 1, rxbuf, n,
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
 */
#define lsm6ds0I2CWriteRegister(i2cp, sad, txbuf, n)                        \
        i2cMasterTransmitTimeout(i2cp, sad, txbuf, n + 1, NULL, 0,          \
                                  TIME_INFINITE)
#endif /* LSM6DS0_USE_I2C */

/*
 * Interface implementation.
 */
static size_t acc_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LSM6DS0_ACC_NUMBER_OF_AXES;
}

static size_t gyro_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LSM6DS0_GYRO_NUMBER_OF_AXES;
}

static size_t sens_get_axes_number(void *ip) {
  size_t size = 0;
  
  osalDbgCheck(ip != NULL);
  if (((LSM6DS0Driver *)ip)->config->acccfg != NULL)
  size += acc_get_axes_number(ip);
  if (((LSM6DS0Driver *)ip)->config->gyrocfg != NULL)
    size += gyro_get_axes_number(ip);  
  return size;
}

static msg_t acc_read_raw(void *ip, int32_t* axes) {
  int16_t tmp;
  uint8_t i, buff[2 * LSM6DS0_ACC_NUMBER_OF_AXES];
  msg_t msg = MSG_OK;
  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM6DS0Driver *)ip)->config->acccfg != NULL));
  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
              "acc_read_raw(), invalid state");

#if LSM6DS0_USE_I2C
  osalDbgAssert((((LSM6DS0Driver *)ip)->config->i2cp->state == I2C_READY),
                "acc_read_raw(), channel not ready");
#if LSM6DS0_SHARED_I2C
  i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
  i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
           ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
  msg = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                              ((LSM6DS0Driver *)ip)->config->slaveaddress,
                              LSM6DS0_AD_OUT_X_L_XL, buff, 2 * LSM6DS0_ACC_NUMBER_OF_AXES);
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */

  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
    if(msg == MSG_OK) { 
      tmp = buff[2*i] + (buff[2*i+1] << 8);
      axes[i] = (int32_t)tmp;
    }
    else{
      axes[i] = 0;
    }
  }
  return msg;
}

static msg_t gyro_read_raw(void *ip, int32_t* axes) {
  int16_t tmp;
  uint8_t i, buff[2 * LSM6DS0_GYRO_NUMBER_OF_AXES];
  msg_t msg = MSG_OK;
  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM6DS0Driver *)ip)->config->gyrocfg != NULL));
  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
              "gyro_read_raw(), invalid state");

              #if LSM6DS0_USE_I2C
  osalDbgAssert((((LSM6DS0Driver *)ip)->config->i2cp->state == I2C_READY),
                "gyro_read_raw(), channel not ready");
#if LSM6DS0_SHARED_I2C
  i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
  i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
           ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
  msg = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                              ((LSM6DS0Driver *)ip)->config->slaveaddress,
                              LSM6DS0_AD_OUT_X_L_G, buff, 2 * LSM6DS0_GYRO_NUMBER_OF_AXES);
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */

  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
    if(msg == MSG_OK) { 
      tmp = buff[2*i] + (buff[2*i+1] << 8);
      axes[i] = (int32_t)tmp;
    }
    else{
      axes[i] = 0;
    }
  }
  return msg;
}

static msg_t sens_read_raw(void *ip, int32_t axes[]) {
  int32_t* bp = axes;
  msg_t msg;
  if (((LSM6DS0Driver *)ip)->config->acccfg != NULL) {
  msg = acc_read_raw(ip, bp);
  if(msg != MSG_OK)
    return msg;
  bp += LSM6DS0_ACC_NUMBER_OF_AXES;
  }
  if (((LSM6DS0Driver *)ip)->config->gyrocfg != NULL) {
    msg = gyro_read_raw(ip, bp);
  }
  return msg;
}
  
static msg_t acc_read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LSM6DS0_ACC_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM6DS0Driver *)ip)->config->acccfg != NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
              "acc_read_cooked(), invalid state");

  msg = acc_read_raw(ip, raw);
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] * ((LSM6DS0Driver *)ip)->accsensitivity[i];
    axes[i] -= ((LSM6DS0Driver *)ip)->accbias[i];
  }
  return msg;
}

static msg_t gyro_read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LSM6DS0_GYRO_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM6DS0Driver *)ip)->config->gyrocfg != NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
              "gyro_read_cooked(), invalid state");

  msg = gyro_read_raw(ip, raw);
  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] * ((LSM6DS0Driver *)ip)->gyrosensitivity[i];
    axes[i] -= ((LSM6DS0Driver *)ip)->gyrobias[i];
  }
  return msg;
}

static msg_t sens_read_cooked(void *ip, float axes[]) {
  float* bp = axes;
  msg_t msg;
  if (((LSM6DS0Driver *)ip)->config->acccfg != NULL) {
  msg = acc_read_cooked(ip, bp);
  if(msg != MSG_OK)
    return msg;
  bp += LSM6DS0_ACC_NUMBER_OF_AXES;
  }
  if (((LSM6DS0Driver *)ip)->config->gyrocfg != NULL) {
    msg = gyro_read_cooked(ip, bp);
  }
  return msg;
}

static msg_t gyro_sample_bias(void *ip) {
  uint32_t i, j;
  int32_t raw[LSM6DS0_GYRO_NUMBER_OF_AXES];
  float buff[LSM6DS0_GYRO_NUMBER_OF_AXES] = {0.0f, 0.0f, 0.0f};
  msg_t msg;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "sample_bias(), invalid state");

  for(i = 0; i < LSM6DS0_GYRO_BIAS_ACQ_TIMES; i++){
    msg = gyro_read_raw(ip, raw);
    if(msg != MSG_OK)
      return msg;
    for(j = 0; j < LSM6DS0_GYRO_NUMBER_OF_AXES; j++){
      buff[j] += raw[j];
    }
    osalThreadSleepMicroseconds(LSM6DS0_GYRO_BIAS_SETTLING_uS);
  }

  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++){
    ((LSM6DS0Driver *)ip)->gyrobias[i] = buff[i] / LSM6DS0_GYRO_BIAS_ACQ_TIMES;
  ((LSM6DS0Driver *)ip)->gyrobias[i] *= ((LSM6DS0Driver *)ip)->gyrosensitivity[i];
  }
  return msg;
}

static msg_t acc_set_bias(void *ip, float *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY) ||
                (((LSM6DS0Driver *)ip)->state == LSM6DS0_STOP),
                "acc_set_bias(), invalid state");
  
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
    ((LSM6DS0Driver *)ip)->accbias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t gyro_set_bias(void *ip, float *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY) ||
                (((LSM6DS0Driver *)ip)->state == LSM6DS0_STOP),
                "gyro_set_bias(), invalid state");
  
  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
    ((LSM6DS0Driver *)ip)->gyrobias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t acc_reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY) ||
                (((LSM6DS0Driver *)ip)->state == LSM6DS0_STOP),
              "acc_reset_bias(), invalid state");

  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
    ((LSM6DS0Driver *)ip)->accbias[i] = 0.0f;
  return MSG_OK;
}

static msg_t gyro_reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY) ||
                (((LSM6DS0Driver *)ip)->state == LSM6DS0_STOP),
              "gyro_reset_bias(), invalid state");

  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
    ((LSM6DS0Driver *)ip)->gyrobias[i] = 0.0f;
  return MSG_OK;
}

static msg_t acc_set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "acc_set_sensivity(), invalid state");
  
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
    ((LSM6DS0Driver *)ip)->accsensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t gyro_set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "gyro_set_sensivity(), invalid state");
  
  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
    ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t acc_reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "acc_reset_sensivity(), invalid state");

  if(((LSM6DS0Driver *)ip)->config->acccfg->fullscale == LSM6DS0_ACC_FS_2G)
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->accsensitivity[i] = LSM6DS0_ACC_SENS_2G;
  else if(((LSM6DS0Driver *)ip)->config->acccfg->fullscale == LSM6DS0_ACC_FS_4G)
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->accsensitivity[i] = LSM6DS0_ACC_SENS_4G;
  else if(((LSM6DS0Driver *)ip)->config->acccfg->fullscale == LSM6DS0_ACC_FS_8G)
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->accsensitivity[i] = LSM6DS0_ACC_SENS_8G;
  else if(((LSM6DS0Driver *)ip)->config->acccfg->fullscale == LSM6DS0_ACC_FS_16G)
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->accsensitivity[i] = LSM6DS0_ACC_SENS_16G;
  else {
    osalDbgAssert(FALSE, "acc_reset_sensivity(), accelerometer full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t gyro_reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "gyro_reset_sensivity(), invalid state");

  if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_245DPS)
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_245DPS;
  else if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_500DPS)
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_500DPS;
  else if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_2000DPS)
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_2000DPS;
  else {
    osalDbgAssert(FALSE, "gyro_reset_sensivity(), gyroscope full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t acc_set_full_scale(void *ip, lsm6ds0_acc_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr[2];
  msg_t msg;

  if(fs == LSM6DS0_ACC_FS_2G) {
    newfs = LSM6DS0_ACC_2G;
  }
  else if(fs == LSM6DS0_ACC_FS_4G) {
    newfs = LSM6DS0_ACC_4G;
  }
  else if(fs == LSM6DS0_ACC_FS_8G) {
    newfs = LSM6DS0_ACC_8G;
  }
  else if(fs == LSM6DS0_ACC_FS_16G) {
    newfs = LSM6DS0_ACC_16G;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((LSM6DS0Driver *)ip)->accfullscale) {
    scale = newfs / ((LSM6DS0Driver *)ip)->accfullscale;
    ((LSM6DS0Driver *)ip)->accfullscale = newfs;

#if LSM6DS0_SHARED_I2C
    i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
    i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
            ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    /* Updating register.*/
    msg = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                LSM6DS0_AD_CTRL_REG6_XL, &cr[1], 1);
#if LSM6DS0_SHARED_I2C
    i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
     if(msg != MSG_OK)
      return msg;

    cr[0] = LSM6DS0_AD_CTRL_REG6_XL;
    cr[1] &= ~(LSM6DS0_CTRL_REG6_XL_FS_MASK);
    cr[1] |= fs;
    
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
    i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
            ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
    
    msg = lsm6ds0I2CWriteRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress, cr, 1);
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
    if(msg != MSG_OK)
      return msg;
    
    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
      ((LSM6DS0Driver *)ip)->accsensitivity[i] *= scale;
      ((LSM6DS0Driver *)ip)->accbias[i] *= scale;
    }
  }
  return msg;
}

static msg_t gyro_set_full_scale(void *ip, lsm6ds0_gyro_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr[2];
  msg_t msg;

  if(fs == LSM6DS0_GYRO_FS_245DPS) {
    newfs = LSM6DS0_GYRO_245DPS;
  }
  else if(fs == LSM6DS0_GYRO_FS_500DPS) {
    newfs = LSM6DS0_GYRO_500DPS;
  }
  else if(fs == LSM6DS0_GYRO_FS_2000DPS) {
    newfs = LSM6DS0_GYRO_2000DPS;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((LSM6DS0Driver *)ip)->gyrofullscale) {
    scale = newfs / ((LSM6DS0Driver *)ip)->gyrofullscale;
    ((LSM6DS0Driver *)ip)->gyrofullscale = newfs;
    
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
    i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
            ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    /* Updating register.*/
    msg = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                LSM6DS0_AD_CTRL_REG1_G, &cr[1], 1);

#if LSM6DS0_SHARED_I2C
    i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
     if(msg != MSG_OK)
      return msg;

    cr[0] = LSM6DS0_AD_CTRL_REG1_G;
    cr[1] &= ~(LSM6DS0_CTRL_REG1_G_FS_MASK);
    cr[1] |= fs;
    
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
    i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
            ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
    
    msg = lsm6ds0I2CWriteRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                 cr, 1);

#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
    if(msg != MSG_OK)
      return msg;
    
    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] *= scale;
      ((LSM6DS0Driver *)ip)->gyrobias[i] *= scale;
    }
  }  
  return msg;
}

static const struct BaseSensorVMT vmt_basesensor = {
  sens_get_axes_number, sens_read_raw, sens_read_cooked
};

static const struct BaseGyroscopeVMT vmt_basegyroscope = {
  gyro_get_axes_number, gyro_read_raw, gyro_read_cooked,
  gyro_sample_bias, gyro_set_bias, gyro_reset_bias,
  gyro_set_sensivity, gyro_reset_sensivity
};

static const struct BaseAccelerometerVMT vmt_baseaccelerometer = {
  acc_get_axes_number, acc_read_raw, acc_read_cooked,
  acc_set_bias, acc_reset_bias, acc_set_sensivity, acc_reset_sensivity
};

static const struct LSM6DS0ACCVMT vmt_lsm6ds0acc = {
  acc_get_axes_number, acc_read_raw, acc_read_cooked,
  acc_set_bias, acc_reset_bias, acc_set_sensivity, acc_reset_sensivity,
  acc_set_full_scale
};

static const struct LSM6DS0GYROVMT vmt_lsm6ds0gyro = {
  gyro_get_axes_number, gyro_read_raw, gyro_read_cooked,
  gyro_sample_bias, gyro_set_bias, gyro_reset_bias,
  gyro_set_sensivity, gyro_reset_sensivity, gyro_set_full_scale
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p LSM6DS0Driver object
 *
 * @init
 */
void lsm6ds0ObjectInit(LSM6DS0Driver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_baseaccelerometer = &vmt_baseaccelerometer;
  devp->vmt_basegyroscope = &vmt_basegyroscope;
  devp->vmt_lsm6ds0acc = &vmt_lsm6ds0acc;
  devp->vmt_lsm6ds0gyro = &vmt_lsm6ds0gyro;
  devp->config = NULL;
  for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
    devp->accbias[i] = 0.0f;
  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
    devp->gyrobias[i] = 0.0f;
  devp->state  = LSM6DS0_STOP;
}

/**
 * @brief   Configures and activates LSM6DS0 Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LSM6DS0Driver object
 * @param[in] config    pointer to the @p LSM6DS0Config object
 *
 * @api
 */
void lsm6ds0Start(LSM6DS0Driver *devp, const LSM6DS0Config *config) {
  uint32_t i;
  uint8_t cr[5];
  osalDbgCheck((devp != NULL) && (config != NULL) &&
               ((devp->config->acccfg != NULL) ||
                (devp->config->gyrocfg != NULL)));

  osalDbgAssert((devp->state == LSM6DS0_STOP) || (devp->state == LSM6DS0_READY),
              "lsm6ds0Start(), invalid state");        

  devp->config = config;

  /* Control register 8 configuration block.*/
  {
    cr[0] = LSM6DS0_AD_CTRL_REG8;
    cr[1] = LSM6DS0_CTRL_REG8_IF_ADD_INC;
#if LSM6DS0_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] |= devp->config->endianness | devp->config->blockdataupdate;
#endif
  }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */

  i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
  lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                          cr, 1);

#if LSM6DS0_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
  if((devp)->config->acccfg != NULL) {

    cr[0] = LSM6DS0_AD_CTRL_REG5_XL;

    /* Control register 5 configuration block.*/
    {
        cr[1] = LSM6DS0_CTRL_REG5_XL_XEN_XL | LSM6DS0_CTRL_REG5_XL_YEN_XL |
       LSM6DS0_CTRL_REG5_XL_ZEN_XL;
#if LSM6DS0_ACC_USE_ADVANCED || defined(__DOXYGEN__)
        cr[1] |= devp->config->acccfg->decmode;
#endif
    }

    /* Control register 6 configuration block.*/
    {
      cr[2] = devp->config->acccfg->outdatarate |
              devp->config->acccfg->fullscale;
    }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 2);

#if LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
  }

  if((devp)->config->gyrocfg != NULL) {

    cr[0] = LSM6DS0_AD_CTRL_REG1_G;

    /* Control register 1 configuration block.*/
    {
      cr[1] = devp->config->gyrocfg->fullscale |
              devp->config->gyrocfg->outdatarate;
#if LSM6DS0_GYRO_USE_ADVANCED || defined(__DOXYGEN__)
      cr[1] |= devp->config->acccfg->decmode;
#endif
    }

    /* Control register 2 configuration block.*/
    {
      cr[2] = 0;
#if LSM6DS0_GYRO_USE_ADVANCED || defined(__DOXYGEN__)
      cr[2] |= devp->config->gyrocfg->outsel;
#endif
    }

    /* Control register 3 configuration block.*/
    {
      cr[3] = 0;
#if LSM6DS0_GYRO_USE_ADVANCED || defined(__DOXYGEN__)
      cr[3] |= devp->config->gyrocfg->hpfenable |
               devp->config->gyrocfg->lowmodecfg |
               devp->config->gyrocfg->hpcfg;
#endif
    }

    /* Control register 4 configuration block.*/
    {
      cr[4] = LSM6DS0_CTRL_REG4_XEN_G | LSM6DS0_CTRL_REG4_YEN_G |
              LSM6DS0_CTRL_REG4_ZEN_G;
    }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 4);

#if LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */

    cr[0] = LSM6DS0_AD_CTRL_REG9;
    /* Control register 9 configuration block.*/
    {
        cr[1] = 0;
    }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 1);

#if LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
  }

  /* Storing sensitivity information according to full scale value */
  if((devp)->config->acccfg != NULL) {
    if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_2G) {
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
       if((devp)->config->acccfg->bias == NULL)
         devp->accsensitivity[i] = LSM6DS0_ACC_SENS_2G;
       else
         devp->accsensitivity[i] = devp->config->acccfg->sensitivity[i];
      }
      devp->accfullscale = LSM6DS0_ACC_2G;
    }
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_4G) {
     for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
       if((devp)->config->acccfg->bias == NULL)
         devp->accsensitivity[i] = LSM6DS0_ACC_SENS_4G;
       else
         devp->accsensitivity[i] = devp->config->acccfg->sensitivity[i];
      }
     devp->accfullscale = LSM6DS0_ACC_4G;
    }
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_8G) {
     for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
       if((devp)->config->acccfg->bias == NULL)
         devp->accsensitivity[i] = LSM6DS0_ACC_SENS_8G;
       else
         devp->accsensitivity[i] = devp->config->acccfg->sensitivity[i];
      }
     devp->accfullscale = LSM6DS0_ACC_8G;
    }
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_16G) {
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
        if((devp)->config->acccfg->bias == NULL)
          devp->accsensitivity[i] = LSM6DS0_ACC_SENS_16G;
        else
          devp->accsensitivity[i] = devp->config->acccfg->sensitivity[i];
      }
      devp->accfullscale = LSM6DS0_ACC_16G;
    }
    else
      osalDbgAssert(FALSE, "lsm6ds0Start(), accelerometer full scale issue");
  }

  if((devp)->config->gyrocfg != NULL) {
    if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_245DPS) {
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
        if((devp)->config->acccfg->bias == NULL)
          devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_245DPS;
        else
          devp->gyrosensitivity[i] = devp->config->gyrocfg->sensitivity[i];
      }
      devp->gyrofullscale = LSM6DS0_GYRO_245DPS;
    }
    else if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_500DPS) {
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
        if((devp)->config->acccfg->bias == NULL)
          devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_500DPS;
        else
          devp->gyrosensitivity[i] = devp->config->gyrocfg->sensitivity[i];
      }
      devp->gyrofullscale = LSM6DS0_GYRO_500DPS;
    }
    else if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_2000DPS) {
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
        if((devp)->config->acccfg->bias == NULL)
          devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_2000DPS;
        else
          devp->gyrosensitivity[i] = devp->config->gyrocfg->sensitivity[i];
      }
      devp->gyrofullscale = LSM6DS0_GYRO_2000DPS;
    }
    else
      osalDbgAssert(FALSE, "lsm6ds0Start(), gyroscope full scale issue");
  }
  /* This is the Gyroscope transient recovery time */
  osalThreadSleepMilliseconds(5);

  devp->state = LSM6DS0_READY;
} 

/**
 * @brief   Deactivates the LSM6DS0 Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LSM6DS0Driver object
 *
 * @api
 */
void lsm6ds0Stop(LSM6DS0Driver *devp) {
  uint8_t cr[2];

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LSM6DS0_STOP) || (devp->state == LSM6DS0_READY),
                "lsm6ds0Stop(), invalid state");

  if (devp->state == LSM6DS0_READY) {
    if((devp)->config->acccfg != NULL) {
      cr[0] = LSM6DS0_AD_CTRL_REG6_XL;
      /* Control register 6 configuration block.*/
      {
        cr[1] = 0;
      }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 1);

#if LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
    }
    if((devp)->config->gyrocfg != NULL) {
    cr[0] = LSM6DS0_AD_CTRL_REG9;
    /* Control register 9 configuration block.*/
    {
      cr[1] = LSM6DS0_CTRL_REG9_SLEEP_G;
    }
#if LSM6DS0_USE_I2C
#if LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */

    lsm6ds0I2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 1);

    i2cStop((devp)->config->i2cp);
#if LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
    }
  }        
  devp->state = LSM6DS0_STOP;
}
/** @} */
