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
 * @file    RCCv2/stm32_pll3.inc
 * @brief   Shared PLL3 handler.
 *
 * @addtogroup STM32_PLL3_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_RCC_HAS_PLL3)
#define STM32_RCC_HAS_PLL3          FALSE
#endif

#if STM32_RCC_HAS_PLL3

/* More checks on registry.*/
#if !defined(STM32_RCC_PLL3_HAS_P)
#error "STM32_RCC_PLL3_HAS_P not defined in registry"
#endif

#if !defined(STM32_RCC_PLL3_HAS_Q)
#error "STM32_RCC_PLL3_HAS_Q not defined in registry"
#endif

#if !defined(STM32_RCC_PLL3_HAS_R)
#error "STM32_RCC_PLL3_HAS_R not defined in registry"
#endif

/* Checks on configurations.*/
#if !defined(STM32_PLL3SRC)
#error "STM32_PLL3SRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL3DIVM_VALUE)
#error "STM32_PLL3DIVM_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL3DIVN_VALUE)
#error "STM32_PLL3DIVN_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3DIVP_VALUE)
#error "STM32_PLL3DIVP_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3DIVQ_VALUE)
#error "STM32_PLL3DIVQ_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3DIVR_VALUE)
#error "STM32_PLL3DIVR_VALUE not defined in mcuconf.h"
#endif

/* Check on limits.*/
#if !defined(STM32_PLL3INCLK_MAX)
#error "STM32_PLL3INCLK_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3INCLK_MIN)
#error "STM32_PLL3INCLK_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3VCOCLK_MAX)
#error "STM32_PLL3VCOCLK_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3VCOCLK_MIN)
#error "STM32_PLL3VCOCLK_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3PCLK_MAX)
#error "STM32_PLL3PCLK_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3PCLK_MIN)
#error "STM32_PLL3PCLK_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3QCLK_MAX)
#error "STM32_PLL3QCLK_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3QCLK_MIN)
#error "STM32_PLL3QCLK_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3RCLK_MAX)
#error "STM32_PLL3RCLK_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3RCLK_MIN)
#error "STM32_PLL3RCLK_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3DIVM_MAX)
#error "STM32_PLL3DIVM_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3DIVM_MIN)
#error "STM32_PLL3DIVM_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3DIVN_MAX)
#error "STM32_PLL3DIVN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3DIVN_MIN)
#error "STM32_PLL3DIVN_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3DIVP_MAX)
#error "STM32_PLL3DIVP_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3DIVP_MIN)
#error "STM32_PLL3DIVP_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3DIVQ_MAX)
#error "STM32_PLL3DIVQ_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3DIVQ_MIN)
#error "STM32_PLL3DIVQ_MIN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3DIVR_MAX)
#error "STM32_PLL3DIVR_MAX not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3DIVR_MIN)
#error "STM32_PLL3DIVR_MIN not defined in hal_lld.h"
#endif

/* Input checks.*/
#if !defined(STM32_ACTIVATE_PLL3)
#error "STM32_ACTIVATE_PLL3 not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3DIVPEN)
#error "STM32_PLL3DIVPEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3DIVQEN)
#error "STM32_PLL3DIVQEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3DIVREN)
#error "STM32_PLL3DIVREN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3MCLK)
#error "STM32_PLL3MCLK not defined in hal_lld.h"
#endif

#if STM32_ACTIVATE_PLL3 && (STM32_PLL3MCLK == 0)
#error "PLL3 activation required but no PLL3 clock selected"
#endif

/**
 * @brief   STM32_PLL3DIVM field.
 */
#if ((STM32_PLL3DIVM_VALUE >= STM32_PLL3DIVM_MIN) &&                        \
     (STM32_PLL3DIVM_VALUE <= STM32_PLL3DIVM_MAX)) ||                       \
    defined(__DOXYGEN__)
#define STM32_PLL3DIVM              ((STM32_PLL3DIVM_VALUE - 1U) << RCC_PLLCFGR_PLL3DIVM_Pos)

#else
#error "invalid STM32_PLL3DIVM_VALUE value specified"
#endif

/**
 * @brief   Clock at the M divider input.
 */
#define STM32_PLL3INCLK             (STM32_PLL3MCLK / STM32_PLL3DIVM_VALUE)

#if (STM32_PLL3INCLK != 0) &&                                               \
    ((STM32_PLL3INCLK < STM32_PLL3INCLK_MIN) || (STM32_PLL3INCLK > STM32_PLL3INCLK_MAX))
#error "STM32_PLL3INCLK outside acceptable range (STM32_PLL3INCLK_MIN...STM32_PLL3INCLK_MAX)"
#endif

/**
 * @brief   STM32_PLL3DIVN field.
 */
#if ((STM32_PLL3DIVN_VALUE >= STM32_PLL3DIVN_MIN) &&                        \
     (STM32_PLL3DIVN_VALUE <= STM32_PLL3DIVN_MAX)) ||                       \
    defined(__DOXYGEN__)
#define STM32_PLL3DIVN              ((STM32_PLL3DIVN_VALUE - 1U) << RCC_PLLCFGR_PLL3DIVN_Pos)

#else
#error "invalid STM32_PLL3DIVN_VALUE value specified"
#endif

/**
 * @brief   PLL VCO frequency.
 */
#define STM32_PLL3VCOCLK            (STM32_PLL3INCLK * STM32_PLL3DIVN_VALUE)

/*
 * PLL VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3VCOCLK < STM32_PLL3VCOCLK_MIN) || (STM32_PLL3VCOCLK > STM32_PLL3VCOCLK_MAX))
#error "STM32_PLL3VCOCLK outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3DIVP field.
 */
#if ((STM32_PLL3DIVP_VALUE >= STM32_PLL3DIVP_MIN) &&                        \
     (STM32_PLL3DIVP_VALUE <= STM32_PLL3DIVP_MAX)) ||                       \
    defined(__DOXYGEN__)
#define STM32_PLL3DIVP              ((STM32_PLL3DIVP_VALUE - 1) << RCC_PLL3CFGR2_DIVP_Pos)
#else
#error "invalid STM32_PLL3DIVP_VALUE value specified"
#endif

/**
 * @brief   PLL3 P output clock frequency.
 */
#define STM32_PLL3_P_CLKOUT         (STM32_PLL3VCOCLK / STM32_PLL3DIVP_VALUE)

/*
 * PLL3 P output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_P_CLKOUT < STM32_PLL3PCLK_MIN) ||                          \
     (STM32_PLL3_P_CLKOUT > STM32_PLL3PCLK_MAX))
#error "STM32_PLL3_P_CLKOUT outside acceptable range (STM32_PLL3PCLK_MIN...STM32_PLL3PCLK_MAX)"
#endif

#else /* !STM32_RCC_PLL_HAS_P */
#define STM32_PLL3DIVP              0
#endif /* !STM32_RCC_PLL_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3DIVQ field.
 */
#if ((STM32_PLL3DIVQ_VALUE >= STM32_PLL3DIVQ_MIN) &&                        \
     (STM32_PLL3DIVQ_VALUE <= STM32_PLL3DIVQ_MAX)) ||                       \
    defined(__DOXYGEN__)
#define STM32_PLL3DIVQ              ((STM32_PLL3DIVQ_VALUE - 1) << RCC_PLL3CFGR2_DIVQ_Pos)
#else
#error "invalid STM32_PLL3DIVQ_VALUE value specified"
#endif

/**
 * @brief   PLL3 Q output clock frequency.
 */
#define STM32_PLL3_Q_CLKOUT         (STM32_PLL3VCOCLK / STM32_PLL3DIVQ_VALUE)

/*
 * PLL3 Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_Q_CLKOUT < STM32_PLL3QCLK_MIN) ||                          \
     (STM32_PLL3_Q_CLKOUT > STM32_PLL3QCLK_MAX))
#error "STM32_PLL3_Q_CLKOUT outside acceptable range (STM32_PLL3QCLK_MIN...STM32_PLL3QCLK_MAX)"
#endif

#else /* !STM32_RCC_PLL_HAS_Q */
#define STM32_PLL3DIVQ              0
#endif /* !STM32_RCC_PLL_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3DIVQ field.
 */
#if ((STM32_PLL3DIVR_VALUE >= STM32_PLL3DIVR_MIN) &&                        \
     (STM32_PLL3DIVR_VALUE <= STM32_PLL3DIVR_MAX)) ||                       \
    defined(__DOXYGEN__)
#define STM32_PLL3DIVR              ((STM32_PLL3DIVR_VALUE - 1) << RCC_PLL3CFGR2_DIVR_Pos)
#else
#error "invalid STM32_PLL3DIVR_VALUE value specified"
#endif

/**
 * @brief   PLL3 R output clock frequency.
 */
#define STM32_PLL3_R_CLKOUT         (STM32_PLL3VCOCLK / STM32_PLL3DIVR_VALUE)

/*
 * PLL3 R output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_R_CLKOUT < STM32_PLL3RCLK_MIN) ||                          \
     (STM32_PLL3_R_CLKOUT > STM32_PLL3RCLK_MAX))
#error "STM32_PLL3_R_CLKOUT outside acceptable range (STM32_PLL3RCLK_MIN...STM32_PLL3RCLK_MAX)"
#endif

#else /* !STM32_RCC_PLL_HAS_R */
#define STM32_PLL3DIVR              0
#endif /* !STM32_RCC_PLL_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if 0
__STATIC_INLINE bool pll_not_locked(void) {

  return (bool)((RCC->CR & RCC_CR_PLLRDY) == 0U);
}

__STATIC_INLINE void pll_wait_lock(void) {

  while (pll_not_locked()) {
    /* Waiting for PLL lock.*/
  }
}
#endif

#endif /* STM32_RCC_HAS_PLL */

__STATIC_INLINE void pll_init(void) {

#if STM32_RCC_HAS_PLL3
#if STM32_ACTIVATE_PLL3
  /* PLL activation.*/
#if 0
  RCC->PLLCFGR = STM32_PLLPDIV | STM32_PLLR   |
                 STM32_PLLREN  | STM32_PLLQ   |
                 STM32_PLLQEN  | STM32_PLLP   |
                 STM32_PLLPEN  | STM32_PLLN   |
                 STM32_PLLM    | STM32_PLLSRC;
  RCC->CR |= RCC_CR_PLLON;

  pll_wait_lock();
#endif
#endif
#endif
}

__STATIC_INLINE void pll_deinit(void) {

#if STM32_RCC_HAS_PLL3
#if STM32_ACTIVATE_PLL3
  /* PLL de-activation.*/
#if 0
  RCC->PLLCFGR &= ~RCC_CR_PLLON;
#endif
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
