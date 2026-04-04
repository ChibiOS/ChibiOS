#ifndef HALCONF_H
#define HALCONF_H

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_9_1_

#include "mcuconf.h"

#define HAL_USE_PAL                         TRUE

#define HAL_USE_SAFETY                      FALSE
#define HAL_USE_ADC                         FALSE
#define HAL_USE_CAN                         FALSE
#define HAL_USE_CRY                         FALSE
#define HAL_USE_DAC                         FALSE
#define HAL_USE_EFL                         FALSE
#define HAL_USE_GPT                         FALSE
#define HAL_USE_I2C                         FALSE
#define HAL_USE_I2S                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MAC                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SERIAL                      FALSE
#define HAL_USE_SERIAL_USB                  FALSE
#define HAL_USE_SIO                         FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_TRNG                        FALSE
#define HAL_USE_UART                        FALSE
#define HAL_USE_USB                         FALSE
#define HAL_USE_WDG                         FALSE
#define HAL_USE_WSPI                        FALSE

#define PAL_USE_CALLBACKS                   TRUE
#define PAL_USE_WAIT                        TRUE

#endif /* HALCONF_H */
