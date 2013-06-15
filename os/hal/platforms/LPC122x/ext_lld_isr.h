/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x EXT driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC122x/ext_lld_isr.h
 * @brief   LPC122x EXT subsystem low level driver ISR header.
 *
 * @addtogroup EXT
 * @{
 */

#ifndef _EXT_LLD_ISR_H_
#define _EXT_LLD_ISR_H_

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define EXTI0_IRQ           0
#define EXTI1_IRQ           1
#define EXTI2_IRQ           2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EXTI0 interrupt priority level setting.
 */
#if !defined(LPC122x_EXT_EXTI0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_EXT_EXTI0_IRQ_PRIORITY      3
#endif

/**
 * @brief   EXTI1 interrupt priority level setting.
 */
#if !defined(LPC122x_EXT_EXTI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_EXT_EXTI1_IRQ_PRIORITY      3
#endif

/**
 * @brief   EXTI2 interrupt priority level setting.
 */
#if !defined(LPC122x_EXT_EXTI2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_EXT_EXTI2_IRQ_PRIORITY      3
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   EXT irq port identifier.
 */
typedef uint32_t extirq_t;
/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void ext_lld_exti_irq_enable(extirq_t irqn);
  void ext_lld_exti_irq_disable(extirq_t irqn);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EXT */

#endif /* _EXT_LLD_ISR_H_ */

/** @} */
