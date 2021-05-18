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
 * @file    RCCv1/stm32_pllsai2.inc
 * @brief   Shared PLLSAI2 handler.
 *
 * @addtogroup STM32_PLLSAI2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_PLLSAI2)
#define STM32_HAS_PLLSAI2           FALSE
#endif

#if STM32_HAS_PLLSAI2

/* Checks on configurations.*/
#if !defined(STM32_PLLSRC)
#error "STM32_PLLSRC not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI2M_VALUE)
#error "STM32_PLLSAI2M_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI2N_VALUE)
#error "STM32_PLLSAI2N_VALUE not defined in mcuconf.h"
#endif

#if !defined(STM32_PLLSAI2PDIV_VALUE)
#error "STM32_PLLSAI2PDIV_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI2_HAS_P && !defined(STM32_PLLSAI2P_VALUE)
#error "STM32_PLLSAI2P_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI2_HAS_Q && !defined(STM32_PLLSAI2Q_VALUE)
#error "STM32_PLLSAI2Q_VALUE not defined in mcuconf.h"
#endif

#if STM32_PLLSAI2_HAS_R && !defined(STM32_PLLSAI2R_VALUE)
#error "STM32_PLLSAI2R_VALUE not defined in mcuconf.h"
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
#if !defined(STM32_ACTIVATE_PLLSAI2)
#error "STM32_ACTIVATE_PLLSAI2 not defined in hal_lld.h"
#endif


#if STM32_PLLSAI2_HAS_P && !defined(STM32_PLLSAI2PEN)
#error "STM32_PLLSAI2PEN not defined in hal_lld.h"
#endif

#if STM32_PLLSAI2_HAS_Q && !defined(STM32_PLLSAI2QEN)
#error "STM32_PLLSAI2QEN not defined in hal_lld.h"
#endif

#if STM32_PLLSAI2_HAS_R && !defined(STM32_PLLSAI2REN)
#error "STM32_PLLSAI2REN not defined in hal_lld.h"
#endif

#if (STM32_PLLSAI2CLKIN != 0) &&                                                \
    ((STM32_PLLSAI2CLKIN < STM32_PLLIN_MIN) || (STM32_PLLSAI2CLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLSAI2CLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/**
 * @brief   STM32_PLLSAI2M field.
 */
#if ((STM32_PLLSAI2M_VALUE >= 1) && (STM32_PLLSAI2M_VALUE <= 16)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2M              ((STM32_PLLSAI2M_VALUE - 1) << 4)

#else
#error "invalid STM32_PLLSAI2M_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI2N field.
 */
#if ((STM32_PLLSAI2N_VALUE >= 8) && (STM32_PLLSAI2N_VALUE <= 127)) ||       \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2N              (STM32_PLLSAI2N_VALUE << 8)

#else
#error "invalid STM32_PLLSAI2N_VALUE value specified"
#endif

/**
 * @brief   PLLSAI2 VCO frequency.
 */
#define STM32_PLLSAI2VCO            (STM32_PLLSAI2CLKIN * STM32_PLLSAI2N_VALUE)

/*
 * PLLSAI2 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                               \
    ((STM32_PLLSAI2VCO < STM32_PLLVCO_MIN) || (STM32_PLLSAI2VCO > STM32_PLLVCO_MAX))
#error "STM32_PLLSAI2VCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/*---------------------------------------------------------------------------*/
/* P output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI2_HAS_P || defined(__DOXYGEN__)
/**
 * @brief   PLLSAI2 P output clock frequency.
 */
#if (STM32_PLLSAI2PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLLSAI2_P_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2P_VALUE)
#else
#define STM32_PLLSAI2_P_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2PDIV_VALUE)
#endif

/**
 * @brief   STM32_PLLSAI2P field.
 */
#if (STM32_PLLSAI2P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLSAI2P              (0 << 17)

#elif STM32_PLLSAI2P_VALUE == 17
#define STM32_PLLSAI2P              (1 << 17)

#else
#error "invalid STM32_PLLSAI2P_VALUE value specified"
#endif

/*
 * PLLSAI2-P output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                                   \
    ((STM32_PLLSAI2_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLLSAI2_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLLSAI2_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

#else /* !STM32_PLLSAI2_HAS_P */
#define STM32_PLLSAI2P              0
#define STM32_PLLSAI2PEN            0
#endif /* !STM32_PLLSAI2_HAS_P */

/*---------------------------------------------------------------------------*/
/* Q output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI2_HAS_Q || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLSAI2Q field.
 */
#if (STM32_PLLSAI2Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI2Q              (0 << 21)

#elif STM32_PLLSAI2Q_VALUE == 4
#define STM32_PLLSAI2Q              (1 << 21)

#elif STM32_PLLSAI2Q_VALUE == 6
#define STM32_PLLSAI2Q              (2 << 21)

#elif STM32_PLLSAI2Q_VALUE == 8
#define STM32_PLLSAI2Q              (3 << 21)

#else
#error "invalid STM32_PLLSAI2Q_VALUE value specified"
#endif

/**
 * @brief   PLLSAI2 Q output clock frequency.
 */
#define STM32_PLLSAI2_Q_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2Q_VALUE)

/*
 * PLLSAI2-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                                   \
    ((STM32_PLLSAI2_Q_CLKOUT < STM32_PLLQ_MIN) || (STM32_PLLSAI2_Q_CLKOUT > STM32_PLLQ_MAX))
#error "STM32_PLLSAI2_Q_CLKOUT outside acceptable range (STM32_PLLQ_MIN...STM32_PLLQ_MAX)"
#endif

#else /* !STM32_PLLSAI2_HAS_Q */
#define STM32_PLLSAI2Q              0
#define STM32_PLLSAI2QEN            0
#endif /* !STM32_PLLSAI2_HAS_Q */

/*---------------------------------------------------------------------------*/
/* R output, if present.                                                     */
/*---------------------------------------------------------------------------*/
#if STM32_PLLSAI2_HAS_R || defined(__DOXYGEN__)
/**
 * @brief   STM32_PLLSAI2R field.
 */
#if (STM32_PLLSAI2R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI2R              (0 << 25)

#elif STM32_PLLSAI2R_VALUE == 4
#define STM32_PLLSAI2R              (1 << 25)

#elif STM32_PLLSAI2R_VALUE == 6
#define STM32_PLLSAI2R              (2 << 25)

#elif STM32_PLLSAI2R_VALUE == 8
#define STM32_PLLSAI2R              (3 << 25)

#else
#error "invalid STM32_PLLSAI2R_VALUE value specified"
#endif

/**
 * @brief   PLLSAI2 R output clock frequency.
 */
#define STM32_PLLSAI2_R_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2R_VALUE)

/*
 * PLLSAI2-R output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                                   \
    ((STM32_PLLSAI2_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLLSAI2_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLLSAI2_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

#else /* !STM32_PLLSAI2_HAS_R */
#define STM32_PLLSAI2R              0
#define STM32_PLLSAI2REN            0
#endif /* !STM32_PLLSAI2_HAS_R */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void pllsai2_init(void) {

#if STM32_ACTIVATE_PLLSAI2
  /* PLLSAI2 activation.*/
  RCC->PLLSAI2CFGR = STM32_PLLSAI2PDIV | STM32_PLLSAI2R   |
                     STM32_PLLSAI2REN  | STM32_PLLSAI2Q   |
                     STM32_PLLSAI2QEN  | STM32_PLLSAI2P   |
                     STM32_PLLSAI2PEN  | STM32_PLLSAI2N   |
                     STM32_PLLSAI2M    | STM32_PLLSAI2SRC;
  RCC->CR |= RCC_CR_PLLSAI2ON;

  /* Waiting for PLL lock.*/
  while ((RCC->CR & RCC_CR_PLLSAI2RDY) == 0)
    ;
#endif
}

static inline void pllsai2_deinit(void) {

  /* PLLSAI2 de-activation.*/
  RCC->PLLSAI2CFGR &= ~RCC_CR_PLLSAI2ON;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* STM32_HAS_PLLSAI2 */

/** @} */
