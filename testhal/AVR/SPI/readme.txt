*****************************************************************************
** ChibiOS/RT port for Atmel AVR ATmega1280.                                **
*****************************************************************************

** TARGET **

The demo runs on an Arduino board.

** The Demo **

Connect the SPI interfaces of two Arduino boards. Configure one as master, the other
as slave using the #define statement in main.c. 

Once the master starts running, the two boards will exchange messages using SPI 
and output the messages over the USB serial port.
 
** Build Procedure **

The demo was built using the GCC AVR toolchain. It should build with WinAVR too!

** Notes **

This demo can be programmed onto the Arduino while leaving the Arduino bootloader
in place. This means that the programming can be done through the USB connector
as well.
