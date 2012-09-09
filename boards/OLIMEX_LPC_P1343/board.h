/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Olimex LPC-P1343 proto board.
 */

/*
 * Board identifiers.
 */
#define BOARD_OLIMEX_LPC_P1343
#define BOARD_NAME "Olimex LPC-P1343"

/*
 * Board frequencies.
 */
#define SYSOSCCLK               12000000

/*
 * GPIO 0 initial setup.
 */
#define VAL_GPIO0DIR            0x00000000
#define VAL_GPIO0DATA           0x00000000

/*
 * GPIO 1 initial setup.
 */
#define VAL_GPIO1DIR            PAL_PORT_BIT(GPIO1_SW2)

#define VAL_GPIO1DATA           PAL_PORT_BIT(GPIO1_SW2)

/*
 * GPIO 2 initial setup.
 */

#define VAL_GPIO2DIR            PAL_PORT_BIT(GPIO2_SW1)     | \
                                PAL_PORT_BIT(GPIO2_LED5)    | \
                                PAL_PORT_BIT(GPIO2_LED6)    | \
                                PAL_PORT_BIT(GPIO2_LED7)    | \
                                PAL_PORT_BIT(GPIO2_LED8)

#define VAL_GPIO2DATA           PAL_PORT_BIT(GPIO2_LED5)    | \
                                PAL_PORT_BIT(GPIO2_LED6)    | \
                                PAL_PORT_BIT(GPIO2_LED7)    | \
                                PAL_PORT_BIT(GPIO2_LED8)

/*
 * GPIO 3 initial setup.
 */

#define VAL_GPIO3DIR            PAL_PORT_BIT(GPIO3_LED1)    | \
                                PAL_PORT_BIT(GPIO3_LED2)    | \
                                PAL_PORT_BIT(GPIO3_LED3)    | \
                                PAL_PORT_BIT(GPIO3_LED4)

#define VAL_GPIO3DATA           PAL_PORT_BIT(GPIO3_LED1)    | \
                                PAL_PORT_BIT(GPIO3_LED2)    | \
                                PAL_PORT_BIT(GPIO3_LED3)    | \
                                PAL_PORT_BIT(GPIO3_LED4)

/*
 * Pin definitions.
 */
#define GPIO1_SW2               4
#define GPIO1_SPI0SEL           11

#define GPIO2_SW1               9

#define GPIO3_LED1              0
#define GPIO3_LED2              1
#define GPIO3_LED3              2
#define GPIO3_LED4              3
#define GPIO2_LED5              4
#define GPIO2_LED6              5
#define GPIO2_LED7              6
#define GPIO2_LED8              7

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
