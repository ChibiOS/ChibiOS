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

/**
 * @file    lis302dl.h
 * @brief   LIS302DL MEMS interface module through SPI header.
 *
 * @addtogroup lis302dl
 * @{
 */

#ifndef _LIS302DL_H_
#define _LIS302DL_H_

/**
 * @name    LIS302DL register names
 * @{
 */
#define LIS302DL_WHO_AM_I               0x0F
#define LIS302DL_CTRL_REG1              0x20
#define LIS302DL_CTRL_REG2              0x21
#define LIS302DL_CTRL_REG3              0x22
#define LIS302DL_HP_FILTER_RESET        0x23
#define LIS302DL_STATUS_REG             0x27
#define LIS302DL_OUTX                   0x29
#define LIS302DL_OUTY                   0x2B
#define LIS302DL_OUTZ                   0x2D
#define LIS302DL_FF_WU_CFG1             0x30
#define LIS302DL_FF_WU_SRC1             0x31
#define LIS302DL_FF_WU_THS1             0x32
#define LIS302DL_FF_WU_DURATION1        0x33
#define LIS302DL_FF_WU_CFG2             0x34
#define LIS302DL_FF_WU_SRC2             0x35
#define LIS302DL_FF_WU_THS2             0x36
#define LIS302DL_FF_WU_DURATION2        0x37
#define LIS302DL_CLICK_CFG              0x38
#define LIS302DL_CLICK_SRC              0x39
#define LIS302DL_CLICK_THSY_X           0x3B
#define LIS302DL_CLICK_THSZ             0x3C
#define LIS302DL_CLICK_TIMELIMIT        0x3D
#define LIS302DL_CLICK_LATENCY          0x3E
#define LIS302DL_CLICK_WINDOW           0x3F
/** @} */

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t lis302dlReadRegister(SPIDriver *spip, uint8_t reg);
  void lis302dlWriteRegister(SPIDriver *spip, uint8_t reg, uint8_t value);
#ifdef __cplusplus
}
#endif

#endif /* _LIS302DL_H_ */

/** @} */
