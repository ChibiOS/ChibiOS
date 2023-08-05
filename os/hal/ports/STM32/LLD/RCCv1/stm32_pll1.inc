/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file    RCCv1/stm32_pll1.inc
 * @brief   Shared PLL1 handler.
 *
 * @addtogroup STM32_PLL1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on registry.*/
#if !defined(STM32_RCC_HAS_PLL1)
#error "STM32_RCC_HAS_PLL1 not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL1 && !defined(STM32_RCC_PLL1_HAS_P)
#error "STM32_RCC_PLL1_HAS_P not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL1 && !defined(STM32_RCC_PLL1_HAS_Q)
#error "STM32_RCC_PLL1_HAS_Q not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL1 && !defined(STM32_RCC_PLL1_HAS_R)
#error "STM32_RCC_PLL1_HAS_R not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL1

/* Checks on configurations.*/
#if !defined(STM32_PLL1SRC)
#error "STM32_PLL1SRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL1M_VALUE)
#error "STM32_PLL1M_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL1N_VALUE)
#error "STM32_PLL1N_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL1PDIV_VALUE)
#error "STM32_PLL1PDIV_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL1_HAS_P && !defined(STM32_PLL1P_VALUE)
#error "STM32_PLL1P_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL1_HAS_Q && !defined(STM32_PLL1Q_VALUE)
#error "STM32_PLL1Q_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL1_HAS_R && !defined(STM32_PLL1R_VALUE)
#error "STM32_PLL1R_VALUE not defined in mcuconf.h"
#endif

/* Check on limits.*/
#if !defined(STM32_PLL1IN_MAX)
#error "STM32_PLL1IN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1IN_MIN)
#error "STM32_PLL1IN_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1VCO_MAX)
#error "STM32_PLL1VCO_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1VCO_MIN)
#error "STM32_PLL1VCO_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1P_MAX)
#error "STM32_PLL1P_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1P_MIN)
#error "STM32_PLL1P_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1Q_MAX)
#error "STM32_PLL1Q_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1Q_MIN)
#error "STM32_PLL1Q_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1R_MAX)
#error "STM32_PLL1R_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL1R_MIN)
#error "STM32_PLL1R_MIN not defined in hal_lld.h"
#endif

/* Input checks.*/
#if !defined(STM32_ACTIVATE_PLL1)
#error "STM32_ACTIVATE_PLL1 not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL1_HAS_P && !defined(STM32_PLL1PEN)
#error "STM32_PLL1PEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL1_HAS_Q && !defined(STM32_PLL1QEN)
#error "STM32_PLL1QEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL1_HAS_R && !defined(STM32_PLL1REN)
#error "STM32_PLL1REN not defined in hal_lld.h"
#endif

#if STM32_ACTIVATE_PLL1 && (STM32_PLL1CLKIN == 0)
#error "PLL1 activation required but no PLL1 clock selected"
#endif

#if (STM32_PLL1CLKIN != 0) &&                                               \
    ((STM32_PLL1CLKIN < STM32_PLL1IN_MIN) || (STM32_PLL1CLKIN > STM32_PLL1IN_MAX))
#error "STM32_PLL1CLKIN outside acceptable range (STM32_PLL1IN_MIN...STM32_PLL1IN_MAX)"
#endif

/**
 * @brief   STM32_PLL1M field.
 */
#if ((STM32_PLL1M_VALUE >= 1) && (STM32_PLL1M_VALUE <= 16)) ||              \
    defined(__DOXYGEN__)
#define STM32_PLL1M                 ((STM32_PLL1M_VALUE - 1U) << RCC_PLL1CFGR_PLL1M_Pos)

#else
#error "invalid STM32_PLL1M_VALUE value specified"
#endif

/**
 * @brief   STM32_PLL1N field.
 */
#if ((STM32_PLL1N_VALUE >= 8) && (STM32_PLL1N_VALUE <= 127)) ||             \
    defined(__DOXYGEN__)
#define STM32_PLL1N                 (STM32_PLL1N_VALUE << RCC_PLL1CFGR_PLL1N_Pos)

#else
#error "invalid STM32_PLL1N_VALUE value specified"
#endif

/**
 * @brief   PLL1 VCO frequency.
 */
#define STM32_PLL1VCO               (STM32_PLL1CLKIN * STM32_PLL1N_VALUE)

/*
 * PLL1 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL1 &&                                                  \
    ((STM32_PLL1VCO < STM32_PLL1VCO_MIN) || (STM32_PLL1VCO > STM32_PLL1VCO_MAX))
#error "STM32_PLL1VCO outside acceptable range (STM32_PLL1VCO_MIN...STM32_PLL1VCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL1_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL1P field.
 */
#if (STM32_PLL1P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLL1P                 (0U << RCC_PLL1CFGR_PLL1P_Pos)

#elif STM32_PLL1P_VALUE == 17
#define STM32_PLL1P                 (1U << RCC_PLL1CFGR_PLL1P_Pos)

#else
#error "invalid STM32_PLL1P_VALUE value specified"
#endif

/* PDIV is not present on all devices.*/
#if defined(RCC_PLL1CFGR_PLL1PDIV_Pos) || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL1PDIV field.
 */
#if (STM32_PLL1PDIV_VALUE == 0) ||                                          \
    ((STM32_PLL1PDIV_VALUE >= 2) && (STM32_PLL1PDIV_VALUE <= 31)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLL1PDIV              (STM32_PLL1PDIV_VALUE << RCC_PLL1CFGR_PLL1PDIV_Pos)
#else
#error "invalid STM32_PLL1PDIV_VALUE value specified"
#endif

/**
 * @brief   PLL1 P output clock frequency.
 */
#if (STM32_PLL1PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLL1_P_CLKOUT         (STM32_PLL1VCO / STM32_PLL1P_VALUE)
#else
#define STM32_PLL1_P_CLKOUT         (STM32_PLL1VCO / STM32_PLL1PDIV_VALUE)
#endif

#else
#define STM32_PLL1_P_CLKOUT         (STM32_PLL1VCO / STM32_PLL1P_VALUE)
#define STM32_PLL1PDIV              0U
#endif

/*
 * PLL1-P output frequency range check.
 */
#if STM32_ACTIVATE_PLL1 &&                                                  \
    ((STM32_PLL1_P_CLKOUT < STM32_PLL1P_MIN) || (STM32_PLL1_P_CLKOUT > STM32_PLL1P_MAX))
#error "STM32_PLL1_P_CLKOUT outside acceptable range (STM32_PLL1P_MIN...STM32_PLL1P_MAX)"
#endif

#else /* !STM32_RCC_PLL1_HAS_P */
#define STM32_PLL1P                 0U
#define STM32_PLL1PDIV              0U
#endif /* !STM32_RCC_PLL1_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL1_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL1Q field.
 */
#if (STM32_PLL1Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL1Q                 (0U << RCC_PLL1CFGR_PLL1Q_Pos)

#elif STM32_PLL1Q_VALUE == 4
#define STM32_PLL1Q                 (1U << RCC_PLL1CFGR_PLL1Q_Pos)

#elif STM32_PLL1Q_VALUE == 6
#define STM32_PLL1Q                 (2U << RCC_PLL1CFGR_PLL1Q_Pos)

#elif STM32_PLL1Q_VALUE == 8
#define STM32_PLL1Q                 (3U << RCC_PLL1CFGR_PLL1Q_Pos)

#else
#error "invalid STM32_PLL1Q_VALUE value specified"
#endif

/**
 * @brief   PLL1 Q output clock frequency.
 */
#define STM32_PLL1_Q_CLKOUT         (STM32_PLL1VCO / STM32_PLL1Q_VALUE)

/*
 * PLL1-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL1 &&                                                  \
    ((STM32_PLL1_Q_CLKOUT < STM32_PLL1Q_MIN) || (STM32_PLL1_Q_CLKOUT > STM32_PLL1Q_MAX))
#error "STM32_PLL1_Q_CLKOUT outside acceptable range (STM32_PLL1Q_MIN...STM32_PLL1Q_MAX)"
#endif

#else /* !STM32_RCC_PLL1_HAS_Q */
#define STM32_PLL1Q                 0U
#endif /* !STM32_RCC_PLL1_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL1_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL1R field.
 */
#if (STM32_PLL1R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL1R                 (0U << RCC_PLL1CFGR_PLL1R_Pos)

#elif STM32_PLL1R_VALUE == 4
#define STM32_PLL1R                 (1U << RCC_PLL1CFGR_PLL1R_Pos)

#elif STM32_PLL1R_VALUE == 6
#define STM32_PLL1R                 (2U << RCC_PLL1CFGR_PLL1R_Pos)

#elif STM32_PLL1R_VALUE == 8
#define STM32_PLL1R                 (3U << RCC_PLL1CFGR_PLL1R_Pos)

#else
#error "invalid STM32_PLL1R_VALUE value specified"
#endif

/**
 * @brief   PLL1 R output clock frequency.
 */
#define STM32_PLL1_R_CLKOUT         (STM32_PLL1VCO / STM32_PLL1R_VALUE)

/*
 * PLL1-R output frequency range check.
 */
#if STM32_ACTIVATE_PLL1 &&                                                  \
    ((STM32_PLL1_R_CLKOUT < STM32_PLL1R_MIN) || (STM32_PLL1_R_CLKOUT > STM32_PLL1R_MAX))
#error "STM32_PLL1_R_CLKOUT outside acceptable range (STM32_PLL1R_MIN...STM32_PLL1R_MAX)"
#endif

#else /* !STM32_RCC_PLL1_HAS_R */
#define STM32_PLL1R                 0U
#endif /* !STM32_RCC_PLL1_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE bool pll1_not_locked(void) {

  return (bool)((RCC->CR & RCC_CR_PLL1RDY) == 0U);
}

__STATIC_INLINE void pll1_wait_lock(void) {

  while (pll1_not_locked()) {
    /* Waiting for PLL1 lock.*/
  }
}

#endif /* STM32_RCC_HAS_PLL1 */

__STATIC_INLINE void pll1_init(void) {

#if STM32_RCC_HAS_PLL1
#if STM32_ACTIVATE_PLL1
  /* PLL1 activation.*/
  RCC->PLL1CFGR = STM32_PLL1PDIV | STM32_PLL1R   |
                  STM32_PLL1REN  | STM32_PLL1Q   |
                  STM32_PLL1QEN  | STM32_PLL1P   |
                  STM32_PLL1PEN  | STM32_PLL1N   |
                  STM32_PLL1M    | STM32_PLL1SRC;
  RCC->CR |= RCC_CR_PLL1ON;

  pll1_wait_lock();
#endif
#endif
}

__STATIC_INLINE void pll1_deinit(void) {

#if STM32_RCC_HAS_PLL1
#if STM32_ACTIVATE_PLL1
  /* PLL1 de-activation.*/
  RCC->CR &= ~RCC_CR_PLL1ON;
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
