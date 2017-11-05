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

*** 17.6.3 ***
- NIL: Fixed core and Heap allocators not functional in NIL (bug #902).
- HAL: Fixed function uartSendFullTimeout() failing on STM32 USARTv1 and
       v2 drivers (bug #901).
- HAL: Fixed broken I2C fallback driver (bug #900).
- LIB: Fixed heap buffer alignment not enforced (bug #899).
- LIB: Fixed call protocol violation in chCoreAlloc() (bug #896).
- RT:  Fixed trace Buffer activation bits state reversed in chconf.h
       (bug #895).
- BLD: Fixed USE_OPT not passed to assembler in rules.mk (bug #892).
- HAL: Fixed IRQ sharing issue in STM32 DMAv1 driver (bug #891).
- HAL: Fixed CHPRINTF_USE_FLOAT defaulted to TRUE (bug #890).
- HAL: Fixed various STM32 registry problems (more instances)(bug #889).

*** 17.6.2 ***
- HAL: Fixed various STM32 registry problems (bug #889).
- LIB: Fixed heap allocator returning unaligned blocks (bug #888).

*** 17.6.1 ***
- NEW: Integrated the latest FatFS 0.13 with patches.
- NEW: Improved RT and NIL test suite to report version numbers and
       configuration settings.
- NEW: Added a test suite generator tool written in FTL.
- NEW: Added to the HAL USB driver a new function usbWakeupHost() for
       standby exit.
- NEW: Added shared Eclipse debug configurations for OpenOCD under
       ./tools/eclipse/debug. Now it is no more required to re-create
       those each time a new workspace is created, just import the global
       ChibiOS project in it. The configurations will appear under the
       Eclipse Tools menu. It is required to create an OPENOCD environment
       variable pointing to the OpenOCD executable. It will be done in
       ChibiStudio 20 by default.
- NIL: Fixed duplicated entries in NIL documentation (bug #887).
- HAL: Fixed EXT low level driver bug on AVR port (bug #886).
- HAL: Fixed USB GET_DESCRIPTOR not handled for Interface Recipients (bug #885).
- RT:  MAILBOX_DECL size parameter is actually a count (bug #884).
- HAL: Fixed error in uartReceiveTimeout() and uartSendTimeout() (bug #883).
- HAL: Fixed TIMx DBL field macro broken (bug #880).
- HAL: Fixed STM32 SPI problem in spi_lld_start() (bug #879).
- HAL: Fixed invalid STM32 CAN3 filters initialization (bug #878).
- HAL: Fixed missing CAN definitions in STM32L432 registry entry (bug #877).
- HAL: Fixed missing STM32_TIM_MAX_CHANNELS definition in STM32L0 registry
       (bug #876).
- HAL: Fixed STM32 OTGv1 driver fails on STM32L4 (bug #875).
- HAL: Fixed wrong I2S and SAI freq divisor (bug #874).
- HAL: Fixed wrong SAI1 and SAI2 clock selection (bug #873).
- HAL: Fixed invalid number of DMA channels on STM32L011 (bug #872).
- HAL: Fixed STM32 USARTv2 serial incorrect buffer size declarations
       (bug #871).
- HAL: Fixed bug in STM32L0xx port related to STM32L0x1 (bug #870).
- HAL: Fixed board file configuration for STM32F3 Discovery REVC (bug #869).
- HAL: Fixed wrong PPRE2 and LSI related macros in STM32L0 hal lld (bug #868).
- HAL: Fixed wrong bit mask in STM32L0 hal lld (bug #866).
- RT:  Fixed misplaced assertion in semaphores code (bug #865).
- RT:  Fixed event cast cleanup for compilation warnings (bug #864).
- HAL: Fixed STM32 USBv1 fails the state check when USB_USE_WAIT is TRUE
       (bug #863).
- HAL: Fixed incorrect OTG stepping in STM32F412 registry (bug #861).
- HAL: Fixed missing DMA I2C3 streams in STM32F411 registry (bug #860).
- HAL: Fixed missing Ethernet PHY in some STM32 Nucleo-144 board files (bug 
       #859).
- VAR: Fixed priority issue in STM32 Nucleo-64 F401RE demo (bug #858).
- VAR: Fixed STM32L053 Discovery demo which is unaligned to standard demos (bug 
       #857).
- HAL: Fixed HSI48 which is not correctly enabled in STM32L0xx port (bug #856).
- HAL: Fixed unaligned STM32F0xx mcuconf.h files (bug #855).
- HAL: Fixed invalid handling of DST flag in STM32 RTCv2 (bug #854).
- HAL: Fixed extra right parenthesis in STM32F4 registry (bug #853).
- EX:  Fixed documentation-related issues (bug #852).
- HAL: Fixed documentation-related issues (bug #852).
- HAL: Fixed wrong frame size code in STM32 USARTv2 UART driver (bug #851).
- NIL: Fixed documentation-related issues (bug #850).
- RT:  Fixed documentation-related issues (bug #850).
- RT:  Fixed leftover chcustomer.h file (bug #849).
- RT:  Fixed invalid check in chchecks.h (bug #848).
- HAL: Fixed STM32F070xB: USART invalid DMA channels (bug #847).
- VAR: Fixed CMSIS_OS issue in timers (bug #846).

*** 17.6.0 ***
- First 17.6.0 release, see release note 17.6.0.
