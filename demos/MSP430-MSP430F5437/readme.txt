*****************************************************************************
** ChibiOS/RT port for Texas Instruments MSP430X.                          **
*****************************************************************************

** TARGET **

The demo runs on an customized MSP430-F5437 board with a 32768kHz XTAL1
installed. DCOCLK (=MCLK, =SMCLK) is set up to 20Mhz. From mcuconf.h:
#define MSP430_USE_CLOCK            MSP430_CLOCK_SOURCE_DCOCLK

UART0 (38400/8N1) ist used for printing some board information.

** The Demo **

The demo runs first some tests, just to check port correctness. Meanwhile,
green led light. After that the board red and green leds will flash in the
cycle of ~0.5 and ~1.5 seconds respectively.

** Build Procedure **

The demo was built using the
a) IAR for MSP430 5.10.1 (5.10.1.50144) toolchain;
b) GCC version 4.6.3 20120301 (mspgcc LTS 20120406 unpatched).

*** Notes ***

On porting to an another MSP430X-MCU remember to check your linker settings 
for __heap_base__ and __heap_end__ symbols (or redefine these in
asmdefines.s43).
