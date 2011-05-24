/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/hal_lld.c
 * @brief   STM32 HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#define AIRCR_VECTKEY           0x05FA0000

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

#if HAL_USE_ADC || HAL_USE_SPI || HAL_USE_UART
  dmaInit();
#endif
}

/**
 * @brief   STM32 clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) ||                    \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) ||                    \
    defined(STM32F10X_HD) || defined(STM32F10X_XL) ||                       \
    defined(__DOXYGEN__)
/*
 * Clocks initialization for the LD, MD and HD sub-families.
 */
void stm32_clock_init(void) {

  /* HSI setup, it enforces the reset situation in order to handle possible
     problems with JTAG probes and re-initializations.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  RCC->CFGR = 0;                            /* CFGR reset value.            */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Wait until HSI is the source.*/

  /* HSE setup, it is only performed if the HSE clock is selected as source
     of the system clock (directly or through the PLL).*/
#if (STM32_SW == STM32_SW_HSE) ||                                           \
    ((STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSE))
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                           /* Waits until HSE is stable.               */
#endif

  /* PLL setup, it is only performed if the PLL is the selected source of
     the system clock else it is left disabled.*/
#if STM32_SW == STM32_SW_PLL
  RCC->CFGR |= STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
  RCC->CR   |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL is stable.               */
#endif

  /* Clock settings.*/
#if STM32_HAS_USB
  RCC->CFGR = STM32_MCO    | STM32_USBPRE | STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#else
  RCC->CFGR = STM32_MCO    |                STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#endif

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS; /* Flash wait states depending on clock.    */

  /* Switching on the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif
}

#elif defined(STM32F10X_CL)
/*
 * Clocks initialization for the CL sub-family.
 */
void stm32_clock_init(void) {

  /* HSI setup, it enforces the reset situation in order to handle possible
     problems with JTAG probes and re-initializations.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  RCC->CFGR = 0;                            /* CFGR reset value.            */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Wait until HSI is the source.*/
  RCC->CFGR2 = 0;

  /* HSE setup, it is only performed if the HSE clock is selected as source
     of the system clock (directly or through the PLLs).*/
#if (STM32_SW == STM32_SW_HSE) ||                                           \
    ((STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_PREDIV1))
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                           /* Waits until HSE is stable.               */
#endif

  /* PLL2 setup, it is only performed if the PLL2 clock is selected as source
     for the PLL clock else it is left disabled.*/
#if STM32_SW == STM32_SW_PLL
#if STM32_PREDIV1SRC == STM32_PREDIV1SRC_PLL2
  RCC->CFGR2 |= STM32_PREDIV2 | STM32_PLL2MUL;
  RCC->CR    |= RCC_CR_PLL2ON;
  while (!(RCC->CR & RCC_CR_PLL2RDY))
    ;                           /* Waits until PLL is stable.               */
#endif

  /* PLL setup, it is only performed if the PLL is the selected source of
     the system clock else it is left disabled.*/
  RCC->CFGR2 |= STM32_PREDIV1 | STM32_PREDIV1SRC;
  RCC->CFGR  |= STM32_PLLMUL | STM32_PLLSRC;
  RCC->CR    |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL2 is stable.              */
#endif

  /* Clock settings.*/
#if STM32_HAS_OTG1
  RCC->CFGR = STM32_MCO    | STM32_OTGFSPRE | STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#else
  RCC->CFGR = STM32_MCO    |                  STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#endif

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS; /* Flash wait states depending on clock.    */

  /* Switching on the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif
}
#else
void stm32_clock_init(void) {}
#endif

/** @} */
