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
  +--ext/                       - External libraries, not part of ChibiOS/RT.
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
  |  |  +--Bosch /              - EX complex drivers for Bosch devices.
  |  |  +--Micron/              - EX complex drivers for Micron devices.
  |  |  +--ST/                  - EX complex drivers for ST devices.
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
  |  +--lib/                    - RTOS modules usable by both RT and NIL.
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
- NEW: The callback of drivers with circular buffers (ADC, DAC, I2S, SPI) has
       been simplified, no parameters. A driver function xxxIsBufferComplete()
       has been added to determine if it is the half buffer callback or the
       final callback.
- NEW: ADC driver state machine change, now the state ADC_COMPLETE is set
       before calling the 2nd callback even in circular mode. This has been
       done for consistency with other drivers with circular buffers.
- NEW: Low level drivers simplification. There is a new template of LLD, now
       driver and configuration types are defined in the HLD, LLD just exports
       macros with the fields to be added to the structures.
       So far the drivers updated are: ADC, DAC, I2S, RTC, SPI, TRNG, WSPI.
- NEW: Added UART7/8 support to STM32 UART USARTv1 driver.
- NEW: Added persistent storage interface to the STM32 RTCv2 driver.
- NEW: STM32 RTCv2 driver now supports callbacks on events.
- NEW: Added an EXTI helper driver for STM32.
- NEW: Added demo for STM32L4R9I-Discovery board.
- NEW: Support for new serial NOR device MX25LM51245G. Other Macronix devices
       should work but not tested.
- NEW: Serial NOR flash infrastructure reworked and made simpler.
- NEW_ Modified the RTC driver to implement the persistent storage interface
       instead of files interface.
- NEW: Added a new "persistent storage" base class to HAL.
- NEW: Added support for TIM21 and TIM22 in STM32 GPT driver.
- NEW: Reinforced checks in TIM-related drivers.
- NEW: Added support for STM32L072 and STM32L073.
- NEW: Added chThdResume() function to NIL.
- NEW: Removed QSPI driver model, entirely replaced by WSPI.
- NEW: Added demos regarding WSPI, serial nor driver and MFS.
- NEW: Modified the serial nor driver to work with WSPI instead of QSPI,
       improved it in several way, simplicity mainly.
- NEW: Added an STM32 QUADSPIv1 implementation for WSPI.
- NEW: Added a new WSPI driver model to HAL, it is a redesign of the
       existing QSPI for generic "Wide SPI" interfaces. It now supports up
       to eight data lines so "quad" was no more appropriate.
- NEW: Added a new SIO driver model to HAL, it is a low level abstraction of
       an UART.
- NEW: Independent TRNG driver model added to HAL. Implemented a RNGv1 driver
       for STM32, added test application.
- NEW: Added a new "pipes" subsystem to the OS library.
- NEW: Added mcuconf.h generators for STM32L432xx, STM32L476xx, STM32L496xx,
       STM32L4Rxxx, STM32F72x/73x, STM32F746/756, STM32F76x/77x, STM32F413xx
       and STM32F303xx devices.
- NEW: Added demo for STM32L496ZG-Nucleo144 and STM32L4R5ZI-Nucleo144 boards.
- NEW: Modified USARTv2 to support HW FIFOs where present.
- NEW: STM32 DMAv1, ADCv3, DACv1, I2Cv2, SPIv2 and USARTv2 are now
       DMAMUX-aware.
- NEW: Introduced support for STM32L4+ devices.
- NEW: TRNG API now takes a new "size" parameter, the API can now generate
       random numbers of variable size. The crypto driver now does not store
       a copy of the transient key inside, the low level can do that if
       required.
- NEW: Added analog watchdog functionality to STM32 ADCv2 driver.
- NEW: Added a termination check to the shell.
- NEW: Updated CMSIS to version 5.4.0.
- NEW: Now chconf.h files have preprocessor checks around each definition,
       this allows to override settings from makefiles.
- NEW: Added new functions to I/O queues: qSetLink()(backported to 18.2.2).
- NEW: Added new functions to objects fifos: chFifoReturnObjectS()(backported
       to 18.2.2),
       chFifoSendObjectAheadI(), chFifoSendObjectAheadS() and
       chFifoSendObjectAhead()(backported to 18.2.2).
- NEW: Added new functions to guarded pools: chGuardedPoolFreeS() and
       chGuardedPoolAddS().
- NEW: Added initializer sections for flash0...flash7 memory areas in
       GCC Cortex-M linker scripts.
- NEW: Added support for oversampling in STM32 ADCv3 driver.
- NEW: Restructured the STM32F4xx HAL support, added support for STM32F413,
       added ability to handle the TIMPRE bit, separated the clock tree in
       two distinct implementation to reduce the proliferation of compiler
       time conditionals, added more checks to the input parameters.
- NEW: Added optional support for character match callback in the UART
       high level driver.
- NEW: Change, chMtxGetNextMutexS() renamed to chMtxGetNextMutexX().
- NEW: RT C++ wrapper reworked, now it is mostly inline code, added some new
       wrappers and methods. Added wrappers for more API functions. BaseThreads
       are no more descendants of ThreadReference.
- NEW: Updated STM32L4xx headers to version 1.11.0.
- NEW: Added HAL support for STM32L443.
- NEW: Added support for LDM303AGR 6 axis Accelerometer\Magnetometer MEMS.
- NEW: Added support for LSM6DSL 6 axis Accelerometer\Gyroscope MEMS.
- NEW: Added support for LPS22HB 2 axis Barometer\Thermometer MEMS.
- NEW: Separated OSLIB from RT and NIL, now it is a separate "product" with
       its own version, configuration file and licensing. The library will
       grow to include more functionalities.
       RT and NIL will contain only the core functionalities, everything else
       is shared library code.
- EX.  Global version number moved to 1.1.0.
- EX:  Updated HTS221 to 1.1.1 (backported to 18.2.2).
- EX:  Updated L3GD20 to 1.1.1 (backported to 18.2.2).
- EX:  Updated LIS3DSH to 1.1.1 (backported to 18.2.2).
- EX:  Updated LIS3MDL to 1.1.1 (backported to 18.2.2).
- EX:  Updated LPS25H to 1.1.1 (backported to 18.2.2).
- EX:  Updated LSM303DLHC to 1.1.1 (backported to 18.2.2).
- EX:  Updated LSM6DS0 to 1.1.1 (backported to 18.2.2).
- EX:  Updated HTS221 to 1.1.0 (backported to 18.2.1).
- EX:  Updated L3GD20 to 1.1.0 (backported to 18.2.1).
- EX:  Updated LIS3DSH to 1.1.0 (backported to 18.2.1).
- EX:  Updated LIS3MDL to 1.1.0 (backported to 18.2.1).
- EX:  Updated LIS302DL to 1.1.0 (backported to 18.2.1).
- EX:  Updated LPS25H to 1.1.0 (backported to 18.2.1).
- EX:  Updated LSM303DLHC to 1.1.0 (backported to 18.2.1).
- HAL: Fixed STM32F7xx I2SPLL not fully initialized (bug #992)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed ethernet registry error for STM32F469/479 (bug #990)
       (backported to 18.2.2 and 17.6.5).
- RT:  Fixed bug in chEvtBroadcastFlagsI (bug #989)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed OS-Less Cortex-M HAL functionality broken (bug #987)
       (backported to 18.2.2).
- HAL: Fixed broken functionality of MFS module (bug #986)
       (backported to 18.2.2).
- NIL: Fixed scheduler misbehaving in rare cases (bug #983)
       (backported to 18.2.2 and 17.6.5).
- NIL: Fixed function chThdSuspendTimeoutS() ignoring TIME_IMMEDIATE (bug #982)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed mTM32L071/72 entries in registry (bug #981)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed TIM3 missing from STM32L0xx RCC macros (bug #980)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed invalid STM32 ADCv3 clock selection for L4 and L4+ (bug #979)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed wrong number of endpoints fot STM32F412/413 (bug #978)
       (backported to 18.2.2 and 17.6.5).
- RT:  Fixed chEvtGetAndClearFlags(...) does not mask events (bug #977)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed HAL channels chnControl() macro broken (bug #976)
       (backported to 18.2.2).
- OTH: Fixed wrong timeout handling in CMSIS OS layer (bug #975)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed invalid checks in STM32F37x HAL related to SDADC (bug #974)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed mii_find_phy excludes PHY address 31 (bug #971)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed option STM32_LSCOSEL not written in STM32L4 HAL (bug #970)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed invalid DFSDM1SEL option in STM32L4 HAL (bug #969)(backported
       to 18.2.2 and 17.6.5).
- HAL: Fixed incorrect checks on STM32_SAI2SEL option in STM32L4 HAL
       (bug #968)(backported to 18.2.2 and 17.6.5).
- HAL: Fixed incorrect handling of PDIV dividers in STM32L4 HAL (bug #967)
       (backported to 18.2.2).
- HAL: Fixed documentation error in spiStop() (bug #966)(backported
       to 18.2.2 and 17.6.5).
- HAL: Fixed missing parenthesis in STM32L073 registry entry (bug #965)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed problem with STM32 RTCv1 and GCC7 compiler (bug #964)(backported
       to 18.2.2 and 17.6.5).
- HAL: Fixed invalid timeout calculation in hal_buffers (bug #963)(backported
       to 18.2.2 and 17.6.5).
- RT:  Fixed invalid parameter in CH_CFG_SYSTEM_INIT_HOOK hook macro
       (bug #962)(backported to 18.2.2).
- OTH: Fixed demos failing to compile (bug #961)(backported to 18.2.2).
- HAL: Fixed issue in hal_queues (bug #960)(backported to 18.2.2).
- HAL: Fixed incorrect state change in I2S driver (bug #959)(backported
       to 18.2.2 and 17.6.5).
- HAL: Fixed incorrect TCIE handling in STM32 serial drivers (bug #958)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed invalid period calculation in STM32 GPT driver (bug #957)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed missing USART7/8 definitions in STM32F0 HAL (bug #956)(backported
       to 18.2.2).
- LIB: Fixed heap allocation issue (bug #955)(backported to 18.2.2 and 17.6.5).
- HAL: Fixed win32 simulator HAL broken because a typo (bug #954)(backported
       to 18.2.2).
- HAL: Fixed race condition in STM32 ADCv3 driver (bug #953)(backported to
       18.2.2 and 17.6.5).
- HAL: Fixed wrong registry entries for STM32F030x4 (bug #952)(backported
       to 18.2.2).
- HAL: Fixed invalid divider settings in Serial and UART STM32 drivers
       when USART_CR1_OVER8 is specified (bug #951)(backported to 18.2.2
       and 17.6.5).
- NIL: Fixed missing extern declaration in IAR Cortex-M port (bug #950)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed ASCR register invalid handling in STM32 GPIOv3 driver (bug #949)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed missing definition in UART driver (bug #948)(backported
       to 18.2.2).
- OTH: Fixed wrong macro check in GCC Cortex-M startup files (bug #947)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed binary instead of logic operator in STM32F4 HAL (bug #946)
       (backported to 18.2.2 and 17.6.5).
- HAL: Fixed Mikroe clicker 2 misaligned board file (bug #945)
       (backported to 18.2.2 and 17.6.5).
- EX:  Fixed ChibiOS/EX documentation misalignments (bug #943).
- HAL: Fixed invalid settings in STM32F769I-Discovery board files (bug #942)
       (backported to 18.2.1 and 17.6.5).
- OTH: Fixed short branch to _unhandled_exception in vectors.S (bug #941)
       (backported to 18.2.1).
- HAL: Fixed IOBus PAL functions missing the const qualifier (bug #940)
       (backported to 18.2.1 and 17.6.5).
- HAL: Fixed STM32 USBv1 driver does not reset data toggling bits on endpoint
       initialization (bug #939)(backported to 18.2.1 and 17.6.5).
- HAL: Fixed incorrect behavior of USB driver on SET CONFIGURATION (bug #938)
       (backported to 18.2.1 and 17.6.5).
- HAL: Fixed macro expansion problem in SPI high level driver (bug #937)
       (backported to 18.2.1).
- HAL: Fixed missing CAN2 macros from STM32L4xx stm32_rcc.h file (bug #936)
       (backported to 18.2.1).
- OTH: Fixed inclusion order problem in STM32L4 cmparams.h file (bug #935)
       (backported to 18.2.1 and 17.6.5).
- HAL: Fixed problem clearing UIF timer flag in STM32 PWM driver (bug #934)
       (backported to 18.2.1 and 17.6.5).
- HAL: Fixed USB Serial driver problem with zero-size OUT transactions
       (bug #933)(backported to 18.2.1 and 17.6.5).
- HAL: Fixed race condition in STM32 QSPI driver (bug #932)(backported to
       18.2.1 and 17.6.5).
- HAL: Fixed function mfsReadRecord() causes memory corruption because a
       buffer overflow (bug #931)(backported to 18.2.1).
- HAL: Fixed silence GCC 7.3.0 warning (bug #930)(backported to 18.2.1
       and 17.6.5).
- HAL: Fixed invalid SAI1 clock selection on STM32F7xx (bug #929)(backported
       to 18.2.1 and 17.6.4).
- HAL: Fixed invalid clock checks for SDMMC1 and SDMMC2 on STM32F7xx
       (bug #928)(backported to 18.2.1).
- HAL: Fixed useless writes in read-only CFGR_SWS field on all STM32Fxx
       (bug #927)(backported to 18.2.1 and 17.6.4).
- HAL: Fixed typo in hal_pal.h (bug #926)(backported to 18.2.1).
- HAL: Fixed I2C address not accepted (bug #923)(backported to 18.2.1
       and 17.6.4).
- HAL: Fixed problem with HSI48 on STM32L4xx (bug #922)(backported to 18.2.1).
- HAL: Fixed invalid implementation of palWaitPadTimeoutS() and
       palWaitLineTimeoutS() APIs (bug #921)(backported to 18.2.1).
- HAL: Fixed wrong DMA settings for STM32F76x I2C3 and I2C4 (bug #920)
       (backported to 18.2.1 and 17.6.4).
- HAL: Fixed wrong flash waiting state for STM32F7xx (bug #918)
       (backported to 18.2.1 and 17.6.4).
