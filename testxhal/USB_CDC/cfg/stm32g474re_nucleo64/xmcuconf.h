/*
    Wrapper over the existing G474 XHAL MCU configuration.
*/

#ifndef TESTXHAL_USB_CDC_XMCUCONF_WRAPPER_H
#define TESTXHAL_USB_CDC_XMCUCONF_WRAPPER_H

#include "../../../RTC/cfg/stm32g474re_nucleo64/xmcuconf.h"

#define STM32_USB_USE_USB1                  TRUE
#define STM32_USB_LOW_POWER_ON_SUSPEND      FALSE
#define STM32_USB_USB1_HP_IRQ_PRIORITY      13
#define STM32_USB_USB1_LP_IRQ_PRIORITY      14

#endif /* TESTXHAL_USB_CDC_XMCUCONF_WRAPPER_H */
