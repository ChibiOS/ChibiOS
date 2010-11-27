*****************************************************************************
** ChibiOS/RT port for Texas Instruments MSP430.                           **
*****************************************************************************

** TARGET **

The demo runs on an Olimex MSP430-P1611 board.

** The Demo **

The demo flashes the board LED using a thread, by pressing the button located
on the board the test procedure is activated with output on the serial port
COM1 (USART0).

** Build Procedure **

The demo was built using the MSPGCC toolchain.

** Notes **

The demo requires include files from MSPGCC that are not part of the ChibiOS/RT
distribution, please install MSPGCC.

                         http://mspgcc.sourceforge.net/
