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
 * @file    USB/stm32_usb1_lp_hp.inc
 * @brief   Shared USB1 low/high priority handlers.
 *
 * @addtogroup STM32_USB1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_USB)
#error "STM32_HAS_USB not defined in registry"
#endif

#if STM32_HAS_USB

#if !defined(STM32_USB1_HP_NUMBER)
#error "STM32_USB1_HP_NUMBER not defined in stm32_isr.h"
#endif

#if !defined(STM32_USB1_HP_HANDLER)
#error "STM32_USB1_HP_HANDLER not defined in stm32_isr.h"
#endif

#if !defined(STM32_USB1_LP_NUMBER)
#error "STM32_USB1_LP_NUMBER not defined in stm32_isr.h"
#endif

#if !defined(STM32_USB1_LP_HANDLER)
#error "STM32_USB1_LP_HANDLER not defined in stm32_isr.h"
#endif

#if !defined(STM32_USB_USE_USB1)
#error "STM32_USB_USE_USB1 not defined in xmcuconf.h"
#endif

#if (STM32_USB1_HP_NUMBER != STM32_USB1_LP_NUMBER) &&                       \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_USB_USB1_HP_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_USB_USB1_HP_IRQ_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_USB_USB1_LP_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_USB_USB1_LP_IRQ_PRIORITY"
#endif

#endif /* STM32_HAS_USB */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void usb1_irq_init(void) {
#if defined(STM32_USB1_IS_USED)
#if STM32_USB1_HP_NUMBER != STM32_USB1_LP_NUMBER
#if STM32_USB_USE_ISOCHRONOUS
  nvicEnableVector(STM32_USB1_HP_NUMBER, STM32_USB_USB1_HP_IRQ_PRIORITY);
#endif
#endif
  nvicEnableVector(STM32_USB1_LP_NUMBER, STM32_USB_USB1_LP_IRQ_PRIORITY);
#endif
}

static inline void usb1_irq_deinit(void) {
#if defined(STM32_USB1_IS_USED)
#if STM32_USB1_HP_NUMBER != STM32_USB1_LP_NUMBER
#if STM32_USB_USE_ISOCHRONOUS
  nvicDisableVector(STM32_USB1_HP_NUMBER);
#endif
#endif
  nvicDisableVector(STM32_USB1_LP_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(STM32_USB1_IS_USED) || defined(__DOXYGEN__)
#if STM32_USB1_HP_NUMBER != STM32_USB1_LP_NUMBER
#if STM32_USB_USE_ISOCHRONOUS
OSAL_IRQ_HANDLER(STM32_USB1_HP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  usb_lld_serve_endpoints_interrupt(&USBD1);
  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

OSAL_IRQ_HANDLER(STM32_USB1_LP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  usb_lld_serve_interrupt(&USBD1);
  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
