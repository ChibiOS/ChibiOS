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
- NEW: Implemented TIMPRE setting for STM32F7xx HAL.
- NEW: Merged FatFS 0.13c.
- NEW: Added a "library generator" project for RT, it allows to
       generate a library with a pre-configured RT. It also includes
       an "header generator" able to generate an unified "ch.h" with
       all options resolved.
- LIB: Improved OSLIB initialization.
- NEW: Lots of style fixes thanks to the new tool.
- NEW: Code style checker tool added.
- NEW: Added and embedded flash driver model in HAL. Added an implementation
       for STM32F1xx, STM32L4xx, STM32L4xx+. 
- NEW: Modified AES GCM function signatures.
- HAL: Added H753 to all H7 mcuconf.h files.
- NEW: Added transactional updates to MFS. Doubled data headers magic numbers
       for improved safety and to keep the final write aligned to 64 bits, it
       is required for STM32 ECC flash.
- VAR: Modified syscalls.c to allocate memory from bottom upward, ChibiOS
       allocators take memory from top downward. This way the memory taken
       using _sbrk_r() does not contain "holes" caused by other allocators.
- LIB: Re-introduced missing chGuardedPoolGetCounterI() function to guarded
       pools allocator.
- LIB: Modified core allocator to be able to get blocks starting from bottom
       or top of the available memory range.
       Removed alignment enforcement for requested block size. Alignment is
       only ensured on the returned pointer, this should reduce memory usage
       is some cases.
- HAL: Added a new interface for range-finder devices (used by EX).
- HAL: Added mcuconf.h updater tool for STM32F407 (backported to 19.1.1).
- NIL: Integrated NIL 4.0.
- FIX: Fixed some M7 demos compile as M4 bug #1037)(backported to 19.1.3).
- FIX: Fixed missing I2C4 RCC definitions for L4/L4+ (bug #1036)
       (backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed missing delay after STM32 wait states setup (bug #1035)
       (backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed reduced time slices in RT (bug #1034)
       (backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed GCC scatter files alignment problem (bug #1033)
       (backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed long intervals fail when interval type is larger than time type
       (bug #1031)(backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed Round Robin check missing when in tick-less mode (bug #1030)
       (backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed RCC_AHB1ENR_BKPSRAMEN not present in all STMF4xx devices
       (bug #1029)(backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed MPU fix #1027 broke stack checking on Cortex-M devices without
       MPU (bug #1028)(backported to 19.1.3)(backported to 18.2.3).
- FIX: Fixed MPU setup missing on thread start (bug #1027)
       (backported to 19.1.2)(backported to 18.2.3).
- FIX: Fixed invalid I2C4 DMAs for STM32F76x (bug #1026)
       (backported to 19.1.2)(backported to 18.2.3).
- FIX: Fixed invalid STM32_UART7_RX_DMA_CHN for STM32F469 (bug #1025)
       (backported to 19.1.2)(backported to 18.2.3).
- FIX: Fixed invalid EXTI registry constant for STM32L4+ (bug #1024)
       (backported to 19.1.2).
- FIX: Fixed missing RTC definitions in STM32L1xx registry (bug #1023)
       (backported to 19.1.2).
- FIX: Fixed missing EXTI driver integration on some platforms (bug #1022)
       (backported to 19.1.2).
- FIX: Fixed various UART clock naming errors in STM32H7 HAL (bug #1021)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed missing STM32L4+ check in GPIOv3 driver (bug #1020)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed call to obsolete dmaStreamRelease() in STM32 I2Cv3 driver
       (bug #1019)(backported to 19.1.1).
- FIX: Fixed misconfiguration in STM32L4R9I DIscovery board files (bug #1018)
       (backported to 19.1.1).
- FIX: Fixed wrong Debug launch configuration in STM32L4Rx demos (bug #1017)
       (backported to 19.1.1).
- FIX: Fixed wrong ADCSEL definitions in STM32H7 HAL (bug #1016)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed chTimeIsInRangeX() failing under some configurations (bug #1015)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed invalid AXI errata fix for STM32H7xx (bug #1014)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed invalid ADCD3 initialization in STM32 ADCv3 driver (bug #1013)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed invalid call to dmaStreamRelease() in STM32 SDIOv1 driver
       (bug #1012)(backported to 19.1.1).
- FIX: Fixed wrong license restriction check in Nil (bug #1011)
       (backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed uninitialized variables in STM32 DMA drivers (bug #1010)
       (backported to 19.1.1).
- FIX: Fixed wrong mcuconf.h in some testex demos related to STM32F407 
       (bug #1008)(backported to 19.1.1)(backported to 18.2.3).
- FIX: Fixed problem in STM32 mcuconf.h template files (bug #1007)
       (backported to 19.1.1)(backported to 18.2.3).
- EX:  Fixed I2C Acquire bus called twice in the HTS221 initialization 
       (bug #1006)(backported to 19.1.1)(backported to 18.2.3).
- EX:  Fixed missing I2C release bus in LPS22HB initialization (bug #1005)
       (backported to 19.1.1).
