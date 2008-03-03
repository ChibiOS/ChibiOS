/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#ifndef _LPC214x_SSP_H_
#define _LPC214x_SSP_H_

/*
 * Configuration parameter, if defined this macro enforces mutual exclusion
 * when invoking \p sspAcquireBus() and \p sspReleaseBus().
 */
#define SSP_USE_MUTEX

#ifdef __cplusplus
}
#endif
  void InitSSP(void);
  void SetSSP(int cpsr, int cr0, int cr1);

  void sspAcquireBus(void);
  void sspReleaseBus(void);
  void sspRW(uint8_t *in, uint8_t *out, t_size n);
#ifdef __cplusplus
}
#endif

#endif /* _LPC214x_SSP_H_*/
