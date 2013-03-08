/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_

#define ADC_GRP1_NUM_CHANNELS   5
#define ADC_GRP1_BUF_DEPTH      8

#define ADC_GRP2_NUM_CHANNELS   8
#define ADC_GRP2_BUF_DEPTH      16

extern const ADCConversionGroup adcgrpcfg1;
extern const ADCConversionGroup adcgrpcfg2;

#ifdef __cplusplus
extern "C" {
#endif
  void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n);
  void adcerrorcallback(ADCDriver *adcp, adcerror_t err);
#ifdef __cplusplus
}
#endif

#endif /* _ADC_CFG_H_ */
