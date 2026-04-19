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
 * @file    STM32U5xx/stm32_isr.h
 * @brief   STM32U5xx ISR handler header.
 *
 * @addtogroup STM32U5xx_ISR
 * @{
 */

#ifndef STM32_ISR_H
#define STM32_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * EXTI unit.
 */
#define STM32_EXTI0_HANDLER                 Vector6C
#define STM32_EXTI1_HANDLER                 Vector70
#define STM32_EXTI2_HANDLER                 Vector74
#define STM32_EXTI3_HANDLER                 Vector78
#define STM32_EXTI4_HANDLER                 Vector7C
#define STM32_EXTI5_HANDLER                 Vector80
#define STM32_EXTI6_HANDLER                 Vector84
#define STM32_EXTI7_HANDLER                 Vector88
#define STM32_EXTI8_HANDLER                 Vector8C
#define STM32_EXTI9_HANDLER                 Vector90
#define STM32_EXTI10_HANDLER                Vector94
#define STM32_EXTI11_HANDLER                Vector98
#define STM32_EXTI12_HANDLER                Vector9C
#define STM32_EXTI13_HANDLER                VectorA0
#define STM32_EXTI14_HANDLER                VectorA4
#define STM32_EXTI15_HANDLER                VectorA8

#define STM32_EXTI0_NUMBER                  11
#define STM32_EXTI1_NUMBER                  12
#define STM32_EXTI2_NUMBER                  13
#define STM32_EXTI3_NUMBER                  14
#define STM32_EXTI4_NUMBER                  15
#define STM32_EXTI5_NUMBER                  16
#define STM32_EXTI6_NUMBER                  17
#define STM32_EXTI7_NUMBER                  18
#define STM32_EXTI8_NUMBER                  19
#define STM32_EXTI9_NUMBER                  20
#define STM32_EXTI10_NUMBER                 21
#define STM32_EXTI11_NUMBER                 22
#define STM32_EXTI12_NUMBER                 23
#define STM32_EXTI13_NUMBER                 24
#define STM32_EXTI14_NUMBER                 25
#define STM32_EXTI15_NUMBER                 26

/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                Vector134
#define STM32_USART3_HANDLER                Vector13C
#define STM32_UART4_HANDLER                 Vector140
#define STM32_UART5_HANDLER                 Vector144
#define STM32_LPUART1_HANDLER               Vector148

#define STM32_USART1_NUMBER                 61
#define STM32_USART3_NUMBER                 63
#define STM32_UART4_NUMBER                  64
#define STM32_UART5_NUMBER                  65
#define STM32_LPUART1_NUMBER                66

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void irqInit(void);
  void irqDeinit(void);
#ifdef __cplusplus
}
#endif

#endif /* STM32_ISR_H */

/** @} */
