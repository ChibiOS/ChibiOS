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
 * @addtogroup lis3dsh
 * @{
 */

#include "hal.h"
#include "lis3dsh.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define  LIS3DSH_SENS_2G                    ((float)0.06f)
#define  LIS3DSH_SENS_4G                    ((float)0.12f)
#define  LIS3DSH_SENS_6G                    ((float)0.18f)
#define  LIS3DSH_SENS_8G                    ((float)0.24f)
#define  LIS3DSH_SENS_16G                   ((float)0.73f)

#define  LIS3DSH_DI                         ((uint8_t)0xFF)
#define  LIS3DSH_DI_0                       ((uint8_t)0x01)
#define  LIS3DSH_DI_1                       ((uint8_t)0x02)
#define  LIS3DSH_DI_2                       ((uint8_t)0x04)
#define  LIS3DSH_DI_3                       ((uint8_t)0x08)
#define  LIS3DSH_DI_4                       ((uint8_t)0x10)
#define  LIS3DSH_DI_5                       ((uint8_t)0x20)
#define  LIS3DSH_DI_6                       ((uint8_t)0x40)
#define  LIS3DSH_DI_7                       ((uint8_t)0x80)

#define  LIS3DSH_AD                         ((uint8_t)0x3F)
#define  LIS3DSH_AD_0                       ((uint8_t)0x01)
#define  LIS3DSH_AD_1                       ((uint8_t)0x02)
#define  LIS3DSH_AD_2                       ((uint8_t)0x04)
#define  LIS3DSH_AD_3                       ((uint8_t)0x08)
#define  LIS3DSH_AD_4                       ((uint8_t)0x10)
#define  LIS3DSH_AD_5                       ((uint8_t)0x20)

#define  LIS3DSH_MS                         ((uint8_t)0x40)
#define  LIS3DSH_RW                         ((uint8_t)0x80)

#define  LIS3DSH_AD_INFO1                   ((uint8_t)0x0D)
#define  LIS3DSH_AD_INFO2                   ((uint8_t)0x0E)
#define  LIS3DSH_AD_WHO_AM_I                ((uint8_t)0x0F)
#define  LIS3DSH_AD_CTRL_REG3               ((uint8_t)0x23)
#define  LIS3DSH_AD_CTRL_REG4               ((uint8_t)0x20)
#define  LIS3DSH_AD_CTRL_REG5               ((uint8_t)0x24)
#define  LIS3DSH_AD_CTRL_REG6               ((uint8_t)0x25)
#define  LIS3DSH_AD_STATUS                  ((uint8_t)0x27)
#define  LIS3DSH_AD_OUT_T                   ((uint8_t)0x0C)
#define  LIS3DSH_AD_OFF_X                   ((uint8_t)0x10)
#define  LIS3DSH_AD_OFF_Y                   ((uint8_t)0x11)
#define  LIS3DSH_AD_OFF_Z                   ((uint8_t)0x12)
#define  LIS3DSH_AD_CS_X                    ((uint8_t)0x13)
#define  LIS3DSH_AD_CS_Y                    ((uint8_t)0x14)
#define  LIS3DSH_AD_CS_Z                    ((uint8_t)0x15)
#define  LIS3DSH_AD_LC_L                    ((uint8_t)0x16)
#define  LIS3DSH_AD_LC_H                    ((uint8_t)0x17)
#define  LIS3DSH_AD_STAT                    ((uint8_t)0x18)
#define  LIS3DSH_AD_VFC_1                   ((uint8_t)0x1B)
#define  LIS3DSH_AD_VFC_2                   ((uint8_t)0x1C)
#define  LIS3DSH_AD_VFC_3                   ((uint8_t)0x1D)
#define  LIS3DSH_AD_VFC_4                   ((uint8_t)0x1E)
#define  LIS3DSH_AD_THRS3                   ((uint8_t)0x1F)
#define  LIS3DSH_AD_OUT_X_L                 ((uint8_t)0x28)
#define  LIS3DSH_AD_OUT_X_H                 ((uint8_t)0x29)
#define  LIS3DSH_AD_OUT_Y_L                 ((uint8_t)0x2A)
#define  LIS3DSH_AD_OUT_Y_H                 ((uint8_t)0x2B)
#define  LIS3DSH_AD_OUT_Z_L                 ((uint8_t)0x2C)
#define  LIS3DSH_AD_OUT_Z_H                 ((uint8_t)0x2D)
#define  LIS3DSH_AD_FIFO_CTRL               ((uint8_t)0x2E)
#define  LIS3DSH_AD_FIFO_SRC                ((uint8_t)0x2F)
#define  LIS3DSH_AD_CTRL_REG1               ((uint8_t)0x21)
#define  LIS3DSH_AD_ST1_0                   ((uint8_t)0x40)
#define  LIS3DSH_AD_ST1_1                   ((uint8_t)0x41)
#define  LIS3DSH_AD_ST1_2                   ((uint8_t)0x42)
#define  LIS3DSH_AD_ST1_3                   ((uint8_t)0x43)
#define  LIS3DSH_AD_ST1_4                   ((uint8_t)0x44)
#define  LIS3DSH_AD_ST1_5                   ((uint8_t)0x45)
#define  LIS3DSH_AD_ST1_6                   ((uint8_t)0x46)
#define  LIS3DSH_AD_ST1_7                   ((uint8_t)0x47)
#define  LIS3DSH_AD_ST1_8                   ((uint8_t)0x48)
#define  LIS3DSH_AD_ST1_9                   ((uint8_t)0x49)
#define  LIS3DSH_AD_ST1_A                   ((uint8_t)0x4A)
#define  LIS3DSH_AD_ST1_B                   ((uint8_t)0x4B)
#define  LIS3DSH_AD_ST1_C                   ((uint8_t)0x4C)
#define  LIS3DSH_AD_ST1_D                   ((uint8_t)0x4D)
#define  LIS3DSH_AD_ST1_E                   ((uint8_t)0x4E)
#define  LIS3DSH_AD_ST1_F                   ((uint8_t)0x4F)
#define  LIS3DSH_AD_TIM4_1                  ((uint8_t)0x50)
#define  LIS3DSH_AD_TIM3_1                  ((uint8_t)0x51)
#define  LIS3DSH_AD_TIM2_1_L                ((uint8_t)0x52)
#define  LIS3DSH_AD_TIM2_1_H                ((uint8_t)0x53)
#define  LIS3DSH_AD_TIM1_1_L                ((uint8_t)0x54)
#define  LIS3DSH_AD_TIM1_1_H                ((uint8_t)0x55)
#define  LIS3DSH_AD_THRS2_1                 ((uint8_t)0x56)
#define  LIS3DSH_AD_THRS1_1                 ((uint8_t)0x57)
#define  LIS3DSH_AD_MASK1_B                 ((uint8_t)0x59)
#define  LIS3DSH_AD_MASK1_A                 ((uint8_t)0x5A)
#define  LIS3DSH_AD_SETT1                   ((uint8_t)0x5B)
#define  LIS3DSH_AD_PR1                     ((uint8_t)0x5C)
#define  LIS3DSH_AD_TC1_L                   ((uint8_t)0x5D)
#define  LIS3DSH_AD_TC1_H                   ((uint8_t)0x5E)
#define  LIS3DSH_AD_OUTS1                   ((uint8_t)0x5F)
#define  LIS3DSH_AD_PEAK1                   ((uint8_t)0x19)
#define  LIS3DSH_AD_CTRL_REG2               ((uint8_t)0x22)
#define  LIS3DSH_AD_ST2_0                   ((uint8_t)0x60)
#define  LIS3DSH_AD_ST2_1                   ((uint8_t)0x61)
#define  LIS3DSH_AD_ST2_2                   ((uint8_t)0x62)
#define  LIS3DSH_AD_ST2_3                   ((uint8_t)0x63)
#define  LIS3DSH_AD_ST2_4                   ((uint8_t)0x64)
#define  LIS3DSH_AD_ST2_5                   ((uint8_t)0x65)
#define  LIS3DSH_AD_ST2_6                   ((uint8_t)0x66)
#define  LIS3DSH_AD_ST2_7                   ((uint8_t)0x67)
#define  LIS3DSH_AD_ST2_8                   ((uint8_t)0x68)
#define  LIS3DSH_AD_ST2_9                   ((uint8_t)0x69)
#define  LIS3DSH_AD_ST2_A                   ((uint8_t)0x6A)
#define  LIS3DSH_AD_ST2_B                   ((uint8_t)0x6B)
#define  LIS3DSH_AD_ST2_C                   ((uint8_t)0x6C)
#define  LIS3DSH_AD_ST2_D                   ((uint8_t)0x6D)
#define  LIS3DSH_AD_ST2_E                   ((uint8_t)0x6E)
#define  LIS3DSH_AD_ST2_F                   ((uint8_t)0x6F)
#define  LIS3DSH_AD_TIM4_2                  ((uint8_t)0x70)
#define  LIS3DSH_AD_TIM3_2                  ((uint8_t)0x71)
#define  LIS3DSH_AD_TIM2_2_L                ((uint8_t)0x72)
#define  LIS3DSH_AD_TIM2_2_H                ((uint8_t)0x73)
#define  LIS3DSH_AD_TIM1_2_L                ((uint8_t)0x74)
#define  LIS3DSH_AD_TIM1_2_H                ((uint8_t)0x75)
#define  LIS3DSH_AD_THRS2_2                 ((uint8_t)0x76)
#define  LIS3DSH_AD_THRS1_2                 ((uint8_t)0x77)
#define  LIS3DSH_AD_MASK2_B                 ((uint8_t)0x79)
#define  LIS3DSH_AD_MASK2_A                 ((uint8_t)0x7A)
#define  LIS3DSH_AD_SETT2                   ((uint8_t)0x7B)
#define  LIS3DSH_AD_PR2                     ((uint8_t)0x7C)
#define  LIS3DSH_AD_TC2_L                   ((uint8_t)0x7D)
#define  LIS3DSH_AD_TC2_H                   ((uint8_t)0x7E)
#define  LIS3DSH_AD_OUTS2                   ((uint8_t)0x7F)
#define  LIS3DSH_AD_PEAK2                   ((uint8_t)0x1A)
#define  LIS3DSH_AD_DES2                    ((uint8_t)0x78)

#define  LIS3DSH_CTRL_REG5_FS_MASK          ((uint8_t)0x38)
#define  TO_G                               ((float)0.001f)
#define  TO_SI                              ((float)0.00981f)
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
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       register number
 * @return              register value.
 */
static uint8_t lis3dshSPIReadRegister(SPIDriver *spip, uint8_t reg) {
  uint8_t txbuf[2] = {LIS3DSH_RW | reg, 0xFF};
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
static void lis3dshSPIWriteRegister(SPIDriver *spip, uint8_t reg,
                                   uint8_t value) {

  switch (reg) {
    default:
      /* Reserved register must not be written, according to the datasheet
       * this could permanently damage the device.
       */
      osalDbgAssert(FALSE, "lis3dshSPIWriteRegister(), reserved register");
    case LIS3DSH_AD_INFO1:
    case LIS3DSH_AD_INFO2:
    case LIS3DSH_AD_WHO_AM_I:
    case LIS3DSH_AD_STATUS:
    case LIS3DSH_AD_OUT_T:
    case LIS3DSH_AD_STAT:
    case LIS3DSH_AD_OUT_X_L:
    case LIS3DSH_AD_OUT_X_H:
    case LIS3DSH_AD_OUT_Y_L:
    case LIS3DSH_AD_OUT_Y_H:
    case LIS3DSH_AD_OUT_Z_L:
    case LIS3DSH_AD_OUT_Z_H:
    case LIS3DSH_AD_FIFO_SRC:
	case LIS3DSH_AD_FIFO_CTRL:
    case LIS3DSH_AD_PR1:
    case LIS3DSH_AD_TC1_L:
    case LIS3DSH_AD_TC1_H:
    case LIS3DSH_AD_OUTS1:
    case LIS3DSH_AD_PEAK1:
    case LIS3DSH_AD_PR2:
    case LIS3DSH_AD_TC2_L:
    case LIS3DSH_AD_TC2_H:
    case LIS3DSH_AD_OUTS2:
    case LIS3DSH_AD_PEAK2:
    /* Read only registers cannot be written, the command is ignored.*/
      return;
    case LIS3DSH_AD_CTRL_REG3:
    case LIS3DSH_AD_CTRL_REG4:
    case LIS3DSH_AD_CTRL_REG5:
    case LIS3DSH_AD_CTRL_REG6:
    case LIS3DSH_AD_OFF_X:
    case LIS3DSH_AD_OFF_Y:
    case LIS3DSH_AD_OFF_Z:
    case LIS3DSH_AD_CS_X:
    case LIS3DSH_AD_CS_Y:
    case LIS3DSH_AD_CS_Z:
    case LIS3DSH_AD_LC_L:
    case LIS3DSH_AD_LC_H:
    case LIS3DSH_AD_VFC_1:
    case LIS3DSH_AD_VFC_2:
    case LIS3DSH_AD_VFC_3:
    case LIS3DSH_AD_VFC_4:
    case LIS3DSH_AD_THRS3:
    case LIS3DSH_AD_CTRL_REG1:
	case LIS3DSH_AD_ST1_0:
    case LIS3DSH_AD_ST1_1:
    case LIS3DSH_AD_ST1_2:
    case LIS3DSH_AD_ST1_3:
    case LIS3DSH_AD_ST1_4:
    case LIS3DSH_AD_ST1_5:
    case LIS3DSH_AD_ST1_6:
    case LIS3DSH_AD_ST1_7:
    case LIS3DSH_AD_ST1_8:
    case LIS3DSH_AD_ST1_9:
    case LIS3DSH_AD_ST1_A:
    case LIS3DSH_AD_ST1_B:
    case LIS3DSH_AD_ST1_C:
    case LIS3DSH_AD_ST1_D:
    case LIS3DSH_AD_ST1_E:
    case LIS3DSH_AD_ST1_F:
    case LIS3DSH_AD_TIM4_1:
    case LIS3DSH_AD_TIM3_1:
    case LIS3DSH_AD_TIM2_1_L:
    case LIS3DSH_AD_TIM2_1_H:
    case LIS3DSH_AD_TIM1_1_L:
    case LIS3DSH_AD_TIM1_1_H:
    case LIS3DSH_AD_THRS2_1:
    case LIS3DSH_AD_THRS1_1:
    case LIS3DSH_AD_MASK1_B:
    case LIS3DSH_AD_MASK1_A:
    case LIS3DSH_AD_SETT1:
	case LIS3DSH_AD_CTRL_REG2:
	case LIS3DSH_AD_ST2_0:
    case LIS3DSH_AD_ST2_1:
    case LIS3DSH_AD_ST2_2:
    case LIS3DSH_AD_ST2_3:
    case LIS3DSH_AD_ST2_4:
    case LIS3DSH_AD_ST2_5:
    case LIS3DSH_AD_ST2_6:
    case LIS3DSH_AD_ST2_7:
    case LIS3DSH_AD_ST2_8:
    case LIS3DSH_AD_ST2_9:
    case LIS3DSH_AD_ST2_A:
    case LIS3DSH_AD_ST2_B:
    case LIS3DSH_AD_ST2_C:
    case LIS3DSH_AD_ST2_D:
    case LIS3DSH_AD_ST2_E:
    case LIS3DSH_AD_ST2_F:
    case LIS3DSH_AD_TIM4_2:
    case LIS3DSH_AD_TIM3_2:
    case LIS3DSH_AD_TIM2_2_L:
    case LIS3DSH_AD_TIM2_2_H:
    case LIS3DSH_AD_TIM1_2_L:
    case LIS3DSH_AD_TIM1_2_H:
    case LIS3DSH_AD_THRS2_2:
    case LIS3DSH_AD_THRS1_2:
    case LIS3DSH_AD_MASK2_B:
    case LIS3DSH_AD_MASK2_A:
    case LIS3DSH_AD_SETT2:
	case LIS3DSH_AD_DES2:
      spiSelect(spip);
      uint8_t txbuf[2] = {reg, value};
      spiSend(spip, 2, txbuf);
      spiUnselect(spip);
  }
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
  if(((LIS3DSHDriver *)ip)->config->axesenabling & LIS3DSH_AE_X){
    tmp = lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                  LIS3DSH_AD_OUT_X_L);
    tmp += lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                   LIS3DSH_AD_OUT_X_H) << 8;
    axes[0] = (int32_t)tmp + ((LIS3DSHDriver *)ip)->bias[0];
  }
  if(((LIS3DSHDriver *)ip)->config->axesenabling & LIS3DSH_AE_Y){
    tmp = lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                  LIS3DSH_AD_OUT_Y_L);
    tmp += lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                   LIS3DSH_AD_OUT_Y_H) << 8;
    axes[1] = (int32_t)tmp + ((LIS3DSHDriver *)ip)->bias[1];
  }
  if(((LIS3DSHDriver *)ip)->config->axesenabling & LIS3DSH_AE_Z){
    tmp = lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                  LIS3DSH_AD_OUT_Z_L);
    tmp += lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                   LIS3DSH_AD_OUT_Z_H) << 8;
    axes[2] = (int32_t)tmp + ((LIS3DSHDriver *)ip)->bias[2];
  }
#if	LIS3DSH_SHARED_SPI
  spiReleaseBus(((LIS3DSHDriver *)ip)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */   
#endif /* LIS3DSH_USE_SPI */ 
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
    axes[i] = raw[i] * ((LIS3DSHDriver *)ip)->sensitivity[i];
    if(((LIS3DSHDriver *)ip)->config->unit == LIS3DSH_ACC_UNIT_G){
      axes[i] *= TO_G;
    }
    else if(((LIS3DSHDriver *)ip)->config->unit == LIS3DSH_ACC_UNIT_SI){
      axes[i] *= TO_SI;
    }
  }
  return msg;
}

static msg_t set_bias(void *ip, int32_t *bp) {
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

static msg_t get_temperature(void *ip, int8_t* tempp) {
#if LIS3DSH_USE_SPI
  osalDbgAssert((((LIS3DSHDriver *)ip)->config->spip->state == SPI_READY),
                "read_raw(), channel not ready");
#if	LIS3DSH_SHARED_SPI
  spiAcquireBus(((LIS3DSHDriver *)ip)->config->spip);
  spiStart(((LIS3DSHDriver *)ip)->config->spip,
           ((LIS3DSHDriver *)ip)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */   
  *tempp = lis3dshSPIReadRegister(((LIS3DSHDriver *)ip)->config->spip,
                                    LIS3DSH_AD_OUT_T);
#if	LIS3DSH_SHARED_SPI
  spiReleaseBus(((LIS3DSHDriver *)ip)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */   
#endif /* LIS3DSH_USE_SPI */ 
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
  get_temperature
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
    devp->bias[i] = 0;
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

  osalDbgAssert((devp->state == LIS3DSH_STOP) || (devp->state == LIS3DSH_READY),
              "lis3dshStart(), invalid state");			  

  devp->config = config;
  
#if LIS3DSH_USE_SPI
#if	LIS3DSH_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */
  spiStart((devp)->config->spip,
           (devp)->config->spicfg);
  lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG4,
                         devp->config->axesenabling |
                         devp->config->blockdataupdate |
						 devp->config->outputdatarate);
  lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG5,
                         devp->config->fullscale |
                         devp->config->antialiasing );
#if	LIS3DSH_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */  
#endif /* LIS3DSH_USE_SPI */
  
  /* Storing sensitivity information according to full scale value */
  if(devp->config->fullscale == LIS3DSH_FS_2G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3DSH_SENS_2G;
  else if(devp->config->fullscale == LIS3DSH_FS_4G)
	for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3DSH_SENS_4G;
  else if(devp->config->fullscale == LIS3DSH_FS_6G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3DSH_SENS_6G;
  else if(devp->config->fullscale == LIS3DSH_FS_8G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3DSH_SENS_8G;
  else if(devp->config->fullscale == LIS3DSH_FS_16G)
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      devp->sensitivity[i] = LIS3DSH_SENS_16G;
  else {
    osalDbgAssert(FALSE, "lis3dshStart(), accelerometer full scale issue");
  }
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

  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == LIS3DSH_STOP) || (devp->state == LIS3DSH_READY),
                "lis3dshStop(), invalid state");

#if (LIS3DSH_USE_SPI)
  if (devp->state == LIS3DSH_STOP) {
#if	LIS3DSH_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* LIS3DSH_SHARED_SPI */
    lis3dshSPIWriteRegister(devp->config->spip, LIS3DSH_AD_CTRL_REG4,
                           LIS3DSH_ODR_PD | LIS3DSH_AE_DISABLED);
    spiStop((devp)->config->spip);
#if	LIS3DSH_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* LIS3DSH_SHARED_SPI */    
  }			  
#endif /* LIS3DSH_USE_SPI */
  devp->state = LIS3DSH_STOP;
}
/** @} */
