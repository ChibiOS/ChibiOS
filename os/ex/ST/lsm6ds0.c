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
 * @addtogroup lsm6ds0
 * @{
 */

#include "hal.h"

#include "lsm6ds0.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  LSM6DS0_ACC_SENS_2G                ((float)0.061f)
#define  LSM6DS0_ACC_SENS_4G                ((float)0.122f)
#define  LSM6DS0_ACC_SENS_8G                ((float)0.244f)
#define  LSM6DS0_ACC_SENS_16G               ((float)0.732f)

#define  LSM6DS0_GYRO_SENS_245DPS           ((float)0.00875f)
#define  LSM6DS0_GYRO_SENS_500DPS           ((float)0.01750f)
#define  LSM6DS0_GYRO_SENS_2000DPS          ((float)0.07000f)

#define  LSM6DS0_TEMP_SENS                  ((float)16.0f)
#define  LSM6DS0_TEMP_SENS_OFF              ((float)25.0f)

#define  LSM6DS0_DI                         ((uint8_t)0xFF)
#define  LSM6DS0_DI_0                       ((uint8_t)0x01)
#define  LSM6DS0_DI_1                       ((uint8_t)0x02)
#define  LSM6DS0_DI_2                       ((uint8_t)0x04)
#define  LSM6DS0_DI_3                       ((uint8_t)0x08)
#define  LSM6DS0_DI_4                       ((uint8_t)0x10)
#define  LSM6DS0_DI_5                       ((uint8_t)0x20)
#define  LSM6DS0_DI_6                       ((uint8_t)0x40)
#define  LSM6DS0_DI_7                       ((uint8_t)0x80)

#define  LSM6DS0_AD_0                       ((uint8_t)0x01)
#define  LSM6DS0_AD_1                       ((uint8_t)0x02)
#define  LSM6DS0_AD_2                       ((uint8_t)0x04)
#define  LSM6DS0_AD_3                       ((uint8_t)0x08)
#define  LSM6DS0_AD_4                       ((uint8_t)0x10)
#define  LSM6DS0_AD_5                       ((uint8_t)0x20)
#define  LSM6DS0_AD_6                       ((uint8_t)0x40)

#define  LSM6DS0_RW                         ((uint8_t)0x80)

#define  LSM6DS0_AD_ACT_THS                 ((uint8_t)0x04)
#define  LSM6DS0_AD_ACT_DUR                 ((uint8_t)0x05)
#define  LSM6DS0_AD_INT_GEN_CFG_XL          ((uint8_t)0x06)
#define  LSM6DS0_AD_INT_GEN_THS_X_XL        ((uint8_t)0x07)
#define  LSM6DS0_AD_INT_GEN_THS_Y_XL        ((uint8_t)0x08)
#define  LSM6DS0_AD_INT_GEN_THS_Z_XL        ((uint8_t)0x09)
#define  LSM6DS0_AD_INT_GEN_DUR_XL          ((uint8_t)0x0A)
#define  LSM6DS0_AD_REFERENCE_G             ((uint8_t)0x0B)
#define  LSM6DS0_AD_INT_CTRL                ((uint8_t)0x0C)
#define  LSM6DS0_AD_WHO_AM_I                ((uint8_t)0x0F)
#define  LSM6DS0_AD_CTRL_REG1_G             ((uint8_t)0x10)
#define  LSM6DS0_AD_CTRL_REG2_G             ((uint8_t)0x11)
#define  LSM6DS0_AD_CTRL_REG3_G             ((uint8_t)0x12)
#define  LSM6DS0_AD_ORIENT_CFG_G            ((uint8_t)0x13)
#define  LSM6DS0_AD_INT_GEN_SRC_G           ((uint8_t)0x14)
#define  LSM6DS0_AD_OUT_TEMP_L              ((uint8_t)0x15)
#define  LSM6DS0_AD_OUT_TEMP_H              ((uint8_t)0x16)
#define  LSM6DS0_AD_STATUS_REG1             ((uint8_t)0x17)
#define  LSM6DS0_AD_OUT_X_L_G               ((uint8_t)0x18)
#define  LSM6DS0_AD_OUT_X_H_G               ((uint8_t)0x19)
#define  LSM6DS0_AD_OUT_Y_L_G               ((uint8_t)0x1A)
#define  LSM6DS0_AD_OUT_Y_H_G               ((uint8_t)0x1B)
#define  LSM6DS0_AD_OUT_Z_L_G               ((uint8_t)0x1C)
#define  LSM6DS0_AD_OUT_Z_H_G               ((uint8_t)0x1D)
#define  LSM6DS0_AD_CTRL_REG4               ((uint8_t)0x1E)
#define  LSM6DS0_AD_CTRL_REG5_XL            ((uint8_t)0x1F)
#define  LSM6DS0_AD_CTRL_REG6_XL            ((uint8_t)0x20)
#define  LSM6DS0_AD_CTRL_REG7_XL            ((uint8_t)0x21)
#define  LSM6DS0_AD_CTRL_REG8               ((uint8_t)0x22)
#define  LSM6DS0_AD_CTRL_REG9               ((uint8_t)0x23)
#define  LSM6DS0_AD_CTRL_REG10              ((uint8_t)0x24)
#define  LSM6DS0_AD_INT_GEN_SRC_XL          ((uint8_t)0x26)
#define  LSM6DS0_AD_STATUS_REG2             ((uint8_t)0x27)
#define  LSM6DS0_AD_OUT_X_L_XL              ((uint8_t)0x28)
#define  LSM6DS0_AD_OUT_X_H_XL              ((uint8_t)0x29)
#define  LSM6DS0_AD_OUT_Y_L_XL              ((uint8_t)0x2A)
#define  LSM6DS0_AD_OUT_Y_H_XL              ((uint8_t)0x2B)
#define  LSM6DS0_AD_OUT_Z_L_XL              ((uint8_t)0x2C)
#define  LSM6DS0_AD_OUT_Z_H_XL              ((uint8_t)0x2D)
#define  LSM6DS0_AD_FIFO_CTRL               ((uint8_t)0x2E)
#define  LSM6DS0_AD_FIFO_SRC                ((uint8_t)0x2F)
#define  LSM6DS0_AD_INT_GEN_CFG_G           ((uint8_t)0x30)
#define  LSM6DS0_AD_INT_GEN_THS_XH_G        ((uint8_t)0x31)
#define  LSM6DS0_AD_INT_GEN_THS_XL_G        ((uint8_t)0x32)
#define  LSM6DS0_AD_INT_GEN_THS_YH_G        ((uint8_t)0x33)
#define  LSM6DS0_AD_INT_GEN_THS_YL_G        ((uint8_t)0x34)
#define  LSM6DS0_AD_INT_GEN_THS_ZH_G        ((uint8_t)0x35)
#define  LSM6DS0_AD_INT_GEN_THS_ZL_G        ((uint8_t)0x36)
#define  LSM6DS0_AD_INT_GEN_DUR_G           ((uint8_t)0x37)

#define  LSM6DS0_CTRL_REG1_G_FS_MASK        ((uint8_t)0x18)
#define  LSM6DS0_CTRL_REG6_XL_FS_MASK       ((uint8_t)0x18)

#define  TO_G                               ((float)0.001f)
#define  TO_SI                              ((float)0.00981f)
/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   LSM6DS0 address increment mode.
 */
typedef enum {
  LSM6DS0_IF_ADD_INC_DIS = 0x00,    /**< Address increment disabled.        */
  LSM6DS0_IF_ADD_INC_EN = 0x04      /**< Address increment disabled.        */
}lsm6ds0_id_add_inc_t;

/**
 * @brief   LSM6DS0 gyroscope subsystem sleep mode.
 */
typedef enum {
  LSM6DS0_GYRO_SLP_DISABLED = 0x00, /**< Gyroscope in normal mode.          */
  LSM6DS0_GYRO_SLP_ENABLED  = 0x40  /**< Gyroscope in sleep mode.           */
}lsm6ds0_gyro_slp_t;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (LSM6DS0_USE_I2C) || defined(__DOXYGEN__)
/**
 * @brief   Reads registers value using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 * @note    IF_ADD_INC bit must be 1 in CTRL_REG8
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] sad        slave address without R bit
 * @param[in] reg        first sub-register address
 * @return               the read value.
 */
uint8_t lsm6ds0I2CReadRegister(I2CDriver *i2cp, lsm6ds0_sad_t sad, uint8_t reg,
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
msg_t lsm6ds0I2CWriteRegister(I2CDriver *i2cp, lsm6ds0_sad_t sad, uint8_t reg,
                              uint8_t value) {
  uint8_t rxbuf;
  uint8_t txbuf[2];
  switch (reg) {
  default:
    /* Reserved register must not be written, according to the datasheet
     * this could permanently damage the device.
     */
    osalDbgAssert(FALSE, "lsm6ds0WriteRegisterI2C(), reserved register");
  case LSM6DS0_AD_WHO_AM_I:
  case LSM6DS0_AD_INT_GEN_SRC_G:
  case LSM6DS0_AD_OUT_TEMP_L:
  case LSM6DS0_AD_OUT_TEMP_H:
  case LSM6DS0_AD_STATUS_REG1:
  case LSM6DS0_AD_OUT_X_L_G:
  case LSM6DS0_AD_OUT_X_H_G:
  case LSM6DS0_AD_OUT_Y_L_G:
  case LSM6DS0_AD_OUT_Y_H_G:
  case LSM6DS0_AD_OUT_Z_L_G:
  case LSM6DS0_AD_OUT_Z_H_G:
  case LSM6DS0_AD_INT_GEN_SRC_XL:
  case LSM6DS0_AD_STATUS_REG2:
  case LSM6DS0_AD_OUT_X_L_XL:
  case LSM6DS0_AD_OUT_X_H_XL:
  case LSM6DS0_AD_OUT_Y_L_XL:
  case LSM6DS0_AD_OUT_Y_H_XL:
  case LSM6DS0_AD_OUT_Z_L_XL:
  case LSM6DS0_AD_OUT_Z_H_XL:
  case LSM6DS0_AD_FIFO_SRC:
  /* Read only registers cannot be written, the command is ignored.*/
    return MSG_RESET;
  case LSM6DS0_AD_ACT_THS:
  case LSM6DS0_AD_ACT_DUR:
  case LSM6DS0_AD_INT_GEN_CFG_XL:
  case LSM6DS0_AD_INT_GEN_THS_X_XL:
  case LSM6DS0_AD_INT_GEN_THS_Y_XL:
  case LSM6DS0_AD_INT_GEN_THS_Z_XL:
  case LSM6DS0_AD_INT_GEN_DUR_XL:
  case LSM6DS0_AD_REFERENCE_G:
  case LSM6DS0_AD_INT_CTRL:
  case LSM6DS0_AD_CTRL_REG1_G:
  case LSM6DS0_AD_CTRL_REG2_G:
  case LSM6DS0_AD_CTRL_REG3_G:
  case LSM6DS0_AD_ORIENT_CFG_G:
  case LSM6DS0_AD_CTRL_REG4:
  case LSM6DS0_AD_CTRL_REG5_XL:
  case LSM6DS0_AD_CTRL_REG6_XL:
  case LSM6DS0_AD_CTRL_REG7_XL:
  case LSM6DS0_AD_CTRL_REG8:
  case LSM6DS0_AD_CTRL_REG9:
  case LSM6DS0_AD_CTRL_REG10:
  case LSM6DS0_AD_FIFO_CTRL:
  case LSM6DS0_AD_INT_GEN_CFG_G:
  case LSM6DS0_AD_INT_GEN_THS_XH_G:
  case LSM6DS0_AD_INT_GEN_THS_XL_G:
  case LSM6DS0_AD_INT_GEN_THS_YH_G:
  case LSM6DS0_AD_INT_GEN_THS_YL_G:
  case LSM6DS0_AD_INT_GEN_THS_ZH_G:
  case LSM6DS0_AD_INT_GEN_THS_ZL_G:
  case LSM6DS0_AD_INT_GEN_DUR_G:
    txbuf[0] = reg;
    txbuf[1] = value;
    return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, &rxbuf, 0, TIME_INFINITE);
    break;
  }
}
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

static msg_t acc_read_raw(void *ip, int32_t axes[]) {
  int16_t tmp;
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
  if(((LSM6DS0Driver *)ip)->config->acccfg->axesenabling & LSM6DS0_ACC_AE_X){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                 LSM6DS0_AD_OUT_X_L_XL, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                  LSM6DS0_AD_OUT_X_H_XL, NULL) << 8;
    axes[0] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->accbias[0];
  }
  if(((LSM6DS0Driver *)ip)->config->acccfg->axesenabling & LSM6DS0_ACC_AE_Y){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                 LSM6DS0_AD_OUT_Y_L_XL, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                  LSM6DS0_AD_OUT_Y_H_XL, NULL) << 8;
    axes[1] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->accbias[1];
  }
  if(((LSM6DS0Driver *)ip)->config->acccfg->axesenabling & LSM6DS0_ACC_AE_Z){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                 LSM6DS0_AD_OUT_Z_L_XL, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                  LSM6DS0_AD_OUT_Z_H_XL, NULL) << 8;
    axes[2] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->accbias[2];
  }
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
  return MSG_OK;
}

static msg_t gyro_read_raw(void *ip, int32_t axes[]) {
  int16_t tmp;
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
  if(((LSM6DS0Driver *)ip)->config->gyrocfg->axesenabling & LSM6DS0_GYRO_AE_X){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                   LSM6DS0_AD_OUT_X_L_G, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                    LSM6DS0_AD_OUT_X_H_G, NULL) << 8;
	axes[0] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->gyrobias[0];
  }
  if(((LSM6DS0Driver *)ip)->config->acccfg->axesenabling & LSM6DS0_GYRO_AE_Y){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                   LSM6DS0_AD_OUT_Y_L_G, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                    LSM6DS0_AD_OUT_Y_H_G, NULL) << 8;
    axes[1] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->gyrobias[1];
  }
  if(((LSM6DS0Driver *)ip)->config->acccfg->axesenabling & LSM6DS0_GYRO_AE_Z){
    tmp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                   LSM6DS0_AD_OUT_Z_L_G, NULL);
    tmp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                  ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                    LSM6DS0_AD_OUT_Z_H_G, NULL) << 8;
    axes[2] = (int32_t)tmp - ((LSM6DS0Driver *)ip)->gyrobias[2];
  }
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */ 
  return MSG_OK;
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
    if(((LSM6DS0Driver *)ip)->config->acccfg->unit == LSM6DS0_ACC_UNIT_G){
      axes[i] *= TO_G;
    }
    else if(((LSM6DS0Driver *)ip)->config->acccfg->unit == LSM6DS0_ACC_UNIT_SI){
      axes[i] *= TO_SI;
    }
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
  int32_t buff[LSM6DS0_GYRO_NUMBER_OF_AXES] = {0, 0, 0};
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
  }
  return msg;
}

static msg_t acc_set_bias(void *ip, int32_t *bp) {
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

static msg_t gyro_set_bias(void *ip, int32_t *bp) {
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
    ((LSM6DS0Driver *)ip)->accbias[i] = 0;
  return MSG_OK;
}

static msg_t gyro_reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY) ||
                (((LSM6DS0Driver *)ip)->state == LSM6DS0_STOP),
              "gyro_reset_bias(), invalid state");

  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
    ((LSM6DS0Driver *)ip)->gyrobias[i] = 0;
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
  return MSG_OK;
}

static msg_t gyro_reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM6DS0Driver *)ip)->state == LSM6DS0_READY),
                "gyro_reset_sensivity(), invalid state");

  if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_245DSP)
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_245DPS;
  else if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_500DSP)
	for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_500DPS;
  else if(((LSM6DS0Driver *)ip)->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_2000DSP)
	for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      ((LSM6DS0Driver *)ip)->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_2000DPS;
  return MSG_OK;
}

static msg_t sens_get_temperature(void *ip, float* tempp) {
  int16_t temp;
#if LSM6DS0_USE_I2C
  osalDbgAssert((((LSM6DS0Driver *)ip)->config->i2cp->state == I2C_READY),
                "gyro_read_raw(), channel not ready");
#if LSM6DS0_SHARED_I2C
  i2cAcquireBus(((LSM6DS0Driver *)ip)->config->i2cp);
  i2cStart(((LSM6DS0Driver *)ip)->config->i2cp,
           ((LSM6DS0Driver *)ip)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
  temp = lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                  LSM6DS0_AD_OUT_TEMP_L, NULL);
  temp += lsm6ds0I2CReadRegister(((LSM6DS0Driver *)ip)->config->i2cp,
                                 ((LSM6DS0Driver *)ip)->config->slaveaddress,
                                   LSM6DS0_AD_OUT_TEMP_H, NULL) << 8;
#if LSM6DS0_SHARED_I2C
  i2cReleaseBus(((LSM6DS0Driver *)ip)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
#endif /* LSM6DS0_USE_I2C */
  *tempp = ((float)temp / LSM6DS0_TEMP_SENS) + LSM6DS0_TEMP_SENS_OFF;
  return MSG_OK;
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
  acc_set_bias, acc_reset_bias, acc_set_sensivity, acc_reset_sensivity
};

static const struct LSM6DS0GYROVMT vmt_lsm6ds0gyro = {
  gyro_get_axes_number, gyro_read_raw, gyro_read_cooked,
  gyro_sample_bias, gyro_set_bias, gyro_reset_bias,
  gyro_set_sensivity, gyro_reset_sensivity, sens_get_temperature
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
    devp->accbias[i] = 0;
  for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
    devp->gyrobias[i] = 0;
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
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LSM6DS0_STOP) || (devp->state == LSM6DS0_READY),
              "lsm6ds0Start(), invalid state");			  

  devp->config = config;

#if LSM6DS0_USE_I2C
#if	LSM6DS0_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
  if((devp)->config->acccfg != NULL) {
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG5_XL,
                            devp->config->acccfg->decmode |
                            devp->config->acccfg->axesenabling);
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG6_XL,
                            devp->config->acccfg->outdatarate |
                            devp->config->acccfg->fullscale );
  }
  if((devp)->config->gyrocfg != NULL) {
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG1_G,
                            devp->config->gyrocfg->fullscale |
                            devp->config->gyrocfg->outdatarate);
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG2_G,
                            devp->config->gyrocfg->outsel);
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG3_G,
                            devp->config->gyrocfg->hpfenable |
                            devp->config->gyrocfg->lowmodecfg |
                            devp->config->gyrocfg->hpcfg);
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG4,
                            devp->config->gyrocfg->axesenabling);
    lsm6ds0I2CWriteRegister(devp->config->i2cp,
                            devp->config->slaveaddress,
                            LSM6DS0_AD_CTRL_REG9,
                            LSM6DS0_GYRO_SLP_DISABLED);
  }
  lsm6ds0I2CWriteRegister(devp->config->i2cp,
                          devp->config->slaveaddress,
                          LSM6DS0_AD_CTRL_REG8,
                          devp->config->endianness |
                          devp->config->blockdataupdate);
#if	LSM6DS0_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */  
#endif /* LSM6DS0_USE_I2C */
  /* Storing sensitivity information according to full scale value */
  if((devp)->config->acccfg != NULL) {
    if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_2G)
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM6DS0_ACC_SENS_2G;
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_4G)
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM6DS0_ACC_SENS_4G;
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_8G)
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM6DS0_ACC_SENS_8G;
    else if(devp->config->acccfg->fullscale == LSM6DS0_ACC_FS_16G)
      for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM6DS0_ACC_SENS_16G;
    else
      osalDbgAssert(FALSE, "lsm6ds0Start(), accelerometer full scale issue");
  }
  if((devp)->config->gyrocfg != NULL) {
    if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_245DSP)
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
        devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_245DPS;
    else if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_500DSP)
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
        devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_500DPS;
    else if(devp->config->gyrocfg->fullscale == LSM6DS0_GYRO_FS_2000DSP)
      for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
        devp->gyrosensitivity[i] = LSM6DS0_GYRO_SENS_2000DPS;
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

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LSM6DS0_STOP) || (devp->state == LSM6DS0_READY),
                "lsm6ds0Stop(), invalid state");

#if (LSM6DS0_USE_I2C)
  if (devp->state == LSM6DS0_STOP) {
#if	LSM6DS0_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LSM6DS0_SHARED_I2C */
    if((devp)->config->acccfg != NULL) {
      lsm6ds0I2CWriteRegister(devp->config->i2cp,
                              devp->config->slaveaddress,
                              LSM6DS0_AD_CTRL_REG6_XL,
                              LSM6DS0_ACC_ODR_PD);
    }
    if((devp)->config->gyrocfg != NULL) {
      lsm6ds0I2CWriteRegister(devp->config->i2cp,
                              devp->config->slaveaddress,
                              LSM6DS0_AD_CTRL_REG9,
                              LSM6DS0_GYRO_SLP_ENABLED);
    }
    i2cStop((devp)->config->i2cp);
#if	LSM6DS0_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM6DS0_SHARED_I2C */    
  }			  
#endif /* LSM6DS0_USE_I2C */
  devp->state = LSM6DS0_STOP;
}
/** @} */
