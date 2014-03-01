/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC4330 SBC board config file - Copyright (C) 2013 Marcin Jokel

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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for non standard board LPC4330 SBC.
 */

/*
 * Board identifiers.
 */
#define BOARD_NONSTANDARD_LPC4330_SBC
#define BOARD_NAME "LPC4330 SBC"

#define RESET_ON_RESTART_DEBUG  /* Only for debug. */

/*
 * Ethernet PHY type.
 */
#define MII_KSZ8031_ID          0x00221556
#define BOARD_PHY_ID            MII_KSZ8031_ID
#define BOARD_PHY_RMII

/*
 * Board frequencies.
 */
#define XTAL                     12000000UL
#define RTC_XTAL                 32768UL
#define ENET_RX_CLK              50000000UL
#define ENET_TX_CLK              50000000UL

/*
 * GPIO 0 initial setup.
 */
#define VAL_GPIO0DIR            0x00000000
#define VAL_GPIO0DATA           0x00000000

/*
 * GPIO 1 initial setup.
 */
#define VAL_GPIO1DIR            0x00000000
#define VAL_GPIO1DATA           0x00000000

/*
 * GPIO 2 initial setup.
 */
#define VAL_GPIO2DIR            PAL_PORT_BIT(GPIO2_LED)
#define VAL_GPIO2DATA           PAL_PORT_BIT(GPIO2_LED)

/*
 * GPIO 3 initial setup.
 */
#define VAL_GPIO3DIR            0x00000000
#define VAL_GPIO3DATA           0x00000000

/*
 * GPIO 4 initial setup.
 */
#define LPC_HAS_GPIO4           TRUE
#define VAL_GPIO4DIR            0x00000000
#define VAL_GPIO4DATA           0x00000000

/*
 * GPIO 5 initial setup.
 */
#define VAL_GPIO5DIR            PAL_PORT_BIT(GPIO5_SD_CS)
#define VAL_GPIO5DATA           PAL_PORT_BIT(GPIO5_SD_CS)

/*
 * GPIO 6 initial setup.
 */
#define LPC_HAS_GPIO6           FALSE

/*
 * GPIO 7 initial setup.
 */
#define LPC_HAS_GPIO7           FALSE

/*
 * Pin definitions.
 */
#define GPIO2_LED               0

#define GPIO3_SW_USER1          13
#define GPIO3_SW_USER2          14

#define GPIO5_SD_CS             8

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
