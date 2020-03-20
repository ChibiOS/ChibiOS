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

*** 18.2.3 ***
- NEW: Added a check on defective GCC versions, Cortex-M0 only.
- NEW: Extra timer checks in STM32 ST driver.
- FIX: Fixed problem in chMtxUnlockAllS() (bug #1076).
- FIX: Fixed state check in CMSIS osKernelInitialize() wrapper (bug #1075).
- FIX: Fixed missing IRQ disabling in ADCv1 and ADCv3 STM32 drivers (bug #1073).
- FIX: Fixed missing parenthesis in ADC _adc_isr_error_code macro (bug #1072).
- FIX: Fixed non-standard declaration in STM32 ADCv3 driver (bug #1070).
- FIX: Fixed problem in the I2C fallback driver (bug #1069).
- FIX: Fixed TIM8 missing on STM32H7xx (bug #1068).
- FIX: Fixed Sharing issues with GPT TIMv1 driver (bug #1067).
- FIX: Fixed wrong check on STM32_DAC_DUAL_MODE in DACv1 STM32 driver
       (bug #1065).
- FIX: Fixed I2Cv1 extra interrupts (bug #1064).
- FIX: Fixed I2C4 broken on STM32H7xx (bug #1063).
- FIX: Fixed I2C fallback driver broken (bug #1061).
- FIX: Fixed STM32 ADC1 sample time macros (bug #1059).
- FIX: Fixed STM32 ADCv1 error callback disabled on some devices (bug #1058).
- FIX: Fixed error in uartSendFullTimeout() HAL function (bug #1057).
- FIX: Fixed OS-less Cortex-M OSAL problem with critical zones (bug #1056).
- FIX: Fixed assertion triggered in STM32 OTGv1 driver (bug #1053).
- FIX: Fixed STM32 CANv1 compile fails if CAN3 is used alone (bug #1052).
- FIX: Fixed wrong clock disable check in STM32 DACv1 driver (bug #1050).
- FIX: Fixed wrong debug check in STM32 I2Cv1 driver (bug #1048).
- FIX: Fixed warning in simulator PAL driver (bug #1047).
- FIX: Fixed board initialization failure on STM32F030x6 (bug #1046).
- FIX: Fixed STM32 I2Cv1 BTF event not cleared (bug #1045).
- FIX: Fixed STM32F7 I2C registry dma channel (bug #1043).
- FIX: Fixed missing DMA definitions for STM32F412 I2C3 (bug #1041).
- FIX: Fixed missing I2C4 RCC definitions for L4/L4+ (bug #1036).
- FIX: Fixed missing delay after STM32 wait states setup (bug #1035).
- FIX: Fixed reduced time slices in RT (bug #1034).
- FIX: Fixed GCC scatter files alignment problem (bug #1033).
- FIX: Fixed long intervals fail when interval type is larger than time type
       (bug #1031).
- FIX: Fixed Round Robin check missing when in tick-less mode (bug #1030).
- FIX: Fixed RCC_AHB1ENR_BKPSRAMEN not present in all STMF4xx devices
       (bug #1029).
- FIX: Fixed MPU fix #1027 broke stack checking on Cortex-M devices without
       MPU (bug #1028).
- FIX: Fixed MPU setup missing on thread start (bug #1027).
- FIX: Fixed invalid I2C4 DMAs for STM32F76x (bug #1026).
- FIX: Fixed invalid STM32_UART7_RX_DMA_CHN for STM32F469 (bug #1025).
- FIX: Fixed various UART clock naming errors in STM32H7 HAL (bug #1021).
- FIX: Fixed wrong ADCSEL definitions in STM32H7 HAL (bug #1016).
- FIX: Fixed chTimeIsInRangeX() failing under some configurations (bug #1015).
- FIX: Fixed invalid AXI errata fix for STM32H7xx (bug #1014).
- FIX: Fixed invalid ADCD3 initialization in STM32 ADCv3 driver (bug #1013).
- FIX: Fixed wrong license restriction check in Nil (bug #1011).
- FIX: Fixed incorrect pointer type in lwipthread.c low_level_input causes
       panic on packet reception (bug #1009).

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
- FIX: Fixed wrong mcuconf.h in some testex demos related to STM32F407 
  (bug #1008).
- EX:  Fixed I2C Acquire bus called twice in the HTS221 initialization 
  (bug #1006)
- HAL: Fixed invalid checks in STM32H7 HAL (bug #1000).
- OTH: Fixed problem in STM32H743 GCC linker file (bug #998).
- HAL: Fixed extra parenthesis in STM32F4 registry (bug #997).
- HAL: Fixed timing problem in STM32 OTGv1 driver (bug #996).
- HAL: Fixed restrictive check in MMC-SPI driver (bug #995).
- HAL: Fixed misplaced check in STM32 ST driver (bug #994).
- OTH: Fixed ARM CMx vectors table without thumb bit set for unused vectors
       (bug #993).
- HAL: Fixed STM32F7xx I2SPLL not fully initialized (bug #992).
- HAL: Fixed ethernet registry error for STM32F469/479 (bug #990).
- RT:  Fixed bug in chEvtBroadcastFlagsI (bug #989).
- HAL: Fixed NULL pointer dereferenced in N25Q driver (bug #988).
- HAL: Fixed OS-Less Cortex-M HAL functionality broken (bug #987).
- HAL: Fixed broken functionality of MFS module (bug #986).
- NIL: Fixed scheduler misbehaving in rare cases (bug #983).
- NIL: Fixed function chThdSuspendTimeoutS() ignoring TIME_IMMEDIATE (bug #982).
- HAL: Fixed STM32L071/72 entries in registry (bug #981).
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

