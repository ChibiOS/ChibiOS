*****************************************************************************
** ChibiOS/HAL + ChibiOS/EX - I2C + BMP085 demo for STM32F4xxxx.           **
*****************************************************************************

** TARGET **

The demo runs on an STM32F401RE Nucleo board.

** The Demo **

The application demonstrates the use of the STM32F4xx I2C driver in order
to acquire data from BMP085 sensor by using ChibiOS/EX and the shell.

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
