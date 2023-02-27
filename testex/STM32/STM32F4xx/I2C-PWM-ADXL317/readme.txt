*****************************************************************************
** ChibiOS/HAL + ChibiOS/EX - I2C + PWM + ADXL317 demo for STM32F4xx.      **
*****************************************************************************

** TARGET **

The demo runs on an ADI EVAL-SDP-CK1Z board. It has been tested 
connecting an external the EVAL-ADXL317Z.

** The Demo **

The application demonstrates the use of the STM32F4xx I2C driver in order
to acquire data from ADXL317 using ChibiOS/EX.

** Board Setup **

With reference to the schematic of EVAL-ADXL317Z and
to the Schematic of the SDP-K1 the following connection
are needed:
 ---------------------------------------------
|     EVAL-ADXL317Z    |        SDP-K1        |
|---------------------------------------------|
|                      |                      |
|         P1.1         |       ARD_GND        |
|         P1.5         |       ARD_3          |
|         P1.7         |       ARD_D15        |
|         P1.8         |       ARD_D14        |
|         P1.9         |       ARD_3V3        |
 ---------------------------------------------

On the SDP-K1 move the Jumper P14 to 1.8V.

** Build Procedure **

The demo has been tested using the free Codesourcery GCC-based toolchain
and YAGARTO.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com