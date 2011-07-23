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
 * @file    STM32/pal_lld.c
 * @brief   STM32 GPIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

#if STM32_HAS_GPIOG
#define APB2_EN_MASK  (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |            \
                       RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN |            \
                       RCC_APB2ENR_IOPEEN | RCC_APB2ENR_IOPFEN |            \
                       RCC_APB2ENR_IOPGEN | RCC_APB2ENR_AFIOEN)
#elif STM32_HAS_GPIOE
#define APB2_EN_MASK  (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |            \
                       RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN |            \
                       RCC_APB2ENR_IOPEEN | RCC_APB2ENR_AFIOEN)
#else
#define APB2_EN_MASK  (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |            \
                       RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN |            \
                       RCC_APB2ENR_AFIOEN)
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
  gpiop->AFRL    = 0;
  gpiop->AFRH    = 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 I/O ports configuration.
 * @details Ports A-D(E, F, G) clocks enabled, AFIO clock enabled.
 *
 * @param[in] config    the STM32 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  /*
   * Enables the GPIO related clocks.
   */
  RCC->AHBENR   |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN |
                   RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN |
                   RCC_AHBENR_GPIOEEN | RCC_AHBENR_GPIOHEN;
  RCC->AHBLPENR |= RCC_AHBLPENR_GPIOALPEN | RCC_AHBLPENR_GPIOBLPEN |
                   RCC_AHBLPENR_GPIOCLPEN | RCC_AHBLPENR_GPIODLPEN |
                   RCC_AHBLPENR_GPIOELPEN | RCC_AHBLPENR_GPIOHLPEN;

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
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull output at 2MHz.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
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
