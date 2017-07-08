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
 * @file    hts221.c
 * @brief   HTS221 MEMS interface module code.
 *
 * @addtogroup HTS221
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "hts221.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define HTS221_SEL(mask, offset)    (int16_t)(mask << offset)

#define HTS221_FLAG_HYGRO_BIAS      0x01
#define HTS221_FLAG_HYGRO_SENS      0x02         
#define HTS221_FLAG_THERMO_BIAS     0x04
#define HTS221_FLAG_THERMO_SENS     0x08

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (HTS221_USE_I2C) || defined(__DOXYGEN__)
/**
 * @brief   Reads registers value using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in]  i2cp      pointer to the I2C interface
 * @param[in]  reg       first sub-register address
 * @param[out] rxbuf     pointer to an output buffer
 * @param[in]  n         number of consecutive register to read
 * @return               the operation status.
 * @notapi
 */
msg_t hts221I2CReadRegister(I2CDriver *i2cp, uint8_t reg, uint8_t* rxbuf,
                             size_t n) {
  uint8_t txbuf = reg;
  if(n > 1)
    txbuf |= HTS221_SUB_MS;

  return i2cMasterTransmitTimeout(i2cp, HTS221_SAD, &txbuf, 1, rxbuf, n,
                                  TIME_INFINITE);
}

/**
 * @brief   Writes a value into a register using I2C.
 * @pre     The I2C interface must be initialized and the driver started.
 *
 * @param[in] i2cp       pointer to the I2C interface
 * @param[in] txbuf      buffer containing sub-address value in first position
 *                       and values to write
 * @param[in] n          size of txbuf less one (not considering the first
 *                       element)
 * @return               the operation status.
 * @notapi
 */
msg_t hts221I2CWriteRegister(I2CDriver *i2cp, uint8_t* txbuf, size_t n) {
  if (n > 1)
    (*txbuf) |= HTS221_SUB_MS;

  return i2cMasterTransmitTimeout(i2cp, HTS221_SAD, txbuf, n + 1, NULL, 0,
                                  TIME_INFINITE);
}
#endif /* HTS221_USE_I2C */

/**
 * @brief   Compute biases and sensitivities starting from data stored in
 *          calibration registers.
 * @notapi
 *
 * @param[in] devp       pointer to the HTS221 interface
 * @param[in] flag       flag to select parameters
 * @return               the operation status.
 */
msg_t hts221Calibrate(HTS221Driver *devp, uint8_t flag) {
  msg_t msg;
  uint8_t calib[16], H0_rH_x2, H1_rH_x2, msb;
  int16_t H0_T0_OUT, H1_T0_OUT, T0_degC_x8, T1_degC_x8, T0_OUT, T1_OUT;
  float sens;

#if HTS221_SHARED_I2C
  i2cAcquireBus(devp->config->i2cp);
  i2cStart(devp->config->i2cp, devp->config->i2ccfg);
#endif /* HTS221_SHARED_I2C */

  /* Retrieving rH values from Calibration registers */
  msg = hts221I2CReadRegister(devp->config->i2cp,
                               HTS221_AD_CALIB_0, calib, 16);
                               
#if HTS221_SHARED_I2C
    i2cReleaseBus(devp->config->i2cp);
#endif /* HTS221_SHARED_I2C */
                              
  H0_rH_x2 = calib[0];
  H1_rH_x2 = calib[1];
  H0_T0_OUT = calib[6];
  H0_T0_OUT += calib[7] << 8;
  H1_T0_OUT = calib[10];
  H1_T0_OUT += calib[11] << 8;
  
  T0_degC_x8 = calib[2];
  /* Completing T0_degC_x8 value */
  msb = (calib[5] & HTS221_SEL(0x03, 0));
  if(msb & HTS221_SEL(0x01, 1)) {
    msb |= HTS221_SEL(0x3F, 2);
  }
  T0_degC_x8 += msb << 8;

  T1_degC_x8 = calib[3];
  /* Completing T1_degC_x8 value */
  msb = ((calib[5] & HTS221_SEL(0x03, 2)) >> 2);
  if(msb & HTS221_SEL(0x01, 1)) {
    msb |= HTS221_SEL(0x3F, 2);
  }
  T1_degC_x8 += msb << 8;

  T0_OUT = calib[12];
  T0_OUT += calib[13] << 8;
  T1_OUT = calib[14];
  T1_OUT += calib[15] << 8;
  
  sens = ((float)H1_rH_x2 - (float)H0_rH_x2) /
         (2.0f * ((float)H1_T0_OUT - (float)H0_T0_OUT));
                   
  if(flag & HTS221_FLAG_HYGRO_SENS)
      devp->sensitivity[0] = sens;

  if(flag & HTS221_FLAG_HYGRO_BIAS)
      devp->bias[0] = (sens * (float)H0_T0_OUT) -
                  ((float)H0_rH_x2 / 2.0f);

  sens = ((float)T1_degC_x8 - (float)T0_degC_x8) /
         (8.0f * ((float)T1_OUT - (float)T0_OUT));
         
  if(flag & HTS221_FLAG_THERMO_SENS)
  devp->sensitivity[1] = sens;

  if(flag & HTS221_FLAG_THERMO_BIAS)
  devp->bias[1] = (sens * (float)T0_OUT) -
                  ((float)T0_degC_x8 / 8.0f);
                    
  return msg;
}

/*
 * Interface implementation.
 */
static size_t hygro_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return HTS221_HYGRO_NUMBER_OF_AXES;
}

static size_t thermo_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return HTS221_THERMO_NUMBER_OF_AXES;
}

static size_t sens_get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return (thermo_get_axes_number(ip) + hygro_get_axes_number(ip));
}

static msg_t hygro_read_raw(void *ip, int32_t* axis) {
  int16_t tmp;
  uint8_t buff[2];
  msg_t msg = MSG_OK;
  
  *axis = 0;
  
  osalDbgCheck((ip != NULL) && (axis != NULL));
  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY),
              "hygro_read_raw(), invalid state");              
#if HTS221_USE_I2C
  osalDbgAssert((((HTS221Driver *)ip)->config->i2cp->state == I2C_READY),
                "hygro_read_raw(), channel not ready");
                
#if HTS221_SHARED_I2C
  i2cAcquireBus(((HTS221Driver *)ip)->config->i2cp);
  i2cStart(((HTS221Driver *)ip)->config->i2cp,
           ((HTS221Driver *)ip)->config->i2ccfg);
#endif /* HTS221_SHARED_I2C */

  msg = hts221I2CReadRegister(((HTS221Driver *)ip)->config->i2cp,
                              HTS221_AD_HUMIDITY_OUT_L, buff, 2);
                              
#if HTS221_SHARED_I2C
  i2cReleaseBus(((HTS221Driver *)ip)->config->i2cp);
#endif /* HTS221_SHARED_I2C */
#endif /* HTS221_USE_I2C */

  if(msg == MSG_OK) {
    tmp = buff[0] + (buff[1] << 8);
    *axis = (int32_t)tmp;
  }
  return msg;
}

static msg_t thermo_read_raw(void *ip, int32_t axis[]) {
  int16_t tmp;
  uint8_t buff[2];
  msg_t msg = MSG_OK;

  *axis = 0.0f;
  
  osalDbgCheck((ip != NULL) && (axis != NULL));
  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY),
              "thermo_read_raw(), invalid state");  
  
#if HTS221_USE_I2C
  osalDbgAssert((((HTS221Driver *)ip)->config->i2cp->state == I2C_READY),
                "thermo_read_raw(), channel not ready");
                
#if HTS221_SHARED_I2C
  i2cAcquireBus(((HTS221Driver *)ip)->config->i2cp);
  i2cStart(((HTS221Driver *)ip)->config->i2cp,
           ((HTS221Driver *)ip)->config->i2ccfg);
#endif /* HTS221_SHARED_I2C */

  msg = hts221I2CReadRegister(((HTS221Driver *)ip)->config->i2cp,
                                HTS221_AD_TEMP_OUT_L, buff, 2);
                                  
#if HTS221_SHARED_I2C
  i2cReleaseBus(((HTS221Driver *)ip)->config->i2cp);
#endif /* HTS221_SHARED_I2C */
#endif /* HTS221_USE_I2C */

  if(msg == MSG_OK) {
    tmp = buff[0] + (buff[1] << 8);
    *axis = (int32_t)tmp;
  }
  return msg;
}

static msg_t sens_read_raw(void *ip, int32_t axes[]) {
  int32_t* bp = axes;
  msg_t msg;
  msg = hygro_read_raw(ip, bp);
  if (msg != MSG_OK){
    return msg;
  }
  bp += HTS221_HYGRO_NUMBER_OF_AXES;
  return thermo_read_raw(ip, bp);
}

static msg_t hygro_read_cooked(void *ip, float* axis) {
  int32_t raw;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axis != NULL));
  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY),
              "hygro_read_cooked(), invalid state");

  msg = hygro_read_raw(ip, &raw);

  *axis = raw * ((HTS221Driver *)ip)->sensitivity[0];
  *axis -= ((HTS221Driver *)ip)->bias[0];
  return msg;
}

static msg_t thermo_read_cooked(void *ip, float* axis) {
  int32_t raw;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axis != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY),
              "thermo_read_cooked(), invalid state");

  msg = thermo_read_raw(ip, &raw);

  *axis = raw * ((HTS221Driver *)ip)->sensitivity[1];
  *axis -= ((HTS221Driver *)ip)->bias[1];
  return msg;
}

static msg_t sens_read_cooked(void *ip, float axes[]) {
  float *dp = axes;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY),
              "sens_read_cooked(), invalid state");

  msg = hygro_read_cooked(ip, dp);
  if(msg != MSG_OK)
    return msg;
  dp += HTS221_THERMO_NUMBER_OF_AXES;
  return thermo_read_cooked(ip, dp);
}

static msg_t hygro_set_bias(void *ip, float *bp) {
  osalDbgCheck((ip != NULL) && (bp != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_set_bias(), invalid state");

  ((HTS221Driver *)ip)->bias[0] = *bp;
  return MSG_OK;
}

static msg_t thermo_set_bias(void *ip, float *bp) {
  osalDbgCheck((ip != NULL) && (bp != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_set_bias(), invalid state");

  ((HTS221Driver *)ip)->bias[1] = *bp;
  return MSG_OK;
}

static msg_t hygro_reset_bias(void *ip) {
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "hygro_reset_bias(), invalid state");

  return hts221Calibrate(ip, HTS221_FLAG_HYGRO_BIAS);
}

static msg_t thermo_reset_bias(void *ip) {
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_reset_bias(), invalid state");

  return hts221Calibrate(ip, HTS221_FLAG_THERMO_BIAS);
}

static msg_t hygro_set_sensitivity(void *ip, float *sp) {
  osalDbgCheck((ip != NULL) && (sp != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_set_sensitivity(), invalid state");

  ((HTS221Driver *)ip)->sensitivity[0] = *sp;
  return MSG_OK;
}

static msg_t thermo_set_sensitivity(void *ip, float *sp) {
  osalDbgCheck((ip != NULL) && (sp != NULL));

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_set_sensitivity(), invalid state");

  return hts221Calibrate(ip, HTS221_FLAG_THERMO_SENS);
}

static msg_t hygro_reset_sensitivity(void *ip) {
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "hygro_reset_sensitivity(), invalid state");

  return hts221Calibrate(ip, HTS221_FLAG_HYGRO_SENS);
}

static msg_t thermo_reset_sensitivity(void *ip) {
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((HTS221Driver *)ip)->state == HTS221_READY) ||
                (((HTS221Driver *)ip)->state == HTS221_STOP),
                "thermo_reset_sensitivity(), invalid state");

  ((HTS221Driver *)ip)->sensitivity[1] = HTS221_THERMO_SENS;
  return MSG_OK;
}


static const struct BaseSensorVMT vmt_basesensor = {
  sens_get_axes_number, sens_read_raw, sens_read_cooked
};

static const struct BaseHygrometerVMT vmt_basehygrometer = {
  hygro_get_axes_number, hygro_read_raw, hygro_read_cooked,
  hygro_set_bias, hygro_reset_bias,
  hygro_set_sensitivity, hygro_reset_sensitivity
};

static const struct BaseThermometerVMT vmt_basethermometer = {
  thermo_get_axes_number, thermo_read_raw, thermo_read_cooked,
  thermo_set_bias, thermo_reset_bias,
  thermo_set_sensitivity, thermo_reset_sensitivity
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p HTS221Driver object
 *
 * @init
 */
void hts221ObjectInit(HTS221Driver *devp) {

  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_basehygrometer = &vmt_basehygrometer;
  devp->vmt_basethermometer = &vmt_basethermometer;
  devp->config = NULL;
  devp->state  = HTS221_STOP;
  devp->bias[0] = 0.0;
  devp->bias[1] = 0.0;
  devp->sensitivity[0] = 0.0;
  devp->sensitivity[1] = 0.0;
}

/**
 * @brief   Configures and activates HTS221 Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p HTS221Driver object
 * @param[in] config    pointer to the @p HTS221Config object
 *
 * @api
 */
void hts221Start(HTS221Driver *devp, const HTS221Config *config) {
  uint8_t cr[2];
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == HTS221_STOP) || (devp->state == HTS221_READY),
              "hts221Start(), invalid state");        
  devp->config = config;
  
#if HTS221_USE_I2C

  /* Control register 1 configuration block.*/
  {
    cr[0] = HTS221_AD_CTRL_REG1;
    cr[1] = devp->config->outputdatarate | HTS221_CTRL_REG1_PD;
#if HTS221_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] |= devp->config->blockdataupdate;
#endif

#if  HTS221_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
#endif /* HTS221_SHARED_I2C */
  i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
           
  hts221I2CWriteRegister(devp->config->i2cp, cr, 1);
  
#if  HTS221_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* HTS221_SHARED_I2C */
  }

  /* Average register configuration block.*/
  {
    cr[0] = HTS221_AD_AV_CONF;
    cr[1] = 0x05;
#if HTS221_USE_ADVANCED || defined(__DOXYGEN__)
    cr[1] = devp->config->reshumidity | devp->config->restemperature;
#endif
#if  HTS221_SHARED_I2C
    i2cAcquireBus((devp)->config->i2cp);
    i2cStart((devp)->config->i2cp,
           (devp)->config->i2ccfg);
#endif /* HTS221_SHARED_I2C */
           
    hts221I2CWriteRegister(devp->config->i2cp, cr, 1);
  
#if  HTS221_SHARED_I2C
    i2cReleaseBus((devp)->config->i2cp);
#endif /* HTS221_SHARED_I2C */
  }  
#endif /* HTS221_USE_I2C */

  if(devp->config->sensitivity == NULL) {
    hts221Calibrate(devp, HTS221_FLAG_HYGRO_SENS | HTS221_FLAG_THERMO_SENS);
  }
  else{
    /* Taking Sensitivity from user configurations */
    devp->sensitivity[0] = devp->config->sensitivity[0];
    devp->sensitivity[1] = devp->config->sensitivity[1];
  }

  if(devp->config->bias == NULL) {
    hts221Calibrate(devp, HTS221_FLAG_HYGRO_BIAS | HTS221_FLAG_THERMO_BIAS);
  }
  else {
    /* Taking Bias from user configurations */
    devp->bias[0] = devp->config->bias[0];
    devp->bias[1] = devp->config->bias[1];
  }

  /* This is the MEMS transient recovery time */
  osalThreadSleepMilliseconds(5);

  devp->state = HTS221_READY;
} 

/**
 * @brief   Deactivates the HTS221 Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p HTS221Driver object
 *
 * @api
 */
void hts221Stop(HTS221Driver *devp) {
  uint8_t cr[2];

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == HTS221_STOP) || (devp->state == HTS221_READY),
                "hts221Stop(), invalid state");
                
  if (devp->state == HTS221_READY) {
#if (HTS221_USE_I2C)
#if  HTS221_SHARED_I2C
  i2cAcquireBus((devp)->config->i2cp);
  i2cStart((devp)->config->i2cp, (devp)->config->i2ccfg);
#endif /* HTS221_SHARED_I2C */

  cr[0] = HTS221_AD_CTRL_REG1;
  cr[1] = 0;
  hts221I2CWriteRegister(devp->config->i2cp, cr, 1);
  
  i2cStop((devp)->config->i2cp);
#if  HTS221_SHARED_I2C
  i2cReleaseBus((devp)->config->i2cp);
#endif /* HTS221_SHARED_I2C */           
#endif /* HTS221_USE_I2C */
  } 
  devp->state = HTS221_STOP;
}
/** @} */
