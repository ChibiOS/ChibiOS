[#ftl]
[#--
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
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="board.h" /]
/*
[@license.EmitLicenseAsText /]
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for ${doc1.board.@name[0]} board.
 */

/*
 * Board identifier.
 */
#define BOARD_${doc1.board.@BoardID[0]}
#define BOARD_NAME                  "${doc1.board.@name[0]}"

[#if doc1.board.@BoardPHYID[0]??]
/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                ${doc1.board.@BoardPHYID[0]}
[#if doc1.board.@BoardPHYType[0]?string == "RMII"]
#define BOARD_PHY_RMII
[/#if]
[/#if]

/*
 * Board oscillators-related settings.
[#if doc1.board.@LSEFrequency[0]?number == 0]
 * NOTE: LSE not fitted.
[/#if]
[#if doc1.board.@HSEFrequency[0]?number == 0]
 * NOTE: HSE not fitted.
[/#if]
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                ${doc1.board.@LSEFrequency[0]}
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                ${doc1.board.@HSEFrequency[0]}
#endif

[#if doc1.board.@HSEBypass[0]?string == "true"]
#define STM32_HSE_BYPASS
[/#if]

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   ${doc1.board.@VDD[0]}

/*
 * MCU type as defined in the ST header file stm32f4xx.h.
 */
#define STM32F4XX

/*
 * IO pins assignments.
 */
[#list doc1.board.ports.* as port]
  [#assign pinidx = 0 /]
  [#list port.* as pin]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin?node_name?upper_case /]
    [/#if]
#define ${(port?node_name + "_" + name)?right_pad(27, " ")} ${pinidx?string}
    [#assign pinidx = pinidx + 1 /]
  [/#list]

[/#list]
/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

[#list doc1.board.ports.* as port]
/*
 * ${port?node_name} setup:
 *
  [#assign pinidx = 0 /]
  [#list port.* as pin]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin?node_name?upper_case /]
    [/#if]
    [#assign mode = pin.@Mode[0] /]
    [#assign type = pin.@Type[0] /]
    [#assign resistor = pin.@Resistor[0] /]
    [#assign speed = pin.@Speed[0] /]
    [#assign alternate = pin.@Alternate[0] /]
    [#if mode == "Input"]
      [#assign desc = mode + " " + resistor /]
    [#elseif mode == "Output"]
      [#assign desc = mode + " " + type + " " + speed /]
    [#elseif mode == "Alternate"]
      [#assign desc = mode + " " + alternate /]
    [#else]
      [#assign desc = "Analog" /]
    [/#if]
 * P${(port?node_name[4..] + pinidx?string)?right_pad(3, " ")} - ${name?right_pad(26, " ")}(${desc?lower_case}).
    [#assign pinidx = pinidx + 1 /]
  [/#list]
 */

[/#list]

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
