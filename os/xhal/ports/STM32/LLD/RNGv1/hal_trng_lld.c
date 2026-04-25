/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    RNGv1/hal_trng_lld.c
 * @brief   STM32 TRNG subsystem low level driver source.
 *
 * @addtogroup TRNG
 * @{
 */

#include "hal.h"

#if HAL_USE_TRNG || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define RNG_CR_DRIVER_MASK                  (RNG_CR_RNGEN | RNG_CR_IE)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_TRNG_USE_RNG1 || defined(__DOXYGEN__)
TRNGDriver TRNGD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const TRNGConfig default_cfg = {
  .cr = 0U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void trng_apply_config(TRNGDriver *trngp, const TRNGConfig *config) {

  trngp->rng->CR = (config->cr & ~RNG_CR_DRIVER_MASK) | RNG_CR_RNGEN;
}

static const TRNGConfig *trng_validate_config(const TRNGConfig *config) {

  if (config == NULL) {
    return NULL;
  }

  return config;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level TRNG driver initialization.
 *
 * @notapi
 */
void trng_lld_init(void) {

#if STM32_TRNG_USE_RNG1
  trngObjectInit(&TRNGD1);
  TRNGD1.rng = RNG;
#endif
}

/**
 * @brief   Configures and activates the TRNG peripheral.
 *
 * @param[in,out] trngp         Pointer to the TRNG driver instance.
 * @return                      The operation status.
 *
 * @notapi
 */
msg_t trng_lld_start(TRNGDriver *trngp) {
  const TRNGConfig *config;

  osalDbgCheck(trngp != NULL);

  config = (const TRNGConfig *)trngp->config;
  osalDbgAssert(config != NULL, "config missing");

#if !defined(STM32_DISABLE_RNG_CLOCK_CHECK)
  osalDbgAssert(((STM32_RNGCLK >= 47000000U) && (STM32_RNGCLK <= 49000000U)) ||
                ((STM32_RNGCLK >=  3500000U) && (STM32_RNGCLK <=  4500000U)),
                "invalid RNG frequency");
#endif

#if STM32_TRNG_USE_RNG1
  if (&TRNGD1 == trngp) {
    rccEnableRNG(false);
  }
#endif

  trng_apply_config(trngp, config);

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the TRNG peripheral.
 *
 * @param[in,out] trngp         Pointer to the TRNG driver instance.
 *
 * @notapi
 */
void trng_lld_stop(TRNGDriver *trngp) {

  osalDbgCheck(trngp != NULL);

  if (trngp->state != HAL_DRV_STATE_STOP) {
    trngp->rng->CR = 0U;

#if STM32_TRNG_USE_RNG1
    if (&TRNGD1 == trngp) {
      rccDisableRNG();
    }
#endif
  }
}

/**
 * @brief   Applies a configuration.
 *
 * @param[in,out] trngp         Pointer to the TRNG driver instance.
 * @param[in]     config        Pointer to the configuration structure.
 * @return                      The accepted configuration or @p NULL.
 *
 * @notapi
 */
const TRNGConfig *trng_lld_setcfg(TRNGDriver *trngp,
                                  const TRNGConfig *config) {

  osalDbgCheck(trngp != NULL);

  config = trng_validate_config(config);
  if (config == NULL) {
    return NULL;
  }

  if (trngp->state == HAL_DRV_STATE_READY) {
    trng_apply_config(trngp, config);
  }

  return config;
}

/**
 * @brief   Selects one of the predefined configurations.
 *
 * @param[in,out] trngp         Pointer to the TRNG driver instance.
 * @param[in]     cfgnum        Configuration selector.
 * @return                      The selected configuration or @p NULL.
 *
 * @notapi
 */
const TRNGConfig *trng_lld_selcfg(TRNGDriver *trngp, unsigned cfgnum) {

  osalDbgCheck(trngp != NULL);
  (void)trngp;

  if (cfgnum == 0U) {
    return &default_cfg;
  }

  return NULL;
}

/**
 * @brief   True random numbers generator.
 * @note    The function is blocking and performs polled waiting inside the
 *          low level implementation.
 *
 * @param[in,out] trngp         Pointer to the TRNG driver instance.
 * @param[in]     size          Size of output buffer.
 * @param[out]    out           Output buffer.
 * @return                      The operation status.
 *
 * @notapi
 */
msg_t trng_lld_generate(TRNGDriver *trngp, size_t size, uint8_t *out) {

  osalDbgCheck((trngp != NULL) && (size > 0U) && (out != NULL));

  while (true) {
    uint32_t r, tmo;
    size_t i;

    tmo = STM32_TRNG_ERROR_CLEAR_ATTEMPTS;
    while ((tmo > 0U) &&
           ((trngp->rng->SR & (RNG_SR_CECS | RNG_SR_SECS)) != 0U)) {
      tmo--;
      if (tmo == 0U) {
        return HAL_RET_HW_FAILURE;
      }
    }

    tmo = STM32_TRNG_DATA_FETCH_ATTEMPTS;
    while ((tmo > 0U) && ((trngp->rng->SR & RNG_SR_DRDY) == 0U)) {
      tmo--;
      if (tmo == 0U) {
        return HAL_RET_HW_FAILURE;
      }
    }

    r = trngp->rng->DR;

    for (i = 0U; i < sizeof (uint32_t); i++) {
      *out++ = (uint8_t)r;
      r >>= 8;
      size--;
      if (size == 0U) {
        return HAL_RET_SUCCESS;
      }
    }
  }
}

#endif /* HAL_USE_TRNG */

/** @} */
