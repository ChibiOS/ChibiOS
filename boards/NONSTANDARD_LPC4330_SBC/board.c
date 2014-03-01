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

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
 {VAL_GPIO0DATA, VAL_GPIO0DIR},
 {VAL_GPIO1DATA, VAL_GPIO1DIR},
 {VAL_GPIO2DATA, VAL_GPIO2DIR},
 {VAL_GPIO3DATA, VAL_GPIO3DIR},
 {VAL_GPIO4DATA, VAL_GPIO4DIR},
 {VAL_GPIO5DATA, VAL_GPIO5DIR}
};
#endif

/*
 * Board Ethernet pins configuration.
 * ENET_REF_CLK pin must be set before macInit().
 */
static void board_eth_pin_config(void) {

  /* Ethernet I/O config */
  LPC_SCU->SFSP0_0  = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x02;          /* Set ENET RXD1 P0.0, disable pull-up resistor.*/
  LPC_SCU->SFSP0_1  = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x06;          /* Set ENET TX EN P0.1, disable pull-up resistor.*/
  LPC_SCU->SFSP1_15 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x03;          /* Set ENET RXD0 P1.15, disable pull-up resistor.*/
  LPC_SCU->SFSP1_16 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x07;          /* Set ENET CRS P1.16, disable pull-up resistor.*/
  LPC_SCU->SFSP1_17 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x03;          /* Set ENET MDIO P1.17, disable pull-up resistor.*/
  LPC_SCU->SFSP1_18 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x03;          /* Set ENET TXD0  P1.18, disable pull-up resistor.*/
  LPC_SCU->SFSP1_19 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x00;          /* Set ENET REF CLK P1.19, disable pull-up resistor.*/
  LPC_SCU->SFSP1_20 = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x03;          /* Set ENET TXD1 P1.20, disable pull-up resistor.*/
  LPC_SCU->SFSP7_7  = (1 << 7) | (1 << 5) | (1 << 6) | (1 << 4) | 0x06;          /* Set ENET MDC P7.7, disable pull-up resistor.*/
}

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

#ifdef RESET_ON_RESTART_DEBUG
  uint32_t i;
  __disable_irq();

  /* Reset peripherals. */
  LPC_RGU->RESET_CTRL0 = 0x10DF1000;
  LPC_RGU->RESET_CTRL1 = 0x01DFF7FF;

  for (i = 0; i < 8; i++) {
    NVIC->ICPR[i] = 0xFFFFFFFF;     /* Clear pending interrupts in the NVIC. */
  }

  __enable_irq();

#endif

  lpc_clock_init();

#if HAL_USE_MAC
  board_eth_pin_config();
#endif

}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * Extra, board-specific, initializations.
   */

  /* UART3 I/O config */
  LPC_SCU->SFSP2_3 = (1 << 4) | 0x02;                       /* Set UART3 TXD3 P2.3, disable pull-up resistor.*/
  LPC_SCU->SFSP2_4 = (1 << 6) | (1 << 4) | 0x02;            /* Set UART3 RXD3 P2.4, disable pull-up resistor.*/

  /* SD card SSP1 I/O config */
  LPC_SCU->SFSPF_4 = (1 << 4) | 0x00;                       /* Set SSP1 SCK PF.4, disable pull-up resistor.*/
  LPC_SCU->SFSP1_3 = (1 << 6) | (1 << 4) | 0x05;            /* Set SSP1 MISO P1.3, disable pull-up resistor.*/
  LPC_SCU->SFSP1_4 = (1 << 4) | 0x05;                       /* Set SSP1 MOSI P1.4, disable pull-up resistor.*/
  LPC_SCU->SFSP3_1 = (1 << 4) | 0x04;                       /* Set SSP1 CS GPIO P3.1, disable pull-up resistor.*/

  /* I2C0 I/O config */
  //LPC_SCU->SFSI2C0 = (1 << 11) | (1 << 3);                /* Set I2C0 SDA and SCL, standard/fast mode, enable the input receiver.*/

  /* I/O for switches config */
  LPC_SCU->SFSP7_5 = (1 << 6) | (1 << 4);                   /* Set SW_1 P7.5, enable input buffer, disable pull-up resistor.*/
  LPC_SCU->SFSP7_6 = (1 << 6) | (1 << 4);                   /* Set SW_2 P7.6, enable input buffer, disable pull-up resistor.*/

  /* I2S0 I/O config */
  //LPC_SCU->SFSP3_0  = (1 << 4) | 0x03;                    /* Set I2S0 TX MCLK P3.0, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_2  = (1 << 4) | 0x01;                    /* Set I2S0 RX SDA P3.2, disable pull-up resistor.*/
  //LPC_SCU->SFSP4_7 = (1 << 4) | 0x07;                     /* Set I2S0 TX SCK P4.7, disable pull-up resistor.*/
  //LPC_SCU->SFSP7_1 = (1 << 4) | 0x02;                     /* Set I2S0 TX WS P7.1, disable pull-up resistor.*/
  //LPC_SCU->SFSP7_2 = (1 << 4) | 0x02;                     /* Set I2S0 TX SDA P7.2, disable pull-up resistor.*/

  /* SDRAM I/O config */
  /* Address lines */
  //LPC_SCU->SFSP2_9  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A0 P2.9, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_10 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A1 P2.10, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_11 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A2 P2.11, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_12 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A3 P2.12, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_13 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A4 P2.13, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_0  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A5 P1.0, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_1  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A6 P1.1, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_2  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A7 P1.2, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_8  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A8 P2.8, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_7  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;    /* Set EMC A9 P2.9, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_6  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A10 P2.6, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_2  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A11 P2.2, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_1  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A12 P2.1, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP2_0  = (1 << 7) | (1 << 5) | (1 << 4) | 0x02;    /* Set EMC A13 P2.0, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_8  = (1 << 7) | (1 << 5) | (1 << 4) | 0x01;    /* Set EMC A14 P2.13, disable glitch filter, fast mode, disable pull-up resistor.*/

  /* Data lines */
  //LPC_SCU->SFSP1_7  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D0 P1.7, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_8  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D1 P1.8, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_9  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D2 P1.9, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_10 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D3 P1.10, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_11 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D4 P1.11, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_12 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D5 P1.12, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_13 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D6 P1.13, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP1_14 = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set EMC D7 P1.14, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_4  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D8 P5.4, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_5  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D9 P5.5, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_6  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D10 P5.6, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_7  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D11 P5.7, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_0  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D12 P5.0, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_1  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D13 P5.1, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_2  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D14 P5.2, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/
  //LPC_SCU->SFSP5_3  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x02;     /* Set EMC D15 P5.3, disable glitch filter, fast mode, input buffer, disable pull-up resistor.*/

  //LPC_SCU->SFSP1_6  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC WE P1.6, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSCLK_0 = (1 << 7) | (1 << 5) | (1 << 4) | 0x00;        /* Set EMC CLK0, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSCLK_2 = (1 << 7) | (1 << 5) | (1 << 4) | 0x00;        /* Set EMC CLK2, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->EMCDELAYCLK = 0;     /* No clock delay.*/

  //LPC_SCU->SFSP6_4  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC CAS P6.4, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_5  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC RAS P6.5, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_11 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC CKEOUT0 P6.11, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_12 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC DQMOUT0 P6.12, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_10 = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC DQMOUT1 P6.10, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP6_9  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;        /* Set EMC DYCS0 P6.9, disable glitch filter, fast mode, disable pull-up resistor.*/

  /* SPIFI I/O config */
  //LPC_SCU->SFSP3_3  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;                /* Set SPIFI SCK P3.3, disable glitch filter, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_4  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set SPIFI SIO3 P3.4, disable glitch filter, input buffer, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_5  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set SPIFI SIO2 P3.5, disable glitch filter, input buffer, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_6  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set SPIFI SIO1 P3.6, disable glitch filter, input buffer, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_7  = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | 0x03;     /* Set SPIFI SIO0 P3.7, disable glitch filter, input buffer, fast mode, disable pull-up resistor.*/
  //LPC_SCU->SFSP3_8  = (1 << 7) | (1 << 5) | (1 << 4) | 0x03;                /* Set SPIFI CS P3.8, disable glitch filter, fast mode, disable pull-up resistor.*/

  /* DAC I/O config */
  /* No need configuration. */
}
