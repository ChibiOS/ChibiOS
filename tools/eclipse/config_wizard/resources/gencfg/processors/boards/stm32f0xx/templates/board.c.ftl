[#ftl]
[#--
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
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="board.c" /]
/*
[@license.EmitLicenseAsText /]
*/

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)
/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
const PALConfig pal_default_config =
{
  {VAL_GPIOA_MODER, VAL_GPIOA_OTYPER, VAL_GPIOA_OSPEEDR, VAL_GPIOA_PUPDR,
   VAL_GPIOA_ODR,   VAL_GPIOA_AFRL,   VAL_GPIOA_AFRH},
  {VAL_GPIOB_MODER, VAL_GPIOB_OTYPER, VAL_GPIOB_OSPEEDR, VAL_GPIOB_PUPDR,
   VAL_GPIOB_ODR,   VAL_GPIOB_AFRL,   VAL_GPIOB_AFRH},
  {VAL_GPIOC_MODER, VAL_GPIOC_OTYPER, VAL_GPIOC_OSPEEDR, VAL_GPIOC_PUPDR,
   VAL_GPIOC_ODR,   VAL_GPIOC_AFRL,   VAL_GPIOC_AFRH},
  {VAL_GPIOD_MODER, VAL_GPIOD_OTYPER, VAL_GPIOD_OSPEEDR, VAL_GPIOD_PUPDR,
   VAL_GPIOD_ODR,   VAL_GPIOD_AFRL,   VAL_GPIOD_AFRH},
  {VAL_GPIOF_MODER, VAL_GPIOF_OTYPER, VAL_GPIOF_OSPEEDR, VAL_GPIOF_PUPDR,
   VAL_GPIOF_ODR,   VAL_GPIOF_AFRL,   VAL_GPIOF_AFRH}
};
#endif

/**
 * @brief   Early initialization code.
 * @details This initialization must be performed just after stack setup
 *          and before any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
[#if doc1.board.board_functions.__early_init[0]??]
  ${doc1.board.board_functions.__early_init[0]}
[/#if]
}

#if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
/**
 * @brief   MMC_SPI card detection.
 */
bool_t mmc_lld_is_card_inserted(MMCDriver *mmcp) {
[#if doc1.board.board_functions.mmc_lld_is_card_inserted[0]??]
${doc1.board.board_functions.mmc_lld_is_card_inserted[0]}
[#else]

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return TRUE;
[/#if]
}

/**
 * @brief   MMC_SPI card write protection detection.
 */
bool_t mmc_lld_is_write_protected(MMCDriver *mmcp) {
[#if doc1.board.board_functions.mmc_lld_is_write_protected[0]??]
${doc1.board.board_functions.mmc_lld_is_write_protected[0]}
[#else]

  (void)mmcp;
  /* TODO: Fill the implementation.*/
  return FALSE;
[/#if]
}
#endif

/**
 * @brief   Board-specific initialization code.
 * @todo    Add your board-specific code, if any.
 */
void boardInit(void) {
}
