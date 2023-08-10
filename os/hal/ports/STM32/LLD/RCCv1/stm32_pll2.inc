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
 * @file    RCCv1/stm32_pll2.inc
 * @brief   Shared PLL2 handler.
 *
 * @addtogroup STM32_PLL2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on registry.*/
#if !defined(STM32_RCC_HAS_PLL2)
#error "STM32_RCC_HAS_PLL2 not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL2 && !defined(STM32_RCC_PLL2_HAS_P)
#error "STM32_RCC_PLL2_HAS_P not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL2 && !defined(STM32_RCC_PLL2_HAS_Q)
#error "STM32_RCC_PLL2_HAS_Q not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL2 && !defined(STM32_RCC_PLL2_HAS_R)
#error "STM32_RCC_PLL2_HAS_R not defined in stm32_registry.h"
#endif

#if STM32_RCC_HAS_PLL2

/* Checks on configurations.*/
#if !defined(STM32_PLL2SRC)
#error "STM32_PLL2SRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL2M_VALUE)
#error "STM32_PLL2M_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLL2N_VALUE)
#error "STM32_PLL2N_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL2_HAS_P && !defined(STM32_PLL2P_VALUE)
#error "STM32_PLL2P_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL2_HAS_Q && !defined(STM32_PLL2Q_VALUE)
#error "STM32_PLL2Q_VALUE not defined in mcuconf.h"
#endif

#if STM32_RCC_PLL2_HAS_R && !defined(STM32_PLL2R_VALUE)
#error "STM32_PLL2R_VALUE not defined in mcuconf.h"
#endif

/* Check on limits.*/
#if !defined(STM32_PLL2IN_MAX)
#error "STM32_PLL2IN_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2IN_MIN)
#error "STM32_PLL2IN_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2VCO_MAX)
#error "STM32_PLL2VCO_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2VCO_MIN)
#error "STM32_PLL2VCO_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2P_MAX)
#error "STM32_PLL2P_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2P_MIN)
#error "STM32_PLL2P_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2Q_MAX)
#error "STM32_PLL2Q_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2Q_MIN)
#error "STM32_PLL2Q_MIN not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2R_MAX)
#error "STM32_PLL2R_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_PLL2R_MIN)
#error "STM32_PLL2R_MIN not defined in hal_lld.h"
#endif

/* Input checks.*/
#if !defined(STM32_ACTIVATE_PLL2)
#error "STM32_ACTIVATE_PLL2 not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL2_HAS_P && !defined(STM32_PLL2PEN)
#error "STM32_PLL2PEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL2_HAS_Q && !defined(STM32_PLL2QEN)
#error "STM32_PLL2QEN not defined in hal_lld.h"
#endif

#if STM32_RCC_PLL2_HAS_R && !defined(STM32_PLL2REN)
#error "STM32_PLL2REN not defined in hal_lld.h"
#endif

#if STM32_ACTIVATE_PLL2 && (STM32_PLL2CLKIN == 0)
#error "PLL2 activation required but no PLL2 clock selected"
#endif

#if (STM32_PLL2CLKIN != 0) &&                                               \
    ((STM32_PLL2CLKIN < STM32_PLL2IN_MIN) || (STM32_PLL2CLKIN > STM32_PLL2IN_MAX))
#error "STM32_PLL2CLKIN outside acceptable range (STM32_PLL2IN_MIN...STM32_PLL2IN_MAX)"
#endif

/**
 * @brief   STM32_PLL2M field.
 */
#if ((STM32_PLL2M_VALUE >= 1) && (STM32_PLL2M_VALUE <= 16)) ||              \
    defined(__DOXYGEN__)
#define STM32_PLL2M                 ((STM32_PLL2M_VALUE - 1U) << RCC_PLL2CFGR_PLL2M_Pos)

#else
#error "invalid STM32_PLL2M_VALUE value specified"
#endif

/**
 * @brief   STM32_PLL2N field.
 */
#if ((STM32_PLL2N_VALUE >= 8) && (STM32_PLL2N_VALUE <= 127)) ||             \
    defined(__DOXYGEN__)
#define STM32_PLL2N                 (STM32_PLL2N_VALUE << RCC_PLL2CFGR_PLL2N_Pos)

#else
#error "invalid STM32_PLL2N_VALUE value specified"
#endif

/**
 * @brief   PLL2 VCO frequency.
 */
#define STM32_PLL2VCO               (STM32_PLL2CLKIN * STM32_PLL2N_VALUE)

/*
 * PLL2 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL2 &&                                                  \
    ((STM32_PLL2VCO < STM32_PLL2VCO_MIN) || (STM32_PLL2VCO > STM32_PLL2VCO_MAX))
#error "STM32_PLL2VCO outside acceptable range (STM32_PLL2VCO_MIN...STM32_PLL2VCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL2_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL2P field.
 */
#if (STM32_PLL2P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLL2P                 (0U << RCC_PLL2CFGR_PLL2P_Pos)

#elif STM32_PLL2P_VALUE == 17
#define STM32_PLL2P                 (1U << RCC_PLL2CFGR_PLL2P_Pos)

#else
#error "invalid STM32_PLL2P_VALUE value specified"
#endif

/* PDIV is not present on all devices.*/
#if defined(RCC_PLL2CFGR_PLL2PDIV_Pos) || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL2PDIV field.
 */
#if (STM32_PLL2PDIV_VALUE == 0) ||                                          \
    ((STM32_PLL2PDIV_VALUE >= 2) && (STM32_PLL2PDIV_VALUE <= 31)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLL2PDIV              (STM32_PLL2PDIV_VALUE << RCC_PLL2CFGR_PLL2PDIV_Pos)
#else
#error "invalid STM32_PLL2PDIV_VALUE value specified"
#endif

/**
 * @brief   PLL2 P output clock frequency.
 */
#if (STM32_PLL2PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLL2_P_CLKOUT         (STM32_PLL2VCO / STM32_PLL2P_VALUE)
#else
#define STM32_PLL2_P_CLKOUT         (STM32_PLL2VCO / STM32_PLL2PDIV_VALUE)
#endif

#else
#define STM32_PLL2_P_CLKOUT         (STM32_PLL2VCO / STM32_PLL2P_VALUE)
#define STM32_PLL2PDIV              0U
#endif

/*
 * PLL2-P output frequency range check.
 */
#if STM32_ACTIVATE_PLL2 &&                                                  \
    ((STM32_PLL2_P_CLKOUT < STM32_PLL2P_MIN) || (STM32_PLL2_P_CLKOUT > STM32_PLL2P_MAX))
#error "STM32_PLL2_P_CLKOUT outside acceptable range (STM32_PLL2P_MIN...STM32_PLL2P_MAX)"
#endif

#else /* !STM32_RCC_PLL2_HAS_P */
#define STM32_PLL2P                 0U
#define STM32_PLL2PDIV              0U
#endif /* !STM32_RCC_PLL2_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL2_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL2Q field.
 */
#if (STM32_PLL2Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL2Q                 (0U << RCC_PLL2CFGR_PLL2Q_Pos)

#elif STM32_PLL2Q_VALUE == 4
#define STM32_PLL2Q                 (1U << RCC_PLL2CFGR_PLL2Q_Pos)

#elif STM32_PLL2Q_VALUE == 6
#define STM32_PLL2Q                 (2U << RCC_PLL2CFGR_PLL2Q_Pos)

#elif STM32_PLL2Q_VALUE == 8
#define STM32_PLL2Q                 (3U << RCC_PLL2CFGR_PLL2Q_Pos)

#else
#error "invalid STM32_PLL2Q_VALUE value specified"
#endif

/**
 * @brief   PLL2 Q output clock frequency.
 */
#define STM32_PLL2_Q_CLKOUT         (STM32_PLL2VCO / STM32_PLL2Q_VALUE)

/*
 * PLL2-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL2 &&                                                  \
    ((STM32_PLL2_Q_CLKOUT < STM32_PLL2Q_MIN) || (STM32_PLL2_Q_CLKOUT > STM32_PLL2Q_MAX))
#error "STM32_PLL2_Q_CLKOUT outside acceptable range (STM32_PLL2Q_MIN...STM32_PLL2Q_MAX)"
#endif

#else /* !STM32_RCC_PLL2_HAS_Q */
#define STM32_PLL2Q                 0U
#endif /* !STM32_RCC_PLL2_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_RCC_PLL2_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLL2R field.
 */
#if (STM32_PLL2R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLL2R                 (0U << RCC_PLL2CFGR_PLL2R_Pos)

#elif STM32_PLL2R_VALUE == 4
#define STM32_PLL2R                 (1U << RCC_PLL2CFGR_PLL2R_Pos)

#elif STM32_PLL2R_VALUE == 6
#define STM32_PLL2R                 (2U << RCC_PLL2CFGR_PLL2R_Pos)

#elif STM32_PLL2R_VALUE == 8
#define STM32_PLL2R                 (3U << RCC_PLL2CFGR_PLL2R_Pos)

#else
#error "invalid STM32_PLL2R_VALUE value specified"
#endif

/**
 * @brief   PLL2 R output clock frequency.
 */
#define STM32_PLL2_R_CLKOUT         (STM32_PLL2VCO / STM32_PLL2R_VALUE)

/*
 * PLL2-R output frequency range check.
 */
#if STM32_ACTIVATE_PLL2 &&                                                  \
    ((STM32_PLL2_R_CLKOUT < STM32_PLL2R_MIN) || (STM32_PLL2_R_CLKOUT > STM32_PLL2R_MAX))
#error "STM32_PLL2_R_CLKOUT outside acceptable range (STM32_PLL2R_MIN...STM32_PLL2R_MAX)"
#endif

#else /* !STM32_RCC_PLL2_HAS_R */
#define STM32_PLL2R                 0U
#endif /* !STM32_RCC_PLL2_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE bool pll2_not_locked(void) {

  return (bool)((RCC->CR & RCC_CR_PLL2RDY) == 0U);
}

__STATIC_INLINE void pll2_wait_lock(void) {

  while (pll2_not_locked()) {
    /* Waiting for PLL2 lock.*/
  }
}

#endif /* STM32_RCC_HAS_PLL2 */

__STATIC_INLINE void pll2_init(void) {

#if STM32_RCC_HAS_PLL2
#if STM32_ACTIVATE_PLL2
  /* PLL2 activation.*/
  RCC->PLL2CFGR = STM32_PLL2PDIV | STM32_PLL2R   |
                  STM32_PLL2REN  | STM32_PLL2Q   |
                  STM32_PLL2QEN  | STM32_PLL2P   |
                  STM32_PLL2PEN  | STM32_PLL2N   |
                  STM32_PLL2M    | STM32_PLL2SRC;
  RCC->CR |= RCC_CR_PLL2ON;

  pll2_wait_lock();
#endif
#endif
}

__STATIC_INLINE void pll2_deinit(void) {

#if STM32_RCC_HAS_PLL2
#if STM32_ACTIVATE_PLL2
  /* PLL2 de-activation.*/
  RCC->CR &= ~RCC_CR_PLL2ON;
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
