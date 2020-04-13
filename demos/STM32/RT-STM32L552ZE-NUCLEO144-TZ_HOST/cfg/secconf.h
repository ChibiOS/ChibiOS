/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

/*
 * STM32L5xx security configuration.
 */

#ifndef SECCONF_H
#define SECCONF_H

#define STM32L5xx_SECCONF
#define STM32L552_SECCONF
#define STM32L562_SECCONF

/*
 * Security flash settings.
 */
#define STM32_FLASH_OVERRIDE_SETTINGS       TRUE
#define STM32_FLASH_SECWM1R1                (STM32_FLASH_SECWM_START(0x00) | STM32_FLASH_SECWM_END(0x3F))
#define STM32_FLASH_SECWM2R1                (STM32_FLASH_SECWM_START(0x7F) | STM32_FLASH_SECWM_END(0x00))
#define STM32_FLASH_SECBB1R1                0x00000000
#define STM32_FLASH_SECBB1R2                0x00000000
#define STM32_FLASH_SECBB1R3                0x00000000
#define STM32_FLASH_SECBB1R4                0x00000000
#define STM32_FLASH_SECBB2R1                0x00000000
#define STM32_FLASH_SECBB2R2                0x00000000
#define STM32_FLASH_SECBB2R3                0x00000000
#define STM32_FLASH_SECBB2R4                0x00000000

/*
 * Security RAM settings.
 * VCTR:   256b per bit.
 * LCKVTR: 8kB per bit.
 */
#define STM32_MPCBB1_CR                     (GTZC_MPCBB_CR_SRWILADIS_Msk | GTZC_MPCBB_CR_LCK_Msk)
#define STM32_MPCBB1_LCKVTR1                0x00000000
#define STM32_MPCBB1_VCTR0                  0xFFFFFFFF
#define STM32_MPCBB1_VCTR1                  0xFFFFFFFF
#define STM32_MPCBB1_VCTR2                  0xFFFFFFFF
#define STM32_MPCBB1_VCTR3                  0xFFFFFFFF
#define STM32_MPCBB1_VCTR4                  0x00000000
#define STM32_MPCBB1_VCTR5                  0x00000000
#define STM32_MPCBB1_VCTR6                  0x00000000
#define STM32_MPCBB1_VCTR7                  0x00000000
#define STM32_MPCBB1_VCTR8                  0x00000000
#define STM32_MPCBB1_VCTR9                  0x00000000
#define STM32_MPCBB1_VCTR10                 0x00000000
#define STM32_MPCBB1_VCTR11                 0x00000000
#define STM32_MPCBB1_VCTR12                 0x00000000
#define STM32_MPCBB1_VCTR13                 0x00000000
#define STM32_MPCBB1_VCTR14                 0x00000000
#define STM32_MPCBB1_VCTR15                 0x00000000
#define STM32_MPCBB1_VCTR16                 0x00000000
#define STM32_MPCBB1_VCTR17                 0x00000000
#define STM32_MPCBB1_VCTR18                 0x00000000
#define STM32_MPCBB1_VCTR19                 0x00000000
#define STM32_MPCBB1_VCTR20                 0x00000000
#define STM32_MPCBB1_VCTR21                 0x00000000
#define STM32_MPCBB1_VCTR22                 0x00000000
#define STM32_MPCBB1_VCTR23                 0x00000000

#define STM32_MPCBB2_CR                     (GTZC_MPCBB_CR_SRWILADIS_Msk | GTZC_MPCBB_CR_LCK_Msk)
#define STM32_MPCBB2_LCKVTR1                0x00000000
#define STM32_MPCBB2_VCTR0                  0x00000000
#define STM32_MPCBB2_VCTR1                  0x00000000
#define STM32_MPCBB2_VCTR2                  0x00000000
#define STM32_MPCBB2_VCTR3                  0x00000000
#define STM32_MPCBB2_VCTR4                  0x00000000
#define STM32_MPCBB2_VCTR5                  0x00000000
#define STM32_MPCBB2_VCTR6                  0x00000000
#define STM32_MPCBB2_VCTR7                  0x00000000

#endif /* SECCONF_H */
