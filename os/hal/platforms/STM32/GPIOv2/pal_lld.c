/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
*/

/**
 * @file    STM32/GPIOv2/pal_lld.c
 * @brief   STM32L1xx/STM32F2xx GPIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

#if defined(STM32L1XX_MD)
#define AHB_EN_MASK     (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |           \
                         RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN |           \
                         RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOHEN)
#define AHB_LPEN_MASK   AHB_EN_MASK
#elif defined(STM32F2XX)
#define AHB1_EN_MASK    (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |            \
                         RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN |            \
                         RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |            \
                         RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN |            \
					     RCC_AHB1ENR_GPIOIEN)
#define AHB1_LPEN_MASK  AHB1_EN_MASK
#else
#error "missing or usupported platform for GPIOv2 PAL driver"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void initgpio(GPIO_TypeDef *gpiop, const stm32_gpio_setup_t *config) {

  gpiop->MODER   = config->moder;
  gpiop->OTYPER  = config->otyper;
  gpiop->OSPEEDR = config->ospeedr;
  gpiop->PUPDR   = config->pupdr;
  gpiop->ODR     = config->odr;
  gpiop->AFRL    = config->afrl;
  gpiop->AFRH    = config->afrh;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 I/O ports configuration.
 * @details Ports A-D(E, F, G, H) clocks enabled.
 *
 * @param[in] config    the STM32 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  /*
   * Enables the GPIO related clocks.
   */
#if defined(STM32L1XX_MD)
  RCC->AHBENR   |= AHB_EN_MASK;
  RCC->AHBLPENR |= AHB_LPEN_MASK;
#elif defined(STM32F2XX)
  RCC->AHB1ENR   |= AHB1_EN_MASK;
  RCC->AHB1LPENR |= AHB1_LPEN_MASK;
#endif

  /*
   * Initial GPIO setup.
   */
  initgpio(GPIOA, &config->PAData);
  initgpio(GPIOB, &config->PBData);
  initgpio(GPIOC, &config->PCData);
  initgpio(GPIOD, &config->PDData);
#if STM32_HAS_GPIOE
  initgpio(GPIOE, &config->PEData);
#endif
#if STM32_HAS_GPIOF
  initgpio(GPIOF, &config->PFData);
#endif
#if STM32_HAS_GPIOG
  initgpio(GPIOG, &config->PGData);
#endif
#if STM32_HAS_GPIOH
  initgpio(GPIOH, &config->PHData);
#endif
#if STM32_HAS_GPIOI
  initgpio(GPIOI, &config->PIData);
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull at minimum
 *          speed.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {
}

#endif /* HAL_USE_PAL */

/** @} */
