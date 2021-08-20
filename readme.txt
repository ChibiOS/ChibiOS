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
- NEW: EFL driver and demo for STM32F3xx.
- NEW: New unit test subsystem under /os/test. Now it is officially
       ChibiOS/TEST.
- FIX: Fixed DMAv1 compile fail on STM32L011 (bug #1181)
       (TBP)(backported to 20.3.4).
- FIX: Fixed error in STM32_ADCCLK_MIN for STM32F37x/hal_lld.h (bug #1180)
       (TBP)(backported to 20.3.4).
- FIX: Fixed direct calls to RT in STM32 RTCv2 and RTCv3 (bug #1179)
       (TBP)(backported to 20.3.4).
- FIX: Fixed STM32G071 mcuconf.h template not showing TIM14/15/16/17
       (bug #1178)(TBP).
- FIX: Fixed path ambiguity between STM32 USARTv2 and USARTv3 drivers
       (bug #1177)(TBP).
- FIX: Fixed invalid DMAMUX settings in DMAv1 for some devices (bug #1176)
       (TBP)(backported to 20.3.4).
- FIX: Fixed wrong macro in the demo STM32F7xx-SPI-ADXL355 (bug #1175) 
       (backported to 21.6.1).
- FIX: Fixed problem with RC initialization mask of the GPIO for the 
       STM32L4x3 (bug #1174) (backported to 20.3.4).
- FIX: Fixed problem with N25Q driver (bug #1173)
       (backported to 21.6.1)(backported to 20.3.4).
- FIX: Fixed semaphores broken when CH_CFG_USE_SEMAPHORES_PRIORITY is enabled
       (bug #1172)(TBP).
- FIX: Fixed idle thread stack area not cleared when CH_DBG_FILL_THREADS
       is enabled (bug #1171)(TBP).
- FIX: Fixed function chRegNextThread() broken when CH_CFG_USE_DYNAMIC
       is disabled (bug #1170)(TBP).
- FIX: Fixed insufficient information in RT registry (bug #1169)(TBP).
- FIX: Fixed code base not compatible with -Wcast-align=strict (bug #1168)(TBP).
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
