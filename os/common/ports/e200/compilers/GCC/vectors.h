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

/**
 * @file    vectors.h
 * @brief   ISR vector module header.
 *
 * @addtogroup VECTORS
 * @{
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include "ppcparams.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#if PPC_INTC_TYPE == 0
#define INTC_IACKR_ADDR     (PPC_INTC_BASE + 0x10)
#define INTC_EOIR_ADDR      (PPC_INTC_BASE + 0x18)

#elif PPC_INTC_TYPE == 1
#define INTC_IACKR_ADDR     (PPC_INTC_BASE + 0x20)
#define INTC_EOIR_ADDR      (PPC_INTC_BASE + 0x30)

#else
#error "unknown INTC type"
#endif

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

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/**
 * @name    INTC-related macros
 * @{
 */
#if PPC_INTC_TYPE == 0
#define INTC_BCR            (*((volatile uint32_t *)(PPC_INTC_BASE + 0)))
#define INTC_CPR(n)         (*((volatile uint32_t *)(PPC_INTC_BASE + 8 + ((n) * sizeof (uint32_t)))))
#define INTC_IACKR(n)       (*((volatile uint32_t *)(PPC_INTC_BASE + 0x10 + ((n) * sizeof (uint32_t)))))
#define INTC_EOIR(n)        (*((volatile uint32_t *)(PPC_INTC_BASE + 0x18 + ((n) * sizeof (uint32_t)))))
#define INTC_PSR(n)         (*((volatile uint8_t *)(PPC_INTC_BASE + 0x40 + ((n) * sizeof (uint8_t)))))

#elif PPC_INTC_TYPE == 1
#define INTC_BCR            (*((volatile uint32_t *)(PPC_INTC_BASE + 0)))
#define INTC_MPROT          (*((volatile uint32_t *)(PPC_INTC_BASE + 4)))
#define INTC_CPR(n)         (*((volatile uint32_t *)(PPC_INTC_BASE + 0x10 + ((n) * sizeof (uint32_t)))))
#define INTC_IACKR(n)       (*((volatile uint32_t *)(PPC_INTC_BASE + 0x20 + ((n) * sizeof (uint32_t)))))
#define INTC_EOIR(n)        (*((volatile uint32_t *)(PPC_INTC_BASE + 0x30 + ((n) * sizeof (uint32_t)))))
#define INTC_PSR(n)         (*((volatile uint16_t *)(PPC_INTC_BASE + 0x60 + ((n) * sizeof (uint16_t)))))

#else
#error "unknown INTC type"
#endif

/**
 * @brief   PSR register content helper
 */
#define INTC_PSR_ENABLE(cores, prio) ((uint32_t)(cores) | (uint32_t)(prio))

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

#if !defined(__DOXYGEN__)
extern uint32_t _vectors[PPC_NUM_VECTORS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _unhandled_irq(void);
#ifdef __cplusplus
}
#endif

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

static inline void intc_init(void) {
  unsigned i;

  /* INTC initialization, software vector mode, 4 bytes vectors, starting
     at priority 0.*/
  INTC_BCR = 0;
  for (i = 0; i < PPC_CORE_NUMBER; i++) {
    INTC_CPR(i)   = 0;
    INTC_IACKR(i) = (uint32_t)_vectors;
  }
}

#endif /* !defined(_FROM_ASM_) */

#endif /* _VECTORS_H_ */

/** @} */
