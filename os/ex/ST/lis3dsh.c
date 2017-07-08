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
 * @file    lis3dsh.c
 * @brief   LIS3DSH MEMS interface module code.
 *
 * @addtogroup LIS3DSH
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "lis3dsh.h"

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

#if (LIS3DSH_USE_SPI) || defined(__DOXYGEN__)
/**
 * @brief   Reads a generic register value using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 * @note    Multiple write/read requires proper settings in CTRL_REG6.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       starting register address
 * @param[in] n         number of adjacent registers to write
 * @param[in] b         pointer to a buffer.
 */
static void lis3dshSPIReadRegister(SPIDriver *spip, uint8_t reg,  size_t n,
                                   uint8_t* b) {
  uint8_t cmd;
  cmd = reg | LIS3DSH_RW;
  spiSelect(spip);
  spiSend(spip, 1, &cmd);
  spiReceive(spip, n, b);
  spiUnselect(spip);
}

/**
 * @brief   Writes a value into a generic register using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 * @note    Multiple write/read requires proper settings in CTRL_REG6.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       starting register address
 * @param[in] n         number of adjacent registers to write
 * @param[in] b         pointer to a buffer of values.
 */
static void lis3dshSPIWriteRegister(SPIDriver *spip, uint8_t reg, size_t n,
                                    uint8_t* b) {
  uint8_t cmd;
  cmd = reg;
  spiSelect(spip);
  spiSend(spip, 1, &cmd);
  spiSend(spip, n, b);
  spiUnselect(spip);
}
#endif /* LIS3DSH_USE_SPI */

/*
 * Interface implementation.
 */
static size_t get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return LIS3DSH_NUMBER_OF_AXES;
}

static msg_t read_raw(void *ip, int32_t axes[LIS3DSH_NUMBER_OF_AXES]) {
  uint8_t buff [LIS3DSH_NUMBER_OF_AXES * 2], i;
  int16_t tmp;
  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY),
              "read_raw(), invalid state");

#if LIS3DSH_USE_SPI
  osalDbgAssert((((LIS3DSHDriver *)ip)->config->spip->state == SPI_READY),
                "read_raw(), channel not ready");

#if	LIS3DSH_SHARED_SPI
  spiAcquireBus(((LIS3DSHDriver *)ip)->config->spip);
  spiStart(((LIS3DSHDriver *)ip)->config->spip,
           ((LIS3DSHDriver *)ip)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */

    lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip, LIS3DSH_AD_OUT_X_L,
                          LIS3DSH_NUMBER_OF_AXES * 2, buff);

#if	LIS3DSH_SHARED_SPI
  spiReleaseBus(((LIS3DSHDriver *)ip)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */   
#endif /* LIS3DSH_USE_SPI */

  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
    tmp = buff[2*i] + (buff[2*i+1] << 8);
    axes[i] = (int32_t)tmp;
  }
  return MSG_OK;
}


static msg_t read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[LIS3DSH_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY),
                "read_cooked(), invalid state");

  msg = read_raw(ip, raw);
  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES ; i++){
    axes[i] = (raw[i] * ((LIS3DSHDriver *)ip)->sensitivity[i]);
    axes[i] -= ((LIS3DSHDriver *)ip)->bias[i];
  }
  return msg;
}

static msg_t set_bias(void *ip, float *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY) ||
                (((LIS3DSHDriver *)ip)->state == LIS3DSH_STOP),
                "set_bias(), invalid state");
  
  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
    ((LIS3DSHDriver *)ip)->bias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY) ||
                (((LIS3DSHDriver *)ip)->state == LIS3DSH_STOP),
              "reset_bias(), invalid state");

  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
    ((LIS3DSHDriver *)ip)->bias[i] = 0;
  return MSG_OK;
}

static msg_t set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY),
                "set_sensivity(), invalid state");
  
  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
    ((LIS3DSHDriver *)ip)->sensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((LIS3DSHDriver *)ip)->state == LIS3DSH_READY),
                "reset_sensivity(), invalid state");

  if(((LIS3DSHDriver *)ip)->config->fullscale == LIS3DSH_FS_2G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      ((LIS3DSHDriver *)ip)->sensitivity[i] = LIS3DSH_SENS_2G;
  else if(((LIS3DSHDriver *)ip)->config->fullscale == LIS3DSH_FS_4G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      ((LIS3DSHDriver *)ip)->sensitivity[i] = LIS3DSH_SENS_4G;
  else if(((LIS3DSHDriver *)ip)->config->fullscale == LIS3DSH_FS_6G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      ((LIS3DSHDriver *)ip)->sensitivity[i] = LIS3DSH_SENS_6G;
  else if(((LIS3DSHDriver *)ip)->config->fullscale == LIS3DSH_FS_8G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      ((LIS3DSHDriver *)ip)->sensitivity[i] = LIS3DSH_SENS_8G;
  else if(((LIS3DSHDriver *)ip)->config->fullscale == LIS3DSH_FS_16G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      ((LIS3DSHDriver *)ip)->sensitivity[i] = LIS3DSH_SENS_16G;
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), accelerometer full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t set_full_scale(void *ip, lis3dsh_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr;

  if(fs == LIS3DSH_FS_2G) {
    newfs = LIS3DSH_2G;
  }
  else if(fs == LIS3DSH_FS_4G) {
    newfs = LIS3DSH_4G;
  }
  else if(fs == LIS3DSH_FS_6G) {
    newfs = LIS3DSH_6G;
  }
  else if(fs == LIS3DSH_FS_8G) {
    newfs = LIS3DSH_8G;
  }
  else if(fs == LIS3DSH_FS_16G) {
    newfs = LIS3DSH_16G;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((LIS3DSHDriver *)ip)->fullscale) {
    scale = newfs / ((LIS3DSHDriver *)ip)->fullscale;
    ((LIS3DSHDriver *)ip)->fullscale = newfs;

#if LIS3DSH_USE_SPI
#if LIS3DSH_SHARED_SPI
  spiAcquireBus(((LIS3DSHDriver *)ip)->config->spip);
  spiStart(((LIS3DSHDriver *)ip)->config->spip,
           ((LIS3DSHDriver *)ip)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */
    lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                          LIS3DSH_AD_CTRL_REG5, 1, &cr);
#if LIS3DSH_SHARED_SPI
  spiReleaseBus(((LIS3DSHDriver *)ip)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
#endif /* LIS3DSH_USE_SPI */

    cr &= ~(LIS3DSH_CTRL_REG5_FS_MASK);
    cr |= fs;

#if LIS3DSH_USE_SPI
#if LIS3DSH_SHARED_SPI
  spiAcquireBus(((LIS3DSHDriver *)ip)->config->spip);
  spiStart(((LIS3DSHDriver *)ip)->config->spip,
           ((LIS3DSHDriver *)ip)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */
    lis3dshSPIWriteRegister(((LIS3DSHDriver *)ip)->config->spip,
                           LIS3DSH_AD_CTRL_REG5, 1, &cr);
#if LIS3DSH_SHARED_SPI
  spiReleaseBus(((LIS3DSHDriver *)ip)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
#endif /* LIS3DSH_USE_SPI */

    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
      ((LIS3DSHDriver *)ip)->sensitivity[i] *= scale;
      ((LIS3DSHDriver *)ip)->bias[i] *= scale;
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

static const struct LIS3DSHVMT vmt_lis3dsh = {
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
 * @param[out] devp     pointer to the @p LIS3DSHDriver object
 *
 * @init
 */
void lis3dshObjectInit(LIS3DSHDriver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_baseaccelerometer = &vmt_baseaccelerometer;
  devp->vmt_lis3dsh = &vmt_lis3dsh;
  devp->config = NULL;
  for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
    devp->bias[i] = 0.0f;
  devp->state  = LIS3DSH_STOP;
}

/**
 * @brief   Configures and activates LIS3DSH Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p LIS3DSHDriver object
 * @param[in] config    pointer to the @p LIS3DSHConfig object
 *
 * @api
 */
void lis3dshStart(LIS3DSHDriver *devp, const LIS3DSHConfig *config) {
  uint32_t i;
  osalDbgCheck((devp != NULL) && (config != NULL));
  uint8_t cr;
  osalDbgAssert((devp->state == LIS3DSH_STOP) || (devp->state == LIS3DSH_READY),
              "lis3dshStart(), invalid state");			  

  devp->config = config;
  
  /* Control register 4 configuration block.*/
  {
    cr = LIS3DSH_CTRL_REG4_XEN | LIS3DSH_CTRL_REG4_YEN | LIS3DSH_CTRL_REG4_ZEN |
         devp->config->outputdatarate;
#if LIS3DSH_USE_ADVANCED || defined(__DOXYGEN__)
    cr |= devp->config->blockdataupdate;
#endif
  }

#if LIS3DSH_USE_SPI
#if LIS3DSH_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
  spiStart((devp)->config->spip, (devp)->config->spicfg);

  lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG4,
                          1, &cr);
  
#if LIS3DSH_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
#endif /* LIS3DSH_USE_SPI */

  /* Control register 5 configuration block.*/
  {
    cr = devp->config->fullscale;
#if LIS3DSH_USE_ADVANCED || defined(__DOXYGEN__)
    cr |= devp->config->antialiasing;
#endif
  }

#if LIS3DSH_USE_SPI
#if LIS3DSH_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
  spiStart((devp)->config->spip, (devp)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */

  lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG5,
                          1, &cr);

#if LIS3DSH_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
#endif /* LIS3DSH_USE_SPI */

  /* Control register 6 configuration block.*/
  {
    cr = LIS3DSH_CTRL_REG6_ADD_INC;
#if LIS3DSH_USE_ADVANCED || defined(__DOXYGEN__)
    cr |= devp->config->blockdataupdate;
#endif
  }

#if LIS3DSH_USE_SPI
#if LIS3DSH_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
  spiStart((devp)->config->spip, (devp)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */

  lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG6,
                          1, &cr);

#if	LIS3DSH_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */  
#endif /* LIS3DSH_USE_SPI */
  
  /* Storing sensitivity information according to user setting */
  if(devp->config->fullscale == LIS3DSH_FS_2G) {
    devp->fullscale = LIS3DSH_2G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS3DSH_SENS_2G;
    else
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else if(devp->config->fullscale == LIS3DSH_FS_4G) {
    devp->fullscale = LIS3DSH_4G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS3DSH_SENS_4G;
    else
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else if(devp->config->fullscale == LIS3DSH_FS_6G) {
    devp->fullscale = LIS3DSH_6G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS3DSH_SENS_6G;
    else
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else if(devp->config->fullscale == LIS3DSH_FS_8G) {
    devp->fullscale = LIS3DSH_8G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS3DSH_SENS_8G;
    else
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else if(devp->config->fullscale == LIS3DSH_FS_16G) {
    devp->fullscale = LIS3DSH_16G;
    if(devp->config->sensitivity == NULL)
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = LIS3DSH_SENS_16G;
    else
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
        devp->sensitivity[i] = devp->config->sensitivity[i];
  }
  else {
    osalDbgAssert(FALSE, "lis3dshStart(), accelerometer full scale issue");
  }

  /* Storing bias information according to user setting */
  if(devp->config->bias != NULL)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->bias[i] = devp->config->bias[i];

  /* This is the Accelerometer transient recovery time */
  osalThreadSleepMilliseconds(10);

  devp->state = LIS3DSH_READY;
} 

/**
 * @brief   Deactivates the LIS3DSH Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p LIS3DSHDriver object
 *
 * @api
 */
void lis3dshStop(LIS3DSHDriver *devp) {
  uint8_t cr4;
  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS3DSH_STOP) || (devp->state == LIS3DSH_READY),
                "lis3dshStop(), invalid state");

  if (devp->state == LIS3DSH_READY) {
#if (LIS3DSH_USE_SPI)
#if	LIS3DSH_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */
    /* Disabling all axes and enabling power down mode.*/
    cr4 = 0;
    lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG4, 
                           1, &cr4);
    spiStop((devp)->config->spip);
#if	LIS3DSH_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */    		  
#endif /* LIS3DSH_USE_SPI */
  }	
  devp->state = LIS3DSH_STOP;
}
/** @} */
