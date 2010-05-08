*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F103.                            **
*****************************************************************************

** TARGET **

The demo will on an Olimex STM32-P103 board.

** The Demo **

The demo is a stress test for the STM32 I/O subsystem, simultaneous activity
is performed on a SPI, an ADC, the CAN and a serial port all while executing
the complex test suite.
The demo is DMA and IRQ intensive so the debugging can be difficoult, so be
warned if you see anomalies in your debugger.

** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain,
YAGARTO and an experimental WinARM build including GCC 4.3.0.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distribited
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
