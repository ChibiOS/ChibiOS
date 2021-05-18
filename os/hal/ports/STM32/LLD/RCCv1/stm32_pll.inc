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
 * @file    RCCv1/stm32_pll.inc
 * @brief   Shared PLL handler.
 *
 * @addtogroup STM32_PLL_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_PLL)
#define STM32_HAS_PLL           FALSE
#endif

#if STM32_HAS_PLL

/* Checks on configurations.*/
#if !defined(STM32_PLLSRC)
#error "STM32_PLLSRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLM_VALUE)
#error "STM32_PLLM_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLN_VALUE)
#error "STM32_PLLN_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLPDIV_VALUE)
#error "STM32_PLLPDIV_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLL_HAS_P && !defined(STM32_PLLP_VALUE)
#error "STM32_PLLP_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLL_HAS_Q && !defined(STM32_PLLQ_VALUE)
#error "STM32_PLLQ_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLL_HAS_R && !defined(STM32_PLLR_VALUE)
#error "STM32_PLLR_VALUE not defined in mcuconf.h"
#endif

/* Check on limits.*/
#if !defined(STM32_PLLIN_MAX)
#error "STM32_PLLIN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLIN_MIN)
#error "STM32_PLLIN_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLVCO_MAX)
#error "STM32_PLLIN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLVCO_MIN)
#error "STM32_PLLIN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLP_MAX)
#error "STM32_PLLP_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLP_MIN)
#error "STM32_PLLP_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLQ_MAX)
#error "STM32_PLLQ_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLQ_MIN)
#error "STM32_PLLQ_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLR_MAX)
#error "STM32_PLLR_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLLR_MIN)
#error "STM32_PLLR_MIN not defined in hal_lld.h"
#endif

/* Input checks.*/
#if !defined(STM32_ACTIVATE_PLL)
#error "STM32_ACTIVATE_PLL not defined in hal_lld.h"
#endif

#if STM32_PLL_HAS_P && !defined(STM32_PLLPEN)
#error "STM32_PLLPEN not defined in hal_lld.h"
#endif

#if STM32_PLL_HAS_Q && !defined(STM32_PLLQEN)
#error "STM32_PLLQEN not defined in hal_lld.h"
#endif

#if STM32_PLL_HAS_R && !defined(STM32_PLLREN)
#error "STM32_PLLREN not defined in hal_lld.h"
#endif

#if (STM32_PLLCLKIN != 0) &&                                                \
    ((STM32_PLLCLKIN < STM32_PLLIN_MIN) || (STM32_PLLCLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLCLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/**
 * @brief   STM32_PLLM field.
 */
#if ((STM32_PLLM_VALUE >= 1) && (STM32_PLLM_VALUE <= 16)) ||                \
    defined(__DOXYGEN__)
#define STM32_PLLM                  ((STM32_PLLM_VALUE - 1) << 4)

#else
#error "invalid STM32_PLLM_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLN field.
 */
#if ((STM32_PLLN_VALUE >= 8) && (STM32_PLLN_VALUE <= 127)) ||               \
    defined(__DOXYGEN__)
#define STM32_PLLN                  (STM32_PLLN_VALUE << 8)

#else
#error "invalid STM32_PLLN_VALUE value specified"
#endif

/**
 * @brief   PLL VCO frequency.
 */
#define STM32_PLLVCO                (STM32_PLLCLKIN * STM32_PLLN_VALUE)

/*
 * PLL VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLLVCO < STM32_PLLVCO_MIN) || (STM32_PLLVCO > STM32_PLLVCO_MAX))
#error "STM32_PLLVCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLL_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   PLL P output clock frequency.
 */
#if (STM32_PLLPDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLL_P_CLKOUT          (STM32_PLLVCO / STM32_PLLP_VALUE)
#else
#define STM32_PLL_P_CLKOUT          (STM32_PLLVCO / STM32_PLLPDIV_VALUE)
#endif

/**
 * @brief   STM32_PLLP field.
 */
#if (STM32_PLLP_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLP                  (0 << 17)

#elif STM32_PLLP_VALUE == 17
#define STM32_PLLP                  (1 << 17)

#else
#error "invalid STM32_PLLP_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLPDIV field.
 */
#if (STM32_PLLPDIV_VALUE == 0) ||                                           \
    ((STM32_PLLPDIV_VALUE >= 2) && (STM32_PLLPDIV_VALUE <= 31)) ||          \
    defined(__DOXYGEN__)
#define STM32_PLLPDIV               (STM32_PLLPDIV_VALUE << 27)

#else
#error "invalid STM32_PLLPDIV_VALUE value specified"
#endif

/*
 * PLL-P output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLL_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLL_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

#else /* !STM32_PLL_HAS_P */
#define STM32_PLLP                  0
#define STM32_PLLPEN                0
#endif /* !STM32_PLL_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLL_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLQ field.
 */
#if (STM32_PLLQ_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLQ                  (0 << 21)

#elif STM32_PLLQ_VALUE == 4
#define STM32_PLLQ                  (1 << 21)

#elif STM32_PLLQ_VALUE == 6
#define STM32_PLLQ                  (2 << 21)

#elif STM32_PLLQ_VALUE == 8
#define STM32_PLLQ                  (3 << 21)

#else
#error "invalid STM32_PLLQ_VALUE value specified"
#endif

/**
 * @brief   PLL Q output clock frequency.
 */
#define STM32_PLL_Q_CLKOUT          (STM32_PLLVCO / STM32_PLLQ_VALUE)

/*
 * PLL-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_Q_CLKOUT < STM32_PLLQ_MIN) || (STM32_PLL_Q_CLKOUT > STM32_PLLQ_MAX))
#error "STM32_PLL_Q_CLKOUT outside acceptable range (STM32_PLLQ_MIN...STM32_PLLQ_MAX)"
#endif

#else /* !STM32_PLL_HAS_Q */
#define STM32_PLLQ                  0
#define STM32_PLLQEN                0
#endif /* !STM32_PLL_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLL_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLR field.
 */
#if (STM32_PLLR_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLR                  (0 << 25)

#elif STM32_PLLR_VALUE == 4
#define STM32_PLLR                  (1 << 25)

#elif STM32_PLLR_VALUE == 6
#define STM32_PLLR                  (2 << 25)

#elif STM32_PLLR_VALUE == 8
#define STM32_PLLR                  (3 << 25)

#else
#error "invalid STM32_PLLR_VALUE value specified"
#endif

/**
 * @brief   PLL R output clock frequency.
 */
#define STM32_PLL_R_CLKOUT          (STM32_PLLVCO / STM32_PLLR_VALUE)

/*
 * PLL-R output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLL_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLL_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

#else /* !STM32_PLL_HAS_R */
#define STM32_PLLR                  0
#define STM32_PLLREN                0
#endif /* !STM32_PLL_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void pll_init(void) {

#if STM32_ACTIVATE_PLL
  /* PLL activation.*/
  RCC->PLLCFGR = STM32_PLLPDIV | STM32_PLLR   |
                 STM32_PLLREN  | STM32_PLLQ   |
                 STM32_PLLQEN  | STM32_PLLP   |
                 STM32_PLLPEN  | STM32_PLLN   |
                 STM32_PLLM    | STM32_PLLSRC;
  RCC->CR |= RCC_CR_PLLON;

  /* Waiting for PLL lock.*/
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ;
#endif
}

static inline void pll_deinit(void) {

  /* PLL de-activation.*/
  RCC->PLLCFGR &= ~RCC_CR_PLLON;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* STM32_HAS_PLL */

/** @} */
