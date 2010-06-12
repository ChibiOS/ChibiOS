/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "settings.h"

#if CH_HAL_USE_ADC
/*
 * ADC configuration.
 */
const ADCConfig adccfg = {};
const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_GRP1_NUM_CHANNELS,
  0,
  ADC_CR2_EXTSEL_SWSTART | ADC_CR2_TSVREFE | ADC_CR2_CONT,
  0,
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  ADC_SQR2_SQ7_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ6_N(ADC_CHANNEL_VREFINT),
  ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN10) |
  ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10) |
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ0_N(ADC_CHANNEL_IN10)
};
#endif

#if CH_HAL_USE_CAN

#define CAN_BTR_PRESCALER(n)        (n)
#undef CAN_BTR_TS1
#define  CAN_BTR_TS1(n)             ((n) << 16)
#undef CAN_BTR_TS2
#define  CAN_BTR_TS2(n)             ((n) << 20)
#undef CAN_BTR_SJW
#define CAN_BTR_SJW(n)              ((n) << 24)

/*
 * Internal loopback mode, 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 * See section 22.7.7 on the STM32 reference manual.
 */
const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_LBKM | CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
  CAN_BTR_TS1(8) | CAN_BTR_PRESCALER(6),
  0,
  NULL
};
#endif /* CH_HAL_USE_CAN */

#if CH_HAL_USE_SPI
/*
 * SPI configuration, maximum speed.
 */
const SPIConfig spicfg = {
  IOPORT1, GPIOA_SPI1NSS, 0
};
#endif
