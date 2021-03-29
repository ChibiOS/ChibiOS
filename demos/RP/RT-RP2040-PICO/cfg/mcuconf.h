/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * RP2040_MCUCONF drivers configuration.
 */

#define RP2040_MCUCONF

/*
 * HAL driver system settings.
 */
#define RP_NO_INIT                          FALSE

/*
 * IRQ system settings.
 */
#define RP_IRQ_SYSTICK_PRIORITY             2
#define RP_IRQ_TIMER_ALARM0_PRIORITY        2
#define RP_IRQ_TIMER_ALARM1_PRIORITY        2
#define RP_IRQ_TIMER_ALARM2_PRIORITY        2
#define RP_IRQ_TIMER_ALARM3_PRIORITY        2
#define RP_IRQ_UART0_PRIORITY               3
#define RP_IRQ_UART1_PRIORITY               3

/*
 * SIO driver system settings.
 */
#define RP_SIO_USE_UART0                    TRUE
#define RP_SIO_USE_UART1                    TRUE

#endif /* MCUCONF_H */
