*****************************************************************************
*** Files Organization                                                    ***
*****************************************************************************

--{root}                  - ChibiOS directory.
  +--readme.txt           - This file.
  +--documentation.html   - Shortcut to the web documentation page.
  +--license.txt          - GPL license text.
  +--demos/               - Demo projects, one directory per platform.
  +--docs/                - Documentation.
  |  +--common/           - Documentation common build resources.
  |  +--hal/              - Builders for HAL.
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--html/          - Local HTML documentation (after rebuild).
  |  |  +--reports/       - Test reports.
  |  |  +--rsc/           - Documentation resource files (required for rebuild).
  |  |  +--src/           - Documentation source files (required for rebuild).
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--index.html     - Local documentation access (after rebuild).
  |  +--nil/              - Builders for NIL.
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--html/          - Local HTML documentation (after rebuild).
  |  |  +--reports/       - Test reports.
  |  |  +--rsc/           - Documentation resource files (required for rebuild).
  |  |  +--src/           - Documentation source files (required for rebuild).
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--index.html     - Local documentation access (after rebuild).
  |  +--rt/               - Builders for RT.
  |  |  +--html/          - Local HTML documentation (after rebuild).
  |  |  +--reports/       - Test reports.
  |  |  +--rsc/           - Documentation resource files (required for rebuild).
  |  |  +--src/           - Documentation source files (required for rebuild).
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--index.html     - Local documentation access (after rebuild).
  +--ext/                 - External libraries, not part of ChibiOS/RT.
  +--os/                  - ChibiOS components.
  |  +--common/           - Shared OS modules.
  |  |  +--abstractions/  - API emulator wrappers.
  |  |  |  +--cmsis_os/   - CMSIS OS emulation layer for RT (ARMCMx port only).
  |  |  |  +--nasa_osal/  - NASA Operating System Abstraction Layer for RT.
  |  |  +--ext/           - Vendor files used by the OS.
  |  |  +--oslib/         - RTOS modules usable by both RT and NIL.
  |  |  +--ports/         - RTOS ports usable by both RT and NIL.
  |  |  +--startup/       - Startup support for all compilers and platforms.
  |  +--ex/               - EX component.
  |  |  +--dox/           - EX documentation resources.
  |  |  +--Micron/        - EX complex drivers for Micron devices.
  |  |  +--ST/            - EX complex drivers for STMicroelectronics devices.
  |  |  +--subsystems/    - EX subsystems.
  |  |  |  +--mfs/        - EX Managed Flash Storage module.
  |  +--hal/              - HAL component.
  |  |  +--boards/        - HAL board support files.
  |  |  +--dox/           - HAL documentation resources.
  |  |  +--include/       - HAL high level headers.
  |  |  +--lib/           - HAL libraries.
  |  |  |  +--fallback/   - HAL fall back software drivers.
  |  |  |  +--peripherals/- HAL peripherals interfaces.
  |  |  |  +--streams/    - HAL streams.
  |  |  +--osal/          - HAL OSAL implementations.
  |  |  |  +--lib/        - HAL OSAL common modules.
  |  |  +--src/           - HAL high level source.
  |  |  +--ports/         - HAL ports.
  |  |  +--templates/     - HAL driver template files.
  |  |     +--osal/       - HAL OSAL templates.
  |  +--nil/              - NIL RTOS component.
  |  |  +--dox/           - NIL documentation resources.
  |  |  +--include/       - NIL high level headers.
  |  |  +--src/           - NIL high level source.
  |  |  +--templates/     - NIL configuration template files.
  |  +--rt/               - RT RTOS component.
  |  |  +--dox/           - RT documentation resources.
  |  |  +--include/       - RT high level headers.
  |  |  +--src/           - RT high level source.
  |  |  +--templates/     - RT configuration template files.
  |  +--various/          - Various portable support files.
  +--test/                - Kernel test suite source code.
  |  +--lib/              - Portable test engine.
  |  +--hal/              - HAL test suites.
  |  |  +--testbuild/     - HAL build test and MISRA check.
  |  +--nil/              - NIL test suites.
  |  |  +--testbuild/     - NIL build test and MISRA check.
  |  +--rt/               - RT test suites.
  |  |  +--testbuild/     - RT build test and MISRA check.
  |  |  +--coverage/      - RT code coverage project.
  +--testhal/             - HAL integration test demos.

*****************************************************************************
*** Releases and Change Log                                               ***
*****************************************************************************

*** Next ***
- HAL: Fixed STM32 USBv1 fails the state check when USB_USE_WAIT is TRUE
       (bug #863)(backported to 17.6.1 and 16.1.9).
- HAL: Fixed incorrect OTG stepping in STM32F412 registry (bug #861)
       (backported to 17.6.1).
- HAL: Fixed missing DMA I2C3 streams in STM32F411 registry (bug #860)
       (backported to 17.6.1).
- HAL: Fixed missing Ethernet PHY in some STM32 Nucleo-144 board files
       (bug #859)(backported to 17.6.1).
- VAR: Fixed priority issue in STM32 Nucleo-64 F401RE demo (bug #858)(backported 
       to 17.6.1).
- VAR: Fixed STM32L053 Discovery demo which is unaligned to standard demos (bug 
       #857)(backported to 17.6.1).
- HAL: Fixed HSI48 which is not correctly enabled in STM32L0xx port (bug #856)
       (backported to 17.6.1).
- HAL: Fixed unaligned STM32F0xx mcuconf.h files (bug #855)(backported 
       to 17.6.1).
- HAL: Fixed invalid handling of DST flag in STM32 RTCv2 (bug #854)(backported 
       to 17.6.1 and 16.1.9).
- HAL: Fixed extra right parenthesis in STM32F4 registry (bug #853)(backported 
       to 17.6.1).
- EX:  Fixed documentation-related issues (bug #852)(backported to 17.6.1).
- HAL: Fixed documentation-related issues (bug #852)(backported to 17.6.1).
- HAL: Fixed wrong frame size code in STM32 USARTv2 UART driver (bug #851)
       (backported to 17.6.1 and 16.1.9).
- NIL: Fixed documentation-related issues (bug #850)(backported to 17.6.1).
- RT:  Fixed documentation-related issues (bug #850)(backported to 17.6.1).
- RT:  Fixed leftover chcustomer.h file (bug #849)(backported to 17.6.1).
- RT:  Fixed invalid check in chchecks.h (bug #848)(backported to 17.6.1).
- HAL: Fixed STM32F070xB: USART invalid DMA channels (bug #847)(backported
       to 17.6.1).
- VAR: Fixed CMSIS_OS issue in timers (bug #846)(backported to 17.6.1
       and 16.1.9).
