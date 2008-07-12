*****************************************************************************
** ChibiOS/RT port for ARM7TDMI LPC214X.                                   **
*****************************************************************************

** TARGET **

The demo runs on an Olimex LPC-P2148 board. The port on other boards or other
members of the LPC2000 family should be an easy task.

** The Demo **

This is a minimal demo, it just blinks the leds on the board by using multiple
threads, most subsystems are disabled.

** Build Procedure **

The demo was built using the YAGARTO toolchain but any toolchain based on GCC
and GNU userspace programs will work.
