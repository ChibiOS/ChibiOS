*****************************************************************************
** ChibiOS/RT port for Atmel AVR ATmega328p.                                **
*****************************************************************************

** TARGET **

The demo runs on an Arduino Uno board.

** The Demo **

The demo blinks the onboard LED whenver one of the enabled interrupt sources
fires: INT0, INT1, PORTC0..3

** Build Procedure **

The demo was built using the GCC AVR toolchain.

** Notes **

This demo runs natively so the Arduino bootloader must be removed and the FUSEs
reprogrammed. The values used for fuses are LFUSE=0xe7 and HFUSE=0x99.
