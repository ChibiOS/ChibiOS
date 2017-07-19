*****************************************************************************
** ChibiOS/RT port for Atmel AVR ATTiny167.                                **
*****************************************************************************

** TARGET **

The demo runs on a Digispark Pro board based on an ATtiny167.

** The Demo **

The demo currently just toggles the test/status LED every second.
The LED is connected to the PB1 pin of the ATtiny167 MCU.
This pin corresponds to P1 on the Digispark Pro board.

** Build Procedure **

The demo was built using the GCC AVR toolchain.
It should build with WinAVR too!
Just run the command "make" to build this example.

** Notes **

The ATtiny167 is pre-programmed with a bootloader.
Thus it can act as an USB device.
The Digispark Pro runs the “micronucleus” bootloader.
This bootloader comes with a command line tool which can be used to
program the board.
Form more details, look at the Makefile or the github repository:
https://github.com/micronucleus/micronucleus
