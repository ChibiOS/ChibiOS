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
 * @addtogroup lis302dl
 * @{
 */

#include "hal.h"

#include "lis302dl.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  LIS302DL_SENS_2G                   ((float)18.0f)
#define  LIS302DL_SENS_8G                   ((float)72.0f)

#define  LIS302DL_DI                        ((uint8_t)0xFF)
#define  LIS302DL_DI_0                      ((uint8_t)0x01)
#define  LIS302DL_DI_1                      ((uint8_t)0x02)
#define  LIS302DL_DI_2                      ((uint8_t)0x04)
#define  LIS302DL_DI_3                      ((uint8_t)0x08)
#define  LIS302DL_DI_4                      ((uint8_t)0x10)
#define  LIS302DL_DI_5                      ((uint8_t)0x20)
#define  LIS302DL_DI_6                      ((uint8_t)0x40)
#define  LIS302DL_DI_7                      ((uint8_t)0x80)

#define  LIS302DL_AD                        ((uint8_t)0x3F)
#define  LIS302DL_AD_0                      ((uint8_t)0x01)
#define  LIS302DL_AD_1                      ((uint8_t)0x02)
#define  LIS302DL_AD_2                      ((uint8_t)0x04)
#define  LIS302DL_AD_3                      ((uint8_t)0x08)
#define  LIS302DL_AD_4                      ((uint8_t)0x10)
#define  LIS302DL_AD_5                      ((uint8_t)0x20)

#define  LIS302DL_MS                        ((uint8_t)0x40)
#define  LIS302DL_RW                        ((uint8_t)0x80)

#define  LIS302DL_AD_WHO_AM_I               ((uint8_t)0x0F)
#define  LIS302DL_AD_CTRL_REG1              ((uint8_t)0x20)
#define  LIS302DL_AD_CTRL_REG2              ((uint8_t)0x21)
#define  LIS302DL_AD_CTRL_REG3              ((uint8_t)0x22)
#define  LIS302DL_AD_HP_FILER_RESET         ((uint8_t)0x23)
#define  LIS302DL_AD_STATUS_REG             ((uint8_t)0x27)
#define  LIS302DL_AD_OUT_X                  ((uint8_t)0x29)
#define  LIS302DL_AD_OUT_Y                  ((uint8_t)0x2B)
#define  LIS302DL_AD_OUT_Z                  ((uint8_t)0x2D)
#define  LIS302DL_AD_FF_WU_CFG_1            ((uint8_t)0x30)
#define  LIS302DL_AD_FF_WU_SRC_1            ((uint8_t)0x31)
#define  LIS302DL_AD_FF_WU_THS_1            ((uint8_t)0x32)
#define  LIS302DL_AD_FF_WU_DURATION_1       ((uint8_t)0x33)
#define  LIS302DL_AD_FF_WU_CFG_2            ((uint8_t)0x34)
#define  LIS302DL_AD_FF_WU_SRC_2            ((uint8_t)0x35)
#define  LIS302DL_AD_FF_WU_THS_2            ((uint8_t)0x36)
#define  LIS302DL_AD_FF_WU_DURATION_2       ((uint8_t)0x37)
#define  LIS302DL_AD_CLICK_CFG              ((uint8_t)0x38)
#define  LIS302DL_AD_CLICK_SRC              ((uint8_t)0x39)
#define  LIS302DL_AD_CLICK_THSY_X           ((uint8_t)0x3B)
#define  LIS302DL_AD_CLICK_THSZ             ((uint8_t)0x3C)
#define  LIS302DL_AD_CLICK_TIME_LIMIT       ((uint8_t)0x3D)
#define  LIS302DL_AD_CLICK_LATENCY          ((uint8_t)0x3E)
#define  LIS302DL_AD_CLICK_WINDOW           ((uint8_t)0x3F)


#define  LIS302DL_CTRL_REG1_FS_MASK         ((uint8_t)0x20)
#define  TO_G                               ((float)0.001f)
#define  TO_SI                              ((float)0.00981f)
/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief  LIS302DL Power Mode
 */
typedef enum {
  LIS302DL_PM_POWER_DOWN   = 0x00,  /**< Power down enabled.                */          
  LIS302DL_PM_SLEEP_NORMAL = 0x40   /**< Normal operation mode.             */          
}lis302dl_pm_t;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (LIS302DL_USE_SPI) || defined(__DOXYGEN__)
/**
 * @brief   Reads a generic register value using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       register number
 * @return              register value.
 */
static uint8_t lis302dlSPIReadRegister(SPIDriver *spip, uint8_t reg) {
  uint8_t txbuf[2] = {LIS302DL_RW | reg, 0xFF};
  uint8_t rxbuf[2] = {0x00, 0x00};
  spiSelect(spip);
  spiExchange(spip, 2, txbuf, rxbuf);
  spiUnselect(spip);
  return rxbuf[1];
}

/**
 * @brief   Writes a value into a generic register using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       register number
 * @param[in] value     register value.
 */
static void lis302dlSPIWriteRegister(SPIDriver *spip, uint8_t reg,
                                   uint8_t value) {

  switch (reg) {
    default:
      /* Reserved register must not be written, according to the datasheet
       * this could permanently damage the device.
       */
      osalDbgAssert(FALSE, "lis302dlSPIWriteRegister(), reserved register");
    case LIS302DL_AD_WHO_AM_I:

    case LIS302DL_AD_HP_FILER_RESET:
    case LIS302DL_AD_STATUS_REG:
    case LIS302DL_AD_OUT_X:
    case LIS302DL_AD_OUT_Y:
    case LIS302DL_AD_OUT_Z:
    case LIS302DL_AD_FF_WU_SRC_1:
    case LIS302DL_AD_FF_WU_SRC_2:
    case LIS302DL_AD_CLICK_SRC:
    /* Read only registers cannot be written, the command is ignored.*/
      return;
    case LIS302DL_AD_CTRL_REG1:
    case LIS302DL_AD_CTRL_REG2:
    case LIS302DL_AD_CTRL_REG3:
    case LIS302DL_AD_FF_WU_CFG_1:
    case LIS302DL_AD_FF_WU_THS_1:
    case LIS302DL_AD_FF_WU_DURATION_1:
    case LIS302DL_AD_FF_WU_CFG_2:
    case LIS302DL_AD_FF_WU_THS_2:
    case LIS302DL_AD_FF_WU_DURATION_2:
    case LIS302DL_AD_CLICK_CFG:
    case LIS302DL_AD_CLICK_THSY_X:
    case LIS302DL_AD_CLICK_THSZ:
    case LIS302DL_AD_CLICK_TIME_LIMIT:
    case LIS302DL_AD_CLICK_LATENCY:
    case LIS302DL_AD_CLICK_WINDOW:
      spiSelect(spip);
      uint8_t txbuf[2] = {reg, value};
      spiSend(spip, 2, txbuf);
      spiUnselect(spip);
  }
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
  int8_t tmp;
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
  if(((LIS302DLDriver *)ip)->config->axesenabling & LIS302DL_AE_X){
    tmp = lis302dlSPIReadRegister(((LIS302DLDriver *)ip)->config->spip,
										LIS302DL_AD_OUT_X);
    axes[0] = tmp + ((LIS302DLDriver *)ip)->bias[0];
  }
  if(((LIS302DLDriver *)ip)->config->axesenabling & LIS302DL_AE_Y){
    tmp = lis302dlSPIReadRegister(((LIS302DLDriver *)ip)->config->spip,
										LIS302DL_AD_OUT_Y);
    axes[1] = tmp  + ((LIS302DLDriver *)ip)->bias[1];
  }
  if(((LIS302DLDriver *)ip)->config->axesenabling & LIS302DL_AE_Z){
    tmp = lis302dlSPIReadRegister(((LIS302DLDriver *)ip)->config->spip,
										LIS302DL_AD_OUT_Z);
    axes[2] = tmp + ((LIS302DLDriver *)ip)->bias[2];
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
    axes[i] = raw[i] * ((LIS302DLDriver *)ip)->sensitivity[i];
    if(((LIS302DLDriver *)ip)->config->unit == LIS302DL_ACC_UNIT_G){
      axes[i] *= TO_G;
    }
    else if(((LIS302DLDriver *)ip)->config->unit == LIS302DL_ACC_UNIT_SI){
      axes[i] *= TO_SI;
    }
  }
  return msg;
}

static msg_t set_bias(void *ip, int32_t *bp) {
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
    osalDbgAssert(FALSE, "reset_sensivity(), accelerometer full scale issue");
    return MSG_RESET;
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
  set_bias, reset_bias, set_sensivity, reset_sensivity 
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
    devp->bias[i] = 0;
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
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == LIS302DL_STOP) || (devp->state == LIS302DL_READY),
              "lis302dlStart(), invalid state");			  

  devp->config = config;
  
#if LIS302DL_USE_SPI
#if	LIS302DL_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */
  spiStart((devp)->config->spip,
           (devp)->config->spicfg);
  lis302dlSPIWriteRegister(devp->config->spip, LIS302DL_AD_CTRL_REG1,
                         LIS302DL_PM_SLEEP_NORMAL |
                         devp->config->axesenabling |
						 devp->config->fullscale |
						 devp->config->outputdatarate);
  lis302dlSPIWriteRegister(devp->config->spip, LIS302DL_AD_CTRL_REG2,
                         devp->config->highpass);
#if	LIS302DL_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */  
#endif /* LIS302DL_USE_SPI */
  
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == LIS302DL_FS_2G)
    for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS302DL_SENS_2G;
  else if(devp->config->fullscale == LIS302DL_FS_8G)
	for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS302DL_SENS_8G;
  else {
    osalDbgAssert(FALSE, "lis302dlStart(), accelerometer full scale issue");
  }
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

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS302DL_STOP) || (devp->state == LIS302DL_READY),
                "lis302dlStop(), invalid state");

#if (LIS302DL_USE_SPI)
  if (devp->state == LIS302DL_STOP) {
#if	LIS302DL_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* LIS302DL_SHARED_SPI */
    lis302dlSPIWriteRegister(devp->config->spip, LIS302DL_AD_CTRL_REG1,
                           LIS302DL_PM_POWER_DOWN | LIS302DL_AE_DISABLED);
    spiStop((devp)->config->spip);
#if	LIS302DL_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* LIS302DL_SHARED_SPI */    
  }			  
#endif /* LIS302DL_USE_SPI */
  devp->state = LIS302DL_STOP;
}
/** @} */
