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

[#--
  -- Emits the STM32F4xx ADC driver constant configuration structures.
  --]
[#macro EmitADCConfig config]
  [#local cfg_name = config.@name[0]?string /]
/**
  [@code.EmitDoxygenBrief config.@brief /]
  [@code.EmitDoxygenDetails config.details /]
 */
const ADCConfig ${cfg_name} = {0};

  [#list config.groups.group as group]
    [#local grpcfg_name = group.@name[0]?string /]
/**
    [@code.EmitDoxygenBrief group.@brief /]
    [@code.EmitDoxygenDetails group.details /]
 */
const ADCGroupConfig ${grpcfg_name} = {
  /* Circular conversion flag.*/
  ${group.@circular[0]?string?upper_case},
  /* Number of channels sampled in the conversion group.*/
  ${group.channels_sequence.channel?size},
  /* End of conversion callback or NULL.*/
    [#if group.@conversion_callback[0]?string?trim == ""]
  NULL,
    [#else]
  ${group.@conversion_callback[0]?string?trim},
    [/#if]
  /* Error callback or NULL.*/
    [#if group.@error_callback[0]?string?trim == ""]
  NULL,
    [#else]
  ${group.@error_callback[0]?string?trim},
    [/#if]
};
  [/#list]
[/#macro]

[#--
  -- Emits the STM32F4xx ADC driver configuration external declarations.
  --]
[#macro EmitADCConfigExtern config]
  [#local cfg_name = config.@name[0]?string /]
  [#list config.groups.group as group]
    [#local grpcfg_name = group.@name[0]?string /]
    [#-- Only emits the comment if there is at least a callback defined.--]
  /* ADC configuration "${cfg_name}".*/
  extern const ADCConfig ${cfg_name};
  /* ADC conversion group "${grpcfg_name}".*/
  extern const ADCGroupConfig ${grpcfg_name};
    [#if group.@conversion_callback[0]?string?trim != ""]
  void ${group.@conversion_callback[0]?string?trim}(ADCDriver *, adcsample_t *, size_t);
    [/#if]
    [#if group.@error_callback[0]?string?trim != ""]
  void ${group.@error_callback[0]?string?trim}(ADCDriver *, adcerror_t);
    [/#if]

  [/#list]
[/#macro]
