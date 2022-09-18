*****************************************************************************
*** Files Organization                                                    ***
*****************************************************************************

--{root}                        - ChibiOS directory.
  +--readme.txt                 - This file.
  +--documentation.html         - Shortcut to the web documentation page.
  +--license.txt                - GPL license text.
  +--demos/                     - Demo projects, one directory per platform.
  +--docs/                      - Documentation.
  |  +--build/                  - Documentation builders.
  |  +--manual/                 - Manuals directory.
  |  +--quality/                - Reports and metrics.
  |  +--various/                - Various support files.
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

*** 21.11.3 ***
- NEW: Added EFL driver support for STM32F401/411.
- FIX: Fixed old bugs in serial driver header (bug #1236).
- FIX: Fixed virtual timers lockup uder rare conditions (bug #1235).
- FIX: Fixed STM32 RTCv2 locks for a second (bug #1234).

*** 21.11.2 ***
- NEW: Added dubby cycles support for SNOR using the normal SPI driver.
- NEW: Disabled priority check on STM32 EXTI interrupts when the default
       ISR is disabled. This allows for fast interrupts.
- NEW: Added support for UART9 and USART10 on STM32H7.
- NEW: Improved MFS to use explicitly non-cacheable buffers for potentially
       DMA-accessible I/O areas.
- NEW: FatFS now functional on STM32H7xx, added a target to the VFS demo.
- NEW: Improved cache settings in STM32H7xx mcuconf.h.
- NEW: Modified SDMMCv2 to allow for uncached buffers, tested on STM32H7xx.
- NEW: Added OCTOSPIv2 driver using MDMA for STM32H7xx.
- NEW: Added demos for STM32H723ZG Nucleo144 and STM32H735ZI Discovery boards.
- NEW: Added support for STM32H723/25/33/35/A3/B3/A3Q/B3Q.
- NEW: Updated ST Cube headers for STM32H7xx.
- NEW: Improved HAL flash interface with mutual exclusion methods, improved
       EFL and SNOR drivers to use it.
- NEW: Added EFL driver implementation for STM32G4xx.
- NEW: STM32G0B1 USBv2 driver.
- NEW: USBv1 driver optimization and removal of a potential race condition
       (not demonstrated).
- NEW: Added elfAcquireBus()/eflReleaseBus() functions to EFL driver.
- NEW: Added option to copy vectors in RAM on GCC startup for ARMv6-M,
       ARMv7-M and ARMv8-M-ML.
- NEW: On STM32WBxx added a check on STM32_LSI_ENABLE required by IWDG.
- NEW: Added SPIv2 support also to STM32WB and STM32WL.
- FIX: Re-opened and fixed bug #1100.
- FIX: Fixed wrong buffers toggling in STM32 USBv1 isochronous mode (bug #1232).
- FIX: Fixed STM32 RTCv2 registers synchronization errata (bug #1231).
- FIX: Fixed STM32 ADCv1 and ADCv5 do not allow prescaler divide value of 1
       (bug #1230).
- FIX: Fixed missing check on STM32 SPIv2 DMA settings for SPI1 (bug #1229).
- FIX: Fixed ARMv6-M port Keil compiler fail (bug #1228).
- FIX: Fixed invalid handling of lwIP NETIF_FLAG_LINK_UP flag (bug #1227).
- FIX: Fixed missing TIM16/17 errata handling for STM32G0xx (bug #1226).
- FIX: Fixed missing ADC errata handling for STM32G0xx (bug #1225).
- FIX: Fixed problem in the HAL I2C fallback driver (bug #1224).
- FIX: Fixed GPIOH clock not enabled on STM32L432 (bug #1223).
- FIX: Fixed invalid cumulative time stat in RT (bug #1222).
- FIX: Fixed incorrect type cast in TIME_I2US() (bug #1221).
- FIX: Fixed missing clock disable for STM32 OCTOSPI2 (bug #1220).
- FIX: Fixed wrong condition in STM32 sio_lld_read() function (bug #1219).
- FIX: Fixed STM32 Ethernet driver causes system hang after 2^31 packets
       sent/received (bug #1218).
- FIX: Fixed clock re-initialization problem in STM32 USARTv2 and USARTv3
       drivers (bug #1217).
- FIX: Fixed assertion on initialization of STM32H7xx (bug #1216).
- FIX: Fixed Virtual Timers failure in a specific condition (bug #1215).
- FIX: Fixed invalid STM32_OTG_STEPPING for STM32F40_41xxx (bug #1214).
- FIX: Fixed SPIv2 driver compile fails when SPI_USE_SYNCHRONIZATION is FALSE
       (bug #1213).
- FIX: Fixed invalid state transition in SNOR flash driver (bug #1212).
- FIX: Fixed missing exit condition in sioSynchronizeRX() and
       sioSynchronizeTX() (bug #1211).
- FIX: Some MISRA-related fixes.
- FIX: Fixed missing check in chobjcaches.h (bug #1210).
- FIX: Fixed misspelled chTraceSuspendI() function name (bug #1209).
- FIX: Fixed RT testbuild application broken (bug #1208).

*** 21.11.1 ***
- NEW: Added EFL driver implementation for STM32G4xx.
- NEW: STM32G0B1 USBv2 driver.
- NEW: USBv1 driver optimization and removal of a potential race condition
       (not demonstrated).
- NEW: Added elfAcquireBus()/eflReleaseBus() functions to EFL driver.
- NEW: Added STM32L073RZ-Nucleo64 to USB-CDC "multi" demo. Removed old demo.
- NEW: Added an STM32 WDG "multi" demo. Removed all old WDG demos.
- NEW: Added option to copy vectors in RAM on GCC startup for ARMv6-M,
       ARMv7-M and ARMv8-M-ML.
- NEW: On STM32WBxx added a check on STM32_LSI_ENABLE required by IWDG.
- NEW: Added SPIv2 support also to STM32WB and STM32WL.
- FIX: Fixed PWR_CR2_USV not set in STM32L4+ mcuconf.h file (bug #1207).
- FIX: Fixed USB not enabled on STM32F103x6 (bug #1206).
- FIX: Fixed RT test suite build fails when CH_CFG_USE_TIMESTAMP is FALSE
       (bug #1205).
- FIX: Fixed wrong number of CAN filters for STM32L496/9A6 (bug #1204).
- FIX: Fixed DMA stream not disabled in STM32 QUADSPIv1 driver (bug #1203).
- FIX: Fixed I2C4 DMA streams for STM32L496 (bug #1202).
- FIX: Fixed STM32_SDMMC2_NUMBER on STM32H7 (bug #1201).
- FIX: Fixed STM32G0B1 demo application hangs debuggers (bug #1200).

*** 21.11.0 ***
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
