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
- NEW: Implemented tickless mode on ADuCM36x family
- NEW: STM32 ICU driver now allows to setup the ARR register in the
       configuration structure, the default value should be 0xFFFFFFFFU.
- NEW: More time conversion macros added to HAL OSAL.
- NEW: Updated debug tools to be independent from the toolchain position:
       they now rely on the environment variable CHIBISTUDIO.
- NEW: Mail Queues test implementation in CMSIS RTOS wrapper.
- NEW: Added dynamic reconfiguration API to lwIP bindings.
- FIX: Corrected I2C4 BDMA #define conditional in I2Cv3 (bug #1082)
- RT:  Relocated the "ctx" field in the thread structure in order to save
       some RAM, it caused unused space in the "ch" variable.
- EX:  Implemented cache handling in the ADXL355 device driver.
- EX:  Added support for ADXL355 Low Noise, Low Drift, Low Power, 3-Axis
       MEMS Accelerometers.
- NEW: Safer messages mechanism for sandboxes (to be backported to 20.3.1).
- NEW: Added latency measurement test application.
- FIX: Fixed STM32L0 missing LPUART IRQ initialization (bug #1099)
       (backported to 20.3.2).
- FIX: Fixed invalid EXTI definitions for STM32L0xx (bug #1098)
       (backported to 20.3.2).
- FIX: Fixed compilation error in file nvic.c (bug #1097)
       (backported to 20.3.2).
- FIX: Fixed STM32_DMAx_CH8_HANDLER not defined for DMAv1 (bug #1096)
       (backported to 20.3.2).
- FIX: Fixed STM32G4 demos compile fails if smart mode is disabled (bug #1094)
       (backported to 20.3.2).
- FIX: Fixed failure in chSemReset() function when counter is equal to MAXINT
       (bug #1093)(backported to 20.3.2)(backported to 19.1.5).
- FIX: Fixed swapped definition in ST_STM32F746G_DISCOVERY board files
       (bug #1092)(backported to 20.3.1)(backported to 19.1.5).
- FIX: Fixed missing symbols in GCC scatter files (bug #1091)
       (backported to 20.3.1).
- FIX: Fixed wrong SAI1 clock selection for STM32G4xx (bug #1090)
       (backported to 20.3.1).
- FIX: Fixed STM32H7xx ADC problem in dual mode (bug #1089)
       (backported to 20.3.1)(backported to 19.1.4).
- FIX: Fixed invalid CHSEL DMA setting in STM32 UART drivers (bug #1088)
       (backported to 20.3.1)(backported to 19.1.4).
- FIX: Fixed wrong arguments for the cacheBufferInvalidate in the STM32 SPI 
       demo (bug #1086)(backported to 20.3.1)(backported to 19.1.4).
- FIX: Fixed sector count incorrect in STM32G07/8 EFL driver (bug #1085)
       (backported to 20.3.1).
- FIX: Fixed sector size incorrect in STM32F413 EFL driver (bug #1084)
       (backported to 20.3.1).
- FIX: Fixed race condition in HAL MAC driver (bug #1083)
       (backported to 20.3.1)(backported to 19.1.4).
- FIX: Fixed STM32H7 compile fails for I2C4 (bug #1082)
       (backported to 20.3.1).
- FIX: Fixed early interrupts enable in ARMv7-M port (bug #1081)
       (backported to 20.3.1).
- FIX: Fixed I2CD4 interrupt vectors are swapped versus I2CD1-I2CD3 (bug #1080)
       (backported to 20.3.1).
- FIX: Fixed incorrect clock check when using PLLSAI1R in ADCv3 (bug #1079)
       (backported to 20.3.1).
- FIX: Fixed missing checks in TIM6 and TIM7 STM32 mini drivers (bug #1078)
       (backported to 20.3.1).
- FIX: Fixed error in EXTIv1 ISRs (bug #1077)
       (backported to 20.3.1).
- FIX: Fixed problem in chMtxUnlockAllS() (bug #1076).
       (backported to 20.3.1)(backported to 19.1.4)(backported to 18.2.3).
    
