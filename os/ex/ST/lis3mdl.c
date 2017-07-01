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
 * @addtogroup LIS3MDL
 * @ingroup EX_ST
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
 * @param[in]  i2cp      pointer to the I2C interface
 * @param[in]  sad       slave address without R bit
 * @param[in]  reg       first sub-register address
 * @param[out] rxbuf     pointer to an output buffer
 * @param[in]  n         number of consecutive register to read
 * @return               the operation status.
 * @notapi
 */
msg_t lis3mdlI2CReadRegister(I2CDriver *i2cp, lis3mdl_sad_t sad, uint8_t reg,
                             uint8_t* rxbuf, size_t n) {
  uint8_t txbuf = reg;
  if(n > 1)
    txbuf |= LIS3MDL_SUB_MS;

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
 */
msg_t lis3mdlI2CWriteRegister(I2CDriver *i2cp, lis3mdl_sad_t sad, uint8_t* txbuf,
                              uint8_t n) {
  if (n > 1)
    (*txbuf) |= LIS3MDL_SUB_MS;

  return i2cMasterTransmitTimeout(i2cp, sad, txbuf, n + 1, NULL, 0,
                                  TIME_INFINITE);
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
  uint8_t i, buff[2 * LIS3MDL_NUMBER_OF_AXES];
  msg_t msg = MSG_OK;
  
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

  msg = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                              ((LIS3MDLDriver *)ip)->config->slaveaddress,
                              LIS3MDL_AD_OUT_X_L, buff, 2 * LIS3MDL_NUMBER_OF_AXES);

#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
                              
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
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

static msg_t read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LIS3MDL_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS3MDLDriver *)ip)->state == LIS3MDL_READY),
               "read_cooked(), invalid state");

  msg = read_raw(ip, raw);
  for(i = 0; i < LIS3MDL_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] * ((LIS3MDLDriver *)ip)->sensitivity[i];
    axes[i] -= ((LIS3MDLDriver *)ip)->bias[i];
  }
  return msg;
}

static msg_t set_bias(void *ip, float *bp) {
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
  uint8_t i, cr[2];
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
    msg = lis3mdlI2CReadRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                ((LIS3MDLDriver *)ip)->config->slaveaddress,
                                LIS3MDL_AD_CTRL_REG2, &cr[1], 1);
#if LIS3MDL_SHARED_I2C
    i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */                              
    if(msg != MSG_OK)
      return msg;


    cr[0] = LIS3MDL_AD_CTRL_REG2;
    cr[1] &= ~(LIS3MDL_CTRL_REG2_FS_MASK);
    cr[1] |= fs;

#if LIS3MDL_SHARED_I2C
    i2cAcquireBus(((LIS3MDLDriver *)ip)->config->i2cp);
    i2cStart(((LIS3MDLDriver *)ip)->config->i2cp,
            ((LIS3MDLDriver *)ip)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */
    
    msg = lis3mdlI2CWriteRegister(((LIS3MDLDriver *)ip)->config->i2cp,
                                 ((LIS3MDLDriver *)ip)->config->slaveaddress, cr, 1);
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus(((LIS3MDLDriver *)ip)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
    if(msg != MSG_OK)
      return msg;

    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
      ((LIS3MDLDriver *)ip)->sensitivity[i] *= scale;
      ((LIS3MDLDriver *)ip)->bias[i] *= scale;
    }
  }
  return msg;
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
    devp->bias[i] = 0.0f;
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
  uint8_t cr[6];
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LIS3MDL_STOP) || (devp->state == LIS3MDL_READY),
              "lis3mdlStart(), invalid state");

  devp->config = config;
         
  /* Control register 1 configuration block.*/
  {
    cr[0] = LIS3MDL_AD_CTRL_REG1;
    cr[1] = devp->config->outputdatarate;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] |= devp->config->operationmodexy;
#else
    cr[1] |= LIS3MDL_CTRL_REG1_OM0 | LIS3MDL_CTRL_REG1_OM1;
#endif
  }

  /* Control register 2 configuration block.*/
  {
    cr[2] = devp->config->fullscale;
  }

  /* Control register 3 configuration block.*/
  {
    cr[3] = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr[3] = devp->config->conversionmode;
#endif
  }

  /* Control register 4 configuration block.*/
  {
    cr[4] = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr[4] = devp->config->operationmodez | devp->config->endianness;
#endif
  }

  /* Control register 5 configuration block.*/
  {
    cr[5] = 0;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
    cr[5] = devp->config->blockdataupdate;
#endif
  }
  
#if LIS3MDL_USE_I2C
#if LIS3MDL_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
  lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                          cr, 5);
                              
#if LIS3MDL_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */

  if(devp->config->sensitivity == NULL) {
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
  }
  else{
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = devp->config->sensitivity[i];
  }

  if(devp->config->bias == NULL) {
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->bias[i] = 0.0f;
  }
  else {
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      devp->bias[i] = devp->config->bias[i];
  }
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
  uint8_t cr[2];
  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS3MDL_STOP) || (devp->state == LIS3MDL_READY),
                "lis3mdlStop(), invalid state");
                
  if (devp->state == LIS3MDL_READY) {
#if (LIS3MDL_USE_I2C)
#if LIS3MDL_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
             (devp)->config->i2ccfg);
#endif /* LIS3MDL_SHARED_I2C */
    cr[0] = LIS3MDL_AD_CTRL_REG3;
    cr[1] = LIS3MDL_CTRL_REG3_MD0 | LIS3MDL_CTRL_REG3_MD1;
    lis3mdlI2CWriteRegister(devp->config->i2cp, devp->config->slaveaddress,
                            cr, 1);
    i2cStop((devp)->config->i2cp);
#if LIS3MDL_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* LIS3MDL_SHARED_I2C */
#endif /* LIS3MDL_USE_I2C */
  }
  devp->state = LIS3MDL_STOP;
}
/** @} */
