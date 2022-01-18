/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    crt0.S
 * @brief   Generic ARMv7-M sandbox startup file for ChibiOS.
 *
 * @addtogroup ARMCMx_GCC_STARTUP_V7M_SB
 * @{
 */

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                               0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                                1
#endif

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Stack segments initialization switch.
 */
#if !defined(CRT0_STACKS_FILL_PATTERN) || defined(__DOXYGEN__)
#define CRT0_STACKS_FILL_PATTERN            0x55555555
#endif

/**
 * @brief   Stack segments initialization switch.
 */
#if !defined(CRT0_INIT_STACKS) || defined(__DOXYGEN__)
#define CRT0_INIT_STACKS                    TRUE
#endif

/**
 * @brief   DATA segment initialization switch.
 */
#if !defined(CRT0_INIT_DATA) || defined(__DOXYGEN__)
#define CRT0_INIT_DATA                      TRUE
#endif

/**
 * @brief   BSS segment initialization switch.
 */
#if !defined(CRT0_INIT_BSS) || defined(__DOXYGEN__)
#define CRT0_INIT_BSS                       TRUE
#endif

/**
 * @brief   Constructors invocation switch.
 */
#if !defined(CRT0_CALL_CONSTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_CONSTRUCTORS              TRUE
#endif

/**
 * @brief   Destructors invocation switch.
 */
#if !defined(CRT0_CALL_DESTRUCTORS) || defined(__DOXYGEN__)
#define CRT0_CALL_DESTRUCTORS               TRUE
#endif

/*===========================================================================*/
/* Code section.                                                             */
/*===========================================================================*/

#if !defined(__DOXYGEN__)

                .syntax unified
                .cpu    cortex-m3

                .thumb

                .section    .sandbox, "ax"

                .align      4
                .globl      __sandbox
__sandbox:      .long       0xFE9154C0
                .long       0x0C4519EF
                .long       32
                .long       __crt0_entry
                .long       __crt0_exit
                .long       0
                .long       0
                .long       0

                .bss

                .align      2
                .global     __sb_parameters
__sb_parameters:
                .ds.l       1
                .ds.l       1
                .global     environ             /* Required by newlib.*/
environ:
                .ds.l       1
                .ds.l       1
                .ds.l       1

                .text

                /* Default exit method, calling OS exit().*/
                .align      2
                .thumb_func
                .global     __crt0_exit
__crt0_exit:
                svc         #1
.exitloop:      b           .exitloop

                /* Default entry point, note, the "nop" is meant to be there
                   the loader could overwrite it with a "bkpt".*/
                .align      2
                .thumb_func
                .global     __crt0_entry
__crt0_entry:
                nop

                /* Popping from the stack the information passed by the
                   loader, saving the stack position as end of heap.*/
                pop     {r7, r8, r9, r10}
                mov     r11, sp

                /* PSP stack pointers initialization.*/
                ldr     r0, =__user_psp_end__
                mov     sp, r0

#if CRT0_INIT_STACKS == TRUE
                /* User process Stack initialization. Note, it assumes that the
                   stack size is a multiple of 4 so the linker file must
                   ensure this.*/
                ldr     r0, =CRT0_STACKS_FILL_PATTERN
                ldr     r1, =__user_psp_base__
                ldr     r2, =__user_psp_end__
upsloop:
                cmp     r1, r2
                itt     lo
                strlo   r0, [r1], #4
                blo     upsloop
#endif /* CRT0_INIT_STACKS == TRUE */

#if CRT0_INIT_DATA == TRUE
                /* Data initialization. Note, it assumes that the DATA size
                  is a multiple of 4 so the linker file must ensure this.*/
                ldr     r1, =__textdata_base__
                ldr     r2, =__data_base__
                ldr     r3, =__data_end__
dloop:
                cmp     r2, r3
                ittt    lo
                ldrlo   r0, [r1], #4
                strlo   r0, [r2], #4
                blo     dloop
#endif /* CRT0_INIT_DATA == TRUE */

#if CRT0_INIT_BSS == TRUE
                /* BSS initialization. Note, it assumes that the DATA size
                  is a multiple of 4 so the linker file must ensure this.*/
                movs    r0, #0
                ldr     r1, =__bss_base__
                ldr     r2, =__bss_end__
bloop:
                cmp     r1, r2
                itt     lo
                strlo   r0, [r1], #4
                blo     bloop
#endif /* CRT0_INIT_BSS == TRUE */

                /* Storing the sandbox parameters block.*/
                ldr     r0, =__sb_parameters
                stmia   r0, {r7, r8, r9, r10, r11}

#if CRT0_CALL_CONSTRUCTORS == TRUE
                /* Constructors invocation.*/
                ldr     r4, =__init_array_base__
                ldr     r5, =__init_array_end__
initloop:
                cmp     r4, r5
                bge     endinitloop
                ldr     r1, [r4], #4
                blx     r1
                b       initloop
endinitloop:
#endif /* CRT0_CALL_CONSTRUCTORS == TRUE */

                /* Main program invocation, r0 contains the returned value.*/
                mov     r0, r7
                mov     r1, r8
                mov     r2, r9
                bl      main
                /* Falls into _exit().*/

                .thumb_func
                .global     _exit               /* Required by newlib.*/
_exit:
#if CRT0_CALL_DESTRUCTORS == TRUE
                /* Destructors invocation.*/
                mov     r6, r0
                ldr     r4, =__fini_array_base__
                ldr     r5, =__fini_array_end__
finiloop:
                cmp     r4, r5
                bge     endfiniloop
                ldr     r1, [r4], #4
                blx     r1
                b       finiloop
                mov     r0, r6
endfiniloop:
#endif /* CRT0_CALL_DESTRUCTORS == TRUE */
                ldr     r1, =__sandbox
                ldr     r2, [r1, #16]           /* Predefined exit vector.*/
                bx      r2

#endif /* !defined(__DOXYGEN__) */

/** @} */
