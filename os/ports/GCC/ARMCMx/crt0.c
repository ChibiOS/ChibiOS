/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    ARMCMx/crt0.c
 * @brief   Generic ARMvx-M (Cortex-M0/M1/M3/M4) startup file for ChibiOS/RT.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#include "chtypes.h"

/**
 * @brief   Control special register initialization value.
 * @details The system is setup to run in privileged mode using the PSP
 *          stack (dual stack mode).
 */
#if !defined(CRT0_CONTROL_INIT) || defined(__DOXYGEN__)
#define CRT0_CONTROL_INIT       0x00000002
#endif

/**
 * @brief   DATA segment initialization switch.
 */
#if !defined(CRT0_INIT_DATA) || defined(__DOXYGEN__)
#define CRT0_INIT_DATA          TRUE
#endif

/**
 * @brief   BSS segment initialization switch.
 */
#if !defined(CRT0_INIT_BSS) || defined(__DOXYGEN__)
#define CRT0_INIT_BSS           TRUE
#endif

/**
 * @brief   Constructors invocation switch.
 */
#if !defined(CRT0_CALL_CONSTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_CONSTRUCTORS  TRUE
#endif

/**
 * @brief   Destructors invocation switch.
 */
#if !defined(CRT0_CALL_DESTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_DESTRUCTORS   TRUE
#endif

#define SYMVAL(sym) (uint32_t)(((uint8_t *)&(sym)) - ((uint8_t *)0))

/**
 * @brief   Ram end.
 * @details This symbol must be exported by the linker script and represents
 *          the location after the last RAM location.
 */
extern uint8_t __ram_end__;

/**
 * @brief   Main stack size.
 * @details This symbol must be exported by the linker script and represents
 *          the main stack size.
 * @note    The main stack is the stack where interrupts and exceptions are
 *          processed.
 */
extern uint8_t __main_stack_size__;

/**
 * @brief   Process stack size.
 * @details This symbol must be exported by the linker script and represents
 *          the process stack size.
 * @note    The process stack is the stack used by the @p main() function.
 */
extern uint8_t __process_stack_size__;

/**
 * @brief   ROM image of the data segment start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern uint32_t _textdata;

/**
 * @brief   Data segment start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern uint32_t _data;

/**
 * @brief   Data segment end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern uint32_t _edata;

/**
 * @brief   BSS segment start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern uint32_t _bss_start;

/**
 * @brief   BSS segment end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern uint32_t _bss_end;

/**
 * @brief   Constructors table start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern void __init_array_start(void);

/**
 * @brief   Constructors table end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern void __init_array_end(void);

/**
 * @brief   Destructors table start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern void __fini_array_start(void);

/**
 * @brief   Destructors table end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern void __fini_array_end(void);

/**
 * @brief   Application @p main() function.
 *
 * @param[in] argc      Number of arguments, always zero.
 * @param[in] argv      Pointer to an array of arguments, always @p NULL.
 */
extern void main(int argc, char **argv);

/**
 * @brief   Default initialization hook 0.
 * @details This hook is invoked immediately after the stack initialization
 *          and before the DATA and BSS segments initialization. The
 *          default behavior is to do nothing.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void hwinit0(void) {}

/**
 * @brief   Default initialization hook 1.
 * @details This hook is invoked immediately after the DATA and BSS segments
 *          initialization and before entering the @p main() function. The
 *          default behavior is to do nothing.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void hwinit1(void) {}

/**
 * @brief   Default @p main() function exit handler.
 * @details This handler is invoked or the @p main() function exit. The
 *          default behavior is to enter an infinite loop.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak, naked))
#endif
void _main_exit_handler(void) {
  while (1)
    ;
}

/**
 * @brief   Reset vector.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void ResetHandler(void) {
  uint32_t sz, ctl;

  /* Process Stack initialization, it is allocated below the main stack. The
     main stack is assumed to be allocated starting from @p __ram_end__
     extending downward.*/
  asm volatile ("cpsid   i");
  sz = SYMVAL(__ram_end__) - SYMVAL(__main_stack_size__);
  asm volatile ("msr     PSP, %0" : : "r" (sz));

  ctl = CRT0_CONTROL_INIT;
  asm volatile ("msr     CONTROL, %0" : : "r" (ctl));
  asm volatile ("isb");

  /* Initialization hook 0 invocation.*/
  hwinit0();

#if CRT0_INIT_DATA
  /* DATA segment initialization.*/
  {
    uint32_t *tp, *dp;

    tp = &_textdata;
    dp = &_data;
    while (dp < &_edata)
      *dp++ = *tp++;
  }
#endif

#if CRT0_INIT_BSS
  /* BSS segment initialization.*/
  {
    uint32_t *bp;

    bp = &_bss_start;
    while (bp < &_bss_end)
      *bp++ = 0;
  }
#endif

  /* Initialization hook 1 invocation.*/
  hwinit1();

#if CRT0_CALL_CONSTRUCTORS
  /* Constructors invocation.*/
  {
    void (*dpp)(void);

    dpp = &__init_array_start;
    while (dpp < &__init_array_end) {
      (*dpp)();
      dpp++;
    }
  }
#endif

  /* Invoking application main() function.*/
  main(0, 0);

#if CRT0_CALL_DESTRUCTORS
  /* Destructors invocation.*/
  {
    void (*dpp)(void);

    dpp = &__fini_array_start;
    while (dpp < &__fini_array_end) {
      (*dpp)();
      dpp++;
    }
  }
#endif

  /* Invoking the main() exit handler.*/
  asm volatile ("b       _main_exit_handler");
}

/** @} */
