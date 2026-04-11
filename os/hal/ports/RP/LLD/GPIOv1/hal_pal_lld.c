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
 * @file    GPIOv1/hal_pal_lld.c
 * @brief   RP PAL low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define RP_PAL_LINE_REG(line)           ((uint32_t)(line) >> 3U)
#define RP_PAL_LINE_SHIFT(line)         (4U * ((uint32_t)(line) & 0x07U))
#define RP_PAL_LINE_MASK(line)          (0x0FU << RP_PAL_LINE_SHIFT(line))
#define RP_PAL_REG_ALIAS_SET            0x2000U
#define RP_PAL_REG_ALIAS_CLR            0x3000U

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Event records for GPIO lines.
 */
palevent_t _pal_events[RP_GPIO_NUM_LINES];
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void rp_pal_reg_set(volatile uint32_t *reg, uint32_t mask) {

  *(volatile uint32_t *)((uintptr_t)reg + RP_PAL_REG_ALIAS_SET) = mask;
}

static inline void rp_pal_reg_clr(volatile uint32_t *reg, uint32_t mask) {

  *(volatile uint32_t *)((uintptr_t)reg + RP_PAL_REG_ALIAS_CLR) = mask;
}

static void rp_pal_pad_set_mode(ioportid_t port,
                                iopadid_t pad,
                                iomode_t mode) {
  uint32_t ctrlbits, padbits, oebits, bit, abspad;

  ctrlbits = mode & 0x007FFFFFU;
  oebits   = (mode >> 23U) & 1U;
  padbits  = mode >> 24U;
  bit      = 1U << pad;
  abspad   = ((uint32_t)port << 5U) | (uint32_t)pad;

  if ((pad >= PAL_IOPORTS_WIDTH) ||
      (((RP_PAL_VALID_MASK(port) >> pad) & 1U) == 0U) ||
      (abspad >= RP_GPIO_NUM_LINES)) {
    return;
  }

  /* Release the output driver while reprogramming mux and pad control. */
  RP_PAL_SIO_REG(GPIO_OE_CLR, port) = bit;

  IO_BANK0->GPIO[abspad].CTRL = ctrlbits;
  PADS_BANK0->GPIO[abspad] = padbits;

  if (oebits != 0U) {
    RP_PAL_SIO_REG(GPIO_OE_SET, port) = bit;
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (PAL_USE_WAIT || PAL_USE_CALLBACKS) || defined(__DOXYGEN__)
/**
 * @brief   IO_BANK0 GPIO interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_IO_IRQ_BANK0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  for (int i = 0; i < RP_GPIO_INTR_REGS; ++i) {
    int line;
    uint32_t ints = IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTS[i];

    IO_BANK0->INTR[i] = ints;
    line = i * 8;
    while (ints != 0U) {
      if (ints & 0x0FU) {
        _pal_isr_code(line);
      }
      ints >>= 4;
      ++line;
    }
  }

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   PAL driver initialization.
 *
 * @notapi
 */
void _pal_lld_init(void) {

  /* Ensures a clean peripheral state */
  rp_peripheral_reset(RESETS_ALLREG_IO_BANK0);
  rp_peripheral_reset(RESETS_ALLREG_PADS_BANK0);
  rp_peripheral_unreset(RESETS_ALLREG_IO_BANK0);
  rp_peripheral_unreset(RESETS_ALLREG_PADS_BANK0);

  #if PAL_USE_CALLBACKS || PAL_USE_WAIT || defined(__DOXYGEN__)
  unsigned i;

  for (i = 0U; i < RP_GPIO_NUM_LINES; i++) {
    _pal_init_event(i);
  }

  nvicEnableVector(RP_IO_IRQ_BANK0_NUMBER, RP_IO_IRQ_BANK0_PRIORITY);
  #endif
}

/**
 * @brief   Pads group mode setup.
 * @details Programs each selected pad in the specified port using the
 *          RP pad/mux configuration helper.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask, already shifted to port bit positions
 * @param[in] mode      group mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {
  iopadid_t pad = 0U;

  while (mask != 0U) {
    if ((mask & 1U) != 0U) {
      rp_pal_pad_set_mode(port, pad, mode);
    }
    mask >>= 1U;
    pad++;
  }
}

#if (PAL_USE_WAIT || PAL_USE_CALLBACKS) || defined(__DOXYGEN__)

bool _pal_lld_ispadeventenabled(ioportid_t port, iopadid_t pad) {
  ioline_t line = PAL_LINE(port, pad);

  return (bool)((IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY]
                 .INTE[RP_PAL_LINE_REG(line)] &
                 RP_PAL_LINE_MASK(line)) != 0U);
}

/**
 * @brief   Line event enable.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] line      line identifier
 * @param[in] mode      line event mode
 *
 * @notapi
 */
void _pal_lld_enablelineevent(ioline_t line, ioeventmode_t mode) {
  uint32_t inte = 0U;
  uint32_t reg = RP_PAL_LINE_REG(line);
  uint32_t shift = RP_PAL_LINE_SHIFT(line);
  uint32_t mask = RP_PAL_LINE_MASK(line);
  volatile uint32_t *inte_reg =
    &IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTE[reg];

  osalDbgAssert((IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTE[reg] &
                 mask) == 0U, "channel in use");

  if (mode & PAL_EVENT_MODE_RISING_EDGE) {
    inte |= 8U;
  }
  if (mode & PAL_EVENT_MODE_FALLING_EDGE) {
    inte |= 4U;
  }
  if (mode & RP_PAL_EVENT_MODE_LEVEL_LOW) {
    inte |= 1U;
  }
  if (mode & RP_PAL_EVENT_MODE_LEVEL_HIGH) {
    inte |= 2U;
  }

  IO_BANK0->INTR[reg] = mask;
  rp_pal_reg_clr(inte_reg, mask);
  rp_pal_reg_set(inte_reg, inte << shift);
}

/**
 * @brief   Line event disable.
 * @details This function disables previously programmed event callbacks.
 *
 * @param[in] line      line identifier
 *
 * @notapi
 */
void _pal_lld_disablelineevent(ioline_t line) {
  uint32_t reg = RP_PAL_LINE_REG(line);
  uint32_t mask = RP_PAL_LINE_MASK(line);
  volatile uint32_t *inte_reg =
    &IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTE[reg];

  rp_pal_reg_clr(inte_reg, mask);

  /* Clear pending interrupt status. INTR is W1C.*/
  IO_BANK0->INTR[reg] = mask;

#if PAL_USE_CALLBACKS || PAL_USE_WAIT
  /* Callback cleared and/or thread reset.*/
  _pal_clear_event(line);
#endif
}

/**
 * @brief Force a trigger event on the line.
 * 
 * @param[in]         line identifier
 */
void _pal_lld_forcelineevent(ioline_t line) {
  uint32_t reg = RP_PAL_LINE_REG(line);
  volatile uint32_t *intf =
    &IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTF[reg];
  uint32_t force_mask = IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTE[reg] &
                        RP_PAL_LINE_MASK(line);

  /* Clear then set only the target bits, without an RMW race on sibling bits. */
  rp_pal_reg_clr(intf, force_mask);
  rp_pal_reg_set(intf, force_mask);
}

/**
 * @brief Clear all forced trigger event on the line.
 * 
 * @param[in]         line identifier
 */
void _pal_lld_unforcelineevent(ioline_t line) {
  uint32_t reg = RP_PAL_LINE_REG(line);
  volatile uint32_t *intf =
    &IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTF[reg];
  uint32_t force_mask = IO_BANK0->PROC[RP_PAL_EVENT_CORE_AFFINITY].INTE[reg] &
                        RP_PAL_LINE_MASK(line);

  rp_pal_reg_clr(intf, force_mask);
}

#endif

#endif /* HAL_USE_PAL */

/** @} */
