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

/*
 * VIO drivers configuration.
 */

#ifndef XMCUCONF_H
#define XMCUCONF_H

#define __VIO_XMCUCONF__

/*
 * VRQ assignment.
 * Note, assigned VRQs numbers must match assignments on the host side.
 */
#define VIO_VUART1_IRQ                      8
#define VIO_VUART2_IRQ                      9
#define VIO_VSPI1_IRQ                       6
#define VIO_VSPI2_IRQ                       7

/*
 * SIO driver system settings.
 */
#define VIO_SIO_USE_VUART1                  TRUE
#define VIO_SIO_USE_VUART2                  FALSE

/*
 * SPI driver system settings.
 */
#define VIO_SPI_USE_VSPI1                   TRUE
#define VIO_SPI_USE_VSPI2                   FALSE

#endif /* XMCUCONF_H */
