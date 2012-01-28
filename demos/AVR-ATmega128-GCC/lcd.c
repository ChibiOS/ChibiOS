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

#include "ch.h"
#include "hal.h"
#include "lcd.h"

static void e_pulse(void) {
  volatile uint8_t i;

  PORTC |= PORTC_44780_E_MASK;
  for (i = 0; i < ELOOPVALUE; i++);
    ;
  PORTC &= ~PORTC_44780_E_MASK;
}

static void wait_not_busy(void) {

  chThdSleep(2);
}

/*
 * 44780 soft reset procedure.
 */
void lcdInit(void) {

  PORTC = (PORTC & ~(PORTC_44780_DATA_MASK | PORTC_44780_RS_MASK |
                     PORTC_44780_E_MASK | PORTC_44780_RW_MASK)) |
          (LCD_CMD_INIT8 & PORTC_44780_DATA_MASK);
  chThdSleep(50);
  e_pulse();
  chThdSleep(10);
  e_pulse();
  chThdSleep(2);
  e_pulse();
  wait_not_busy();
  PORTC = (PORTC & ~(PORTC_44780_DATA_MASK | PORTC_44780_RS_MASK |
                     PORTC_44780_E_MASK | PORTC_44780_RW_MASK)) |
          (LCD_CMD_INIT4 & PORTC_44780_DATA_MASK);
  e_pulse();
  lcdCmd(LCD_CMD_INIT4);
  lcdCmd(LCD_SET_DM | LCD_DM_DISPLAY_ON);
  lcdCmd(LCD_SET_INCREMENT_MODE);
}

/*
 * Sends a command byte to the 44780.
 */
void lcdCmd(uint8_t cmd) {

  wait_not_busy();
  PORTC = (PORTC | PORTC_44780_DATA_MASK) & (cmd |
                                             (0x0F & ~PORTC_44780_RS_MASK));
  e_pulse();
  PORTC = (PORTC | PORTC_44780_DATA_MASK) & ((cmd << 4) |
                                             (0x0F & ~PORTC_44780_RS_MASK));
  e_pulse();
}

/*
 * Writes a char on the LCD at the current position.
 */
void lcdPutc(char c) {
  uint8_t b;

  wait_not_busy();
  b = c | 0x0F;
  PORTC = (PORTC | PORTC_44780_DATA_MASK | PORTC_44780_RS_MASK) &
          (c | 0x0F);
  e_pulse();
  PORTC = (PORTC | PORTC_44780_DATA_MASK | PORTC_44780_RS_MASK) &
          ((c << 4) | 0x0F);
  e_pulse();
}

/*
 * Writes a string on the LCD at an absolute address.
 */
void lcdPuts(uint8_t pos, char *p) {

  lcdCmd(LCD_SET_DDRAM_ADDRESS | pos);
  while (*p)
    lcdPutc(*p++);
}
