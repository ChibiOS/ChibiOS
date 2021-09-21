/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    RCCv1/stm32_csi.inc
 * @brief   Shared CSI clock handler.
 *
 * @addtogroup STM32_CSI_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   CSI clock frequency.
 */
#define STM32_CSICLK            4000000U

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_RCC_HAS_CSI)
#error "STM32_RCC_HAS_CSI not defined in stm32_registry.h"
#endif

/* Checks on configurations.*/
#if !defined(STM32_CSI_ENABLED)
#error "STM32_CSI_ENABLED not defined in mcuconf.h"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE void csi_enable(void) {

#if 0
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {
    /* Waiting for CSI activation.*/
  }
#endif
}

__STATIC_INLINE void csi_disable(void) {

#if 0
  RCC->CR &= ~RCC_CR_HSION;
#endif
}

__STATIC_INLINE void csi_reset(void) {

#if 0
  /* Making sure HSI is active and ready.*/
  hsi16_enable();

  /* Clocking from HSI, in case HSI was not the default source.*/
  RCC->CFGR = RCC_CFGR_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
    /* Wait until HSI is selected.*/
  }
#endif
}

__STATIC_INLINE void csi_init(void) {

#if 0
#if STM32_CSI_ENABLED
  /* HSI activation.*/
  hsi16_enable();
#endif
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
