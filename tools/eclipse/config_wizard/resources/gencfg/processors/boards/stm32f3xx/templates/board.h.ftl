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
 * Setup for ${doc1.board.board_name[0]} board.
 */

/*
 * Board identifier.
 */
#define BOARD_${doc1.board.board_id[0]}
#define BOARD_NAME                  "${doc1.board.board_name[0]}"

[#if doc1.board.ethernet_phy[0]??]
/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                ${doc1.board.ethernet_phy.identifier[0]}
[#if doc1.board.ethernet_phy.bus_type[0]?string == "RMII"]
#define BOARD_PHY_RMII
[/#if]

[/#if]
/*
 * Board oscillators-related settings.
[#if doc1.board.clocks.@LSEFrequency[0]?number == 0]
 * NOTE: LSE not fitted.
[/#if]
[#if doc1.board.clocks.@HSEFrequency[0]?number == 0]
 * NOTE: HSE not fitted.
[/#if]
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                ${doc1.board.clocks.@LSEFrequency[0]}
#endif

[#if doc1.board.clocks.@LSEBypass[0]?string == "true"]
#define STM32_LSE_BYPASS

[/#if]
#define STM32_LSEDRV                (${doc1.board.clocks.@LSEDrive[0]?word_list[0]?number} << 3)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                ${doc1.board.clocks.@HSEFrequency[0]}
#endif

[#if doc1.board.clocks.@HSEBypass[0]?string == "true"]
#define STM32_HSE_BYPASS

[/#if]
/*
 * MCU type as defined in the ST header file stm32f30x.h.
 */
#define STM32F30X

/*
 * IO pins assignments.
 */
[#list doc1.board.ports.* as port]
  [#assign port_name = port?node_name?upper_case /]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
#define ${(port_name + "_" + name)?right_pad(27, " ")} ${pin_index?string}
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
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

[#list doc1.board.ports.* as port]
  [#assign port_name = port?node_name?upper_case /]
/*
 * ${port_name} setup:
 *
  [#-- Generating pin descriptions inside the comment.--]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
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
 * P${(port?node_name[4..] + pin_index?string)?right_pad(3, " ")} - ${name?right_pad(26, " ")}(${desc?lower_case}).
  [/#list]
 */
  [#--
    -- Generating MODER register value.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign mode = pin.@Mode[0] /]
    [#if mode == "Input"]
      [#assign out = "PIN_MODE_INPUT(" + port_name + "_" + name + ")" /]
    [#elseif mode == "Output"]
      [#assign out = "PIN_MODE_OUTPUT(" + port_name + "_" + name + ")" /]
    [#elseif mode == "Alternate"]
      [#assign out = "PIN_MODE_ALTERNATE(" + port_name + "_" + name + ")" /]
    [#else]
      [#assign out = "PIN_MODE_ANALOG(" + port_name + "_" + name + ")" /]
    [/#if]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_MODER             (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if pin_index < 15]
${(line + " |")?right_pad(76, " ") + "\\"}
    [#else]
${line + ")"}
    [/#if]
  [/#list]
  [#--
    -- Generating OTYPER register value.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign type = pin.@Type[0] /]
    [#if type == "PushPull"]
      [#assign out = "PIN_OTYPE_PUSHPULL(" + port_name + "_" + name + ")" /]
    [#else]
      [#assign out = "PIN_OTYPE_OPENDRAIN(" + port_name + "_" + name + ")" /]
    [/#if]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_OTYPER            (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if pin_index < 15]
${(line + " |")?right_pad(76, " ") + "\\"}
    [#else]
${line + ")"}
    [/#if]
  [/#list]
  [#--
    -- Generating SPEEDR register value.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign speed = pin.@Speed[0] /]
    [#if speed == "Minimum"]
      [#assign out = "PIN_OSPEED_2M(" + port_name + "_" + name + ")" /]
    [#elseif speed == "Low"]
      [#assign out = "PIN_OSPEED_25M(" + port_name + "_" + name + ")" /]
    [#elseif speed == "High"]
      [#assign out = "PIN_OSPEED_50M(" + port_name + "_" + name + ")" /]
    [#else]
      [#assign out = "PIN_OSPEED_100M(" + port_name + "_" + name + ")" /]
    [/#if]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_OSPEEDR           (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if pin_index < 15]
${(line + " |")?right_pad(76, " ") + "\\"}
    [#else]
${line + ")"}
    [/#if]
  [/#list]
  [#--
    -- Generating PUPDR register value.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign resistor = pin.@Resistor[0] /]
    [#if resistor == "Floating"]
      [#assign out = "PIN_PUPDR_FLOATING(" + port_name + "_" + name + ")" /]
    [#elseif resistor == "PullUp"]
      [#assign out = "PIN_PUPDR_PULLUP(" + port_name + "_" + name + ")" /]
    [#else]
      [#assign out = "PIN_PUPDR_PULLDOWN(" + port_name + "_" + name + ")" /]
    [/#if]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_PUPDR             (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if pin_index < 15]
${(line + " |")?right_pad(76, " ") + "\\"}
    [#else]
${line + ")"}
    [/#if]
  [/#list]
  [#--
    -- Generating ODR register value.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign level = pin.@Level[0] /]
    [#if level == "Low"]
      [#assign out = "PIN_ODR_LOW(" + port_name + "_" + name + ")" /]
    [#else]
      [#assign out = "PIN_ODR_HIGH(" + port_name + "_" + name + ")" /]
    [/#if]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_ODR               (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if pin_index < 15]
${(line + " |")?right_pad(76, " ") + "\\"}
    [#else]
${line + ")"}
    [/#if]
  [/#list]
  [#--
    -- Generating AFRx registers values.
    --]
  [#list port.* as pin]
    [#assign pin_name = pin?node_name?upper_case /]
    [#assign name = pin.@ID[0]?string?trim /]
    [#if name?length == 0]
      [#assign name = pin_name /]
    [/#if]
    [#assign alternate = pin.@Alternate[0]?trim /]
    [#assign out = "PIN_AFIO_AF(" + port_name + "_" + name + ", " + alternate + ")" /]
    [#if pin_index == 0]
      [#assign line = "#define VAL_" + port_name + "_AFRL              (" + out /]
    [#elseif pin_index == 8]
      [#assign line = "#define VAL_" + port_name + "_AFRH              (" + out /]
    [#else]
      [#assign line = "                                     " + out /]
    [/#if]
    [#if (pin_index == 7) || (pin_index == 15)]
${line + ")"}
    [#else]
${(line + " |")?right_pad(76, " ") + "\\"}
    [/#if]
  [/#list]

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
