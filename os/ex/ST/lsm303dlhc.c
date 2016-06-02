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
 * @file    lsm303dlhc.c
 * @brief   LSM303DLHC MEMS interface module code.
 *
 * @addtogroup lsm303dlhc
 * @{
 */

#include "hal.h"
#include "lsm303dlhc.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  LSM303DLHC_ACC_SENS_2G             ((float)0.0610f)
#define  LSM303DLHC_ACC_SENS_4G             ((float)0.1221f)
#define  LSM303DLHC_ACC_SENS_8G             ((float)0.2442f)
#define  LSM303DLHC_ACC_SENS_16G            ((float)0.4884f)

#define  LSM303DLHC_COMP_SENS_XY_1_3GA      ((float)1100.0f)
#define  LSM303DLHC_COMP_SENS_XY_1_9GA      ((float)855.0f)
#define  LSM303DLHC_COMP_SENS_XY_2_5GA      ((float)670.0f)
#define  LSM303DLHC_COMP_SENS_XY_4_0GA      ((float)450.0f)
#define  LSM303DLHC_COMP_SENS_XY_4_7GA      ((float)400.0f)
#define  LSM303DLHC_COMP_SENS_XY_5_6GA      ((float)330.0f)
#define  LSM303DLHC_COMP_SENS_XY_8_1GA      ((float)230.0f)

#define  LSM303DLHC_COMP_SENS_Z_1_3GA       ((float)980.0f)
#define  LSM303DLHC_COMP_SENS_Z_1_9GA       ((float)765.0f)
#define  LSM303DLHC_COMP_SENS_Z_2_5GA       ((float)600.0f)
#define  LSM303DLHC_COMP_SENS_Z_4_0GA       ((float)400.0f)
#define  LSM303DLHC_COMP_SENS_Z_4_7GA       ((float)355.0f)
#define  LSM303DLHC_COMP_SENS_Z_5_6GA       ((float)295.0f)
#define  LSM303DLHC_COMP_SENS_Z_8_1GA       ((float)205.0f)

#define  LSM303DLHC_DI                      ((uint8_t)0xFF)
#define  LSM303DLHC_DI_0                    ((uint8_t)0x01)
#define  LSM303DLHC_DI_1                    ((uint8_t)0x02)
#define  LSM303DLHC_DI_2                    ((uint8_t)0x04)
#define  LSM303DLHC_DI_3                    ((uint8_t)0x08)
#define  LSM303DLHC_DI_4                    ((uint8_t)0x10)
#define  LSM303DLHC_DI_5                    ((uint8_t)0x20)
#define  LSM303DLHC_DI_6                    ((uint8_t)0x40)
#define  LSM303DLHC_DI_7                    ((uint8_t)0x80)

#define  LSM303DLHC_AD_0                    ((uint8_t)0x01)
#define  LSM303DLHC_AD_1                    ((uint8_t)0x02)
#define  LSM303DLHC_AD_2                    ((uint8_t)0x04)
#define  LSM303DLHC_AD_3                    ((uint8_t)0x08)
#define  LSM303DLHC_AD_4                    ((uint8_t)0x10)
#define  LSM303DLHC_AD_5                    ((uint8_t)0x20)
#define  LSM303DLHC_AD_6                    ((uint8_t)0x40)
#define  LSM303DLHC_RW                      ((uint8_t)0x80)

#define  LSM303DLHC_AD_ACC_CTRL_REG1        ((uint8_t)0x20)
#define  LSM303DLHC_AD_ACC_CTRL_REG2        ((uint8_t)0x21)
#define  LSM303DLHC_AD_ACC_CTRL_REG3        ((uint8_t)0x22)
#define  LSM303DLHC_AD_ACC_CTRL_REG4        ((uint8_t)0x23)
#define  LSM303DLHC_AD_ACC_CTRL_REG5        ((uint8_t)0x24)
#define  LSM303DLHC_AD_ACC_CTRL_REG6        ((uint8_t)0x25)
#define  LSM303DLHC_AD_ACC_REFERENCE        ((uint8_t)0x26)
#define  LSM303DLHC_AD_ACC_STATUS_REG       ((uint8_t)0x27)
#define  LSM303DLHC_AD_ACC_OUT_X_L          ((uint8_t)0x28)
#define  LSM303DLHC_AD_ACC_OUT_X_H          ((uint8_t)0x29)
#define  LSM303DLHC_AD_ACC_OUT_Y_L          ((uint8_t)0x2A)
#define  LSM303DLHC_AD_ACC_OUT_Y_H          ((uint8_t)0x2B)
#define  LSM303DLHC_AD_ACC_OUT_Z_L          ((uint8_t)0x2C)
#define  LSM303DLHC_AD_ACC_OUT_Z_H          ((uint8_t)0x2D)
#define  LSM303DLHC_AD_ACC_FIFO_CTRL_REG    ((uint8_t)0x2E)
#define  LSM303DLHC_AD_ACC_FIFO_SRC_REG     ((uint8_t)0x2F)
#define  LSM303DLHC_AD_ACC_INT1_CFG         ((uint8_t)0x30)
#define  LSM303DLHC_AD_ACC_INT1_SRC         ((uint8_t)0x31)
#define  LSM303DLHC_AD_ACC_INT1_THS         ((uint8_t)0x32)
#define  LSM303DLHC_AD_ACC_INT1_DURATION    ((uint8_t)0x33)
#define  LSM303DLHC_AD_ACC_INT2_CFG         ((uint8_t)0x34)
#define  LSM303DLHC_AD_ACC_INT2_SRC         ((uint8_t)0x35)
#define  LSM303DLHC_AD_ACC_INT2_THS         ((uint8_t)0x36)
#define  LSM303DLHC_AD_ACC_INT2_DURATION    ((uint8_t)0x37)
#define  LSM303DLHC_AD_ACC_CLICK_CFG        ((uint8_t)0x38)
#define  LSM303DLHC_AD_ACC_CLICK_SRC        ((uint8_t)0x39)
#define  LSM303DLHC_AD_ACC_CLICK_THS        ((uint8_t)0x3A)
#define  LSM303DLHC_AD_ACC_TIME_LIMIT       ((uint8_t)0x3B)
#define  LSM303DLHC_AD_ACC_TIME_LATENCY     ((uint8_t)0x3C)
#define  LSM303DLHC_AD_ACC_TIME_WINDOW      ((uint8_t)0x3D)
#define  LSM303DLHC_AD_COMP_CRA_REG         ((uint8_t)0x00)
#define  LSM303DLHC_AD_COMP_CRB_REG         ((uint8_t)0x01)
#define  LSM303DLHC_AD_COMP_MR_REG          ((uint8_t)0x02)
#define  LSM303DLHC_AD_COMP_OUT_X_H         ((uint8_t)0x03)
#define  LSM303DLHC_AD_COMP_OUT_X_L         ((uint8_t)0x04)
#define  LSM303DLHC_AD_COMP_OUT_Z_H         ((uint8_t)0x05)
#define  LSM303DLHC_AD_COMP_OUT_Z_L         ((uint8_t)0x06)
#define  LSM303DLHC_AD_COMP_OUT_Y_H         ((uint8_t)0x07)
#define  LSM303DLHC_AD_COMP_OUT_Y_L         ((uint8_t)0x08)
#define  LSM303DLHC_AD_COMP_SR_REG          ((uint8_t)0x09)
#define  LSM303DLHC_AD_COMP_IRA_REG         ((uint8_t)0x0A)
#define  LSM303DLHC_AD_COMP_IRB_REG         ((uint8_t)0x0B)
#define  LSM303DLHC_AD_COMP_IRC_REG         ((uint8_t)0x0C)
#define  LSM303DLHC_AD_COMP_TEMP_OUT_H      ((uint8_t)0x31)
#define  LSM303DLHC_AD_COMP_TEMP_OUT_L      ((uint8_t)0x32)

#define  LSM303DLHC_ACC_CTRL_REG4_FS_MASK   ((uint8_t)0x30)
#define  LSM303DLHC_COMP_CTRL_REGB_FS_MASK  ((uint8_t)0xE0)

#define  TO_G                              ((float)0.001f)
#define  TO_SI                             ((float)0.00981f)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief  Accelerometer Power Mode
 */
typedef enum {
  LSM303DLHC_ACC_PM_NORMAL = 0x00,  /**< Normal mode enabled                */
  LSM303DLHC_ACC_PM_LP = 0x08       /**< Low Power mode enabled             */
} lsm303dlhc_acc_pm_t;

/**
 * @brief  Accelerometer and Compass Slave Address.
 */
typedef enum {
  LSM303DLHC_SAD_ACC = 0x19,        /**< SAD for accelerometer.             */
  LSM303DLHC_SAD_COMP = 0x1E        /**< SAD for compass.                   */
} lsm303dlhc_sad_t;
/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (LSM303DLHC_USE_I2C) || defined(__DOXYGEN__)
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
uint8_t lsm303dlhcI2CReadRegister(I2CDriver *i2cp, lsm303dlhc_sad_t sad, uint8_t reg,
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
msg_t lsm303dlhcI2CWriteRegister(I2CDriver *i2cp, lsm303dlhc_sad_t sad, uint8_t reg,
                              uint8_t value) {
  uint8_t rxbuf;
  uint8_t txbuf[2];
  if(sad == LSM303DLHC_SAD_ACC) {
    switch (reg) {
    default:
      /* Reserved register must not be written, according to the datasheet
       * this could permanently damage the device.
       */
      osalDbgAssert(FALSE, "lsm303dlhcWriteRegisterI2C(), reserved register");
    case LSM303DLHC_AD_ACC_STATUS_REG:
    case LSM303DLHC_AD_ACC_OUT_X_L:
    case LSM303DLHC_AD_ACC_OUT_X_H:
    case LSM303DLHC_AD_ACC_OUT_Y_L:
    case LSM303DLHC_AD_ACC_OUT_Y_H:
    case LSM303DLHC_AD_ACC_OUT_Z_L:
    case LSM303DLHC_AD_ACC_OUT_Z_H:
    case LSM303DLHC_AD_ACC_FIFO_SRC_REG:
    case LSM303DLHC_AD_ACC_INT1_SRC:
    case LSM303DLHC_AD_ACC_INT2_SRC:
    /* Read only registers cannot be written, the command is ignored.*/
      return MSG_RESET;
    case LSM303DLHC_AD_ACC_CTRL_REG1:
    case LSM303DLHC_AD_ACC_CTRL_REG2:
    case LSM303DLHC_AD_ACC_CTRL_REG3:
    case LSM303DLHC_AD_ACC_CTRL_REG4:
    case LSM303DLHC_AD_ACC_CTRL_REG5:
    case LSM303DLHC_AD_ACC_CTRL_REG6:
    case LSM303DLHC_AD_ACC_REFERENCE:
    case LSM303DLHC_AD_ACC_FIFO_CTRL_REG:
    case LSM303DLHC_AD_ACC_INT1_CFG:
    case LSM303DLHC_AD_ACC_INT1_THS:
    case LSM303DLHC_AD_ACC_INT1_DURATION:
    case LSM303DLHC_AD_ACC_INT2_CFG:
    case LSM303DLHC_AD_ACC_INT2_THS:
    case LSM303DLHC_AD_ACC_INT2_DURATION:
    case LSM303DLHC_AD_ACC_CLICK_CFG:
    case LSM303DLHC_AD_ACC_CLICK_SRC:
    case LSM303DLHC_AD_ACC_CLICK_THS:
    case LSM303DLHC_AD_ACC_TIME_LIMIT:
    case LSM303DLHC_AD_ACC_TIME_LATENCY:
    case LSM303DLHC_AD_ACC_TIME_WINDOW:
      txbuf[0] = reg;
      txbuf[1] = value;
      return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, &rxbuf, 0, TIME_INFINITE);
      break;
    }
  }
  if(sad == LSM303DLHC_SAD_COMP) {
    switch (reg) {
    default:
      /* Reserved register must not be written, according to the datasheet
       * this could permanently damage the device.
       */
      osalDbgAssert(FALSE, "lsm303dlhcWriteRegisterI2C(), reserved register");
    case LSM303DLHC_AD_COMP_OUT_X_H:
    case LSM303DLHC_AD_COMP_OUT_X_L:
    case LSM303DLHC_AD_COMP_OUT_Z_H:
    case LSM303DLHC_AD_COMP_OUT_Z_L:
    case LSM303DLHC_AD_COMP_OUT_Y_H:
    case LSM303DLHC_AD_COMP_OUT_Y_L:
    case LSM303DLHC_AD_COMP_SR_REG:
    case LSM303DLHC_AD_COMP_IRA_REG:
    case LSM303DLHC_AD_COMP_IRB_REG:
    case LSM303DLHC_AD_COMP_IRC_REG:
    case LSM303DLHC_AD_COMP_TEMP_OUT_H:
    case LSM303DLHC_AD_COMP_TEMP_OUT_L:
    /* Read only registers cannot be written, the command is ignored.*/
      return MSG_RESET;
    case LSM303DLHC_AD_COMP_CRA_REG:
    case LSM303DLHC_AD_COMP_CRB_REG:
    case LSM303DLHC_AD_COMP_MR_REG:
      txbuf[0] = reg;
      txbuf[1] = value;
      return i2cMasterTransmitTimeout(i2cp, sad, txbuf, 2, &rxbuf, 0, TIME_INFINITE);
      break;
    }
  }
  osalDbgAssert(FALSE, "lsm303dlhcWriteRegisterI2C(), wrong SAD");
  return MSG_RESET;
}
#endif /* LSM303DLHC_USE_I2C */

/*
 * Interface implementation.
 */
static size_t acc_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LSM303DLHC_ACC_NUMBER_OF_AXES;
}

static size_t comp_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LSM303DLHC_COMP_NUMBER_OF_AXES;
}

static size_t sens_get_axes_number(void *ip) {
  size_t size = 0;
  
  osalDbgCheck(ip != NULL);
  if (((LSM303DLHCDriver *)ip)->config->acccfg != NULL)
	size += acc_get_axes_number(ip);
  if (((LSM303DLHCDriver *)ip)->config->compcfg != NULL)
    size += comp_get_axes_number(ip);  
  return size;
}

static msg_t acc_read_raw(void *ip, int32_t axes[]) {
  int16_t tmp;
  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM303DLHCDriver *)ip)->config->acccfg != NULL));
  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
              "acc_read_raw(), invalid state");

#if LSM303DLHC_USE_I2C
  osalDbgAssert((((LSM303DLHCDriver *)ip)->config->i2cp->state == I2C_READY),
                "acc_read_raw(), channel not ready");
#if LSM303DLHC_SHARED_I2C
  i2cAcquireBus(((LSM303DLHCDriver *)ip)->config->i2cp);
  i2cStart(((LSM303DLHCDriver *)ip)->config->i2cp,
           ((LSM303DLHCDriver *)ip)->config->i2ccfg);
#endif /* LSM303DLHC_SHARED_I2C */
  if(((LSM303DLHCDriver *)ip)->config->acccfg->axesenabling & LSM303DLHC_ACC_AE_X){
    tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                 LSM303DLHC_SAD_ACC,
                                 LSM303DLHC_AD_ACC_OUT_X_L, NULL);
    tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                  LSM303DLHC_SAD_ACC,
                                  LSM303DLHC_AD_ACC_OUT_X_H, NULL) << 8;
    axes[0] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->accbias[0];
  }
  if(((LSM303DLHCDriver *)ip)->config->acccfg->axesenabling & LSM303DLHC_ACC_AE_Y){
    tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                 LSM303DLHC_SAD_ACC,
                                 LSM303DLHC_AD_ACC_OUT_Y_L, NULL);
    tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                  LSM303DLHC_SAD_ACC,
                                  LSM303DLHC_AD_ACC_OUT_Y_H, NULL) << 8;
    axes[1] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->accbias[1];
  }
  if(((LSM303DLHCDriver *)ip)->config->acccfg->axesenabling & LSM303DLHC_ACC_AE_Z){
    tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                 LSM303DLHC_SAD_ACC,
                                 LSM303DLHC_AD_ACC_OUT_Z_L, NULL);
    tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                  LSM303DLHC_SAD_ACC,
                                  LSM303DLHC_AD_ACC_OUT_Z_H, NULL) << 8;
    axes[2] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->accbias[2];
  }
#if LSM303DLHC_SHARED_I2C
  i2cReleaseBus(((LSM303DLHCDriver *)ip)->config->i2cp);
#endif /* LSM303DLHC_SHARED_I2C */
#endif /* LSM303DLHC_USE_I2C */
  return MSG_OK;
}

static msg_t comp_read_raw(void *ip, int32_t axes[]) {
  int16_t tmp;
  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM303DLHCDriver *)ip)->config->compcfg != NULL));
  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
              "comp_read_raw(), invalid state");

#if LSM303DLHC_USE_I2C
  osalDbgAssert((((LSM303DLHCDriver *)ip)->config->i2cp->state == I2C_READY),
                "comp_read_raw(), channel not ready");
#if LSM303DLHC_SHARED_I2C
  i2cAcquireBus(((LSM303DLHCDriver *)ip)->config->i2cp);
  i2cStart(((LSM303DLHCDriver *)ip)->config->i2cp,
           ((LSM303DLHCDriver *)ip)->config->i2ccfg);
#endif /* LSM303DLHC_SHARED_I2C */
  tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_OUT_X_L, NULL);
  tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                LSM303DLHC_SAD_COMP,
                                LSM303DLHC_AD_COMP_OUT_X_H, NULL) << 8;
  axes[0] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->compbias[0];

  tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_OUT_Y_L, NULL);
  tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                LSM303DLHC_SAD_COMP,
                                LSM303DLHC_AD_COMP_OUT_Y_H, NULL) << 8;
  axes[1] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->compbias[1];

  tmp = lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_OUT_Z_L, NULL);
  tmp += lsm303dlhcI2CReadRegister(((LSM303DLHCDriver *)ip)->config->i2cp,
                                LSM303DLHC_SAD_COMP,
                                LSM303DLHC_AD_COMP_OUT_Z_H, NULL) << 8;
  axes[2] = (int32_t)tmp - ((LSM303DLHCDriver *)ip)->compbias[2];
#if LSM303DLHC_SHARED_I2C
  i2cReleaseBus(((LSM303DLHCDriver *)ip)->config->i2cp);
#endif /* LSM303DLHC_SHARED_I2C */
#endif /* LSM303DLHC_USE_I2C */ 
  return MSG_OK;
}

static msg_t sens_read_raw(void *ip, int32_t axes[]) {
  int32_t* bp = axes;
  msg_t msg;
  if (((LSM303DLHCDriver *)ip)->config->acccfg != NULL) {
	msg = acc_read_raw(ip, bp);
	if(msg != MSG_OK)
	  return msg;
	bp += LSM303DLHC_ACC_NUMBER_OF_AXES;
  }
  if (((LSM303DLHCDriver *)ip)->config->compcfg != NULL) {
    msg = comp_read_raw(ip, bp);
  }
  return msg;
}
	
static msg_t acc_read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LSM303DLHC_ACC_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM303DLHCDriver *)ip)->config->acccfg != NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
              "acc_read_cooked(), invalid state");

  msg = acc_read_raw(ip, raw);
  for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] * ((LSM303DLHCDriver *)ip)->accsensitivity[i];
    if(((LSM303DLHCDriver *)ip)->config->acccfg->unit == LSM303DLHC_ACC_UNIT_G){
      axes[i] *= TO_G;
    }
    else if(((LSM303DLHCDriver *)ip)->config->acccfg->unit == LSM303DLHC_ACC_UNIT_SI){
      axes[i] *= TO_SI;
    }
  }
  return msg;
}

static msg_t comp_read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LSM303DLHC_COMP_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck(((ip != NULL) && (axes != NULL)) &&
              (((LSM303DLHCDriver *)ip)->config->compcfg != NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
              "comp_read_cooked(), invalid state");

  msg = comp_read_raw(ip, raw);
  for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] / ((LSM303DLHCDriver *)ip)->compsensitivity[i];
  }
  return msg;
}

static msg_t sens_read_cooked(void *ip, float axes[]) {
  float* bp = axes;
  msg_t msg;
  if (((LSM303DLHCDriver *)ip)->config->acccfg != NULL) {
	msg = acc_read_cooked(ip, bp);
	if(msg != MSG_OK)
	  return msg;
	bp += LSM303DLHC_ACC_NUMBER_OF_AXES;
  }
  if (((LSM303DLHCDriver *)ip)->config->compcfg != NULL) {
    msg = comp_read_cooked(ip, bp);
  }
  return msg;
}

static msg_t acc_set_bias(void *ip, int32_t *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY) ||
                (((LSM303DLHCDriver *)ip)->state == LSM303DLHC_STOP),
                "acc_set_bias(), invalid state");
  
  for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
    ((LSM303DLHCDriver *)ip)->accbias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t comp_set_bias(void *ip, int32_t *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY) ||
                (((LSM303DLHCDriver *)ip)->state == LSM303DLHC_STOP),
                "comp_set_bias(), invalid state");
  
  for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
    ((LSM303DLHCDriver *)ip)->compbias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t acc_reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY) ||
                (((LSM303DLHCDriver *)ip)->state == LSM303DLHC_STOP),
              "acc_reset_bias(), invalid state");

  for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
    ((LSM303DLHCDriver *)ip)->accbias[i] = 0;
  return MSG_OK;
}

static msg_t comp_reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY) ||
                (((LSM303DLHCDriver *)ip)->state == LSM303DLHC_STOP),
              "comp_reset_bias(), invalid state");

  for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++)
    ((LSM303DLHCDriver *)ip)->compbias[i] = 0;
  return MSG_OK;
}

static msg_t acc_set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
                "acc_set_sensivity(), invalid state");
  
  for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
    ((LSM303DLHCDriver *)ip)->accsensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t comp_set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
                "comp_set_sensivity(), invalid state");
  
  for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
    ((LSM303DLHCDriver *)ip)->compsensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t acc_reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
                "acc_reset_sensivity(), invalid state");

  if(((LSM303DLHCDriver *)ip)->config->acccfg->fullscale == LSM303DLHC_ACC_FS_2G)
    for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      ((LSM303DLHCDriver *)ip)->accsensitivity[i] = LSM303DLHC_ACC_SENS_2G;
  else if(((LSM303DLHCDriver *)ip)->config->acccfg->fullscale == LSM303DLHC_ACC_FS_4G)
	for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      ((LSM303DLHCDriver *)ip)->accsensitivity[i] = LSM303DLHC_ACC_SENS_4G;
  else if(((LSM303DLHCDriver *)ip)->config->acccfg->fullscale == LSM303DLHC_ACC_FS_8G)
	for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      ((LSM303DLHCDriver *)ip)->accsensitivity[i] = LSM303DLHC_ACC_SENS_8G;
  else if(((LSM303DLHCDriver *)ip)->config->acccfg->fullscale == LSM303DLHC_ACC_FS_16G)
	for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      ((LSM303DLHCDriver *)ip)->accsensitivity[i] = LSM303DLHC_ACC_SENS_16G;
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), accelerometer full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t comp_reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LSM303DLHCDriver *)ip)->state == LSM303DLHC_READY),
                "comp_reset_sensivity(), invalid state");

  if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_1_3_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_1_3GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_1_3GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_1_9_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_1_9GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_1_9GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_2_5_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_2_5GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_2_5GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_4_0_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_4_0GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_4_0GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_4_7_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_4_7GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_4_7GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_5_6_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_5_6GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_5_6GA;
      }
    }
  else if(((LSM303DLHCDriver *)ip)->config->compcfg->fullscale == LSM303DLHC_COMP_FS_8_1_GA)
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
      if(i != 2) {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_8_1GA;
      }
      else {
        ((LSM303DLHCDriver *)ip)->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_8_1GA;
      }
    }
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), compass full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static const struct BaseSensorVMT vmt_basesensor = {
  sens_get_axes_number, sens_read_raw, sens_read_cooked
};

static const struct BaseCompassVMT vmt_basecompass = {
  comp_get_axes_number, comp_read_raw, comp_read_cooked,
  comp_set_bias, comp_reset_bias, comp_set_sensivity, comp_reset_sensivity
};

static const struct BaseAccelerometerVMT vmt_baseaccelerometer = {
  acc_get_axes_number, acc_read_raw, acc_read_cooked,
  acc_set_bias, acc_reset_bias, acc_set_sensivity, acc_reset_sensivity
};

static const struct LSM303DLHCACCVMT vmt_lsm303dlhcacc = {
  acc_get_axes_number, acc_read_raw, acc_read_cooked,
  acc_set_bias, acc_reset_bias, acc_set_sensivity, acc_reset_sensivity
};

static const struct LSM303DLHCCOMPVMT vmt_lsm303dlhccomp = {
  comp_get_axes_number, comp_read_raw, comp_read_cooked,
  comp_set_bias, comp_reset_bias, comp_set_sensivity, comp_reset_sensivity
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p LSM303DLHCDriver object
 *
 * @init
 */
void lsm303dlhcObjectInit(LSM303DLHCDriver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_baseaccelerometer = &vmt_baseaccelerometer;
  devp->vmt_basecompass = &vmt_basecompass;
  devp->vmt_lsm303dlhcacc = &vmt_lsm303dlhcacc;
  devp->vmt_lsm303dlhccomp = &vmt_lsm303dlhccomp;
  devp->config = NULL;
  for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
    devp->accbias[i] = 0;
  for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++)
    devp->compbias[i] = 0;
  devp->state  = LSM303DLHC_STOP;
}

/**
 * @brief   Configures and activates LSM303DLHC Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LSM303DLHCDriver object
 * @param[in] config    pointer to the @p LSM303DLHCConfig object
 *
 * @api
 */
void lsm303dlhcStart(LSM303DLHCDriver *devp, const LSM303DLHCConfig *config) {
  uint32_t i;
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LSM303DLHC_STOP) || (devp->state == LSM303DLHC_READY),
              "lsm303dlhcStart(), invalid state");			  

  devp->config = config;

#if LSM303DLHC_USE_I2C
#if	LSM303DLHC_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LSM303DLHC_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
  if((devp)->config->acccfg != NULL) {
    lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                               LSM303DLHC_SAD_ACC,
                               LSM303DLHC_AD_ACC_CTRL_REG1,
                               devp->config->acccfg->axesenabling |
                               devp->config->acccfg->outdatarate |
                               devp->config->acccfg->lowpower);
    lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                               LSM303DLHC_SAD_ACC,
                               LSM303DLHC_AD_ACC_CTRL_REG4,
                               devp->config->acccfg->endianess |
                               devp->config->acccfg->fullscale |
                               devp->config->acccfg->blockdataupdate |
                               devp->config->acccfg->highresmode);
  }
  if((devp)->config->compcfg != NULL) {
    lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_CRA_REG,
                               devp->config->compcfg->outputdatarate);
    lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_CRB_REG,
                               devp->config->compcfg->fullscale);
    lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                               LSM303DLHC_SAD_COMP,
                               LSM303DLHC_AD_COMP_MR_REG,
                               devp->config->compcfg->mode);
  }
#if	LSM303DLHC_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM303DLHC_SHARED_I2C */  
#endif /* LSM303DLHC_USE_I2C */
  /* Storing sensitivity information according to full scale value */
  if((devp)->config->acccfg != NULL) {
    if(devp->config->acccfg->fullscale == LSM303DLHC_ACC_FS_2G)
      for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM303DLHC_ACC_SENS_2G;
    else if(devp->config->acccfg->fullscale == LSM303DLHC_ACC_FS_4G)
      for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM303DLHC_ACC_SENS_4G;
    else if(devp->config->acccfg->fullscale == LSM303DLHC_ACC_FS_8G)
      for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM303DLHC_ACC_SENS_8G;
    else if(devp->config->acccfg->fullscale == LSM303DLHC_ACC_FS_16G)
      for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
        devp->accsensitivity[i] = LSM303DLHC_ACC_SENS_16G;
    else
      osalDbgAssert(FALSE, "lsm303dlhcStart(), accelerometer full scale issue");
  }
  if((devp)->config->compcfg != NULL) {
    if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_1_3_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_1_3GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_1_3GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_1_9_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_1_9GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_1_9GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_2_5_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_2_5GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_2_5GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_4_0_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_4_0GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_4_0GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_4_7_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_4_7GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_4_7GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_5_6_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_5_6GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_5_6GA;
        }
      }
    else if(devp->config->compcfg->fullscale == LSM303DLHC_COMP_FS_8_1_GA)
      for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
        if(i != 2) {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_XY_8_1GA;
        }
        else {
          devp->compsensitivity[i] = LSM303DLHC_COMP_SENS_Z_8_1GA;
        }
      }
    else
      osalDbgAssert(FALSE, "lsm303dlhcStart(), compass full scale issue");
  }
  /* This is the Compass transient recovery time */
  osalThreadSleepMilliseconds(5);

  devp->state = LSM303DLHC_READY;
} 

/**
 * @brief   Deactivates the LSM303DLHC Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LSM303DLHCDriver object
 *
 * @api
 */
void lsm303dlhcStop(LSM303DLHCDriver *devp) {

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LSM303DLHC_STOP) || (devp->state == LSM303DLHC_READY),
                "lsm303dlhcStop(), invalid state");

#if (LSM303DLHC_USE_I2C)
  if (devp->state == LSM303DLHC_STOP) {
#if	LSM303DLHC_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LSM303DLHC_SHARED_I2C */
    if((devp)->config->acccfg != NULL) {
      lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                                 LSM303DLHC_SAD_ACC,
                                 LSM303DLHC_AD_ACC_CTRL_REG1,
                                 LSM303DLHC_ACC_AE_DISABLED |
                                 LSM303DLHC_ACC_ODR_PD);
    }
    if((devp)->config->compcfg != NULL) {
      lsm303dlhcI2CWriteRegister(devp->config->i2cp,
                                 LSM303DLHC_SAD_COMP,
                                 LSM303DLHC_AD_COMP_MR_REG,
                                 LSM303DLHC_COMP_MD_SLEEP);
    }
    i2cStop((devp)->config->i2cp);
#if	LSM303DLHC_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LSM303DLHC_SHARED_I2C */    
  }			  
#endif /* LSM303DLHC_USE_I2C */
  devp->state = LSM303DLHC_STOP;
}
/** @} */
