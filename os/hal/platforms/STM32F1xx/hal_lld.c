/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32F1xx/hal_lld.c
 * @brief   STM32F1xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes the backup domain.
 * @note    WARNING! Changing clock source impossible without resetting
 *          of the whole BKP domain.
 */
static void hal_lld_backup_domain_init(void) {

  /* Backup domain access enabled and left open.*/
  PWR->CR |= PWR_CR_DBP;

#if HAL_USE_RTC
  /* Reset BKP domain if different clock source selected.*/
  if ((RCC->BDCR & STM32_RTCSEL_MASK) != STM32_RTCSEL){
    /* Backup domain reset.*/
    RCC->BDCR = RCC_BDCR_BDRST;
    RCC->BDCR = 0;
  }

  /* If enabled then the LSE is started.*/
#if STM32_LSE_ENABLED
  RCC->BDCR |= RCC_BDCR_LSEON;
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
    ;                                     /* Waits until LSE is stable.   */
#endif /* STM32_LSE_ENABLED */

#if STM32_RTCSEL != STM32_RTCSEL_NOCLOCK
  /* If the backup domain hasn't been initialized yet then proceed with
     initialization.*/
  if ((RCC->BDCR & RCC_BDCR_RTCEN) == 0) {
    /* Selects clock source.*/
    RCC->BDCR |= STM32_RTCSEL;

    /* RTC clock enabled.*/
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* Prescaler value loaded in registers.*/
    rtc_lld_set_prescaler();
  }
#endif /* STM32_RTCSEL != STM32_RTCSEL_NOCLOCK */
#endif /* HAL_USE_RTC */
}

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

  /* Reset of all peripherals.*/
  rccResetAPB1(0xFFFFFFFF);
  rccResetAPB2(0xFFFFFFFF);

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* DWT cycle counter enable.*/
  SCS_DEMCR |= SCS_DEMCR_TRCENA;
  DWT_CTRL  |= DWT_CTRL_CYCCNTENA;

  /* PWR and BD clocks enabled.*/
  rccEnablePWRInterface(FALSE);
  rccEnableBKPInterface(FALSE);

  /* Initializes the backup domain.*/
  hal_lld_backup_domain_init();

#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif

  /* Programmable voltage detector enable.*/
#if STM32_PVD_ENABLE
  PWR->CR |= PWR_CR_PVDE | (STM32_PLS & STM32_PLS_MASK);
#endif /* STM32_PVD_ENABLE */
}

/**
 * @brief   STM32 clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) ||                    \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) ||                    \
    defined(STM32F10X_HD) || defined(STM32F10X_XL) ||                       \
    defined(__DOXYGEN__)
/*
 * Clocks initialization for all sub-families except CL.
 */
void stm32_clock_init(void) {

#if !STM32_NO_INIT
  /* HSI setup, it enforces the reset situation in order to handle possible
     problems with JTAG probes and re-initializations.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  RCC->CFGR = 0;                            /* CFGR reset value.            */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Waits until HSI is selected. */

#if STM32_HSE_ENABLED
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                                       /* Waits until HSE is stable.   */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                                       /* Waits until LSI is stable.   */
#endif

#if STM32_ACTIVATE_PLL
  /* PLL activation.*/
  RCC->CFGR |= STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
  RCC->CR   |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                                       /* Waits until PLL is stable.   */
#endif

  /* Clock settings.*/
#if STM32_HAS_USB
  RCC->CFGR = STM32_MCOSEL | STM32_USBPRE | STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#else
  RCC->CFGR = STM32_MCOSEL |                STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#endif

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS;

  /* Switching to the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  /* Switches clock source.*/
  RCC->CFGR |= STM32_SW;
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;                                       /* Waits selection complete.    */
#endif
#endif /* !STM32_NO_INIT */
}

#elif defined(STM32F10X_CL)
/*
 * Clocks initialization for the CL sub-family.
 */
void stm32_clock_init(void) {

#if !STM32_NO_INIT
  /* HSI setup.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CFGR = 0;
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Wait until HSI is the source.*/

#if STM32_HSE_ENABLED
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                                       /* Waits until HSE is stable.   */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                                       /* Waits until LSI is stable.   */
#endif

  /* Settings of various dividers and multipliers in CFGR2.*/
  RCC->CFGR2 = STM32_PLL3MUL | STM32_PLL2MUL | STM32_PREDIV2 |
               STM32_PREDIV1 | STM32_PREDIV1SRC;

  /* PLL2 setup, if activated.*/
#if STM32_ACTIVATE_PLL2
  RCC->CR |= RCC_CR_PLL2ON;
  while (!(RCC->CR & RCC_CR_PLL2RDY))
    ;                                        /* Waits until PLL2 is stable. */
#endif

  /* PLL3 setup, if activated.*/
#if STM32_ACTIVATE_PLL3
  RCC->CR |= RCC_CR_PLL3ON;
  while (!(RCC->CR & RCC_CR_PLL3RDY))
    ;                                        /* Waits until PLL3 is stable. */
#endif

  /* PLL1 setup, if activated.*/
#if STM32_ACTIVATE_PLL1
  RCC->CFGR |= STM32_PLLMUL | STM32_PLLSRC;
  RCC->CR   |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL1 is stable.              */
#endif

  /* Clock settings.*/
#if STM32_HAS_OTG1
  RCC->CFGR = STM32_MCOSEL | STM32_OTGFSPRE | STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#else
  RCC->CFGR = STM32_MCO    |                  STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#endif

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS; /* Flash wait states depending on clock.    */

  /* Switching to the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif
#endif /* !STM32_NO_INIT */
}
#else
void stm32_clock_init(void) {}
#endif

/** @} */
