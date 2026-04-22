/*
    Wrapper over the existing G474 XHAL kernel configuration.
*/

#ifndef TESTXHAL_USB_CDC_CHCONF_WRAPPER_H
#define TESTXHAL_USB_CDC_CHCONF_WRAPPER_H

#include "../../../RTC/cfg/stm32g474re_nucleo64/chconf.h"

#define CH_CFG_USE_TM                       TRUE
#define CH_CFG_USE_REGISTRY                 TRUE
#define CH_CFG_USE_MUTEXES                  TRUE
#define CH_CFG_USE_EVENTS                   TRUE
#define CH_CFG_USE_MEMCORE                  TRUE
#define CH_CFG_USE_HEAP                     TRUE
#define CH_CFG_USE_MEMPOOLS                 TRUE
#define CH_CFG_USE_FACTORY                  TRUE

#endif /* TESTXHAL_USB_CDC_CHCONF_WRAPPER_H */
