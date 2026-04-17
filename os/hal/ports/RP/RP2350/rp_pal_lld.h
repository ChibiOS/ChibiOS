/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    RP2350/rp_pal_lld.h
 * @brief   RP2350-specific PAL low level driver definitions.
 */

#ifndef RP2350_RP_PAL_LLD_H
#define RP2350_RP_PAL_LLD_H

/**
 * @name    RP2350-specific alternate function selectors
 * @{
 * @note    Selectors above 7 are RP2350-specific and may map to pin-dependent
 *          signals. Refer to the RP2350 GPIO function tables when using them.
 */
#define PAL_RP_IOCTRL_FUNCSEL_PIO2          PAL_RP_IOCTRL_FUNCSEL(8U)
#define PAL_RP_IOCTRL_FUNCSEL_XIP           PAL_RP_IOCTRL_FUNCSEL(9U)
#define PAL_RP_IOCTRL_FUNCSEL_USB           PAL_RP_IOCTRL_FUNCSEL(10U)

#define PAL_MODE_ALTERNATE_PIO2             (PAL_RP_IOCTRL_FUNCSEL_PIO2    | \
                                             PAL_RP_PAD_IE                 | \
                                             PAL_RP_PAD_SCHMITT)
#define PAL_MODE_ALTERNATE_XIP              (PAL_RP_IOCTRL_FUNCSEL_XIP     | \
                                             PAL_RP_PAD_IE                 | \
                                             PAL_RP_PAD_SCHMITT)
#define PAL_MODE_ALTERNATE_USB              (PAL_RP_IOCTRL_FUNCSEL_USB     | \
                                             PAL_RP_PAD_IE                 | \
                                             PAL_RP_PAD_SCHMITT)
/** @} */

/**
 * @brief   Access a SIO GPIO register by port.
 * @details Given a register name (field of SIO) and port (0, 1), produce a
 *          reference to the correct GPIO register for the port. On the RP2350
 *          every "hi" register immediately follows the "low" register.
 *
 * @notapi
 */
#define RP_PAL_SIO_REG(reg, port)                                               \
  ((&SIO->reg)[(port)])

#endif /* RP2350_RP_PAL_LLD_H */
