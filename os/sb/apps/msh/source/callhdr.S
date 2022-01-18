/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021,2022 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Code section.                                                             */
/*===========================================================================*/

#if !defined(__DOXYGEN__)

                .syntax unified
                .cpu    cortex-m3

                .thumb

                .bss

                .align      2
                .global     __callelf_data
__callelf_data:
                .ds.l       1
                .ds.l       1

                .text

                /* Call elf trampoline function.*/
                .align      2
                .thumb_func
                .global     __callelf
__callelf:
                ldr         r4, =__callelf_data
                str         sp, [r4, #0]
                str         lr, [r4, #4]

                /* Setting up the call stack for the loaded module.*/
                ldr         r5, [r0, #20]       /* Placed by runelf().      */
                mov         sp, r5
                push        {r1, r2, r3, r5}    /* argc, argv, envp, top.   */
                ldr         r6, [r0, #12]
                bx          r6                  /* Jumping to entry point.  */

                /* Call elf return code.*/
                .align      2
                .thumb_func
                .global     __returnelf
__returnelf:
                ldr         r4, =__callelf_data
                ldmia       r4, {r5, r6}
                mov         sp, r5
                bx          r6

#endif /* !defined(__DOXYGEN__) */

/** @} */
