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
 * @file    l3gd20.c
 * @brief   L3GD20 MEMS interface module code.
 *
 * @addtogroup l3gd20
 * @{
 */

#include "hal.h"
#include "l3gd20.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  L3GD20_SENS_250DPS                 ((float)0.00875f)
#define  L3GD20_SENS_500DPS                 ((float)0.01750f)
#define  L3GD20_SENS_2000DPS                ((float)0.07000f)

#define  L3GD20_DI                          ((uint8_t)0xFF)
#define  L3GD20_DI_0                        ((uint8_t)0x01)
#define  L3GD20_DI_1                        ((uint8_t)0x02)
#define  L3GD20_DI_2                        ((uint8_t)0x04)
#define  L3GD20_DI_3                        ((uint8_t)0x08)
#define  L3GD20_DI_4                        ((uint8_t)0x10)
#define  L3GD20_DI_5                        ((uint8_t)0x20)
#define  L3GD20_DI_6                        ((uint8_t)0x40)
#define  L3GD20_DI_7                        ((uint8_t)0x80)

#define  L3GD20_AD                          ((uint8_t)0x3F)
#define  L3GD20_AD_0                        ((uint8_t)0x01)
#define  L3GD20_AD_1                        ((uint8_t)0x02)
#define  L3GD20_AD_2                        ((uint8_t)0x04)
#define  L3GD20_AD_3                        ((uint8_t)0x08)
#define  L3GD20_AD_4                        ((uint8_t)0x10)
#define  L3GD20_AD_5                        ((uint8_t)0x20)
#define  L3GD20_MS                          ((uint8_t)0x40)
#define  L3GD20_RW                          ((uint8_t)0x80)

#define  L3GD20_AD_WHO_AM_I                 ((uint8_t)0x0F)
#define  L3GD20_AD_CTRL_REG1                ((uint8_t)0x20)
#define  L3GD20_AD_CTRL_REG2                ((uint8_t)0x21)
#define  L3GD20_AD_CTRL_REG3                ((uint8_t)0x22)
#define  L3GD20_AD_CTRL_REG4                ((uint8_t)0x23)
#define  L3GD20_AD_CTRL_REG5                ((uint8_t)0x24)
#define  L3GD20_AD_REFERENCE                ((uint8_t)0x25)
#define  L3GD20_AD_OUT_TEMP                 ((uint8_t)0x26)
#define  L3GD20_AD_STATUS_REG               ((uint8_t)0x27)
#define  L3GD20_AD_OUT_X_L                  ((uint8_t)0x28)
#define  L3GD20_AD_OUT_X_H                  ((uint8_t)0x29)
#define  L3GD20_AD_OUT_Y_L                  ((uint8_t)0x2A)
#define  L3GD20_AD_OUT_Y_H                  ((uint8_t)0x2B)
#define  L3GD20_AD_OUT_Z_L                  ((uint8_t)0x2C)
#define  L3GD20_AD_OUT_Z_H                  ((uint8_t)0x2D)
#define  L3GD20_AD_FIFO_CTRL_REG            ((uint8_t)0x2E)
#define  L3GD20_AD_FIFO_SRC_REG             ((uint8_t)0x2F)
#define  L3GD20_AD_INT1_CFG                 ((uint8_t)0x30)
#define  L3GD20_AD_INT1_SRC                 ((uint8_t)0x31)
#define  L3GD20_AD_INT1_TSH_XH              ((uint8_t)0x32)
#define  L3GD20_AD_INT1_TSH_XL              ((uint8_t)0x33)
#define  L3GD20_AD_INT1_TSH_YH              ((uint8_t)0x34)
#define  L3GD20_AD_INT1_TSH_YL              ((uint8_t)0x35)
#define  L3GD20_AD_INT1_TSH_ZH              ((uint8_t)0x36)
#define  L3GD20_AD_INT1_TSH_ZL              ((uint8_t)0x37)
#define  L3GD20_AD_INT1_DURATION            ((uint8_t)0x38)

#define  L3GD20_CTRL_REG4_FS_MASK           ((uint8_t)0x30)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief  L3GD20 Power Mode
 */
typedef enum {
  L3GD20_PM_POWER_DOWN   = 0x00,    /**< Power down enabled.                */          
  L3GD20_PM_SLEEP_NORMAL = 0x08     /**< Normal operation mode.             */          
}l3gd20_pm_t;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (L3GD20_USE_SPI) || defined(__DOXYGEN__)
/**
 * @brief   Reads a generic register value using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       register number
 * @return              register value.
 */
static uint8_t l3gd20SPIReadRegister(SPIDriver *spip, uint8_t reg) {
  uint8_t txbuf[2] = {L3GD20_RW | reg, 0xFF};
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
static void l3gd20SPIWriteRegister(SPIDriver *spip, uint8_t reg,
                                   uint8_t value) {

  switch (reg) {
    default:
      /* Reserved register must not be written, according to the datasheet
       * this could permanently damage the device.
       */
      osalDbgAssert(FALSE, "l3gd20SPIWriteRegister(), reserved register");
    case L3GD20_AD_WHO_AM_I:
    case L3GD20_AD_OUT_TEMP :
    case L3GD20_AD_STATUS_REG:
    case L3GD20_AD_OUT_X_L:
    case L3GD20_AD_OUT_X_H:
    case L3GD20_AD_OUT_Y_L:
    case L3GD20_AD_OUT_Y_H:
    case L3GD20_AD_OUT_Z_L:
    case L3GD20_AD_OUT_Z_H:
    case L3GD20_AD_FIFO_SRC_REG:
    case L3GD20_AD_INT1_SRC:
    /* Read only registers cannot be written, the command is ignored.*/
      return;
    case L3GD20_AD_CTRL_REG1:
    case L3GD20_AD_CTRL_REG2:
    case L3GD20_AD_CTRL_REG3:
    case L3GD20_AD_CTRL_REG4:
    case L3GD20_AD_CTRL_REG5:
    case L3GD20_AD_REFERENCE:
    case L3GD20_AD_FIFO_CTRL_REG:
    case L3GD20_AD_INT1_CFG:
    case L3GD20_AD_INT1_TSH_XH:
    case L3GD20_AD_INT1_TSH_XL:
    case L3GD20_AD_INT1_TSH_YH:
    case L3GD20_AD_INT1_TSH_YL:
    case L3GD20_AD_INT1_TSH_ZH:
    case L3GD20_AD_INT1_TSH_ZL:
    case L3GD20_AD_INT1_DURATION:
      spiSelect(spip);
      uint8_t txbuf[2] = {reg, value};
      spiSend(spip, 2, txbuf);
      spiUnselect(spip);
  }
}
#endif /* L3GD20_USE_SPI */

/*
 * Interface implementation.
 */
static size_t get_axes_number(void *ip) {

  osalDbgCheck(ip != NULL);
  return L3GD20_NUMBER_OF_AXES;
}

static msg_t read_raw(void *ip, int32_t axes[L3GD20_NUMBER_OF_AXES]) {
  int16_t tmp;
  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
              "read_raw(), invalid state");

#if L3GD20_USE_SPI
  osalDbgAssert((((L3GD20Driver *)ip)->config->spip->state == SPI_READY),
                "read_raw(), channel not ready");
#if	L3GD20_SHARED_SPI
  spiAcquireBus(((L3GD20Driver *)ip)->config->spip);
  spiStart(((L3GD20Driver *)ip)->config->spip,
           ((L3GD20Driver *)ip)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */   
  if(((L3GD20Driver *)ip)->config->axesenabling & L3GD20_AE_X){
    tmp = l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                  L3GD20_AD_OUT_X_L);
    tmp += l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                   L3GD20_AD_OUT_X_H) << 8;
    axes[0] = (int32_t)tmp + ((L3GD20Driver *)ip)->bias[0];
  }
  if(((L3GD20Driver *)ip)->config->axesenabling & L3GD20_AE_Y){
    tmp = l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                  L3GD20_AD_OUT_Y_L);
    tmp += l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                   L3GD20_AD_OUT_Y_H) << 8;
    axes[1] = (int32_t)tmp + ((L3GD20Driver *)ip)->bias[1];
  }
  if(((L3GD20Driver *)ip)->config->axesenabling & L3GD20_AE_Z){
    tmp = l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                  L3GD20_AD_OUT_Z_L);
    tmp += l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                   L3GD20_AD_OUT_Z_H) << 8;
    axes[2] = (int32_t)tmp + ((L3GD20Driver *)ip)->bias[2];
  }
#if	L3GD20_SHARED_SPI
  spiReleaseBus(((L3GD20Driver *)ip)->config->spip);
#endif /* L3GD20_SHARED_SPI */   
#endif /* L3GD20_USE_SPI */ 
  return MSG_OK;
}

static msg_t read_cooked(void *ip, float axes[]) {
  uint32_t i;
  int32_t raw[L3GD20_NUMBER_OF_AXES];
  msg_t msg;

  osalDbgCheck((ip != NULL) && (axes != NULL));

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
              "read_cooked(), invalid state");

  msg = read_raw(ip, raw);
  for(i = 0; i < L3GD20_NUMBER_OF_AXES ; i++){
    axes[i] = raw[i] * ((L3GD20Driver *)ip)->sensitivity[i];
  }
  return msg;
}

static msg_t sample_bias(void *ip) {
  uint32_t i, j;
  int32_t raw[L3GD20_NUMBER_OF_AXES];
  int32_t buff[L3GD20_NUMBER_OF_AXES] = {0, 0, 0};

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
                "sample_bias(), invalid state");

  for(i = 0; i < L3GD20_BIAS_ACQ_TIMES; i++){
    read_raw(ip, raw);
    for(j = 0; j < L3GD20_NUMBER_OF_AXES; j++){
      buff[j] += raw[j];
    }
    osalThreadSleepMicroseconds(L3GD20_BIAS_SETTLING_uS);
  }

  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++){
    ((L3GD20Driver *)ip)->bias[i] = buff[i] / L3GD20_BIAS_ACQ_TIMES;
  }
  return MSG_OK;
}

static msg_t set_bias(void *ip, int32_t *bp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (bp !=NULL));

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY) ||
                (((L3GD20Driver *)ip)->state == L3GD20_STOP),
                "set_bias(), invalid state");
  
  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
    ((L3GD20Driver *)ip)->bias[i] = bp[i];
  }
  return MSG_OK;
}

static msg_t reset_bias(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY) ||
                (((L3GD20Driver *)ip)->state == L3GD20_STOP),
              "reset_bias(), invalid state");

  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
    ((L3GD20Driver *)ip)->bias[i] = 0;
  return MSG_OK;
}

static msg_t set_sensivity(void *ip, float *sp) {
  uint32_t i;
  
  osalDbgCheck((ip != NULL) && (sp !=NULL));

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
                "set_sensivity(), invalid state");
  
  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
    ((L3GD20Driver *)ip)->sensitivity[i] = sp[i];
  }
  return MSG_OK;
}

static msg_t reset_sensivity(void *ip) {
  uint32_t i;

  osalDbgCheck(ip != NULL);

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
                "reset_sensivity(), invalid state");

  if(((L3GD20Driver *)ip)->config->fullscale == L3GD20_FS_250DPS)
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      ((L3GD20Driver *)ip)->sensitivity[i] = L3GD20_SENS_250DPS;
  else if(((L3GD20Driver *)ip)->config->fullscale == L3GD20_FS_500DPS)
	for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      ((L3GD20Driver *)ip)->sensitivity[i] = L3GD20_SENS_500DPS;
  else if(((L3GD20Driver *)ip)->config->fullscale == L3GD20_FS_2000DPS)
	for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      ((L3GD20Driver *)ip)->sensitivity[i] = L3GD20_SENS_2000DPS;
  else {
    osalDbgAssert(FALSE, "reset_sensivity(), full scale issue");
    return MSG_RESET;
  }
  return MSG_OK;
}

static msg_t get_temperature(void *ip, float* tempp) {
	
#if L3GD20_USE_SPI
  osalDbgAssert((((L3GD20Driver *)ip)->config->spip->state == SPI_READY),
                "read_raw(), channel not ready");
#if	L3GD20_SHARED_SPI
  spiAcquireBus(((L3GD20Driver *)ip)->config->spip);
  spiStart(((L3GD20Driver *)ip)->config->spip,
           ((L3GD20Driver *)ip)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */   
  *tempp = (int8_t)l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                                       L3GD20_AD_OUT_TEMP);
#if	L3GD20_SHARED_SPI
  spiReleaseBus(((L3GD20Driver *)ip)->config->spip);
#endif /* L3GD20_SHARED_SPI */   
#endif /* L3GD20_USE_SPI */ 
  return MSG_OK;
}

static const struct BaseSensorVMT vmt_basesensor = {
  get_axes_number, read_raw, read_cooked
};

static const struct BaseGyroscopeVMT vmt_basegyroscope = {
  get_axes_number, read_raw, read_cooked,
  sample_bias, set_bias, reset_bias,
  set_sensivity, reset_sensivity
};

static const struct L3GD20VMT vmt_l3gd20 = {
  get_axes_number, read_raw, read_cooked,
  sample_bias, set_bias, reset_bias,
  set_sensivity, reset_sensivity, get_temperature
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p L3GD20Driver object
 *
 * @init
 */
void l3gd20ObjectInit(L3GD20Driver *devp) {
  uint32_t i;
  devp->vmt_basesensor = &vmt_basesensor;
  devp->vmt_basegyroscope = &vmt_basegyroscope;
  devp->vmt_l3gd20 = &vmt_l3gd20;
  devp->config = NULL;
  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
    devp->bias[i] = 0;
  devp->state  = L3GD20_STOP;
}

/**
 * @brief   Configures and activates L3GD20 Complex Driver peripheral.
 *
 * @param[in] devp      pointer to the @p L3GD20Driver object
 * @param[in] config    pointer to the @p L3GD20Config object
 *
 * @api
 */
void l3gd20Start(L3GD20Driver *devp, const L3GD20Config *config) {
  uint32_t i;
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == L3GD20_STOP) || (devp->state == L3GD20_READY),
              "l3gd20Start(), invalid state");			  

  devp->config = config;
  
#if L3GD20_USE_SPI
#if	L3GD20_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */
  spiStart((devp)->config->spip,
           (devp)->config->spicfg);
  l3gd20SPIWriteRegister(devp->config->spip, L3GD20_AD_CTRL_REG1,
                         devp->config->axesenabling |
                         L3GD20_PM_SLEEP_NORMAL |
                         devp->config->outputdatarate);
  l3gd20SPIWriteRegister(devp->config->spip, L3GD20_AD_CTRL_REG4,
                         devp->config->fullscale |
                         devp->config->blockdataupdate |
                         devp->config->endianness);
#if	L3GD20_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */  
#endif /* L3GD20_USE_SPI */
  
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == L3GD20_FS_250DPS)
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = L3GD20_SENS_250DPS;
  else if(devp->config->fullscale == L3GD20_FS_500DPS)
	for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = L3GD20_SENS_500DPS;
  else if(devp->config->fullscale == L3GD20_FS_2000DPS)
	for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = L3GD20_SENS_2000DPS;
  else
    osalDbgAssert(FALSE, "l3gd20Start(), full scale issue");
  /* This is the Gyroscope transient recovery time */
  osalThreadSleepMilliseconds(10);

  devp->state = L3GD20_READY;
} 

/**
 * @brief   Deactivates the L3GD20 Complex Driver peripheral.
 *
 * @param[in] devp       pointer to the @p L3GD20Driver object
 *
 * @api
 */
void l3gd20Stop(L3GD20Driver *devp) {

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == L3GD20_STOP) || (devp->state == L3GD20_READY),
                "l3gd20Stop(), invalid state");

#if (L3GD20_USE_SPI)
  if (devp->state == L3GD20_STOP) {
#if	L3GD20_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */
    l3gd20SPIWriteRegister(devp->config->spip, L3GD20_AD_CTRL_REG1,
                           L3GD20_PM_POWER_DOWN | L3GD20_AE_DISABLED);
    spiStop((devp)->config->spip);
#if	L3GD20_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */    
  }			  
#endif /* L3GD20_USE_SPI */
  devp->state = L3GD20_STOP;
}
/** @} */
