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
 * @file    STM32U5xx/stm32_rcc.h
 * @brief   RCC helper driver header.
 * @note    This file requires definitions from the ST header file
 *          @p stm32u5xx.h.
 *
 * @addtogroup STM32U5xx_RCC
 * @{
 */

#ifndef STM32_RCC_H
#define STM32_RCC_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Generic RCC operations
 * @{
 */
/**
 * @brief   Enables the clock of one or more peripheral on the APB1 bus (R1).
 *
 * @param[in] mask      APB1 R1 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAPB1R1(mask, lp) {                                         \
  RCC->APB1ENR1 |= (mask);                                                  \
  if (lp)                                                                   \
    RCC->APB1SMENR1 |= (mask);                                              \
  else                                                                      \
    RCC->APB1SMENR1 &= ~(mask);                                             \
  (void)RCC->APB1SMENR1;                                                    \
}

/**
 * @brief   Disables the clock of one or more peripheral on the APB1 bus (R1).
 *
 * @param[in] mask      APB1 R1 peripherals mask
 *
 * @api
 */
#define rccDisableAPB1R1(mask) {                                            \
  RCC->APB1ENR1 &= ~(mask);                                                 \
  RCC->APB1SMENR1 &= ~(mask);                                               \
  (void)RCC->APB1SMENR1;                                                    \
}

/**
 * @brief   Resets one or more peripheral on the APB1 bus (R1).
 *
 * @param[in] mask      APB1 R1 peripherals mask
 *
 * @api
 */
#define rccResetAPB1R1(mask) {                                              \
  RCC->APB1RSTR1 |= (mask);                                                 \
  RCC->APB1RSTR1 &= ~(mask);                                                \
  (void)RCC->APB1RSTR1;                                                     \
}

/**
 * @brief   Enables the clock of one or more peripheral on the APB1 bus (R2).
 *
 * @param[in] mask      APB1 R2 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAPB1R2(mask, lp) {                                         \
  RCC->APB1ENR2 |= (mask);                                                  \
  if (lp)                                                                   \
    RCC->APB1SMENR2 |= (mask);                                              \
  else                                                                      \
    RCC->APB1SMENR2 &= ~(mask);                                             \
  (void)RCC->APB1SMENR2;                                                    \
}

/**
 * @brief   Disables the clock of one or more peripheral on the APB1 bus (R2).
 *
 * @param[in] mask      APB1 R2 peripherals mask
 *
 * @api
 */
#define rccDisableAPB1R2(mask) {                                            \
  RCC->APB1ENR2 &= ~(mask);                                                 \
  RCC->APB1SMENR2 &= ~(mask);                                               \
  (void)RCC->APB1SMENR2;                                                    \
}

/**
 * @brief   Resets one or more peripheral on the APB1 bus (R2).
 *
 * @param[in] mask      APB1 R2 peripherals mask
 *
 * @api
 */
#define rccResetAPB1R2(mask) {                                              \
  RCC->APB1RSTR2 |= (mask);                                                 \
  RCC->APB1RSTR2 &= ~(mask);                                                \
  (void)RCC->APB1RSTR2;                                                     \
}

/**
 * @brief   Enables the clock of one or more peripheral on the APB2 bus.
 *
 * @param[in] mask      APB2 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAPB2(mask, lp) {                                           \
  RCC->APB2ENR |= (mask);                                                   \
  if (lp)                                                                   \
    RCC->APB2SMENR |= (mask);                                               \
  else                                                                      \
    RCC->APB2SMENR &= ~(mask);                                              \
  (void)RCC->APB2SMENR;                                                     \
}

/**
 * @brief   Disables the clock of one or more peripheral on the APB2 bus.
 *
 * @param[in] mask      APB2 peripherals mask
 *
 * @api
 */
#define rccDisableAPB2(mask) {                                              \
  RCC->APB2ENR &= ~(mask);                                                  \
  RCC->APB2SMENR &= ~(mask);                                                \
  (void)RCC->APB2SMENR;                                                     \
}

/**
 * @brief   Resets one or more peripheral on the APB2 bus.
 *
 * @param[in] mask      APB2 peripherals mask
 *
 * @api
 */
#define rccResetAPB2(mask) {                                                \
  RCC->APB2RSTR |= (mask);                                                  \
  RCC->APB2RSTR &= ~(mask);                                                 \
  (void)RCC->APB2RSTR;                                                      \
}

/**
 * @brief   Enables the clock of one or more peripheral on the APB3 bus.
 *
 * @param[in] mask      APB3 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAPB3(mask, lp) {                                           \
  RCC->APB3ENR |= (mask);                                                   \
  if (lp)                                                                   \
    RCC->APB3SMENR |= (mask);                                               \
  else                                                                      \
    RCC->APB3SMENR &= ~(mask);                                              \
  (void)RCC->APB3SMENR;                                                     \
}

/**
 * @brief   Disables the clock of one or more peripheral on the APB3 bus.
 *
 * @param[in] mask      APB3 peripherals mask
 *
 * @api
 */
#define rccDisableAPB3(mask) {                                              \
  RCC->APB3ENR &= ~(mask);                                                  \
  RCC->APB3SMENR &= ~(mask);                                                \
  (void)RCC->APB3SMENR;                                                     \
}

/**
 * @brief   Resets one or more peripheral on the APB3 bus.
 *
 * @param[in] mask      APB3 peripherals mask
 *
 * @api
 */
#define rccResetAPB3(mask) {                                                \
  RCC->APB3RSTR |= (mask);                                                  \
  RCC->APB3RSTR &= ~(mask);                                                 \
  (void)RCC->APB3RSTR;                                                      \
}

/**
 * @brief   Enables the clock of one or more peripheral on the AHB1 bus.
 *
 * @param[in] mask      AHB1 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAHB1(mask, lp) {                                           \
  RCC->AHB1ENR |= (mask);                                                   \
  if (lp)                                                                   \
    RCC->AHB1SMENR |= (mask);                                               \
  else                                                                      \
    RCC->AHB1SMENR &= ~(mask);                                              \
  (void)RCC->AHB1SMENR;                                                     \
}

/**
 * @brief   Disables the clock of one or more peripheral on the AHB1 bus.
 *
 * @param[in] mask      AHB1 peripherals mask
 *
 * @api
 */
#define rccDisableAHB1(mask) {                                              \
  RCC->AHB1ENR &= ~(mask);                                                  \
  RCC->AHB1SMENR &= ~(mask);                                                \
  (void)RCC->AHB1SMENR;                                                     \
}

/**
 * @brief   Resets one or more peripheral on the AHB1 bus.
 *
 * @param[in] mask      AHB1 peripherals mask
 *
 * @api
 */
#define rccResetAHB1(mask) {                                                \
  RCC->AHB1RSTR |= (mask);                                                  \
  RCC->AHB1RSTR &= ~(mask);                                                 \
  (void)RCC->AHB1RSTR;                                                      \
}

/**
 * @brief   Enables the clock of one or more peripheral on the AHB2 bus (R1).
 *
 * @param[in] mask      AHB2 R1 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAHB2R1(mask, lp) {                                         \
  RCC->AHB2ENR1 |= (mask);                                                  \
  if (lp)                                                                   \
    RCC->AHB2SMENR1 |= (mask);                                              \
  else                                                                      \
    RCC->AHB2SMENR1 &= ~(mask);                                             \
  (void)RCC->AHB2SMENR1;                                                    \
}

/**
 * @brief   Disables the clock of one or more peripheral on the AHB2 bus (R1).
 *
 * @param[in] mask      AHB2 R1 peripherals mask
 *
 * @api
 */
#define rccDisableAHB2R1(mask) {                                            \
  RCC->AHB2ENR1 &= ~(mask);                                                 \
  RCC->AHB2SMENR1 &= ~(mask);                                               \
  (void)RCC->AHB2SMENR1;                                                    \
}

/**
 * @brief   Resets one or more peripheral on the AHB2 bus (R1).
 *
 * @param[in] mask      AHB2 R1 peripherals mask
 *
 * @api
 */
#define rccResetAHB2R1(mask) {                                              \
  RCC->AHB2RSTR1 |= (mask);                                                 \
  RCC->AHB2RSTR1 &= ~(mask);                                                \
  (void)RCC->AHB2RSTR1;                                                     \
}

/**
 * @brief   Enables the clock of one or more peripheral on the AHB2 bus (R2).
 *
 * @param[in] mask      AHB2 R2 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAHB2R2(mask, lp) {                                         \
  RCC->AHB2ENR2 |= (mask);                                                  \
  if (lp)                                                                   \
    RCC->AHB2SMENR2 |= (mask);                                              \
  else                                                                      \
    RCC->AHB2SMENR2 &= ~(mask);                                             \
  (void)RCC->AHB2SMENR2;                                                    \
}

/**
 * @brief   Disables the clock of one or more peripheral on the AHB2 bus (R2).
 *
 * @param[in] mask      AHB2 R2 peripherals mask
 *
 * @api
 */
#define rccDisableAHB2R2(mask) {                                            \
  RCC->AHB2ENR2 &= ~(mask);                                                 \
  RCC->AHB2SMENR2 &= ~(mask);                                               \
  (void)RCC->AHB2SMENR2;                                                    \
}

/**
 * @brief   Resets one or more peripheral on the AHB2 bus (R2).
 *
 * @param[in] mask      AHB2 R2 peripherals mask
 *
 * @api
 */
#define rccResetAHB2R2(mask) {                                              \
  RCC->AHB2RSTR2 |= (mask);                                                 \
  RCC->AHB2RSTR2 &= ~(mask);                                                \
  (void)RCC->AHB2RSTR2;                                                     \
}

/**
 * @brief   Enables the clock of one or more peripheral on the AHB3 bus.
 *
 * @param[in] mask      AHB3 peripherals mask
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rccEnableAHB3(mask, lp) {                                           \
  RCC->AHB3ENR |= (mask);                                                   \
  if (lp)                                                                   \
    RCC->AHB3SMENR |= (mask);                                               \
  else                                                                      \
    RCC->AHB3SMENR &= ~(mask);                                              \
  (void)RCC->AHB3SMENR;                                                     \
}

/**
 * @brief   Disables the clock of one or more peripheral on the AHB3 bus.
 *
 * @param[in] mask      AHB3 peripherals mask
 *
 * @api
 */
#define rccDisableAHB3(mask) {                                              \
  RCC->AHB3ENR &= ~(mask);                                                  \
  RCC->AHB3SMENR &= ~(mask);                                                \
  (void)RCC->AHB3SMENR;                                                     \
}

/**
 * @brief   Resets one or more peripheral on the AHB3 bus.
 *
 * @param[in] mask      AHB3 peripherals mask
 *
 * @api
 */
#define rccResetAHB3(mask) {                                                \
  RCC->AHB3RSTR |= (mask);                                                  \
  RCC->AHB3RSTR &= ~(mask);                                                \
  (void)RCC->AHB3RSTR;                                                      \
}
/** @} */

/**
 * @name    ADC peripherals specific RCC operations
 * @{
 */
#define rccEnableADC12(lp) rccEnableAHB2R1(RCC_AHB2ENR1_ADC12EN, lp)
#define rccDisableADC12() rccDisableAHB2R1(RCC_AHB2ENR1_ADC12EN)
#define rccResetADC12() rccResetAHB2R1(RCC_AHB2RSTR1_ADC12RST)

#define rccEnableADC4(lp) rccEnableAHB3(RCC_AHB3ENR_ADC4EN, lp)
#define rccDisableADC4() rccDisableAHB3(RCC_AHB3ENR_ADC4EN)
#define rccResetADC4() rccResetAHB3(RCC_AHB3RSTR_ADC4RST)
/** @} */

/**
 * @name    DAC peripherals specific RCC operations
 * @{
 */
#define rccEnableDAC1(lp) rccEnableAHB3(RCC_AHB3ENR_DAC1EN, lp)
#define rccDisableDAC1() rccDisableAHB3(RCC_AHB3ENR_DAC1EN)
#define rccResetDAC1() rccResetAHB3(RCC_AHB3RSTR_DAC1RST)
/** @} */

/**
 * @name    DMA peripherals specific RCC operations
 * @{
 */
#define rccEnableDMA31(lp) rccEnableAHB1(RCC_AHB1ENR_GPDMA1EN, lp)
#define rccDisableDMA31() rccDisableAHB1(RCC_AHB1ENR_GPDMA1EN)
#define rccResetDMA31() rccResetAHB1(RCC_AHB1RSTR_GPDMA1RST)

#define rccEnableDMA32(lp) rccEnableAHB1(RCC_AHB1ENR_GPDMA1EN, lp)
#define rccDisableDMA32() rccDisableAHB1(RCC_AHB1ENR_GPDMA1EN)
#define rccResetDMA32() rccResetAHB1(RCC_AHB1RSTR_GPDMA1RST)

#define rccEnableLPDMA1(lp) rccEnableAHB3(RCC_AHB3ENR_LPDMA1EN, lp)
#define rccDisableLPDMA1() rccDisableAHB3(RCC_AHB3ENR_LPDMA1EN)
#define rccResetLPDMA1() rccResetAHB3(RCC_AHB3RSTR_LPDMA1RST)
/** @} */

/**
 * @name    FDCAN peripherals specific RCC operations
 * @{
 */
#define rccEnableFDCAN1(lp) rccEnableAPB1R2(RCC_APB1ENR2_FDCAN1EN, lp)
#define rccDisableFDCAN1() rccDisableAPB1R2(RCC_APB1ENR2_FDCAN1EN)
#define rccResetFDCAN1() rccResetAPB1R2(RCC_APB1RSTR2_FDCAN1RST)
/** @} */

/**
 * @name    I2C peripherals specific RCC operations
 * @{
 */
#define rccEnableI2C1(lp) rccEnableAPB1R1(RCC_APB1ENR1_I2C1EN, lp)
#define rccDisableI2C1() rccDisableAPB1R1(RCC_APB1ENR1_I2C1EN)
#define rccResetI2C1() rccResetAPB1R1(RCC_APB1RSTR1_I2C1RST)

#define rccEnableI2C2(lp) rccEnableAPB1R1(RCC_APB1ENR1_I2C2EN, lp)
#define rccDisableI2C2() rccDisableAPB1R1(RCC_APB1ENR1_I2C2EN)
#define rccResetI2C2() rccResetAPB1R1(RCC_APB1RSTR1_I2C2RST)

#define rccEnableI2C3(lp) rccEnableAPB3(RCC_APB3ENR_I2C3EN, lp)
#define rccDisableI2C3() rccDisableAPB3(RCC_APB3ENR_I2C3EN)
#define rccResetI2C3() rccResetAPB3(RCC_APB3RSTR_I2C3RST)

#define rccEnableI2C4(lp) rccEnableAPB1R2(RCC_APB1ENR2_I2C4EN, lp)
#define rccDisableI2C4() rccDisableAPB1R2(RCC_APB1ENR2_I2C4EN)
#define rccResetI2C4() rccResetAPB1R2(RCC_APB1RSTR2_I2C4RST)
/** @} */

/**
 * @name    LPTIM peripherals specific RCC operations
 * @{
 */
#define rccEnableLPTIM1(lp) rccEnableAPB3(RCC_APB3ENR_LPTIM1EN, lp)
#define rccDisableLPTIM1() rccDisableAPB3(RCC_APB3ENR_LPTIM1EN)
#define rccResetLPTIM1() rccResetAPB3(RCC_APB3RSTR_LPTIM1RST)

#define rccEnableLPTIM2(lp) rccEnableAPB1R2(RCC_APB1ENR2_LPTIM2EN, lp)
#define rccDisableLPTIM2() rccDisableAPB1R2(RCC_APB1ENR2_LPTIM2EN)
#define rccResetLPTIM2() rccResetAPB1R2(RCC_APB1RSTR2_LPTIM2RST)

#define rccEnableLPTIM3(lp) rccEnableAPB3(RCC_APB3ENR_LPTIM3EN, lp)
#define rccDisableLPTIM3() rccDisableAPB3(RCC_APB3ENR_LPTIM3EN)
#define rccResetLPTIM3() rccResetAPB3(RCC_APB3RSTR_LPTIM3RST)

#define rccEnableLPTIM4(lp) rccEnableAPB3(RCC_APB3ENR_LPTIM4EN, lp)
#define rccDisableLPTIM4() rccDisableAPB3(RCC_APB3ENR_LPTIM4EN)
#define rccResetLPTIM4() rccResetAPB3(RCC_APB3RSTR_LPTIM4RST)
/** @} */

/**
 * @name    OCTOSPI peripherals specific RCC operations
 * @{
 */
#define rccEnableOCTOSPI1(lp) rccEnableAHB2R2(RCC_AHB2ENR2_OCTOSPI1EN, lp)
#define rccDisableOCTOSPI1() rccDisableAHB2R2(RCC_AHB2ENR2_OCTOSPI1EN)
#define rccResetOCTOSPI1() rccResetAHB2R2(RCC_AHB2RSTR2_OCTOSPI1RST)

#define rccEnableOCTOSPI2(lp) rccEnableAHB2R2(RCC_AHB2ENR2_OCTOSPI2EN, lp)
#define rccDisableOCTOSPI2() rccDisableAHB2R2(RCC_AHB2ENR2_OCTOSPI2EN)
#define rccResetOCTOSPI2() rccResetAHB2R2(RCC_AHB2RSTR2_OCTOSPI2RST)
/** @} */

/**
 * @name    Power and system specific RCC operations
 * @{
 */
#define rccEnableDMA2D(lp) rccEnableAHB1(RCC_AHB1ENR_DMA2DEN, lp)
#define rccDisableDMA2D() rccDisableAHB1(RCC_AHB1ENR_DMA2DEN)
#define rccResetDMA2D() rccResetAHB1(RCC_AHB1RSTR_DMA2DRST)

#define rccEnablePWRInterface(lp) rccEnableAHB3(RCC_AHB3ENR_PWREN, lp)
#define rccDisablePWRInterface() rccDisableAHB3(RCC_AHB3ENR_PWREN)
#define rccResetPWRInterface() do { } while (0)

#define rccEnableRTCAPB(lp) rccEnableAPB3(RCC_APB3ENR_RTCAPBEN, lp)
#define rccDisableRTCAPB() rccDisableAPB3(RCC_APB3ENR_RTCAPBEN)
#define rccResetRTCAPB() do { } while (0)

#define rccEnableSYSCFG(lp) rccEnableAPB3(RCC_APB3ENR_SYSCFGEN, lp)
#define rccDisableSYSCFG() rccDisableAPB3(RCC_APB3ENR_SYSCFGEN)
#define rccResetSYSCFG() rccResetAPB3(RCC_APB3RSTR_SYSCFGRST)

#define rccEnableTSC(lp) rccEnableAHB1(RCC_AHB1ENR_TSCEN, lp)
#define rccDisableTSC() rccDisableAHB1(RCC_AHB1ENR_TSCEN)
#define rccResetTSC() rccResetAHB1(RCC_AHB1RSTR_TSCRST)
/** @} */

/**
 * @name    RNG and HASH specific RCC operations
 * @{
 */
#define rccEnableHASH(lp) rccEnableAHB2R1(RCC_AHB2ENR1_HASHEN, lp)
#define rccDisableHASH() rccDisableAHB2R1(RCC_AHB2ENR1_HASHEN)
#define rccResetHASH() rccResetAHB2R1(RCC_AHB2RSTR1_HASHRST)

#define rccEnableRNG(lp) rccEnableAHB2R1(RCC_AHB2ENR1_RNGEN, lp)
#define rccDisableRNG() rccDisableAHB2R1(RCC_AHB2ENR1_RNGEN)
#define rccResetRNG() rccResetAHB2R1(RCC_AHB2RSTR1_RNGRST)
/** @} */

/**
 * @name    SAI peripherals specific RCC operations
 * @{
 */
#define rccEnableSAI1(lp) rccEnableAPB2(RCC_APB2ENR_SAI1EN, lp)
#define rccDisableSAI1() rccDisableAPB2(RCC_APB2ENR_SAI1EN)
#define rccResetSAI1() rccResetAPB2(RCC_APB2RSTR_SAI1RST)

#define rccEnableSAI2(lp) rccEnableAPB2(RCC_APB2ENR_SAI2EN, lp)
#define rccDisableSAI2() rccDisableAPB2(RCC_APB2ENR_SAI2EN)
#define rccResetSAI2() rccResetAPB2(RCC_APB2RSTR_SAI2RST)
/** @} */

/**
 * @name    SDMMC peripherals specific RCC operations
 * @{
 */
#define rccEnableSDMMC1(lp) rccEnableAHB2R1(RCC_AHB2ENR1_SDMMC1EN, lp)
#define rccDisableSDMMC1() rccDisableAHB2R1(RCC_AHB2ENR1_SDMMC1EN)
#define rccResetSDMMC1() rccResetAHB2R1(RCC_AHB2RSTR1_SDMMC1RST)

#define rccEnableSDMMC2(lp) rccEnableAHB2R1(RCC_AHB2ENR1_SDMMC2EN, lp)
#define rccDisableSDMMC2() rccDisableAHB2R1(RCC_AHB2ENR1_SDMMC2EN)
#define rccResetSDMMC2() rccResetAHB2R1(RCC_AHB2RSTR1_SDMMC2RST)
/** @} */

/**
 * @name    SPI peripherals specific RCC operations
 * @{
 */
#define rccEnableSPI1(lp) rccEnableAPB2(RCC_APB2ENR_SPI1EN, lp)
#define rccDisableSPI1() rccDisableAPB2(RCC_APB2ENR_SPI1EN)
#define rccResetSPI1() rccResetAPB2(RCC_APB2RSTR_SPI1RST)

#define rccEnableSPI2(lp) rccEnableAPB1R1(RCC_APB1ENR1_SPI2EN, lp)
#define rccDisableSPI2() rccDisableAPB1R1(RCC_APB1ENR1_SPI2EN)
#define rccResetSPI2() rccResetAPB1R1(RCC_APB1RSTR1_SPI2RST)

#define rccEnableSPI3(lp) rccEnableAPB3(RCC_APB3ENR_SPI3EN, lp)
#define rccDisableSPI3() rccDisableAPB3(RCC_APB3ENR_SPI3EN)
#define rccResetSPI3() rccResetAPB3(RCC_APB3RSTR_SPI3RST)
/** @} */

/**
 * @name    UART/USART peripherals specific RCC operations
 * @{
 */
#define rccEnableUSART1(lp) rccEnableAPB2(RCC_APB2ENR_USART1EN, lp)
#define rccDisableUSART1() rccDisableAPB2(RCC_APB2ENR_USART1EN)
#define rccResetUSART1() rccResetAPB2(RCC_APB2RSTR_USART1RST)

#define rccEnableUSART2(lp) rccEnableAPB1R1(RCC_APB1ENR1_USART2EN, lp)
#define rccDisableUSART2() rccDisableAPB1R1(RCC_APB1ENR1_USART2EN)
#define rccResetUSART2() rccResetAPB1R1(RCC_APB1RSTR1_USART2RST)

#define rccEnableUSART3(lp) rccEnableAPB1R1(RCC_APB1ENR1_USART3EN, lp)
#define rccDisableUSART3() rccDisableAPB1R1(RCC_APB1ENR1_USART3EN)
#define rccResetUSART3() rccResetAPB1R1(RCC_APB1RSTR1_USART3RST)

#define rccEnableUART4(lp) rccEnableAPB1R1(RCC_APB1ENR1_UART4EN, lp)
#define rccDisableUART4() rccDisableAPB1R1(RCC_APB1ENR1_UART4EN)
#define rccResetUART4() rccResetAPB1R1(RCC_APB1RSTR1_UART4RST)

#define rccEnableUART5(lp) rccEnableAPB1R1(RCC_APB1ENR1_UART5EN, lp)
#define rccDisableUART5() rccDisableAPB1R1(RCC_APB1ENR1_UART5EN)
#define rccResetUART5() rccResetAPB1R1(RCC_APB1RSTR1_UART5RST)

#define rccEnableLPUART1(lp) rccEnableAPB3(RCC_APB3ENR_LPUART1EN, lp)
#define rccDisableLPUART1() rccDisableAPB3(RCC_APB3ENR_LPUART1EN)
#define rccResetLPUART1() rccResetAPB3(RCC_APB3RSTR_LPUART1RST)
/** @} */

/**
 * @name    USB and UCPD specific RCC operations
 * @{
 */
#define rccEnableUSB(lp) rccEnableAHB2R1(RCC_AHB2ENR1_OTGEN, lp)
#define rccDisableUSB() rccDisableAHB2R1(RCC_AHB2ENR1_OTGEN)
#define rccResetUSB() rccResetAHB2R1(RCC_AHB2RSTR1_OTGRST)

#define rccEnableUCPD1(lp) rccEnableAPB1R2(RCC_APB1ENR2_UCPD1EN, lp)
#define rccDisableUCPD1() rccDisableAPB1R2(RCC_APB1ENR2_UCPD1EN)
#define rccResetUCPD1() rccResetAPB1R2(RCC_APB1RSTR2_UCPD1RST)
/** @} */

/**
 * @name    DCMI/PSSI specific RCC operations
 * @{
 */
#define rccEnableDCMI_PSSI(lp) rccEnableAHB2R1(RCC_AHB2ENR1_DCMI_PSSIEN, lp)
#define rccDisableDCMI_PSSI() rccDisableAHB2R1(RCC_AHB2ENR1_DCMI_PSSIEN)
#define rccResetDCMI_PSSI() rccResetAHB2R1(RCC_AHB2RSTR1_DCMI_PSSIRST)

#define rccEnableDCMI(lp) rccEnableDCMI_PSSI(lp)
#define rccDisableDCMI() rccDisableDCMI_PSSI()
#define rccResetDCMI() rccResetDCMI_PSSI()
/** @} */

#endif /* STM32_RCC_H */

/** @} */
