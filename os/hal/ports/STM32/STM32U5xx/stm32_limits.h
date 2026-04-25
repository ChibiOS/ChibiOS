/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    STM32U5xx/stm32_limits.h
 * @brief   STM32U5xx device limits header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef STM32_LIMITS_H
#define STM32_LIMITS_H

/**
 * @name    Activation times in microseconds
 */
/**
 * @brief   Timeout tolerance.
 */
#define STM32_RELAXED_TIMEOUT_FACTOR        5U

/**
 * @brief   Regulators transition time.
 * @note    Worst-case value from the datasheet is the SMPS range 4 to range 1
 *          transition.
 */
#define STM32_REGULATORS_TRANSITION_TIME    (76U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Worst case startup and stabilization time of HSI16 oscillator.
 */
#define STM32_HSI_STARTUP_TIME              (6U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Worst case startup time of HSI48 oscillator.
 */
#define STM32_HSI48_STARTUP_TIME            (6U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Worst case startup time of all oscillators controlled in the
 *          RCC_CR register.
 * @note    The value is taken from HSE which is the slowest one according
 *          to datasheet.
 * @note    LSE not included because it is way slower, in the order of
 *          seconds according to datasheet.
 */
#define STM32_OSCILLATORS_STARTUP_TIME      (2000U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Stabilization time of the MSI PLLs.
 * @note    This value does not assume the MSIPLLFAST bit.
 */
#define STM32_MSIPLL_STARTUP_TIME           (800U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Stabilization time of PLLs.
 */
#define STM32_PLL_STARTUP_TIME              (50U * STM32_RELAXED_TIMEOUT_FACTOR)

/**
 * @brief   Time required for system time switch (RCC_CFGR1.SW).
 * @note    This value is estimated and kept "safe", there is no
 *          specification.
 */
#define STM32_SYSCLK_SWITCH_TIME            (50U * STM32_RELAXED_TIMEOUT_FACTOR)
/** @} */

/**
 * @name    Device Limits for VCore range 1
 * @{
 */
#define STM32_RANGE1_BOOSTEN_THRESHOLD      55000000U
#define STM32_RANGE1_SYSCLK_MAX             160000000U
#define STM32_RANGE1_HSECLK_MAX             50000000U
#define STM32_RANGE1_HSECLK_BYP_MAX         50000000U
#define STM32_RANGE1_HSECLK_MIN             4000000U
#define STM32_RANGE1_HSECLK_BYP_MIN         4000000U
#define STM32_RANGE1_LSECLK_MAX             32768U
#define STM32_RANGE1_LSECLK_BYP_MAX         1000000U
#define STM32_RANGE1_LSECLK_MIN             32768U
#define STM32_RANGE1_LSECLK_BYP_MIN         32768U
#define STM32_RANGE1_PLLIN_MAX              16000000U
#define STM32_RANGE1_PLLIN_MIN              4000000U
#define STM32_RANGE1_PLLVCO_MAX             544000000U
#define STM32_RANGE1_PLLVCO_MIN             128000000U
#define STM32_RANGE1_PLLP_MAX               208000000U
#define STM32_RANGE1_PLLP_MIN               1000000U
#define STM32_RANGE1_PLLQ_MAX               208000000U
#define STM32_RANGE1_PLLQ_MIN               1000000U
#define STM32_RANGE1_PLLR_MAX               208000000U
#define STM32_RANGE1_PLLR_MIN               1000000U
#define STM32_RANGE1_PCLK1_MAX              160000000U
#define STM32_RANGE1_PCLK2_MAX              160000000U
#define STM32_RANGE1_PCLK3_MAX              160000000U
#define STM32_RANGE1_ADCCLK_MAX             55000000U

#define STM32_RANGE1_0WS_THRESHOLD          32000000U
#define STM32_RANGE1_1WS_THRESHOLD          64000000U
#define STM32_RANGE1_2WS_THRESHOLD          96000000U
#define STM32_RANGE1_3WS_THRESHOLD          128000000U
#define STM32_RANGE1_4WS_THRESHOLD          160000000U
/** @} */

/**
 * @name    Device Limits for VCore range 2
 * @{
 */
#define STM32_RANGE2_BOOSTEN_THRESHOLD      55000000U
#define STM32_RANGE2_SYSCLK_MAX             110000000U
#define STM32_RANGE2_HSECLK_MAX             50000000U
#define STM32_RANGE2_HSECLK_BYP_MAX         50000000U
#define STM32_RANGE2_HSECLK_MIN             4000000U
#define STM32_RANGE2_HSECLK_BYP_MIN         4000000U
#define STM32_RANGE2_LSECLK_MAX             32768U
#define STM32_RANGE2_LSECLK_BYP_MAX         1000000U
#define STM32_RANGE2_LSECLK_MIN             32768U
#define STM32_RANGE2_LSECLK_BYP_MIN         32768U
#define STM32_RANGE2_PLLIN_MAX              16000000U
#define STM32_RANGE2_PLLIN_MIN              4000000U
#define STM32_RANGE2_PLLVCO_MAX             544000000U
#define STM32_RANGE2_PLLVCO_MIN             128000000U
#define STM32_RANGE2_PLLP_MAX               110000000U
#define STM32_RANGE2_PLLP_MIN               1000000U
#define STM32_RANGE2_PLLQ_MAX               110000000U
#define STM32_RANGE2_PLLQ_MIN               1000000U
#define STM32_RANGE2_PLLR_MAX               110000000U
#define STM32_RANGE2_PLLR_MIN               1000000U
#define STM32_RANGE2_PCLK1_MAX              110000000U
#define STM32_RANGE2_PCLK2_MAX              110000000U
#define STM32_RANGE2_PCLK3_MAX              110000000U
#define STM32_RANGE2_ADCCLK_MAX             55000000U

#define STM32_RANGE2_0WS_THRESHOLD          30000000U
#define STM32_RANGE2_1WS_THRESHOLD          60000000U
#define STM32_RANGE2_2WS_THRESHOLD          90000000U
#define STM32_RANGE2_3WS_THRESHOLD          110000000U
#define STM32_RANGE2_4WS_THRESHOLD          110000000U
/** @} */

/**
 * @name    Device Limits for VCore range 3
 * @{
 */
#define STM32_RANGE3_BOOSTEN_THRESHOLD      56000000U
#define STM32_RANGE3_SYSCLK_MAX             55000000U
#define STM32_RANGE3_HSECLK_MAX             50000000U
#define STM32_RANGE3_HSECLK_BYP_MAX         50000000U
#define STM32_RANGE3_HSECLK_MIN             4000000U
#define STM32_RANGE3_HSECLK_BYP_MIN         4000000U
#define STM32_RANGE3_LSECLK_MAX             32768U
#define STM32_RANGE3_LSECLK_BYP_MAX         1000000U
#define STM32_RANGE3_LSECLK_MIN             32768U
#define STM32_RANGE3_LSECLK_BYP_MIN         32768U
#define STM32_RANGE3_PLLIN_MAX              16000000U
#define STM32_RANGE3_PLLIN_MIN              4000000U
#define STM32_RANGE3_PLLVCO_MAX             330000000U
#define STM32_RANGE3_PLLVCO_MIN             128000000U
#define STM32_RANGE3_PLLP_MAX               55000000U
#define STM32_RANGE3_PLLP_MIN               1000000U
#define STM32_RANGE3_PLLQ_MAX               55000000U
#define STM32_RANGE3_PLLQ_MIN               1000000U
#define STM32_RANGE3_PLLR_MAX               55000000U
#define STM32_RANGE3_PLLR_MIN               1000000U
#define STM32_RANGE3_PCLK1_MAX              55000000U
#define STM32_RANGE3_PCLK2_MAX              55000000U
#define STM32_RANGE3_PCLK3_MAX              55000000U
#define STM32_RANGE3_ADCCLK_MAX             55000000U

#define STM32_RANGE3_0WS_THRESHOLD          24000000U
#define STM32_RANGE3_1WS_THRESHOLD          48000000U
#define STM32_RANGE3_2WS_THRESHOLD          55000000U
#define STM32_RANGE3_3WS_THRESHOLD          55000000U
#define STM32_RANGE3_4WS_THRESHOLD          55000000U
/** @} */

/**
 * @name    Device Limits for VCore range 4
 * @{
 */
#define STM32_RANGE4_BOOSTEN_THRESHOLD      56000000U
#define STM32_RANGE4_SYSCLK_MAX             25000000U
#define STM32_RANGE4_HSECLK_MAX             25000000U
#define STM32_RANGE4_HSECLK_BYP_MAX         25000000U
#define STM32_RANGE4_HSECLK_MIN             4000000U
#define STM32_RANGE4_HSECLK_BYP_MIN         4000000U
#define STM32_RANGE4_LSECLK_MAX             32768U
#define STM32_RANGE4_LSECLK_BYP_MAX         1000000U
#define STM32_RANGE4_LSECLK_MIN             32768U
#define STM32_RANGE4_LSECLK_BYP_MIN         32768U
#define STM32_RANGE4_PLLIN_MAX              0U
#define STM32_RANGE4_PLLIN_MIN              0U
#define STM32_RANGE4_PLLVCO_MAX             0U
#define STM32_RANGE4_PLLVCO_MIN             0U
#define STM32_RANGE4_PLLP_MAX               0U
#define STM32_RANGE4_PLLP_MIN               0U
#define STM32_RANGE4_PLLQ_MAX               0U
#define STM32_RANGE4_PLLQ_MIN               0U
#define STM32_RANGE4_PLLR_MAX               0U
#define STM32_RANGE4_PLLR_MIN               0U
#define STM32_RANGE4_PCLK1_MAX              25000000U
#define STM32_RANGE4_PCLK2_MAX              25000000U
#define STM32_RANGE4_PCLK3_MAX              25000000U
#define STM32_RANGE4_ADCCLK_MAX             25000000U

#define STM32_RANGE4_0WS_THRESHOLD          12000000U
#define STM32_RANGE4_1WS_THRESHOLD          25000000U
#define STM32_RANGE4_2WS_THRESHOLD          25000000U
#define STM32_RANGE4_3WS_THRESHOLD          25000000U
#define STM32_RANGE4_4WS_THRESHOLD          25000000U
/** @} */

/**
 * @name    Device Limits for current Vcore settings
 * @{
 */
#if ((STM32_PWR_VOSR & PWR_VOSR_VOS_Msk) == PWR_VOSR_VOS_RANGE1) || defined(__DOXYGEN__)
#define STM32_BOOSTEN_THRESHOLD             STM32_RANGE1_BOOSTEN_THRESHOLD
#define STM32_SYSCLK_MAX                    STM32_RANGE1_SYSCLK_MAX
#define STM32_HSECLK_MAX                    STM32_RANGE1_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX                STM32_RANGE1_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                    STM32_RANGE1_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN                STM32_RANGE1_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                    STM32_RANGE1_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX                STM32_RANGE1_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                    STM32_RANGE1_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN                STM32_RANGE1_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                     STM32_RANGE1_PLLIN_MAX
#define STM32_PLLIN_MIN                     STM32_RANGE1_PLLIN_MIN
#define STM32_PLLVCO_MAX                    STM32_RANGE1_PLLVCO_MAX
#define STM32_PLLVCO_MIN                    STM32_RANGE1_PLLVCO_MIN
#define STM32_PLLP_MAX                      STM32_RANGE1_PLLP_MAX
#define STM32_PLLP_MIN                      STM32_RANGE1_PLLP_MIN
#define STM32_PLLQ_MAX                      STM32_RANGE1_PLLQ_MAX
#define STM32_PLLQ_MIN                      STM32_RANGE1_PLLQ_MIN
#define STM32_PLLR_MAX                      STM32_RANGE1_PLLR_MAX
#define STM32_PLLR_MIN                      STM32_RANGE1_PLLR_MIN
#define STM32_PCLK1_MAX                     STM32_RANGE1_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_RANGE1_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_RANGE1_PCLK3_MAX
#define STM32_ADCCLK_MAX                    STM32_RANGE1_ADCCLK_MAX

#define STM32_0WS_THRESHOLD                 STM32_RANGE1_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD                 STM32_RANGE1_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD                 STM32_RANGE1_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD                 STM32_RANGE1_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD                 STM32_RANGE1_4WS_THRESHOLD

#elif (STM32_PWR_VOSR & PWR_VOSR_VOS_Msk) == PWR_VOSR_VOS_RANGE2
#define STM32_BOOSTEN_THRESHOLD             STM32_RANGE2_BOOSTEN_THRESHOLD
#define STM32_SYSCLK_MAX                    STM32_RANGE2_SYSCLK_MAX
#define STM32_HSECLK_MAX                    STM32_RANGE2_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX                STM32_RANGE2_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                    STM32_RANGE2_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN                STM32_RANGE2_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                    STM32_RANGE2_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX                STM32_RANGE2_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                    STM32_RANGE2_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN                STM32_RANGE2_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                     STM32_RANGE2_PLLIN_MAX
#define STM32_PLLIN_MIN                     STM32_RANGE2_PLLIN_MIN
#define STM32_PLLVCO_MAX                    STM32_RANGE2_PLLVCO_MAX
#define STM32_PLLVCO_MIN                    STM32_RANGE2_PLLVCO_MIN
#define STM32_PLLP_MAX                      STM32_RANGE2_PLLP_MAX
#define STM32_PLLP_MIN                      STM32_RANGE2_PLLP_MIN
#define STM32_PLLQ_MAX                      STM32_RANGE2_PLLQ_MAX
#define STM32_PLLQ_MIN                      STM32_RANGE2_PLLQ_MIN
#define STM32_PLLR_MAX                      STM32_RANGE2_PLLR_MAX
#define STM32_PLLR_MIN                      STM32_RANGE2_PLLR_MIN
#define STM32_PCLK1_MAX                     STM32_RANGE2_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_RANGE2_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_RANGE2_PCLK3_MAX
#define STM32_ADCCLK_MAX                    STM32_RANGE2_ADCCLK_MAX

#define STM32_0WS_THRESHOLD                 STM32_RANGE2_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD                 STM32_RANGE2_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD                 STM32_RANGE2_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD                 STM32_RANGE2_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD                 STM32_RANGE2_4WS_THRESHOLD

#elif (STM32_PWR_VOSR & PWR_VOSR_VOS_Msk) == PWR_VOSR_VOS_RANGE3
#define STM32_BOOSTEN_THRESHOLD             STM32_RANGE3_BOOSTEN_THRESHOLD
#define STM32_SYSCLK_MAX                    STM32_RANGE3_SYSCLK_MAX
#define STM32_HSECLK_MAX                    STM32_RANGE3_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX                STM32_RANGE3_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                    STM32_RANGE3_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN                STM32_RANGE3_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                    STM32_RANGE3_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX                STM32_RANGE3_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                    STM32_RANGE3_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN                STM32_RANGE3_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                     STM32_RANGE3_PLLIN_MAX
#define STM32_PLLIN_MIN                     STM32_RANGE3_PLLIN_MIN
#define STM32_PLLVCO_MAX                    STM32_RANGE3_PLLVCO_MAX
#define STM32_PLLVCO_MIN                    STM32_RANGE3_PLLVCO_MIN
#define STM32_PLLP_MAX                      STM32_RANGE3_PLLP_MAX
#define STM32_PLLP_MIN                      STM32_RANGE3_PLLP_MIN
#define STM32_PLLQ_MAX                      STM32_RANGE3_PLLQ_MAX
#define STM32_PLLQ_MIN                      STM32_RANGE3_PLLQ_MIN
#define STM32_PLLR_MAX                      STM32_RANGE3_PLLR_MAX
#define STM32_PLLR_MIN                      STM32_RANGE3_PLLR_MIN
#define STM32_PCLK1_MAX                     STM32_RANGE3_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_RANGE3_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_RANGE3_PCLK3_MAX
#define STM32_ADCCLK_MAX                    STM32_RANGE3_ADCCLK_MAX

#define STM32_0WS_THRESHOLD                 STM32_RANGE3_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD                 STM32_RANGE3_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD                 STM32_RANGE3_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD                 STM32_RANGE3_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD                 STM32_RANGE3_4WS_THRESHOLD

#elif (STM32_PWR_VOSR & PWR_VOSR_VOS_Msk) == PWR_VOSR_VOS_RANGE4
#define STM32_BOOSTEN_THRESHOLD             STM32_RANGE4_BOOSTEN_THRESHOLD
#define STM32_SYSCLK_MAX                    STM32_RANGE4_SYSCLK_MAX
#define STM32_HSECLK_MAX                    STM32_RANGE4_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX                STM32_RANGE4_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                    STM32_RANGE4_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN                STM32_RANGE4_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                    STM32_RANGE4_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX                STM32_RANGE4_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                    STM32_RANGE4_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN                STM32_RANGE4_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                     STM32_RANGE4_PLLIN_MAX
#define STM32_PLLIN_MIN                     STM32_RANGE4_PLLIN_MIN
#define STM32_PLLVCO_MAX                    STM32_RANGE4_PLLVCO_MAX
#define STM32_PLLVCO_MIN                    STM32_RANGE4_PLLVCO_MIN
#define STM32_PLLP_MAX                      STM32_RANGE4_PLLP_MAX
#define STM32_PLLP_MIN                      STM32_RANGE4_PLLP_MIN
#define STM32_PLLQ_MAX                      STM32_RANGE4_PLLQ_MAX
#define STM32_PLLQ_MIN                      STM32_RANGE4_PLLQ_MIN
#define STM32_PLLR_MAX                      STM32_RANGE4_PLLR_MAX
#define STM32_PLLR_MIN                      STM32_RANGE4_PLLR_MIN
#define STM32_PCLK1_MAX                     STM32_RANGE4_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_RANGE4_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_RANGE4_PCLK3_MAX
#define STM32_ADCCLK_MAX                    STM32_RANGE4_ADCCLK_MAX

#define STM32_0WS_THRESHOLD                 STM32_RANGE4_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD                 STM32_RANGE4_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD                 STM32_RANGE4_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD                 STM32_RANGE4_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD                 STM32_RANGE4_4WS_THRESHOLD

#else
#error "invalid STM32_PWR_VOSR settings specified"
#endif
/** @} */

/**
 * @name    PLL dividers ranges
 * @{
 */
#define STM32_PLL1M_VALUE_MAX               16U
#define STM32_PLL1M_VALUE_MIN               1U
#define STM32_PLL1N_ODDVALID                TRUE
#define STM32_PLL1N_VALUE_MAX               136U
#define STM32_PLL1N_VALUE_MIN               4U
#define STM32_PLL1P_ODDVALID                TRUE
#define STM32_PLL1P_VALUE_MAX               128U
#define STM32_PLL1P_VALUE_MIN               2U
#define STM32_PLL1Q_ODDVALID                TRUE
#define STM32_PLL1Q_VALUE_MAX               128U
#define STM32_PLL1Q_VALUE_MIN               1U
#define STM32_PLL1R_ODDVALID                TRUE
#define STM32_PLL1R_VALUE_MAX               128U
#define STM32_PLL1R_VALUE_MIN               1U

#define STM32_PLL2M_VALUE_MAX               16U
#define STM32_PLL2M_VALUE_MIN               1U
#define STM32_PLL2N_ODDVALID                TRUE
#define STM32_PLL2N_VALUE_MAX               136U
#define STM32_PLL2N_VALUE_MIN               4U
#define STM32_PLL2P_ODDVALID                TRUE
#define STM32_PLL2P_VALUE_MAX               128U
#define STM32_PLL2P_VALUE_MIN               2U
#define STM32_PLL2Q_ODDVALID                TRUE
#define STM32_PLL2Q_VALUE_MAX               128U
#define STM32_PLL2Q_VALUE_MIN               1U
#define STM32_PLL2R_ODDVALID                TRUE
#define STM32_PLL2R_VALUE_MAX               128U
#define STM32_PLL2R_VALUE_MIN               1U

#define STM32_PLL3M_VALUE_MAX               16U
#define STM32_PLL3M_VALUE_MIN               1U
#define STM32_PLL3N_ODDVALID                TRUE
#define STM32_PLL3N_VALUE_MAX               136U
#define STM32_PLL3N_VALUE_MIN               4U
#define STM32_PLL3P_ODDVALID                TRUE
#define STM32_PLL3P_VALUE_MAX               128U
#define STM32_PLL3P_VALUE_MIN               2U
#define STM32_PLL3Q_ODDVALID                TRUE
#define STM32_PLL3Q_VALUE_MAX               128U
#define STM32_PLL3Q_VALUE_MIN               1U
#define STM32_PLL3R_ODDVALID                TRUE
#define STM32_PLL3R_VALUE_MAX               128U
#define STM32_PLL3R_VALUE_MIN               1U
/** @} */

/**
 * @name    Peripherals limits
 * @{
 */
#define STM32_SDMMC_MAXCLK                  208000000U
/** @} */

#endif /* STM32_LIMITS_H */

/** @} */
