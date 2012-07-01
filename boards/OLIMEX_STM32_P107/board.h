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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Olimex STM32-P107 evaluation board.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_STM32_P107
#define BOARD_NAME              "Olimex STM32-P107"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            25000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F10X_CL

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID            MII_STE101P_ID
#define BOARD_PHY_RMII

/*
 * IO pins assignments.
 */
#define GPIOA_SWITCH_WKUP       0

#define GPIOC_LED_STATUS1       6
#define GPIOC_LED_STATUS2       7
#define GPIOC_SWITCH_TAMPER     13

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0  - Normal input      (WKUP BUTTON).
 * PA1  - Normal input      (ETH_RMII_REF_CLK).
 * PA2  - Alternate output  (ETH_RMII_MDIO).
 * PA3  - Input with PU     (unconnected).
 * PA4  - Push Pull output  (CS_MMC).
 * PA5  - Input with PU     (unconnected).
 * PA6  - Input with PU     (unconnected).
 * PA7  - Normal input      (ETH_RMII_CRS_DV).
 * PA8  - Alternate output  (MCO).
 * PA9  - Normal input      (OTG_VBUS).
 * PA10 - Normal input      (OTG_ID).
 * PA11 - Normal input      (OTG_DM).
 * PA12 - Normal input      (OTG_DP).
 * PA13 - Normal input      (TMS).
 * PA14 - Normal input      (TCK).
 * PA15 - Normal input      (TDI).
 */
#define VAL_GPIOACRL            0x48838B44      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x4444444B      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup:
 * PB0  - Input with PU     (unconnected).
 * PB1  - Input with PU     (unconnected).
 * PB2  - Normal input      (BOOT1).
 * PB3  - Normal input      (TDO).
 * PB4  - Normal input      (TRST).
 * PB5  - Input with PU     (unconnected).
 * PB6  - Input with PU     (unconnected).
 * PB7  - Input with PU     (unconnected).
 * PB8  - Alternate O.D.    (I2C1 SCL, remapped).
 * PB9  - Alternate O.D.    (I2C1 SDA, remapped).
 * PB10 - Input with PU     (unconnected).
 * PB11 - Alternate output  (ETH_RMII_TX_EN).
 * PB12 - Alternate output  (ETH_RMII_TXD0).
 * PB13 - Alternate output  (ETH_RMII_TXD1).
 * PB14 - Input with PU     (unconnected).
 * PB15 - Push Pull output  (CS_UEXT).
 */
#define VAL_GPIOBCRL            0x88844488      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x38BBB8FF      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup:
 * PC0  - Input with PU     (unconnected).
 * PC1  - Alternate output  (ETH_MDC).
 * PC2  - Input with PU     (unconnected).
 * PC3  - Input with PU     (unconnected).
 * PC4  - Normal input      (ETH_RMII_RXD0).
 * PC5  - Normal input      (ETH_RMII_RXD1).
 * PC6  - Push Pull output  (STAT1 green LED).
 * PC7  - Push Pull output  (STAT2 yellow LED).
 * PC8  - Input with PU     (unconnected).
 * PC9  - Input with PU     (unconnected).
 * PC10 - Alternate output  (SPI3 SCK).
 * PC11 - Input with PU     (SPI3 MISO).
 * PC12 - Alternate output  (SPI3 MOSI).
 * PC13 - Normal input      (TAMPER).
 * PC14 - Normal input      (OSC32 IN).
 * PC15 - Normal input      (OSC32 OUT).
 */
#define VAL_GPIOCCRL            0x334488B8      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x444B8B88      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFF3F

/*
 * Port D setup:
 * PD0  - Input with PU     (unconnected).
 * PD1  - Input with PU     (unconnected).
 * PD2  - Input with PU     (unconnected).
 * PD3  - Input with PU     (unconnected).
 * PD4  - Input with PU     (unconnected).
 * PD5  - Alternate output  (USART2 TX, UEXT).
 * PD6  - Input with PU     (USART2 RX, UEXT).
 * PD7  - Push Pull output  (USB_VBUSON).
 * PD8  - Alternate output  (USART2 TX, remapped).
 * PD9  - Normal input      (USART2 RX, remapped).
 * PD10 - Input with PU     (unconnected).
 * PD11 - Normal input      (USART2 CTS, remapped).
 * PD12 - Alternate output  (USART2 RTS, remapped).
 * PD13 - Input with PU     (unconnected).
 * PD14 - Input with PU     (unconnected).
 * PD15 - Input with PU     (unconnected).
 */
#define VAL_GPIODCRL            0x38B88888      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x888B484B      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 * PE14 - Normal input      (ETH_RMII_MDINT).
 * PE15 - Normal input      (USB_FAULT).
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x44888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

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
