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
[#import "/@lib/libcode.ftl" as code /]
[#import "/@lib/libstm32f4xx.ftl" as stm32f4xx /]
[#assign fname = doc1.configuration.name[0] /]
[@pp.changeOutputFile name = fname + ".h" /]
/*
[@license.EmitLicenseAsText /]
*/

#ifndef _${fname?upper_case}_H_
#define _${fname?upper_case}_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
[#list doc1.configuration.configs.* as config]
  [#assign config_type = config?node_name /]
    [#if config_type == "adc_config"]
      [@stm32f4xx.EmitADCConfigExtern config /]
    [/#if]
[/#list]
#ifdef __cplusplus
}
#endif

#endif /* _${fname?upper_case}_H_ */
