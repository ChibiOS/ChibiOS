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
 * @file    RX/crt0.c
 * @brief   Generic RX600 startup file for ChibiOS/RT.
 *
 * @addtogroup RX_STARTUP
 * @{
 */

#include "ch.h"

typedef void (*funcp_t)(void);
typedef funcp_t * funcpp_t;
extern void (*_relocatableVectors[256])(void) __attribute__ ((section (".rvectors")));
extern void __early_init(void);

#define SYMVAL(sym) (uint32_t)(((uint8_t *)&(sym)) - ((uint8_t *)0))

/*===========================================================================*/
/**
 * @name    Startup settings
 * @{
 */
/*===========================================================================*/

/**
 * @brief   Stack segments initialization switch.
 */
#if !defined(CRT0_STACKS_FILL_PATTERN) || defined(__DOXYGEN__)
#define CRT0_STACKS_FILL_PATTERN    0x55555555
#endif

/**
 * @brief   Stack segments initialization switch.
 */
#if !defined(CRT0_INIT_STACKS) || defined(__DOXYGEN__)
#define CRT0_INIT_STACKS            TRUE
#endif

/**
 * @brief   DATA segment initialization switch.
 */
#if !defined(CRT0_INIT_DATA) || defined(__DOXYGEN__)
#define CRT0_INIT_DATA              TRUE
#endif

/**
 * @brief   BSS segment initialization switch.
 */
#if !defined(CRT0_INIT_BSS) || defined(__DOXYGEN__)
#define CRT0_INIT_BSS               TRUE
#endif

/**
 * @brief   Constructors invocation switch.
 */
#if !defined(CRT0_CALL_CONSTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_CONSTRUCTORS      FALSE
#endif

/**
 * @brief   Destructors invocation switch.
 */
#if !defined(CRT0_CALL_DESTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_DESTRUCTORS       FALSE
#endif

/** @} */

/*===========================================================================*/
/**
 * @name    Symbols from the scatter file
 */
/*===========================================================================*/

/**
 * @brief   Main stack lower boundary.
 * @details This symbol must be exported by the linker script and represents
 *          the main stack lower boundary.
 */
extern uint32_t __user_stack_base__;

/**
 *
 * @brief   Main stack initial position.
 * @details This symbol must be exported by the linker script and represents
 *          the main stack initial position.
 */
extern uint32_t __user_stack_end__;

/**
 * @brief   Process stack lower boundary.
 * @details This symbol must be exported by the linker script and represents
 *          the process stack lower boundary.
 */
extern uint32_t __irq_stack_base__;

/**
 * @brief   Process stack initial position.
 * @details This symbol must be exported by the linker script and represents
 *          the process stack initial position.
 */
extern uint32_t __irq_stack_end__;

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
extern funcp_t __init_array_start;

/**
 * @brief   Constructors table end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern funcp_t __init_array_end;

/**
 * @brief   Destructors table start.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern funcp_t __fini_array_start;

/**
 * @brief   Destructors table end.
 * @pre     The symbol must be aligned to a 32 bits boundary.
 */
extern funcp_t __fini_array_end;

/** @} */

/**
 * @brief   Application @p main() function.
 */
extern void main(void);

/**
 * @brief   Early initialization.
 * @details This hook is invoked immediately after the stack initialization
 *          and before the DATA and BSS segments initialization. The
 *          default behavior is to do nothing.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void __early_init(void) {}

/**
 * @brief   Late initialization.
 * @details This hook is invoked after the DATA and BSS segments
 *          initialization and before any static constructor. The
 *          default behavior is to do nothing.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void __late_init(void) {}

/**
 * @brief   Default @p main() function exit handler.
 * @details This handler is invoked or the @p main() function exit. The
 *          default behavior is to enter an infinite loop.
 * @note    This function is a weak symbol.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak, naked))
#endif
void _default_exit(void) {
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

  /* Disable interrupts, ipl=0, supervisor mode, select USP stack */
  asm volatile ("mvtc     #0x00000000, PSW");

  /* Set interrupt stack pointer */
  asm volatile ("mvtc     #__irq_stack_end__, ISP");
  /* Set relocatable vectors base */
  asm volatile ("mvtc     #__relocatableVectors, INTB");

  __early_init();

#if RX_USE_FPU
  asm volatile ("mvtc     #0x100, FPSW");
#endif

#if defined(CRT0_INIT_STACKS)
  /* Stack initialization.*/
  asm volatile ("mov      %0, R2" : : "i" (CRT0_STACKS_FILL_PATTERN));
  asm volatile ("mov      #__irq_stack_base__, R1");
  asm volatile ("mov      #__irq_stack_end__-__irq_stack_base__, R3");
  asm volatile ("sstr.b");
#endif

  /* copy initialized data */
#if defined(CRT0_INIT_DATA)
  asm volatile ("mov      #_data, R1");
  asm volatile ("mov      #_textdata, R2");
  asm volatile ("mov      #_edata-_data, R3");
  asm volatile ("smovf");
#endif

#if defined(CRT0_INIT_BSS)
  /* clear bss section */
  asm volatile ("mov      #0, R2");
  asm volatile ("mov      #_bss_start, R1");
  asm volatile ("mov      #_bss_end-_bss_start, R3");
  asm volatile ("sstr.b");
#endif

#if CRT0_CALL_CONSTRUCTORS
  /* Constructors invocation.*/
  {
    funcpp_t fpp = &__init_array_start;
    while (fpp < &__init_array_end) {
      (*fpp)();
      fpp++;
    }
  }
#endif

  /* Enable interrupts */
  asm volatile ("setpsw   I");

  /* Invoking application main() function.*/
  main();


#if CRT0_CALL_DESTRUCTORS
  /* Destructors invocation.*/
  {
    funcpp_t fpp = &__fini_array_start;
    while (fpp < &__fini_array_end) {
      (*fpp)();
      fpp++;
    }
  }
#endif

  /* Invoking the exit handler.*/
  _default_exit();
}

/** @} */
