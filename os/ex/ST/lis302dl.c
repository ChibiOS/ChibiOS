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
 * @file    lis302dl.c
 * @brief   LIS302DL MEMS interface module code.
 *
 * @addtogroup LIS302DL
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "lis302dl.h"

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

#if (LIS302DL_USE_SPI) || defined(__DOXYGEN__)
/**
 * @brief   Reads a generic register value using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       starting register address
 * @param[in] n         number of adjacent registers to write
 * @param[in] b         pointer to a buffer.
 */
static void lis302dlSPIReadRegister(SPIDriver *spip, uint8_t reg,  size_t n,
                                    uint8_t* b) {
  uint8_t cmd;
  (n == 1) ? (cmd = reg | LIS302DL_RW) : (cmd = reg | LIS302DL_RW | LIS302DL_MS);
  spiSelect(spip);
  spiSend(spip, 1, &cmd);
  spiReceive(spip, n, b);
  spiUnselect(spip);
}

/**
 * @brief   Writes a value into a generic register using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       starting register address
 * @param[in] n         number of adjacent registers to write
 * @param[in] b         pointer to a buffer of values.
 */
static void lis302dlSPIWriteRegister(SPIDriver *spip, uint8_t reg, size_t n,
                                     uint8_t* b) {
  uint8_t cmd;
  (n == 1) ? (cmd = reg) : (cmd = reg | LIS302DL_MS);
  spiSelect(spip);
  spiSend(spip, 1, &cmd);
  spiSend(spip, n, b);
  spiUnselect(spip);
}
#endif /* LIS302DL_USE_SPI */

/*
 * Interface implementation.
 */
static size_t get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LIS302DL_NUMBER_OF_AXES;
}

static msg_t read_raw(void *ip, int32_t axes[LIS302DL_NUMBER_OF_AXES]) {
  uint8_t i, tmp;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY),
              "read_raw(), invalid state");

#if LIS302DL_USE_SPI
  osalDbgAssert((((LIS302DLDriver *)ip)->config->spip->state == SPI_READY),
                "read_raw(), channel not ready");

#if	LIS302DL_SHARED_SPI
  spiAcquireBus(((LIS302DLDriver *)ip)->config->spip);
  spiStart(((LIS302DLDriver *)ip)->config->spip,
          ((LIS302DLDriver *)ip)->config->spicfg);
#endif /* LIS302DL_SHARED_SPI */

    for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
      lis302dlSPIReadRegister(((LIS302DLDriver *)ip)->config->spip,
                              LIS302DL_AD_OUT_X + (i * 2), 1, &tmp);
      axes[i] = (int32_t)((int8_t)tmp);
    }

#if	LIS302DL_SHARED_SPI
  spiReleaseBus(((LIS302DLDriver *)ip)->config->spip);
#endif /* LIS302DL_SHARED_SPI */

#endif /* LIS302DL_USE_SPI */ 
  return MSG_OK;
}

static msg_t read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LIS302DL_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY),
                "read_cooked(), invalid state");

  msg = read_raw(ip, raw);
  for(i = 0; i < LIS302DL_NUMBER_OF_AXES ; i++){
    axes[i] = (raw[i] * ((LIS302DLDriver *)ip)->sensitivity[i]);
    axes[i] -= ((LIS302DLDriver *)ip)->bias[i];
  }
  return msg;
}

static msg_t set_bias(void *ip, float *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY) ||
                (((LIS302DLDriver *)ip)->state == LIS302DL_STOP),
                "set_bias(), invalid state");
  
  for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
    ((LIS302DLDriver *)ip)->bias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY) ||
                (((LIS302DLDriver *)ip)->state == LIS302DL_STOP),
              "reset_bias(), invalid state");

  for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
    ((LIS302DLDriver *)ip)->bias[i] = 0;
  return MSG_OK;
}

static msg_t set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY),
                "set_sensivity(), invalid state");
  
  for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
    ((LIS302DLDriver *)ip)->sensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS302DLDriver *)ip)->state == LIS302DL_READY),
                "reset_sensivity(), invalid state");

  if(((LIS302DLDriver *)ip)->config->fullscale == LIS302DL_FS_2G)
    for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
      ((LIS302DLDriver *)ip)->sensitivity[i] = LIS302DL_SENS_2G;
  else if(((LIS302DLDriver *)ip)->config->fullscale == LIS302DL_FS_8G)
	for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
      ((LIS302DLDriver *)ip)->sensitivity[i] = LIS302DL_SENS_8G;
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t set_full_scale(void *ip, lis302dl_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr;

  if(fs == LIS302DL_FS_2G) {
    newfs = LIS302DL_2G;
  }
  else if(fs == LIS302DL_FS_8G) {
    newfs = LIS302DL_8G;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((LIS302DLDriver *)ip)->fullscale) {
    scale = newfs / ((LIS302DLDriver *)ip)->fullscale;
    ((LIS302DLDriver *)ip)->fullscale = newfs;

#if LIS302DL_USE_SPI
#if LIS302DL_SHARED_SPI
  spiAcquireBus(((LIS302DLDriver *)ip)->config->spip);
  spiStart(((LIS302DLDriver *)ip)->config->spip,
          ((LIS302DLDriver *)ip)->config->spicfg);
#endif /* LIS302DL_SHARED_SPI */
    lis302dlSPIReadRegister(((LIS302DLDriver *)ip)->config->spip,
                            LIS302DL_AD_CTRL_REG1, 1, &cr);
#if LIS302DL_SHARED_SPI
  spiReleaseBus(((LIS302DLDriver *)ip)->config->spip);
#endif /* LIS302DL_SHARED_SPI */
#endif /* LIS302DL_USE_SPI */

    cr &= ~(LIS302DL_CTRL_REG1_FS_MASK);
    cr |= fs;

#if LIS302DL_USE_SPI
#if LIS302DL_SHARED_SPI
  spiAcquireBus(((LIS302DLDriver *)ip)->config->spip);
  spiStart(((LIS302DLDriver *)ip)->config->spip,
          ((LIS302DLDriver *)ip)->config->spicfg);
#endif /* LIS302DL_SHARED_SPI */
    lis302dlSPIWriteRegister(((LIS302DLDriver *)ip)->config->spip,
                             LIS302DL_AD_CTRL_REG1, 1, &cr);
#if LIS302DL_SHARED_SPI
  spiReleaseBus(((LIS302DLDriver *)ip)->config->spip);
#endif /* LIS302DL_SHARED_SPI */
#endif /* LIS302DL_USE_SPI */

    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
      ((LIS302DLDriver *)ip)->sensitivity[i] *= scale;
      ((LIS302DLDriver *)ip)->bias[i] *= scale;
    }
  }
  return MSG_OK;
}

static const struct BaseSensorVMT vmt_basesensor = {
  get_axes_number, read_raw, read_cooked
};

static const struct BaseAccelerometerVMT vmt_baseaccelerometer = {
  get_axes_number, read_raw, read_cooked,
  set_bias, reset_bias, set_sensivity, reset_sensivity 
};

static const struct LIS302DLVMT vmt_lis302dl = {
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
 * @param[out] devp     pointer to the @p LIS302DLDriver object
 *
 * @init
 */
void lis302dlObjectInit(LIS302DLDriver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_baseaccelerometer = &vmt_baseaccelerometer;
  devp->vmt_lis302dl = &vmt_lis302dl;
  devp->config = NULL;
  for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
    devp->bias[i] = 0.0f;
  devp->state  = LIS302DL_STOP;
}

/**
 * @brief   Configures and activates LIS302DL Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LIS302DLDriver object
 * @param[in] config    pointer to the @p LIS302DLConfig object
 *
 * @api
 */
void lis302dlStart(LIS302DLDriver *devp, const LIS302DLConfig *config) {
  uint32_t i;
  uint8_t cr[2] = {0, 0};
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LIS302DL_STOP) || (devp->state == LIS302DL_READY),
              "lis302dlStart(), invalid state");			  

  devp->config = config;

  /* Control register 1 configuration block.*/
  {
    cr[0] = LIS302DL_CTRL_REG1_XEN | LIS302DL_CTRL_REG1_YEN | 
            LIS302DL_CTRL_REG1_ZEN | LIS302DL_CTRL_REG1_PD |
            devp->config->outputdatarate |
            devp->config->fullscale;
  }
  
  /* Control register 2 configuration block.*/
  {
#if LIS302DL_USE_ADVANCED || defined(__DOXYGEN__)
  if(devp->config->hpmode != LIS302DL_HPM_BYPASSED)
    cr[1] = devp->config->highpass;
#endif
  }

#if LIS302DL_USE_SPI
#if LIS302DL_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */
  spiStart((devp)->config->spip, (devp)->config->spicfg);
  
  lis302dlSPIWriteRegister(devp->config->spip, LIS302DL_AD_CTRL_REG1, 
                           2, cr);
                           
#if	LIS302DL_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */  
#endif /* LIS302DL_USE_SPI */
  
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == LIS302DL_FS_2G) {
    devp->fullscale = LIS302DL_2G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS302DL_SENS_2G;
    else
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else if(devp->config->fullscale == LIS302DL_FS_8G) {
    devp->fullscale = LIS302DL_8G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS302DL_SENS_8G;
    else
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else {
    osalDbgAssert(FALSE, "lis302dlStart(), accelerometer full scale issue");
  }

  if(devp->config->bias != NULL)
    for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
      devp->bias[i] = devp->config->bias[i];

  /* This is the Accelerometer transient recovery time */
  osalThreadSleepMilliseconds(10);

  devp->state = LIS302DL_READY;
} 

/**
 * @brief   Deactivates the LIS302DL Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LIS302DLDriver object
 *
 * @api
 */
void lis302dlStop(LIS302DLDriver *devp) {
  uint8_t cr1;
  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS302DL_STOP) || (devp->state == LIS302DL_READY),
                "lis302dlStop(), invalid state");

  if (devp->state == LIS302DL_READY) {
#if LIS302DL_USE_SPI
#if	LIS302DL_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* LIS302DL_SHARED_SPI */
    cr1 = 0;
    lis302dlSPIWriteRegister(devp->config->spip, LIS302DL_AD_CTRL_REG1, 1, &cr1);
    spiStop((devp)->config->spip);
#if	LIS302DL_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */   
#endif /* LIS302DL_USE_SPI */ 
  }
  devp->state = LIS302DL_STOP;
}
/** @} */
