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
 * @addtogroup L3GD20
 * @ingroup EX_ST
 * @{
 */

#include "hal.h"
#include "l3gd20.h"

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

#if (L3GD20_USE_SPI) || defined(__DOXYGEN__)
/**
 * @brief   Reads a generic register value using SPI.
 * @pre     The SPI interface must be initialized and the driver started.
 *
 * @param[in] spip      pointer to the SPI interface
 * @param[in] reg       starting register address
 * @param[in] n         number of consecutive registers to read
 * @param[in] b         pointer to an output buffer.
 */
static void l3gd20SPIReadRegister(SPIDriver *spip, uint8_t reg,  size_t n,
                                     uint8_t* b) {
  uint8_t cmd;
  (n == 1) ? (cmd = reg | L3GD20_RW) : (cmd = reg | L3GD20_RW | L3GD20_MS);
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
static void l3gd20SPIWriteRegister(SPIDriver *spip, uint8_t reg, size_t n,
                                   uint8_t* b) {
  uint8_t cmd;
  (n == 1) ? (cmd = reg) : (cmd = reg | L3GD20_MS);
  spiSelect(spip);
  spiSend(spip, 1, &cmd);
  spiSend(spip, n, b);
  spiUnselect(spip);
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
  uint8_t i, buff [2 * L3GD20_NUMBER_OF_AXES];
  msg_t msg = MSG_OK;
  
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

    l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip, L3GD20_AD_OUT_X_L,
                          L3GD20_NUMBER_OF_AXES * 2, buff);

#if	L3GD20_SHARED_SPI
  spiReleaseBus(((L3GD20Driver *)ip)->config->spip);
#endif /* L3GD20_SHARED_SPI */   
#endif /* L3GD20_USE_SPI */ 

    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      tmp = buff[2*i] + (buff[2*i+1] << 8);
      axes[i] = (int32_t)tmp;
    }
  return msg;
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
    axes[i] -= ((L3GD20Driver *)ip)->bias[i];
  }
  return msg;
}

static msg_t sample_bias(void *ip) {
  uint32_t i, j;
  int32_t raw[L3GD20_NUMBER_OF_AXES];
  int32_t buff[L3GD20_NUMBER_OF_AXES] = {0, 0, 0};
  msg_t msg;
	
  osalDbgCheck(ip != NULL);

  osalDbgAssert((((L3GD20Driver *)ip)->state == L3GD20_READY),
                "sample_bias(), invalid state");

  for(i = 0; i < L3GD20_BIAS_ACQ_TIMES; i++){
    msg = read_raw(ip, raw);
		if(msg != MSG_OK)
			return msg;
    for(j = 0; j < L3GD20_NUMBER_OF_AXES; j++){
      buff[j] += raw[j];
    }
    osalThreadSleepMicroseconds(L3GD20_BIAS_SETTLING_uS);
  }

  for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++){
    ((L3GD20Driver *)ip)->bias[i] = (buff[i] / L3GD20_BIAS_ACQ_TIMES);
    ((L3GD20Driver *)ip)->bias[i] *= ((L3GD20Driver *)ip)->sensitivity[i];
  }
  return msg;
}

static msg_t set_bias(void *ip, float *bp) {
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

static msg_t set_full_scale(void *ip, l3gd20_fs_t fs) {
  float newfs, scale;
  uint8_t i, cr;

  if(fs == L3GD20_FS_250DPS) {
    newfs = L3GD20_250DPS;
  }
  else if(fs == L3GD20_FS_500DPS) {
    newfs = L3GD20_500DPS;
  }
  else if(fs == L3GD20_FS_2000DPS) {
    newfs = L3GD20_2000DPS;
  }
  else {
    return MSG_RESET;
  }

  if(newfs != ((L3GD20Driver *)ip)->fullscale) {
    scale = newfs / ((L3GD20Driver *)ip)->fullscale;
    ((L3GD20Driver *)ip)->fullscale = newfs;

#if L3GD20_USE_SPI
#if	L3GD20_SHARED_SPI
		spiAcquireBus(((L3GD20Driver *)ip)->config->spip);
		spiStart(((L3GD20Driver *)ip)->config->spip,
						 ((L3GD20Driver *)ip)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */ 

    /* Updating register.*/
    l3gd20SPIReadRegister(((L3GD20Driver *)ip)->config->spip,
                          L3GD20_AD_CTRL_REG4, 1, &cr);

#if	L3GD20_SHARED_SPI
		spiReleaseBus(((L3GD20Driver *)ip)->config->spip);
#endif /* L3GD20_SHARED_SPI */ 
#endif /* L3GD20_USE_SPI */
    cr &= ~(L3GD20_CTRL_REG4_FS_MASK);
    cr |= fs;
 
#if L3GD20_USE_SPI 
#if	L3GD20_SHARED_SPI
		spiAcquireBus(((L3GD20Driver *)ip)->config->spip);
		spiStart(((L3GD20Driver *)ip)->config->spip,
						 ((L3GD20Driver *)ip)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */ 

    l3gd20SPIWriteRegister(((L3GD20Driver *)ip)->config->spip,
                           L3GD20_AD_CTRL_REG4, 1, &cr);
#if	L3GD20_SHARED_SPI
		spiReleaseBus(((L3GD20Driver *)ip)->config->spip);
#endif /* L3GD20_SHARED_SPI */ 
#endif /* L3GD20_USE_SPI */

    /* Scaling sensitivity and bias. Re-calibration is suggested anyway. */
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      ((L3GD20Driver *)ip)->sensitivity[i] *= scale;
      ((L3GD20Driver *)ip)->bias[i] *= scale;
    }
  }
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
  set_sensivity, reset_sensivity,
  set_full_scale
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
    devp->bias[i] = 0.0f;
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
  uint8_t cr[5] = {0, 0, 0, 0, 0};
  osalDbgCheck((devp != NULL) && (config != NULL));

  osalDbgAssert((devp->state == L3GD20_STOP) || (devp->state == L3GD20_READY),
              "l3gd20Start(), invalid state");

  devp->config = config;
             
  /* Control register 1 configuration block.*/
  {
    cr[0] = L3GD20_CTRL_REG1_XEN | L3GD20_CTRL_REG1_YEN | 
          L3GD20_CTRL_REG1_ZEN | L3GD20_CTRL_REG1_PD |
          devp->config->outputdatarate;
#if L3GD20_USE_ADVANCED || defined(__DOXYGEN__)
    cr[0] |= devp->config->bandwidth;
#endif
  }
  
  /* Control register 2 configuration block.*/
  {
#if L3GD20_USE_ADVANCED || defined(__DOXYGEN__)
  if(devp->config->hpmode != L3GD20_HPM_BYPASSED)
    cr[1] = devp->config->hpmode | devp->config->hpconfiguration;
#endif
  }
  
  /* Control register 4 configuration block.*/
  {
    cr[3] = devp->config->fullscale;
#if L3GD20_USE_ADVANCED || defined(__DOXYGEN__)
    cr[3] |= devp->config->blockdataupdate |
             devp->config->endianness;
#endif
  }
  
  /* Control register 5 configuration block.*/
  {
    
#if L3GD20_USE_ADVANCED || defined(__DOXYGEN__)
  if((devp->config->hpmode != L3GD20_HPM_BYPASSED)) {
    cr[4] = L3GD20_CTRL_REG5_HPEN;
    if(devp->config->lp2mode != L3GD20_LP2M_BYPASSED) {
      cr[4] |= L3GD20_CTRL_REG5_INT1_SEL1 |
               L3GD20_CTRL_REG5_OUT_SEL1;
    }
    else {
      cr[4] |= L3GD20_CTRL_REG5_INT1_SEL0 |
               L3GD20_CTRL_REG5_OUT_SEL0; 
    }
  }
#endif
  } 

#if L3GD20_USE_SPI
#if	L3GD20_SHARED_SPI
  spiAcquireBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */
  spiStart((devp)->config->spip,
           (devp)->config->spicfg);
           
  l3gd20SPIWriteRegister(devp->config->spip, L3GD20_AD_CTRL_REG1, 
                         5, cr);
#if	L3GD20_SHARED_SPI
  spiReleaseBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */
#endif /* L3GD20_USE_SPI */
  
  /* Storing sensitivity information according to full scale.*/
  if(devp->config->fullscale == L3GD20_FS_250DPS) {
    devp->fullscale = L3GD20_250DPS;
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      if (devp->config->sensitivity == NULL)
        devp->sensitivity[i] = L3GD20_SENS_250DPS;
      else
        devp->sensitivity[i] = devp->config->sensitivity[i];
    }
  }
  else if(devp->config->fullscale == L3GD20_FS_500DPS) {
    devp->fullscale = L3GD20_500DPS;
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      if (devp->config->sensitivity == NULL)
        devp->sensitivity[i] = L3GD20_SENS_500DPS;
      else
        devp->sensitivity[i] = devp->config->sensitivity[i];
    }
  }
  else if(devp->config->fullscale == L3GD20_FS_2000DPS) {
    devp->fullscale = L3GD20_2000DPS;
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      if (devp->config->sensitivity == NULL)
        devp->sensitivity[i] = L3GD20_SENS_2000DPS;
      else
        devp->sensitivity[i] = devp->config->sensitivity[i];
    }
  }
  else
    osalDbgAssert(FALSE, "l3gd20Start(), full scale issue");
  
  if(devp->config->bias != NULL) {
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
      devp->bias[i] = devp->config->bias[i];
    }
  }
  
  /* This is the Gyroscope transient recovery time.*/
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
  uint8_t cr1;
  osalDbgCheck(devp != NULL);

  osalDbgAssert((devp->state == L3GD20_STOP) || (devp->state == L3GD20_READY),
                "l3gd20Stop(), invalid state");

  if (devp->state == L3GD20_READY) {
    /* Disabling all axes and enabling power down mode.*/
    cr1 = 0;
    
#if L3GD20_USE_SPI
#if	L3GD20_SHARED_SPI
    spiAcquireBus((devp)->config->spip);
    spiStart((devp)->config->spip,
             (devp)->config->spicfg);
#endif /* L3GD20_SHARED_SPI */

    l3gd20SPIWriteRegister(devp->config->spip, L3GD20_AD_CTRL_REG1, 
                           1, &cr1);
    spiStop((devp)->config->spip);
    
#if	L3GD20_SHARED_SPI
    spiReleaseBus((devp)->config->spip);
#endif /* L3GD20_SHARED_SPI */ 
#endif /* L3GD20_USE_SPI */
  }			 
  devp->state = L3GD20_STOP;
}
/** @} */
