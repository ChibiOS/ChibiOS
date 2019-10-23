*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 ADICUP360.                            **
*****************************************************************************

** TARGET **

The demo runs on an ADICUP360 board.

** The Demo **

The demo flashes the two on-board LEDs using a thread, by pressing the button 
located on the board (BOOT) the test procedure is activated with output on the 
serial port SD0 (UART0, mapped on mBED debug COM port).

** Board setup **

To redirect P0.1 and P0.2 to the mBED debug COM port it is required to setup
the switch matrix as:
 - S1 -> 0
 - S2 -> 1
 - S3 -> 1
 - S4 -> 0
   
** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain
and YAGARTO.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
Analog Devices and are licensed under a different license.
Also note that not all the files present within the Analog Devices 
Cross Core Embedded Studio are distributed with ChibiOS/RT, 
you can find the whole library on the ADI web site:

                             http://www.analog.com