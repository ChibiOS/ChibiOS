*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F103.                            **
*****************************************************************************

** TARGET **

The demo will on an Olimex STM32-P103 board.

** The Demo **

This demo shows how to integrate the FatFs file system and use the SPI and MMC
drivers.
The demo flashes the board LED using a thread and monitors the MMC slot for
a card insertion. By pressing the button located on the board while a card is
inserted a directory dump on the serial port is performed, if a card is not
inserted then the test procedure is activated.

** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain,
YAGARTO and an experimental WinARM build including GCC 4.3.0.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
