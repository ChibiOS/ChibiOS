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
[#import "/@lib/libutils.ftlc" as utils /]
[#import "/@lib/liblicense.ftlc" as license /]
[#import "/@lib/libclocks.ftlc" as clocktree /]
[@pp.changeOutputFile name="clocktree.h" /]
[#-- Local constants still used by the mux section until it moves to libclocks.ftlc.--]
[#assign prename = doc1.clocktree.settings.prefixes.@clock_points[0]?string /]
[#assign postchoices = "_SEL" /]
[#-- Sequence of the muxed clock points.--]
[#assign clocks_mux = [] /]
[#-- Scanning clock points, gathering data.--]
[#list doc1.clocktree.clocks.clock as clock]
  [#assign clockname = clock.@point[0] /]
  [#-- Determining the type of the clock point by looking at the child element.--]
  [#if clock.mux[0]??]
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
[@clocktree.EmitSourcePoints /]
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
