*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-A5 SAMA5D2-XPLAINED.                     **
*****************************************************************************

** TARGET **

The demo runs on an SAMA5D2-XPLAINED board.

** The Demo **

The demo currently just flashes a LED using a thread and, on user button pressed,
sends HTTPS GET at host defined in web.h on port 443.

** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain
and YAGARTO.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
