/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    RCCv1/stm32_msi.inc
 * @brief   Shared MSI clock handler.
 *
 * @addtogroup STM32_MSI_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    RCC_CR register bits definitions
 * @{
 */
#define STM32_MSIRANGE_MASK     (15 << 4)   /**< MSIRANGE field mask.       */
#define STM32_MSIRANGE_100K     (0 << 4)    /**< 100kHz nominal.            */
#define STM32_MSIRANGE_200K     (1 << 4)    /**< 200kHz nominal.            */
#define STM32_MSIRANGE_400K     (2 << 4)    /**< 400kHz nominal.            */
#define STM32_MSIRANGE_800K     (3 << 4)    /**< 800kHz nominal.            */
#define STM32_MSIRANGE_1M       (4 << 4)    /**< 1MHz nominal.              */
#define STM32_MSIRANGE_2M       (5 << 4)    /**< 2MHz nominal.              */
#define STM32_MSIRANGE_4M       (6 << 4)    /**< 4MHz nominal.              */
#define STM32_MSIRANGE_8M       (7 << 4)    /**< 8MHz nominal.              */
#define STM32_MSIRANGE_16M      (8 << 4)    /**< 16MHz nominal.             */
#define STM32_MSIRANGE_24M      (9 << 4)    /**< 24MHz nominal.             */
#define STM32_MSIRANGE_32M      (10 << 4)   /**< 32MHz nominal.             */
#define STM32_MSIRANGE_48M      (11 << 4)   /**< 48MHz nominal.             */
/** @} */

/**
 * @name    RCC_CSR register bits definitions
 * @{
 */
#define STM32_MSISRANGE_MASK    (15 << 8)   /**< MSISRANGE field mask.      */
#define STM32_MSISRANGE_1M      (4 << 8)    /**< 1MHz nominal.              */
#define STM32_MSISRANGE_2M      (5 << 8)    /**< 2MHz nominal.              */
#define STM32_MSISRANGE_4M      (6 << 8)    /**< 4MHz nominal.              */
#define STM32_MSISRANGE_8M      (7 << 8)    /**< 8MHz nominal.              */
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_MSI)
#error "STM32_HAS_MSI not defined in stm32_registry.h"
#endif

/* Checks on configurations.*/
#if !defined(STM32_MSIPLL_ENABLED)
#error "STM32_MSIPLL_ENABLED not defined in mcuconf.h"
#endif

#if !defined(STM32_MSIRANGE)
#error "STM32_MSIRANGE not defined in mcuconf.h"
#endif

#if !defined(STM32_MSISRANGE)
#error "STM32_MSISRANGE not defined in mcuconf.h"
#endif

#if !defined(STM32_LSE_ENABLED)
#error "STM32_LSE_ENABLED not defined in mcuconf.h"
#endif

#if STM32_MSIPLL_ENABLED && !STM32_LSE_ENABLED
#error "STM32_MSIPLL_ENABLED requires LSE"
#endif

/**
 * @brief   MSI frequency.
 */
#if STM32_MSIRANGE == STM32_MSIRANGE_100K
  #define STM32_MSICLK              100000
#elif STM32_MSIRANGE == STM32_MSIRANGE_200K
  #define STM32_MSICLK              200000
#elif STM32_MSIRANGE == STM32_MSIRANGE_400K
  #define STM32_MSICLK              400000
#elif STM32_MSIRANGE == STM32_MSIRANGE_800K
  #define STM32_MSICLK              800000
#elif STM32_MSIRANGE == STM32_MSIRANGE_1M
  #define STM32_MSICLK              1000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_2M
  #define STM32_MSICLK              2000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_4M
  #define STM32_MSICLK              4000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_8M
  #define STM32_MSICLK              8000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_16M
  #define STM32_MSICLK              16000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_24M
  #define STM32_MSICLK              24000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_32M
  #define STM32_MSICLK              32000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_48M
  #define STM32_MSICLK              48000000
#else
  #error "invalid STM32_MSIRANGE value specified"
#endif

/**
 * @brief   MSIS frequency.
 */
#if STM32_MSISRANGE == STM32_MSISRANGE_1M
  #define STM32_MSISCLK             1000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_2M
  #define STM32_MSISCLK             2000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_4M
  #define STM32_MSISCLK             4000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_8M
  #define STM32_MSISCLK             8000000
#else
  #error "invalid STM32_MSISRANGE value specified"
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

static inline void msi_init(void) {
  uint32_t cr;

  /* Initial clocks setup and wait for MSI stabilization, the MSI clock is
     always enabled because it is the fall back clock when PLL the fails.
     Trim fields are not altered from reset values.*/

  /* MSIRANGE can be set only when MSI is OFF or READY, it is ready after
     reset.*/
#if STM32_MSIPLL_ENABLED
  cr = STM32_MSIRANGE | RCC_CR_MSIPLLEN | RCC_CR_MSION;
#else
  cr = STM32_MSIRANGE | RCC_CR_MSION;
#endif
  RCC->CR = cr;
  while ((RCC->CR & RCC_CR_MSIRDY) == 0)
    ;                                       /* Wait until MSI is stable.    */

  /* Clocking from MSI, in case MSI was not the default source.*/
  RCC->CFGR = 0;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI)
    ;                                       /* Wait until MSI is selected.  */

  /* Updating MSISRANGE value. MSISRANGE can be set only when MSIRGSEL is high.
     This range is used exiting the Standby mode until MSIRGSEL is set.*/
  cr       |= RCC_CR_MSIRGSEL;
  RCC->CR   = cr;
  RCC->CSR |= STM32_MSISRANGE;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
