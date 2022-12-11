/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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
 * Sandbox drivers configuration.
 */

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * SIO driver system settings.
 */
#define SB_SIO_USE_VUART1                   TRUE
#define SB_SIO_USE_VUART2                   FALSE
#define SB_SIO_VUART1_IRQ                   8
#define SB_SIO_VUART2_IRQ                   9

#define SANDBOX_MCUCONF

#endif /* MCUCONF_H */
