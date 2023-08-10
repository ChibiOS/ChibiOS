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
 * @file    RCCv1/stm32_pll3.inc
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

/* Checks on registry.*/
#if !defined(STM32_RCC_HAS_PLL3)
#error "STM32_RCC_HAS_PLL3 not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL3 && !defined(STM32_RCC_PLL3_HAS_P)
#error "STM32_RCC_PLL3_HAS_P not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL3 && !defined(STM32_RCC_PLL3_HAS_Q)
#error "STM32_RCC_PLL3_HAS_Q not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL3 && !defined(STM32_RCC_PLL3_HAS_R)
#error "STM32_RCC_PLL3_HAS_R not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL3

/* Checks on configurations.*/
#if !defined(STM32_PLL3SRC)
#error "STM32_PLL3SRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL3M_VALUE)
#error "STM32_PLL3M_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL3N_VALUE)
#error "STM32_PLL3N_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3P_VALUE)
#error "STM32_PLL3P_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3Q_VALUE)
#error "STM32_PLL3Q_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3R_VALUE)
#error "STM32_PLL3R_VALUE not defined in mcuconf.h"
#endif

/* Check on limits.*/
#if !defined(STM32_PLL3IN_MAX)
#error "STM32_PLL3IN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3IN_MIN)
#error "STM32_PLL3IN_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3VCO_MAX)
#error "STM32_PLL3VCO_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3VCO_MIN)
#error "STM32_PLL3VCO_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3P_MAX)
#error "STM32_PLL3P_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3P_MIN)
#error "STM32_PLL3P_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3Q_MAX)
#error "STM32_PLL3Q_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3Q_MIN)
#error "STM32_PLL3Q_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3R_MAX)
#error "STM32_PLL3R_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL3R_MIN)
#error "STM32_PLL3R_MIN not defined in hal_lld.h"
#endif

/* Input checks.*/
#if !defined(STM32_ACTIVATE_PLL3)
#error "STM32_ACTIVATE_PLL3 not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_P && !defined(STM32_PLL3PEN)
#error "STM32_PLL3PEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_Q && !defined(STM32_PLL3QEN)
#error "STM32_PLL3QEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL3_HAS_R && !defined(STM32_PLL3REN)
#error "STM32_PLL3REN not defined in hal_lld.h"
#endif

#if STM32_ACTIVATE_PLL3 && (STM32_PLL3CLKIN == 0)
#error "PLL3 activation required but no PLL3 clock selected"
#endif

#if (STM32_PLL3CLKIN != 0) &&                                               \
    ((STM32_PLL3CLKIN < STM32_PLL3IN_MIN) || (STM32_PLL3CLKIN > STM32_PLL3IN_MAX))
#error "STM32_PLL3CLKIN outside acceptable range (STM32_PLL3IN_MIN...STM32_PLL3IN_MAX)"
#endif

/**
 * @brief   STM32_PLL3M field.
 */
#if ((STM32_PLL3M_VALUE >= 1) && (STM32_PLL3M_VALUE <= 16)) ||              \
    defined(__DOXYGEN__)
#define STM32_PLL3M                 ((STM32_PLL3M_VALUE - 1U) << RCC_PLL3CFGR_PLL3M_Pos)

#else
#error "invalid STM32_PLL3M_VALUE value specified"
#endif

/**
 * @brief   STM32_PLL3N field.
 */
#if ((STM32_PLL3N_VALUE >= 8) && (STM32_PLL3N_VALUE <= 127)) ||             \
    defined(__DOXYGEN__)
#define STM32_PLL3N                 (STM32_PLL3N_VALUE << RCC_PLL3CFGR_PLL3N_Pos)

#else
#error "invalid STM32_PLL3N_VALUE value specified"
#endif

/**
 * @brief   PLL3 VCO frequency.
 */
#define STM32_PLL3VCO               (STM32_PLL3CLKIN * STM32_PLL3N_VALUE)

/*
 * PLL3 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3VCO < STM32_PLL3VCO_MIN) || (STM32_PLL3VCO > STM32_PLL3VCO_MAX))
#error "STM32_PLL3VCO outside acceptable range (STM32_PLL3VCO_MIN...STM32_PLL3VCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3P field.
 */
#if (STM32_PLL3P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLL3P                 (0U << RCC_PLL3CFGR_PLL3P_Pos)

#elif STM32_PLL3P_VALUE == 17
#define STM32_PLL3P                 (1U << RCC_PLL3CFGR_PLL3P_Pos)

#else
#error "invalid STM32_PLL3P_VALUE value specified"
#endif

/* PDIV is not present on all devices.*/
#if defined(RCC_PLL3CFGR_PLL3PDIV_Pos) || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3PDIV field.
 */
#if (STM32_PLL3PDIV_VALUE == 0) ||                                          \
    ((STM32_PLL3PDIV_VALUE >= 2) && (STM32_PLL3PDIV_VALUE <= 31)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLL3PDIV              (STM32_PLL3PDIV_VALUE << RCC_PLL3CFGR_PLL3PDIV_Pos)
#else
#error "invalid STM32_PLL3PDIV_VALUE value specified"
#endif

/**
 * @brief   PLL3 P output clock frequency.
 */
#if (STM32_PLL3PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLL3_P_CLKOUT         (STM32_PLL3VCO / STM32_PLL3P_VALUE)
#else
#define STM32_PLL3_P_CLKOUT         (STM32_PLL3VCO / STM32_PLL3PDIV_VALUE)
#endif

#else
#define STM32_PLL3_P_CLKOUT         (STM32_PLL3VCO / STM32_PLL3P_VALUE)
#define STM32_PLL3PDIV              0U
#endif

/*
 * PLL3-P output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_P_CLKOUT < STM32_PLL3P_MIN) || (STM32_PLL3_P_CLKOUT > STM32_PLL3P_MAX))
#error "STM32_PLL3_P_CLKOUT outside acceptable range (STM32_PLL3P_MIN...STM32_PLL3P_MAX)"
#endif

#else /* !STM32_RCC_PLL3_HAS_P */
#define STM32_PLL3P                 0U
#define STM32_PLL3PDIV              0U
#endif /* !STM32_RCC_PLL3_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3Q field.
 */
#if (STM32_PLL3Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL3Q                 (0U << RCC_PLL3CFGR_PLL3Q_Pos)

#elif STM32_PLL3Q_VALUE == 4
#define STM32_PLL3Q                 (1U << RCC_PLL3CFGR_PLL3Q_Pos)

#elif STM32_PLL3Q_VALUE == 6
#define STM32_PLL3Q                 (2U << RCC_PLL3CFGR_PLL3Q_Pos)

#elif STM32_PLL3Q_VALUE == 8
#define STM32_PLL3Q                 (3U << RCC_PLL3CFGR_PLL3Q_Pos)

#else
#error "invalid STM32_PLL3Q_VALUE value specified"
#endif

/**
 * @brief   PLL3 Q output clock frequency.
 */
#define STM32_PLL3_Q_CLKOUT         (STM32_PLL3VCO / STM32_PLL3Q_VALUE)

/*
 * PLL3-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_Q_CLKOUT < STM32_PLL3Q_MIN) || (STM32_PLL3_Q_CLKOUT > STM32_PLL3Q_MAX))
#error "STM32_PLL3_Q_CLKOUT outside acceptable range (STM32_PLL3Q_MIN...STM32_PLL3Q_MAX)"
#endif

#else /* !STM32_RCC_PLL3_HAS_Q */
#define STM32_PLL3Q                 0U
#endif /* !STM32_RCC_PLL3_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL3_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL3R field.
 */
#if (STM32_PLL3R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL3R                 (0U << RCC_PLL3CFGR_PLL3R_Pos)

#elif STM32_PLL3R_VALUE == 4
#define STM32_PLL3R                 (1U << RCC_PLL3CFGR_PLL3R_Pos)

#elif STM32_PLL3R_VALUE == 6
#define STM32_PLL3R                 (2U << RCC_PLL3CFGR_PLL3R_Pos)

#elif STM32_PLL3R_VALUE == 8
#define STM32_PLL3R                 (3U << RCC_PLL3CFGR_PLL3R_Pos)

#else
#error "invalid STM32_PLL3R_VALUE value specified"
#endif

/**
 * @brief   PLL3 R output clock frequency.
 */
#define STM32_PLL3_R_CLKOUT         (STM32_PLL3VCO / STM32_PLL3R_VALUE)

/*
 * PLL3-R output frequency range check.
 */
#if STM32_ACTIVATE_PLL3 &&                                                  \
    ((STM32_PLL3_R_CLKOUT < STM32_PLL3R_MIN) || (STM32_PLL3_R_CLKOUT > STM32_PLL3R_MAX))
#error "STM32_PLL3_R_CLKOUT outside acceptable range (STM32_PLL3R_MIN...STM32_PLL3R_MAX)"
#endif

#else /* !STM32_RCC_PLL3_HAS_R */
#define STM32_PLL3R                 0U
#endif /* !STM32_RCC_PLL3_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE bool pll3_not_locked(void) {

  return (bool)((RCC->CR & RCC_CR_PLL3RDY) == 0U);
}

__STATIC_INLINE void pll3_wait_lock(void) {

  while (pll3_not_locked()) {
    /* Waiting for PLL3 lock.*/
  }
}

#endif /* STM32_RCC_HAS_PLL3 */

__STATIC_INLINE void pll3_init(void) {

#if STM32_RCC_HAS_PLL3
#if STM32_ACTIVATE_PLL3
  /* PLL3 activation.*/
  RCC->PLL3CFGR = STM32_PLL3PDIV | STM32_PLL3R   |
                  STM32_PLL3REN  | STM32_PLL3Q   |
                  STM32_PLL3QEN  | STM32_PLL3P   |
                  STM32_PLL3PEN  | STM32_PLL3N   |
                  STM32_PLL3M    | STM32_PLL3SRC;
  RCC->CR |= RCC_CR_PLL3ON;

  pll3_wait_lock();
#endif
#endif
}

__STATIC_INLINE void pll3_deinit(void) {

#if STM32_RCC_HAS_PLL3
#if STM32_ACTIVATE_PLL3
  /* PLL3 de-activation.*/
  RCC->CR &= ~RCC_CR_PLL3ON;
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
