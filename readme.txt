*****************************************************************************
*** Files Organization                                                    ***
*****************************************************************************

--{root}                        - ChibiOS directory.
  +--readme.txt                 - This file.
  +--documentation.html         - Shortcut to the web documentation page.
  +--license.txt                - GPL license text.
  +--demos/                     - Demo projects, one directory per platform.
  +--docs/                      - Documentation.
  |  +--common/                 - Documentation common build resources.
  |  +--hal/                    - Builders for HAL.
  |  +--nil/                    - Builders for NIL.
  |  +--rt/                     - Builders for RT.
  +--ext/                       - External libraries, not part of ChibiOS.
  +--os/                        - ChibiOS components.
  |  +--common/                 - Shared OS modules.
  |  |  +--abstractions/        - API emulator wrappers.
  |  |  |  +--cmsis_os/         - CMSIS OS emulation layer for RT.
  |  |  |  +--nasa_osal/        - NASA Operating System Abstraction Layer.
  |  |  +--ext/                 - Vendor files used by the OS.
  |  |  +--ports/               - RTOS ports usable by both RT and NIL.
  |  |  +--startup/             - Startup support.
  |  +--ex/                     - EX component.
  |  |  +--dox/                 - EX documentation resources.
  |  |  +--include/             - EX header files.
  |  |  +--devices /            - EX complex drivers.
  |  +--hal/                    - HAL component.
  |  |  +--boards/              - HAL board support files.
  |  |  +--dox/                 - HAL documentation resources.
  |  |  +--include/             - HAL high level headers.
  |  |  +--lib/                 - HAL libraries.
  |  |  |  +--complex/          - HAL collection of complex drivers.
  |  |  |  |  +--mfs/           - HAL managed flash storage driver.
  |  |  |  |  +--serial_nor/    - HAL managed flash storage driver.
  |  |  |  +--fallback/         - HAL fall back software drivers.
  |  |  |  +--peripherals/      - HAL peripherals interfaces.
  |  |  |  +--streams/          - HAL streams.
  |  |  +--osal/                - HAL OSAL implementations.
  |  |  |  +--lib/              - HAL OSAL common modules.
  |  |  +--src/                 - HAL high level source.
  |  |  +--ports/               - HAL ports.
  |  |  +--templates/           - HAL driver template files.
  |  |     +--osal/             - HAL OSAL templates.
  |  +--oslib/                  - RTOS modules usable by both RT and NIL.
  |  |  +--include/             - OSLIB high level headers.
  |  |  +--src/                 - OSLIB high level source.
  |  |  +--templates/           - OSLIB configuration template files.
  |  +--nil/                    - NIL RTOS component.
  |  |  +--dox/                 - NIL documentation resources.
  |  |  +--include/             - NIL high level headers.
  |  |  +--src/                 - NIL high level source.
  |  |  +--templates/           - NIL configuration template files.
  |  +--rt/                     - RT RTOS component.
  |  |  +--dox/                 - RT documentation resources.
  |  |  +--include/             - RT high level headers.
  |  |  +--src/                 - RT high level source.
  |  |  +--templates/           - RT configuration template files.
  |  +--various/                - Various portable support files.
  +--test/                      - Kernel test suite source code.
  |  +--lib/                    - Portable test engine.
  |  +--hal/                    - HAL test suites.
  |  |  +--testbuild/           - HAL build test and MISRA check.
  |  +--nil/                    - NIL test suites.
  |  |  +--testbuild/           - NIL build test and MISRA check.
  |  +--rt/                     - RT test suites.
  |  |  +--testbuild/           - RT build test and MISRA check.
  |  |  +--coverage/            - RT code coverage project.
  +--testex/                    - EX integration test demos.
  +--testhal/                   - HAL integration test demos.

*****************************************************************************
*** Releases and Change Log                                               ***
*****************************************************************************

*** Next ***
- NEW: STM32 ADCv2 now supports return code on start function.
- NEW: Integrated FatFS with lwIP HTTPD, now it is possible to serve files
       using HTTP from a storage device.
- NEW: Updated FatFS to version 0.14b.
- NEW: SPIv2 driver has been implemented on: STM32F0, STM32F1, STM32F3,
       STM32F4, STM32F7, STM32G0, STM32G4, STM32L0, STM32L1, STM32L4,
       STM32L4+, STM32H7.
- NEW: New SPIv2 driver model, it is compatible with the previous SPI driver
       and introduces: better runtime errors handling, slave mode,
       data synchronization function, various other improvements.
- NEW: Added an alternate port for ARMv7-M, it uses less RAM and it is
       faster at interrupt processing, it is slightly slower at
       thread-to-thread context switch so it is not a full replacement.
- NEW: Now all xxxStart() functions in HAL are able to report a driver
       activation error.
- NEW: Support for STM32G031, STM32G041, STM32G0B1, STM32G0C1.
- NEW: Made STM32H7 non-cacheable memory option also shareable.
- NEW: EFL driver and demo for STM32F3xx.
- NEW: New unit test subsystem under /os/test. Now it is officially
       ChibiOS/TEST.
- FIX: Fixed function nvicSetSystemHandlerPriority() failing with CM0+ cores
       (bug #1199)(backported to 20.3.4)(TBP to 21.6.1).
- FIX: Fixed several problems in STM32L4xx PLL and GPIO settings (bug #1198)
       (backported to 20.3.4)(TBP to 21.6.1).
- FIX: Fixed PLLSA1x input frequency calculated incorrectly (bug #1197)
       (TBP to 21.6.1).
- FIX: Fixed ARMCMx port, MPU not enabled when PORT_ENABLE_GUARD_PAGES is set
       (bug #1195)(backported to 20.3.4)(TBP to 21.6.1).
- FIX: Fixed removed incorrect ADCv4 checks for MONEN (bug #1194)
- FIX: Fixed hang in spi_lld_ignore() in SPIv3 on H7 (bug #1193)
- FIX: Fixed invalid references in e200 port (bug #1192)
       (TBP to 21.6.1)
- FIX: Fixed NIL invalid references when debug options are enabled (bug #1191)
       (TBP to 21.6.1)
- FIX: Fixed problem in stm32_pll.inc and stm32_pllv2.inc (bug #1190)
       (TBP to 21.6.1)
- FIX: Fixed invalid RCC LPUART1 clock enable on STM32G0xx (bug #1189)
       (TBP to 21.6.1)(backported to 20.3.4).
- FIX: Support for STM32G491 (bug #1187)(TBP to 21.6.1).
- FIX: Fixes for uart clock rounding, missing DMA streams and timers
       (bug #1186)(TBP).
- FIX: Fixed invalid check in chVTResetTimeStamp() function (bug #1185)
       (TBP).
- FIX: Fixed wrong configuration in testex LSM6DSL demos (bug #1184).
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed CH_CFG_NO_IDLE_THREAD option causes compiler errors (bug #1183) 
       (backported to 21.6.1).
- FIX: Fixed STM32 ADCv3 differences in headers (bug #1182)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed DMAv1 compile fail on STM32L011 (bug #1181)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed error in STM32_ADCCLK_MIN for STM32F37x/hal_lld.h (bug #1180)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed direct calls to RT in STM32 RTCv2 and RTCv3 (bug #1179)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed STM32G071 mcuconf.h template not showing TIM14/15/16/17
       (bug #1178)(backported to 21.6.1).
- FIX: Fixed path ambiguity between STM32 USARTv2 and USARTv3 drivers
       (bug #1177)(backported to 21.6.1).
- FIX: Fixed invalid DMAMUX settings in DMAv1 for some devices (bug #1176)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed wrong macro in the demo STM32F7xx-SPI-ADXL355 (bug #1175) 
       (backported to 21.6.1(backported to 21.6.1).
- FIX: Fixed problem with RC initialization mask of the GPIO for the 
       STM32L4x3 (bug #1174)(backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed problem with N25Q driver (bug #1173)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed semaphores broken when CH_CFG_USE_SEMAPHORES_PRIORITY is enabled
       (bug #1172)(backported to 21.6.1).
- FIX: Fixed idle thread stack area not cleared when CH_DBG_FILL_THREADS
       is enabled (bug #1171)(backported to 21.6.1).
- FIX: Fixed function chRegNextThread() broken when CH_CFG_USE_DYNAMIC
       is disabled (bug #1170)(backported to 21.6.1).
- FIX: Fixed insufficient information in RT registry (bug #1169)
       (backported to 21.6.1).
- FIX: Fixed code base not compatible with -Wcast-align=strict (bug #1168)
       (backported to 21.6.1).
- FIX: Fixed wrong chconf.h for ADuCM36x demos (bug #1167)
       (backported to 21.6.1).
- FIX: Fixed missing constant in ADuCM36x hal_lld.c (bug #1166)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed missing chrfcu.c file in rt.mk (bug #1165)
       (backported to 21.6.1).
- FIX: Fixed invalid STM32 TIM21/TIM22 debug freeze setting (bug #1164)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed missing RTC APB enable on STM32G0xx (bug #1163)
       (backported to 21.6.1)
- FIX: Fixed I2S-related definitions missing in STM32F3xx registry (bug #1162)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed AVR port broken (bug #1161)(backported to 21.6.1).
