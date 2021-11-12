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

*** 21.11.1 ***
- NEW: Added option to copy vectors in RAM on GCC startup for ARMv6-M,
       ARMv7-M and ARMv8-M-ML.
- NEW: On STM32WBxx added a check on STM32_LSI_ENABLE required by IWDG.
- NEW: Added SPIv2 support also to STM32WB and STM32WL.
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
