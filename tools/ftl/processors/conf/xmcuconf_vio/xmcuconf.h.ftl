[#ftl]
[#--
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="xmcuconf.h" /]
/*
[@license.EmitLicenseAsText /]
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
#define VIO_VUART1_IRQ                      ${doc.VIO_VUART1_IRQ!"8"}
#define VIO_VUART2_IRQ                      ${doc.VIO_VUART2_IRQ!"9"}
#define VIO_VSPI1_IRQ                       ${doc.VIO_VSPI1_IRQ!"6"}
#define VIO_VSPI2_IRQ                       ${doc.VIO_VSPI2_IRQ!"7"}

/*
 * SIO driver system settings.
 */
#define VIO_SIO_USE_VUART1                  ${doc.VIO_SIO_USE_VUART1!"FALSE"}
#define VIO_SIO_USE_VUART2                  ${doc.VIO_SIO_USE_VUART2!"FALSE"}

/*
 * SPI driver system settings.
 */
#define VIO_SPI_USE_VSPI1                   ${doc.VIO_SPI_USE_VSPI1!"FALSE"}
#define VIO_SPI_USE_VSPI2                   ${doc.VIO_SPI_USE_VSPI2!"FALSE"}

#endif /* XMCUCONF_H */
