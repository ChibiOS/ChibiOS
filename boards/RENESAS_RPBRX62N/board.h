/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * Setup for Renesas RPBRX62N board
 */

/*
 * Board identifiers.
 */
#define BOARD_RPBRX62N
#define BOARD_NAME              "Renesas RPBRX62N"

/*
 * Board frequencies.
 */
#define EXTALCLK                12000000
#define SUBCLK                  32768

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define RX62NXBDFP

/* TODO: move this to hal/include/mii.h */
#define MII_LAN8700_ID          0x0007C0C4

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID            MII_LAN8700_ID

/*
 * Ethernet PHY address.
 */
#define BOARD_PHY_ADDRESS       31

#define RX62N_MAC_PHY_TIMEOUT   0

/*
 * Use ethernet PHY RMII interface (if defined).
 */
/*#define BOARD_PHY_RMII*/

/*
 * Ethernet PHY hardware reset
 * PORTA_ETH_RESETOUT must be held low for at least 100us.
 */
#define BOARD_PHY_RESET()       do {                                          \
  palClearPad(GPIO10, PORTA_ETH_RESETOUT);                                    \
  asm volatile ("mov.l   #2640,r2                                   \n\t"     \
                "1:                                                 \n\t"     \
                "sub     #1,r2                                      \n\t"     \
                "bne.b   1b                                         \n\t");   \
  palSetPad(GPIO10, PORTA_ETH_RESETOUT);                                      \
} while(0)

/*
 * PORT 0 initial setup.
 */
#define VAL_PORT0DIR            0x00
#define VAL_PORT0DATA           0x00
#define VAL_PORT0ICR            0x00
#define VAL_PORT0ODR            0x00

/*
 * PORT 1 initial setup.
 */
#define VAL_PORT1DIR            0x00
#define VAL_PORT1DATA           0x00
#define VAL_PORT1ICR            0x00
#define VAL_PORT1ODR            0x00

/*
 * PORT 2 initial setup.
 */
#define VAL_PORT2DIR            0x00
#define VAL_PORT2DATA           0x00
#define VAL_PORT2ICR            0x00
#define VAL_PORT2ODR            0x00

/*
 * PORT 3 initial setup.
 */
#define VAL_PORT3DIR            0x00
#define VAL_PORT3DATA           0x00
#define VAL_PORT3ICR            0x00
#define VAL_PORT3ODR            0x00

/*
 * PORT 4 initial setup.
 */
#define VAL_PORT4DIR            0x00
#define VAL_PORT4DATA           0x00
#define VAL_PORT4ICR            0x00

/*
 * PORT 5 initial setup.
 */
#define VAL_PORT5DIR            0x00
#define VAL_PORT5DATA           0x00
#define VAL_PORT5ICR            0x00

/*
 * GPIO A initial setup.
 */
#define VAL_PORTADIR            PAL_PORT_BIT(PORTA_LED0) |                  \
                                PAL_PORT_BIT(PORTA_LED1) |                  \
                                PAL_PORT_BIT(PORTA_LED2) |                  \
                                PAL_PORT_BIT(PORTA_MII_MDIO) |              \
                                PAL_PORT_BIT(PORTA_MII_MDC) |               \
                                PAL_PORT_BIT(PORTA_ETH_LINKSTA) |           \
                                PAL_PORT_BIT(PORTA_ETH_RESETOUT)
#define VAL_PORTADATA           PAL_PORT_BIT(PORTA_LED0) |                  \
                                PAL_PORT_BIT(PORTA_LED1) |                  \
                                PAL_PORT_BIT(PORTA_LED2) |                  \
                                PAL_PORT_BIT(PORTA_ETH_RESETOUT)
#define VAL_PORTAICR            PAL_PORT_BIT(PORTA_MII_MDIO) |              \
                                PAL_PORT_BIT(PORTA_ETH_LINKSTA)
#define VAL_PORTAPCR            0x00

/*
 * PORT B initial setup.
 */
#define VAL_PORTBDIR            PAL_PORT_BIT(PORTB_MII_TXD1) |              \
                                PAL_PORT_BIT(PORTB_MII_TXD0) |              \
                                PAL_PORT_BIT(PORTB_MII_TXEN)
#define VAL_PORTBDATA           0x00
#define VAL_PORTBICR            PAL_PORT_BIT(PORTB_MII_CRS) |               \
                                PAL_PORT_BIT(PORTB_MII_RXER) |              \
                                PAL_PORT_BIT(PORTB_MII_RXCLK) |             \
                                PAL_PORT_BIT(PORTB_MII_RXD0) |              \
                                PAL_PORT_BIT(PORTB_MII_RXD1)
#define VAL_PORTBPCR            0x00

/*
 * PORT C initial setup.
 */
#define VAL_PORTCDIR            PAL_PORT_BIT(PORTC_MII_TXD3) |              \
                                PAL_PORT_BIT(PORTC_MII_TXD2) |              \
                                PAL_PORT_BIT(PORTC_MII_TXER)
#define VAL_PORTCDATA           0x00
#define VAL_PORTCICR            PAL_PORT_BIT(PORTC_MII_COL) |               \
                                PAL_PORT_BIT(PORTC_MII_TXCLK) |             \
                                PAL_PORT_BIT(PORTC_MII_TXER) |              \
                                PAL_PORT_BIT(PORTC_MII_RXDV) |              \
                                PAL_PORT_BIT(PORTC_MII_RXD2) |              \
                                PAL_PORT_BIT(PORTC_MII_RXD3)
#define VAL_PORTCODR            0x00
#define VAL_PORTCPCR            0x00

/*
 * PORT D initial setup.
 */
#define VAL_PORTDDIR            0x00
#define VAL_PORTDDATA           0x00
#define VAL_PORTDICR            0x00
#define VAL_PORTDPCR            0x00

/*
 * PORT E initial setup.
 */
#define VAL_PORTEDIR            0x00
#define VAL_PORTEDATA           0x00
#define VAL_PORTEICR            0x00
#define VAL_PORTEPCR            0x00


/*
 * Pin definitions.
 */
#define PORTA_ETH_RESETOUT      7
#define PORTA_ETH_LINKSTA       5
#define PORTA_MII_MDC           4
#define PORTA_MII_MDIO          3
#define PORTA_LED0              2
#define PORTA_LED1              1
#define PORTA_LED2              0

#define PORTB_MII_CRS           7
#define PORTB_MII_TXD1          6
#define PORTB_MII_TXD0          5
#define PORTB_MII_TXEN          4
#define PORTB_MII_RXER          3
#define PORTB_MII_RXCLK         2
#define PORTB_MII_RXD0          1
#define PORTB_MII_RXD1          0

#define PORTC_MII_COL           7
#define PORTC_MII_TXD3          6
#define PORTC_MII_TXD2          5
#define PORTC_MII_TXCLK         4
#define PORTC_MII_TXER          3
#define PORTC_MII_RXDV          2
#define PORTC_MII_RXD2          1
#define PORTC_MII_RXD3          0

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
