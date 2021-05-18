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
 * @file    RCCv1/stm32_pllsai1.inc
 * @brief   Shared PLLSAI1 handler.
 *
 * @addtogroup STM32_PLLSAI1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_PLLSAI1)
#define STM32_HAS_PLLSAI1           FALSE
#endif

#if STM32_HAS_PLLSAI1

/* Checks on configurations.*/
#if !defined(STM32_PLLSRC)
#error "STM32_PLLSRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI1M_VALUE)
#error "STM32_PLLSAI1M_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI1N_VALUE)
#error "STM32_PLLSAI1N_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI1PDIV_VALUE)
#error "STM32_PLLSAI1PDIV_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI1_HAS_P && !defined(STM32_PLLSAI1P_VALUE)
#error "STM32_PLLSAI1P_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI1_HAS_Q && !defined(STM32_PLLSAI1Q_VALUE)
#error "STM32_PLLSAI1Q_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI1_HAS_R && !defined(STM32_PLLSAI1R_VALUE)
#error "STM32_PLLSAI1R_VALUE not defined in mcuconf.h"
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
#if !defined(STM32_ACTIVATE_PLLSAI1)
#error "STM32_ACTIVATE_PLLSAI1 not defined in hal_lld.h"
#endif


#if STM32_PLLSAI1_HAS_P && !defined(STM32_PLLSAI1PEN)
#error "STM32_PLLSAI1PEN not defined in hal_lld.h"
#endif

#if STM32_PLLSAI1_HAS_Q && !defined(STM32_PLLSAI1QEN)
#error "STM32_PLLSAI1QEN not defined in hal_lld.h"
#endif

#if STM32_PLLSAI1_HAS_R && !defined(STM32_PLLSAI1REN)
#error "STM32_PLLSAI1REN not defined in hal_lld.h"
#endif

#if (STM32_PLLSAI1CLKIN != 0) &&                                                \
    ((STM32_PLLSAI1CLKIN < STM32_PLLIN_MIN) || (STM32_PLLSAI1CLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLSAI1CLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/**
 * @brief   STM32_PLLSAI1M field.
 */
#if ((STM32_PLLSAI1M_VALUE >= 1) && (STM32_PLLSAI1M_VALUE <= 16)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1M              ((STM32_PLLSAI1M_VALUE - 1) << 4)

#else
#error "invalid STM32_PLLSAI1M_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1N field.
 */
#if ((STM32_PLLSAI1N_VALUE >= 8) && (STM32_PLLSAI1N_VALUE <= 127)) ||       \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1N              (STM32_PLLSAI1N_VALUE << 8)

#else
#error "invalid STM32_PLLSAI1N_VALUE value specified"
#endif

/**
 * @brief   PLLSAI1 VCO frequency.
 */
#define STM32_PLLSAI1VCO            (STM32_PLLSAI1CLKIN * STM32_PLLSAI1N_VALUE)

/*
 * PLLSAI1 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                               \
    ((STM32_PLLSAI1VCO < STM32_PLLVCO_MIN) || (STM32_PLLSAI1VCO > STM32_PLLVCO_MAX))
#error "STM32_PLLSAI1VCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI1_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   PLLSAI1 P output clock frequency.
 */
#if (STM32_PLLSAI1PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLLSAI1_P_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1P_VALUE)
#else
#define STM32_PLLSAI1_P_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1PDIV_VALUE)
#endif

/**
 * @brief   STM32_PLLSAI1P field.
 */
#if (STM32_PLLSAI1P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLSAI1P              (0 << 17)

#elif STM32_PLLSAI1P_VALUE == 17
#define STM32_PLLSAI1P              (1 << 17)

#else
#error "invalid STM32_PLLSAI1P_VALUE value specified"
#endif

/*
 * PLLSAI1-P output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                                   \
    ((STM32_PLLSAI1_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLLSAI1_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLLSAI1_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

#else /* !STM32_PLLSAI1_HAS_P */
#define STM32_PLLSAI1P              0
#define STM32_PLLSAI1PEN            0
#endif /* !STM32_PLLSAI1_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI1_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLSAI1Q field.
 */
#if (STM32_PLLSAI1Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI1Q              (0 << 21)

#elif STM32_PLLSAI1Q_VALUE == 4
#define STM32_PLLSAI1Q              (1 << 21)

#elif STM32_PLLSAI1Q_VALUE == 6
#define STM32_PLLSAI1Q              (2 << 21)

#elif STM32_PLLSAI1Q_VALUE == 8
#define STM32_PLLSAI1Q              (3 << 21)

#else
#error "invalid STM32_PLLSAI1Q_VALUE value specified"
#endif

/**
 * @brief   PLLSAI1 Q output clock frequency.
 */
#define STM32_PLLSAI1_Q_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1Q_VALUE)

/*
 * PLLSAI1-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                                   \
    ((STM32_PLLSAI1_Q_CLKOUT < STM32_PLLQ_MIN) || (STM32_PLLSAI1_Q_CLKOUT > STM32_PLLQ_MAX))
#error "STM32_PLLSAI1_Q_CLKOUT outside acceptable range (STM32_PLLQ_MIN...STM32_PLLQ_MAX)"
#endif

#else /* !STM32_PLLSAI1_HAS_Q */
#define STM32_PLLSAI1Q              0
#define STM32_PLLSAI1QEN            0
#endif /* !STM32_PLLSAI1_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI1_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLSAI1R field.
 */
#if (STM32_PLLSAI1R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI1R              (0 << 25)

#elif STM32_PLLSAI1R_VALUE == 4
#define STM32_PLLSAI1R              (1 << 25)

#elif STM32_PLLSAI1R_VALUE == 6
#define STM32_PLLSAI1R              (2 << 25)

#elif STM32_PLLSAI1R_VALUE == 8
#define STM32_PLLSAI1R              (3 << 25)

#else
#error "invalid STM32_PLLSAI1R_VALUE value specified"
#endif

/**
 * @brief   PLLSAI1 R output clock frequency.
 */
#define STM32_PLLSAI1_R_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1R_VALUE)

/*
 * PLLSAI1-R output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                                   \
    ((STM32_PLLSAI1_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLLSAI1_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLLSAI1_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

#else /* !STM32_PLLSAI1_HAS_R */
#define STM32_PLLSAI1R              0
#define STM32_PLLSAI1REN            0
#endif /* !STM32_PLLSAI1_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void pllsai1_init(void) {

#if STM32_ACTIVATE_PLLSAI1
  /* PLLSAI1 activation.*/
  RCC->PLLSAI1CFGR = STM32_PLLSAI1PDIV | STM32_PLLSAI1R   |
                     STM32_PLLSAI1REN  | STM32_PLLSAI1Q   |
                     STM32_PLLSAI1QEN  | STM32_PLLSAI1P   |
                     STM32_PLLSAI1PEN  | STM32_PLLSAI1N   |
                     STM32_PLLSAI1M    | STM32_PLLSAI1SRC;
  RCC->CR |= RCC_CR_PLLSAI1ON;

  /* Waiting for PLL lock.*/
  while ((RCC->CR & RCC_CR_PLLSAI1RDY) == 0)
    ;
#endif
}

static inline void pllsai1_deinit(void) {

  /* PLLSAI1 de-activation.*/
  RCC->PLLSAI1CFGR &= ~RCC_CR_PLLSAI1ON;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* STM32_HAS_PLLSAI1 */

/** @} */
