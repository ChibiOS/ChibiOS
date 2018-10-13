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
  |  |  +--rsc/           - Documentation resource files (required for rebuild).
  |  |  +--src/           - Documentation source files (required for rebuild).
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--index.html     - Local documentation access (after rebuild).
  |  +--nil/              - Builders for NIL.
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--html/          - Local HTML documentation (after rebuild).
  |  |  +--rsc/           - Documentation resource files (required for rebuild).
  |  |  +--src/           - Documentation source files (required for rebuild).
  |  |  +--Doxyfile_*     - Doxygen project files (required for rebuild).
  |  |  +--index.html     - Local documentation access (after rebuild).
  |  +--rt/               - Builders for RT.
  |  |  +--html/          - Local HTML documentation (after rebuild).
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

*** 18.2.2 ***
- NEW: Added analog watchdog functionality to STM32 ADCv2 driver.
- NEW: Added a termination check to the shell.
- NEW: Updated CMSIS to version 5.3.0.
- NEW: Now chconf.h files have preprocessor checks around each definition,
       this allows to override settings from makefiles.
- NEW: Added new functions to I/O queues: qSetLink().
- NEW: Added new functions to objects fifos: chFifoReturnObjectS(),
       chFifoSendObjectAheadI(), chFifoSendObjectAheadS() and
       chFifoSendObjectAhead().
- HAL: Fixed mTM32L071/72 entries in registry (bug #981).
- HAL: Fixed TIM3 missing from STM32L0xx RCC macros (bug #980)
- HAL: Fixed invalid STM32 ADCv3 clock selection for L4 and L4+ (bug #979).
- HAL: Fixed wrong number of endpoints for STM32F412/413 (bug #978).
- RT:  Fixed chEvtGetAndClearFlags(...) does not mask events (bug #977).
- HAL: Fixed HAL channels chnControl() macro broken (bug #976).
- OTH: Fixed wrong timeout handling in CMSIS OS layer (bug #975).
- HAL: Fixed mii_find_phy excludes PHY address 31 (bug #971).
- HAL: Fixed invalid checks in STM32F37x HAL related to SDADC (bug #974).
- HAL: Fixed option STM32_LSCOSEL not written in STM32L4 HAL (bug #970).
- HAL: Fixed invalid DFSDM1SEL option in STM32L4 HAL (bug #969).
- HAL: Fixed incorrect checks on STM32_SAI2SEL option in STM32L4 HAL
       (bug #968).
- HAL: Fixed incorrect handling of PDIV dividers in STM32L4 HAL (bug #967).
- HAL: Fixed documentation error in spiStop() (bug #966).
- HAL: Fixed missing parenthesis in STM32L073 registry entry (bug #965).
- HAL: Fixed problem with STM32 RTCv1 and GCC7 compiler (bug #964).
- HAL: Fixed invalid timeout calculation in hal_buffers (bug #963).
- RT:  Fixed invalid parameter in CH_CFG_SYSTEM_INIT_HOOK hook macro
       (bug #962).
- OTH: Fixed demos failing to compile (bug #961).
- HAL: Fixed issue in hal_queues (bug #960).
- HAL: Fixed incorrect state change in I2S driver (bug #959).
- HAL: Fixed incorrect TCIE handling in STM32 serial drivers (bug #958).
- HAL: Fixed invalid period calculation in STM32 GPT driver (bug #957).
- HAL: Fixed missing USART7/8 definitions in STM32F0 HAL (bug #956).
- LIB: Fixed heap allocation issue (bug #955).
- HAL: Fixed win32 simulator HAL broken because a typo (bug #954).
- HAL: Fixed race condition in STM32 ADCv3 driver (bug #953).
- HAL: Fixed wrong registry entries for STM32F030x4 (bug #952).
- HAL: Fixed invalid divider settings in Serial and UART STM32 drivers
       when USART_CR1_OVER8 is specified (bug #951).
- NIL: Fixed missing extern declaration in IAR Cortex-M port (bug #950).
- HAL: Fixed ASCR register invalid handling in STM32 GPIOv3 driver (bug #949).
- HAL: Fixed missing definition in UART driver (bug #948).
- OTH: Fixed wrong macro check in GCC Cortex-M startup files (bug #947).
- NEW: Added optional support for character match callback in the UART
       high level driver.
- NEW: Added support for LDM303AGR 6 axis Accelerometer\Magnetometer MEMS.
- NEW: Added support for LSM6DSL 6 axis Accelerometer\Gyroscope MEMS.
- NEW: Added support for LPS22HB 2 axis Barometer\Thermometer MEMS.
- EX.  Global version number moved to 1.1.0.
- EX:  Updated HTS221 to 1.1.1.
- EX:  Updated L3GD20 to 1.1.1.
- EX:  Updated LIS3DSH to 1.1.1.
- EX:  Updated LIS3MDL to 1.1.1.
- EX:  Updated LPS25H to 1.1.1.
- EX:  Updated LSM303DLHC to 1.1.1.
- EX:  Updated LSM6DS0 to 1.1.1.
- HAL: Fixed binary instead of logic operator in STM32F4 HAL (bug #946).
- HAL: Fixed Mikroe clicker 2 misaligned board file (bug #945).
- EX:  Fixed ChibiOS/EX documentation misalignments (bug #943).
 
*** 18.2.1 ***
- NEW: Added HAL support for STM32L443.
- NEW: Improved some EX drivers and related test applications.
- NEW: Improved VMT mechanisms to allow multiple interfaces.
- NEW: Updated make mechanisms in demos/STM32, testhal/STM32 and testex/STM32:
       now makefile are more shorter and inclusion are easier to do.
- EX:  Updated HTS221 to 1.1.0.
- EX:  Updated L3GD20 to 1.1.0.
- EX:  Updated LIS3DSH to 1.1.0.
- EX:  Updated LIS3MDL to 1.1.0.
- EX:  Updated LIS302DL to 1.1.0.
- EX:  Updated LPS25H to 1.1.0.
- EX:  Updated LSM303DLHC to 1.1.0.
- HAL: Fixed invalid settings in STM32F769I-Discovery board files (bug #942).
- OTH: Fixed short branch to _unhandled_exception in vectors.S (bug #941).
- HAL: Fixed IOBus PAL functions missing the const qualifier (bug #940).
- HAL: Fixed STM32 USBv1 driver does not reset data toggling bits on endpoint
       initialization (bug #939).
- HAL: Fixed incorrect behavior of USB driver on SET CONFIGURATION (bug #938).
- HAL: Fixed macro expansion problem in SPI high level driver (bug #937)
- HAL: Fixed missing CAN2 macros from STM32L4xx stm32_rcc.h file (bug #936).
- OTH: Fixed inclusion order problem in STM32L4 cmparams.h file (bug #935).
- HAL: Fixed problem clearing UIF timer flag in STM32 PWM driver (bug #934).
- HAL: Fixed USB Serial driver problem with zero-size OUT transactions
       (bug #933).
- HAL: Fixed race condition in STM32 QSPI driver (bug #932).
- HAL: Fixed function mfsReadRecord() causes memory corruption because a
       buffer overflow (bug #931).
- HAL: Fixed silence GCC 7.3.0 warning (bug #930).
- HAL: Fixed invalid SAI1 clock selection on STM32F7xx (bug #929).
- HAL: Fixed invalid clock checks for SDMMC1 and SDMMC2 on STM32F7xx
       (bug #928).
- HAL: Fixed useless writes in read-only CFGR_SWS field on all STM32Fxx
       (bug #927).
- HAL: Fixed typo in hal_pal.h (bug #926).
- HAL: Fixed UART driver stop functions confusing returned value (bug #925).
- HAL: Fixed USB driver not stopped (bug #924).
- HAL: Fixed I2C address not accepted (bug #923).
- HAL: Fixed problem with HSI48 on STM32L4xx (bug #922).
- HAL: Fixed invalid implementation of palWaitPadTimeoutS() and
       palWaitLineTimeoutS() APIs (bug #921).
- HAL: Fixed wrong DMA settings for STM32F76x I2C3 and I2C4 (bug #920).
- HAL: Fixed wrong flash waiting state for STM32F7xx (bug #918).

*** 18.2.0 ***
- First 18.2.x release, see release note 18.2.0.

