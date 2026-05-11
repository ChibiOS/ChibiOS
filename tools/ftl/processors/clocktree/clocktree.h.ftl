[#ftl]
[#--
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="clocktree.h" /]
[#-- Getting various constants.--]
[#assign prename = doc1.clocktree.settings.prefixes.@clock_points[0]?string /]
[#-- The current schema only exposes generic suffixes, keep legacy macro names
    for this WIP header until the generator grows the missing sections.--]
[#assign postclocks = "_FREQ" /]
[#assign postchoices = "_SEL" /]
[#-- Sequence of the calculated clock points.--]
[#assign clocks_expr = [] /]
[#-- Sequence of the muxed clock points.--]
[#assign clocks_mux = [] /]
[#-- Scanning clock points, gathering data.--]
[#list doc1.clocktree.clocks.clock as clock]
  [#assign clockname = clock.@point[0] /]
  [#if clock.description[0]??]
    [#assign clockdescr = clock.description[0]?string?word_list?join(" ") /]
  [#else /]
    [#assign clockdescr = "no description" /]
  [/#if]
  [#-- Determining the type of the clock point by looking at the child element.--]
  [#if clock.source[0]??]
    [#-- It is a fixed clock.--]
    [#assign clockfreq = clock.source[0].@frequency[0]?string /]
    [#assign clocks_expr = clocks_expr + [{"description":clockdescr,
                                           "name":clockname,
                                           "frequency":clockfreq}] /]
  [#elseif clock.mux[0]??]
    [#-- It is a muxed clock.--]
    [#assign muxname = clock.mux[0].@name[0] /]
    [#assign inputs = [] /]
    [#list clock.mux.input as input]
      [#assign inputref = input.@point[0] /]
      [#assign inputbits = input.@bits[0]!"" /]
      [#assign inputs = inputs + [{"name":inputref, "bits":inputbits}] /]
    [/#list]
    [#assign clocks_mux = clocks_mux + [{"name":clockname,
                                         "muxname":muxname,
                                         "inputs":inputs}] /]
  [/#if]
[/#list]
/*
[@license.EmitLicenseAsText /]
*/

/**
 * @file    clocktree.h
 * @brief   Generated clock tree description header.
 *
 * @addtogroup CLOCKTREE
 * @{
 */
#ifndef CLOCKTREE_H
#define CLOCKTREE_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Mux configurations
 * @{
 */
[#list clocks_mux as mux]
  [#assign name = prename + mux["name"] + postchoices/]
${"#if !defined(" + name + ") || defined(__DOXYGEN__)"}
${("#define " + name)?right_pad(44) + "0U"}
#endif
[#sep]

[/#sep]
[/#list]
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Calculated or fixed clocks
 * @{
 */
[#list clocks_expr as clock]
/**
 * @brief   ${clock["description"]?cap_first} clock point.
 */
${("#define " + prename + clock["name"] + postclocks)?right_pad(44) + clock["frequency"]}
[#sep]

[/#sep]
[/#list]
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* CLOCKTREE_H */

/** @} */
