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
 * @file    RP2040/rp_pal_lld.h
 * @brief   RP2040-specific PAL low level driver definitions.
 */

#ifndef RP2040_RP_PAL_LLD_H
#define RP2040_RP_PAL_LLD_H

/**
 * @name    RP2040-specific alternate function selectors
 * @{
 */
#define PAL_RP_IOCTRL_FUNCSEL_CLK           PAL_RP_IOCTRL_FUNCSEL(8U)
#define PAL_RP_IOCTRL_FUNCSEL_USB           PAL_RP_IOCTRL_FUNCSEL(9U)

#define PAL_MODE_ALTERNATE_CLK              (PAL_RP_IOCTRL_FUNCSEL_CLK     | \
                                             PAL_RP_PAD_IE                 | \
                                             PAL_RP_PAD_SCHMITT)
#define PAL_MODE_ALTERNATE_USB              (PAL_RP_IOCTRL_FUNCSEL_USB     | \
                                             PAL_RP_PAD_IE                 | \
                                             PAL_RP_PAD_SCHMITT)
/** @} */

/**
 * @brief   Access a SIO GPIO register by port.
 * @details Given a register name (field of SIO) and port (0, 1), produce a
 *          reference to the correct GPIO register for the port. On the RP2040
 *          every "hi" output and output-enable register is eight uint32_t's
 *          after the corresponding "low" register. The input registers
 *          GPIO_IN / GPIO_HI_IN are only one apart, but port 1 does not exist
 *          on RP2040 (only 30 GPIO lines) so the offset is never applied.
 *
 * @notapi
 */
#define RP_PAL_SIO_REG(reg, port)                                               \
  ((&SIO->reg)[(port) ? 8U : 0U])

#endif /* RP2040_RP_PAL_LLD_H */
