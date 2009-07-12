*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F103.                            **
*****************************************************************************

** TARGET **

The demo will on an Olimex STM32-P103 board.

** The Demo **

The demo flashes the board LED using a thread, by pressing the button located
on the board the test procedure is activated with output on the serial port
COM2 (USART2).

** Build Procedure **

The demo was tested by using the free Codesourcery GCC-based toolchain (a
modified 4.2.3 GCC),  YAGARTO 4.3.2 and an experimental WinARM build including
GCC 4.3.0.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license, see the header
present in all the source files under ./demos/ARMCM3-STM32F103/stm32lib for
details.
Also note that not all the files present in the ST library are distribuited
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
