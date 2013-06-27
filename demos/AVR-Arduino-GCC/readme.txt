*****************************************************************************
** ChibiOS/RT demo for Atmel AVR ATmega328.                                **
*****************************************************************************

** TARGET **

The demo runs on an Arduino board with an ATmega328 MCU in it, eg. the Arduino 
Uno or Duemilanove.

** The Demo **

The demo prints the "Hello world" on Serial0, which is available on the board
USB connector, and toggles the LED on PB5 (pin 13 on Arduino IDE) every second.

** Build Procedure **

The demo was built using the GCC AVR toolchain. It should build with WinAVR too!

** Notes **

This demo can be programmed onto the Arduino while leaving the Arduino bootloader
in place. This means that the programming can be done through the USB connector
as well.
