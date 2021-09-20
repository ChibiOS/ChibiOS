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
 * @file    STM32MP1xx/hal_lld.h
 * @brief   STM32MP1xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          .
 *          One of the following macros must also be defined:
 *          - STM32MP157Axx, STM32MP157Cxx, STM32MP157Dxx, STM32MP157Fxx,
 *          - STM32MP153Axx, STM32MP153Cxx, STM32MP153Dxx, STM32MP153Fxx,
 *          - STM32MP151Axx, STM32MP151Cxx, STM32MP151Dxx, STM32MP151Fxx,
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

#include "stm32_registry.h"

#define STM32_HCLK 0

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification
 * @{
 */
#if defined(STM32MP157Axx) || defined(STM32MP157Cxx) ||                     \
    defined(STM32MP157Dxx) || defined(STM32MP157Fxx) ||                     \
    defined(STM32MP153Axx) || defined(STM32MP153Cxx) ||                     \
    defined(STM32MP153Dxx) || defined(STM32MP153Fxx) ||                     \
    defined(STM32MP151Axx) || defined(STM32MP151Cxx) ||                     \
    defined(STM32MP151Dxx) || defined(STM32MP151Fxx) ||                     \
    defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32MP1 Microprocessor"

#else
#error "STM32MP1 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32MP1XX) || defined(__DOXYGEN__)
#define STM32MP1XX
#endif
/** @} */

/**
 * @name   Clock points names
 * @{
 */
#define CLK_MCU_CK              0U
#define CLK_HCLK1               CLK_MCU_CK
#define CLK_HCLK2               CLK_MCU_CK
#define CLK_HCLK3               CLK_MCU_CK
#define CLK_HCLK4               CLK_MCU_CK
#define CLK_PLL3P_CK            1U
#define CLK_PLL3Q_CK            2U
#define CLK_PLL3R_CK            3U
#define CLK_PLL4P_CK            4U
#define CLK_PLL4Q_CK            5U
#define CLK_PLL4R_CK            6U
#define CLK_PCLK1               7U
#define CLK_PCLK1TIM            8U
#define CLK_PCLK2               9U
#define CLK_PCLK2TIM            10U
#define CLK_PCLK3               11U
#define CLK_ARRAY_SIZE          12U
/** @} */

/**
 * @name    RCC_RCK3SELR register bits definitions
 * @{
 */
#define STM32_PLL3SRC_HSI        (0 << 0)   /**< PLL3 clock source is HSI.  */
#define STM32_PLL3SRC_HSE        (1 << 0)   /**< PLL3 clock source is HSE.  */
#define STM32_PLL3SRC_CSI        (2 << 0)   /**< PLL3 clock source is CSI.  */
#define STM32_PLL3SRC_NOCLOCK    (3 << 0)   /**< PLL3 clock source disabled.*/
/** @} */

/**
 * @name    RCC_RCK4SELR register bits definitions
 * @{
 */
#define STM32_PLL4SRC_HSI        (0 << 0)   /**< PLL4 clock source is HSI.  */
#define STM32_PLL4SRC_HSE        (1 << 0)   /**< PLL4 clock source is HSE.  */
#define STM32_PLL4SRC_CSI        (2 << 0)   /**< PLL4 clock source is CSI.  */
#define STM32_PLL4SRC_I2S_CKIN   (3 << 0)   /**< PLL4 clock source is I2SCK.*/
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables initializations in HAL entirely.
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT                       FALSE
#endif

/**
 * @brief   If enabled assumes TZEN active.
 * @note    If @p STM32_TZEN_ENABLED==TRUE and @p STM32_TZEN_MCKPROT==TRUE
 *          then secure initializations are not performed but settings are
 *          still used to calculate the various clock points.
 */
#if !defined(STM32_TZEN_ENABLED) || defined(__DOXYGEN__)
#define STM32_TZEN_ENABLED                  TRUE
#endif

/**
 * @brief   If enabled assumes MCKPROT active.
 * @note    If @p STM32_TZEN_ENABLED==TRUE and @p STM32_TZEN_MCKPROT==TRUE
 *          then secure initializations are not performed but settings are
 *          still used to calculate the various clock points.
 */
#if !defined(STM32_TZEN_MCKPROT) || defined(__DOXYGEN__)
#define STM32_TZEN_MCKPROT                  TRUE
#endif

/**
 * @brief   Enables the dynamic clock handling.
 */
#if !defined(STM32_CLOCK_DYNAMIC) || defined(__DOXYGEN__)
#define STM32_CLOCK_DYNAMIC                 FALSE
#endif

/**
 * @brief   PWR MCUWKUPENR register initialization value.
 */
#if !defined(STM32_PWR_MCUWKUPENR) || defined(__DOXYGEN__)
#define STM32_PWR_MCUWKUPENR                XXXXXXXXXXXXXXX
#endif

/**
 * @brief   Enables or disables the CSI clock source.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_CSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_CSI_ENABLED                   FALSE
#endif

/**
 * @brief   MCU divider setting.
 */
#if !defined(STM32_MCUDIV) || defined(__DOXYGEN__)
#define STM32_MCUDIV                        XXXXXXXXXXXXXXX
#endif

/**
 * @brief   MCU main clock source selection.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_MCUSSRC) || defined(__DOXYGEN__)
#define STM32_MCUSSRC                       XXXXXXXXXXXXXX
#endif

/**
 * @brief   Clock source for the PLL3.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3SRC) || defined(__DOXYGEN__)
#define STM32_PLL3SRC                       STM32_PLL3SRC_HSE
#endif

/**
 * @brief   PLL3 M divider value.
 * @note    The allowed values are 1..64.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3M_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3M_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL3 N multiplier value.
 * @note    The allowed values are 25..200.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3N_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3N_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL3 P divider value or zero if disabled.
 * @note    The allowed values are 1..128.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3P_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3P_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL3 Q divider value.
 * @note    The allowed values are 1..128.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3Q_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3Q_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL3 R divider value.
 * @note    The allowed values are 1..128.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_PLL3R_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3R_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   Clock source for the PLL4.
 */
#if !defined(STM32_PLL4SRC) || defined(__DOXYGEN__)
#define STM32_PLL4SRC                       STM32_PLL4SRC_HSE
#endif

/**
 * @brief   PLL4 M divider value.
 * @note    The allowed values are 1..64.
 */
#if !defined(STM32_PLL4M_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL4M_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL4 N multiplier value.
 * @note    The allowed values are 25..200.
 */
#if !defined(STM32_PLL4N_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL4N_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL4 P divider value or zero if disabled.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL4P_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL4P_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL4 Q divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL4Q_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL4Q_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   PLL4 R divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL4R_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL4R_VALUE                   XXXXXXXXXXXXXXX
#endif

/**
 * @brief   APB1DIV prescaler setting.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_APB1DIV) || defined(__DOXYGEN__)
#define STM32_APB1DIV                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   APB2DIV prescaler setting.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_APB2DIV) || defined(__DOXYGEN__)
#define STM32_APB2DIV                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   APB3DIV prescaler setting.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_APB3DIV) || defined(__DOXYGEN__)
#define STM32_APB3DIV                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   APB4DIV prescaler setting.
 * @note    This initialization is performed only if TZEN=0 or MCKPROT=0.
 */
#if !defined(STM32_APB4DIV) || defined(__DOXYGEN__)
#define STM32_APB4DIV                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   MCO1 clock source.
 */
#if !defined(STM32_MCO1SEL) || defined(__DOXYGEN__)
#define STM32_MCO1SEL                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   MCO1 divider value.
 */
#if !defined(STM32_MCO1DIV_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO1DIV_VALUE                 8
#endif

/**
 * @brief   MCO2 clock source.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       XXXXXXXXXXXXXXX
#endif

/**
 * @brief   MCO2 divider value.
 */
#if !defined(STM32_MCO2DIV_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO2DIV_VALUE                 8
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Clock handling mode selection.*/
#if STM32_CLOCK_DYNAMIC == TRUE
#define HAL_LLD_USE_CLOCK_MANAGEMENT
#endif

/*
 * Configuration-related checks.
 */
#if !defined(STM32MP1xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP1xx_MCUCONF not defined"
#endif

#if defined(STM32MP157Axx) && !defined(STM32MP157A_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP157A_MCUCONF not defined"

#elif defined(STM32MP157Cxx) && !defined(STM32MP157C_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP157C_MCUCONF not defined"

#elif defined(STM32MP157Dxx) && !defined(STM32MP157D_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP157D_MCUCONF not defined"

#elif defined(STM32MP157Fxx) && !defined(STM32MP157F_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP157F_MCUCONF not defined"

#elif defined(STM32MP153Axx) && !defined(STM32MP153A_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP153A_MCUCONF not defined"

#elif defined(STM32MP153Cxx) && !defined(STM32MP153C_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP153C_MCUCONF not defined"

#elif defined(STM32MP153Dxx) && !defined(STM32MP153D_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP153D_MCUCONF not defined"

#elif defined(STM32MP153Fxx) && !defined(STM32MP153F_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP153F_MCUCONF not defined"

#elif defined(STM32MP151Axx) && !defined(STM32MP151A_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP151A_MCUCONF not defined"

#elif defined(STM32MP151Cxx) && !defined(STM32MP151C_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP151C_MCUCONF not defined"

#elif defined(STM32MP151Dxx) && !defined(STM32MP151D_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP151D_MCUCONF not defined"

#elif defined(STM32MP151Fxx) && !defined(STM32MP151F_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32MP151F_MCUCONF not defined"

#endif

/**
 * @name    System Limits
 * @{
 */
#define STM32_BOOST_SYSCLK_MAX          170000000
#define STM32_BOOST_HSECLK_MAX          48000000
#define STM32_BOOST_HSECLK_BYP_MAX      48000000
#define STM32_BOOST_HSECLK_MIN          8000000
#define STM32_BOOST_HSECLK_BYP_MIN      8000000
#define STM32_BOOST_LSECLK_MAX          32768
#define STM32_BOOST_LSECLK_BYP_MAX      1000000
#define STM32_BOOST_LSECLK_MIN          32768
#define STM32_BOOST_LSECLK_BYP_MIN      32768
#define STM32_BOOST_PLLIN_MAX           16000000
#define STM32_BOOST_PLLIN_MIN           2660000
#define STM32_BOOST_PLLVCO_MAX          344000000
#define STM32_BOOST_PLLVCO_MIN          96000000
#define STM32_BOOST_PLLP_MAX            170000000
#define STM32_BOOST_PLLP_MIN            2064500
#define STM32_BOOST_PLLQ_MAX            170000000
#define STM32_BOOST_PLLQ_MIN            8000000
#define STM32_BOOST_PLLR_MAX            170000000
#define STM32_BOOST_PLLR_MIN            8000000
#define STM32_BOOST_PCLK1_MAX           170000000
#define STM32_BOOST_PCLK2_MAX           170000000
#define STM32_BOOST_ADCCLK_MAX          60000000

#define STM32_BOOST_0WS_THRESHOLD       34000000
#define STM32_BOOST_1WS_THRESHOLD       68000000
#define STM32_BOOST_2WS_THRESHOLD       102000000
#define STM32_BOOST_3WS_THRESHOLD       136000000
#define STM32_BOOST_4WS_THRESHOLD       170000000
/** @} */

/* TODO */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a clock point identifier.
 */
typedef unsigned halclkpt_t;

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Type of a clock point frequency in Hz.
 */
typedef uint32_t halfreq_t;

/**
 * @brief   Type of a clock configuration structure.
 */
typedef struct {
  /* TODO */
} halclkcfg_t;
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#if !defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
/**
 * @brief   Returns the frequency of a clock point in Hz.
 * @note    Static implementation.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK   ? STM32_SYSCLK        :                          \
   (clkpt) == CLK_PLLPCLK  ? STM32_PLL_P_CLKOUT  :                          \
   (clkpt) == CLK_PLLQCLK  ? STM32_PLL_Q_CLKOUT  :                          \
   (clkpt) == CLK_PLLRCLK  ? STM32_PLL_R_CLKOUT  :                          \
   (clkpt) == CLK_HCLK     ? STM32_HCLK          :                          \
   (clkpt) == CLK_PCLK1    ? STM32_PCLK1         :                          \
   (clkpt) == CLK_PCLK1TIM ? STM32_TIMP1CLK      :                          \
   (clkpt) == CLK_PCLK2    ? STM32_PCLK2         :                          \
   (clkpt) == CLK_PCLK2TIM ? STM32_TIMP2CLK      :                          \
   (clkpt) == CLK_MCO      ? STM32_MCOCLK        :                          \
   0U)
#endif /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "mpu_v7m.h"
#include "stm32_isr.h"
//#include "stm32_dma.h"
//#include "stm32_exti.h"
#include "stm32_rcc.h"
//#include "stm32_tim.h"

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) && !defined(__DOXYGEN__)
extern const halclkcfg_t hal_clkcfg_reset;
extern const halclkcfg_t hal_clkcfg_default;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
  bool hal_lld_clock_switch_mode(const halclkcfg_t *ccp);
  halfreq_t hal_lld_get_clock_point(halclkpt_t clkpt);
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
