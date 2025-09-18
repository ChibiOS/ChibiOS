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
  |  |  +--oop/                 - Object-oriented framework.
  |  |  +--ports/               - RTOS ports usable by both RT and NIL.
  |  |  +--startup/             - Startup support.
  |  |  +--utils/               - Generic and portable C modules.
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
  |  |  |  |  +--serial_nor/    - HAL legacy SNOR stack.
  |  |  |  |  +--xsnor/         - HAL improved SNOR stack.
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
- NEW: Added multicore memory classes modifiers to RT in order to support
       NUMA architectures and non-coherent cache architectures.
- NEW: Added MPU initialization settings in ARMv7-M, ARv7-M-ALT, ARMv8-ML-ALT
       ports. Settings allow to statically initialize any region.
- NEW: Improved interrupts processing in ARMv7-M-ALT and ARMv8-M-ML-ALT
       ports. Specifically, saved few cycles on the context switch code path.
- NEW: Added missing context switch hook in ARMv7-M-ALT and ARMv8-M-ML-ALT
       ports.
- NEW: I2C slave support in ST low level drivers.
- NEW: Support for STM32G0B0xx.
- NEW: Added chRegGarbageCollect() function to registry for simplified
       dynamic threads management.
- NEW: Added integration demos for VFS+LittleFS/FatFS+XSHELL.
- NEW: Added faster context switch modes to ARMv7-M and ARMv8-M ports. The
       new modes allow to avoid saving FP context for threads that do not
       use the FPU.
- NEW: Added new ARMv8-M port aligned with features of then ARMv7-M port for
       sandboxing.
- NEW: Added XShell support for change prompt, multi-command per line, line 
       editing, user definable extra fields, init and execution hooks.
- NEW: Improved LFS support, now it is possible to make an LFS file system
       at arbitrary positions in flash.
- NEW: New XSNOR framework replacing the old serial_nor. It allows multiple
       device types on multiple interfaces and devices auto-detection.
       STM32-WSPI-MFS demo modified to showcase the new capabilities.
       The old serial_nor is still available but no more developed.
- NEW: New XShell specific for the new RT, it leverages the new thread
       dispose feature.
- NEW: New RT threads spawning API decoupling the thread stack from the
       thread_t structure as required in NUMA multicore devices. Old
       "create" API is still present and supported, long term it could be
       removed.
- NEW: Added capability to associate a "dispose" functions to threads,
       dynamic API modified to use this mechanism.
- NEW: ADCv4 now can use regular DMA also for ADC3 unit.
- NEW: Recursive locks in RT and NIL made optional, only enabled if the
       underlying port supports the capability.
- NEW: Added STM32 FDCANv2 for STM32H7xx
- NEW: Now RT virtual timers can recalculate the value of CH_CFG_ST_TIMEDELTA
       at runtime and continue using the recalculated value. This has two
       consequences: 1) The value is recalculated once 2) it is possible
       to use the new API chVTGetCurrentDelta() and update the static
       setting in order to avoid recalculation.
- NEW: OSLIB release methods now return the value of the reference counter.
- NEW: Support for STM32C0xx.
- NEW: Improved DAC driver, updated STM32 DACv1.
- NEW: STM32 RTCv2 and RTCv3 modified to not use shadow registers.
- NEW: Enhanced STM32F7xx MPU configuration in mcuconf.h.
- NEW: I2C slave support in HAL high level driver.
- NEW: Added settings for STM32 OCTOSPIv1 and OCTOSPIv2 TCR bits SSHIFT and
       DHQC.
- NEW: Automatic removal of duplicated inclusion paths on make command lines.
- NEW: Reworked STM32 SDMMCv1 and SDMMCv2 drivers, better timeout and clock
       handling, improved speed for aligned buffers.
- NEW: Added a "waend" field to the thread structure in RT for debug
       convenience.
- NEW: Added a para-virtualized HAL port for use in sandboxes.
- NEW: Added a VIO subsystem to sandboxes supporting drivers
       para-virtualization, PAL and SIO supported so far.
- NEW: Added and RT port for use in virtualized sandboxes.
- NEW: Added full virtualization support to sandboxes with a virtual IRQ
       mechanism.
- NEW: Added __CH_OWNEROF() macro to RT.
- NEW: Added a Posix-favored shell named "msh" (Mini Shell). The shell is able
       to run sub-apps inside the same sandbox. The shell can either be placed
       statically in flash or loaded dynamically in RAM.
- NEW: Added runnable "apps" capability to SBs, apps available so far: msh, ls.
- NEW: Added ability to load ELF files to SBs.
- NEW: Enhanced Posix API for SBs leveraging the VFS integration.
- NEW: SBs and VFS integration. Each SB can see its own VFS instance.
- NEW: Added integration of LittleFS on top of our flash infrastructure.
- NEW: Added a new MEM_IS_VALID_FUNCTION() macro to RT and NIL.
- NEW: Changed SB configuration options names to be prefixed with SB_CFG_.
- NEW: Added a new CH_CFG_HARDENING_LEVEL option to RT.
- NEW: Added a chXXXDispose() function to all objects in NIL.
- NEW: Added a chXXXDispose() function to all objects in RT.
- NEW: Added VFS-related commands to the shell, disabled by default.
- NEW: Added a new VFS subsystem (Virtual File System), it allows to assemble
       trees of files from multiple "File System Drivers" into a single tree
       and access it as a whole.
- NEW: Added MEM_NATURAL_ALIGN macro to RT and NIL.
- NEW: Added static initializer for virtual timers in RT.
- NEW: Added new function chHeapIntegrityCheck().
- NEW: Function chCoreGetStatusX() changed to return a memory region object
       instead of a simple size.
- NEW: RT and NIL upgraded to support the enhanced OSLIB.
- NEW: Memory areas/pointers checker functions added to OSLIB.
- FIX: Set DAC_HAS_MCR FALSE in STM32F1xx registry (backported to 21.11.4).
- FIX: Fixed wrong IRQ vector for STM32L4+ DCMI.
- FIX: Fixed ADCv4 common registers reset at start (bug 1296).
- FIX: Fixed incorrect RCC clock selection defines for STM32G0 USB.
- FIX: Wrong .ld used in STM32G071 ADC testhal project.
- FIX: Fixed ADCv5 wait for CCRDY after setting CHSELR.
- FIX: Fixed DMA2 not firing on STM32G431 (bug 1295)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed wrong STM32 ADCv2 stop method (bug 1294)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed STM32 OTGv1 driver does not re-enables endpoints on wakeup
       (bug 1293)(backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed missing assertion in OSLIB factory module (bug 1292)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed problem in recursive locks functions (bug #1288)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed ARMv8-M-ML port compile fail when FPU is enabled (bug #1281)
       (backported to 21.11.4).
- FIX: Fixed interrupts not enabled for STM32H735 TIM15, TIM16 and TIM17
       (bug #1280)(backported to 21.11.4).
- FIX: Fixed wrong STM32 LSI activation check (bug #1279)
       (backported to 21.11.4).
- FIX: Fixed STM32 HAL UART ISR flaw (bug #1278)(backported to 21.11.4).
- FIX: Fixed race condition caused by chGuardedPoolAllocI() (bug #1277)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed avoid shadowing with build-in pow10 function in chprintf.c
       (bug #1274)(backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed enabling PWM on TIM1, 3, 4 causes compile errors in
       RT-STM32G0B1RE-NUCLEO64 (bug #1273)(backported to 21.11.4).
- FIX: Wrong assertion in STM32 SPIv3 on SPI6 start.
- FIX: Fixed problems related to TIM3, TIM4 and TIM16 on STM32G0.
- FIX: Fixed uninitialized return message in EX subsystem (bug #1267)
       (backported to 21.11.4).
- FIX: Fixed unnecessary code in SNOR device drivers (bug #1265)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed RP2040 HAL GPIO failed to compile (bug #1264)
       (backported to 21.11.4).
- FIX: Fixed channel 0 corruption on STM32 BDMAv1 (bug #1263)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed wrong statistics in RT7 (bug #1262)(backported to 21.11.4).
- FIX: Fixed missing cache management during Cortex-M RAM initializations
       (bug #1261)(backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed RTC & TAMP interrupts not functional (bug #1260)
       (backported to 21.11.4).
- FIX: Fixed syntax errors in STM32H7xx/hal_lld_type2.h (bug #1259)
       (backported to 21.11.4).
- FIX: Fixed unwanted reset of cache on STM32H7xx (bug #1258)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed invalid HSIDIV in STM32Ggxx clocks initialization (bug #1257)
       (backported to 21.11.4).
- FIX: Fixed incorrect RTC initialization on STM32G4/L4/L4+ (bug #1256)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed syntax error in RP2040 GPIO driver (bug #1255)
       (backported to 21.11.4).
- FIX: Fixed undefined STM32_SDMMC_MAXCLK value for STM32H7 type 1 and 2
       (bug #1254)(backported to 21.11.4).
- FIX: Fixed invalid checks on PLLP/R/Q dividers on STM32H7 (bug #1253)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed remote wakeup failure in STM32 OTGv1 driver (bug #1252)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed wrong use of hooks in RT/NIL (bug #1251)
       (backported to 20.3.5)(backported to 21.11.4).
- FIX: Fixed SPI_MMC driver broken in 21.11.3 (bug #1249)
       (backported to 21.11.3).
- FIX: Fixed broken support for STM32 UART9 and USART10 (bug #1248)
       (backported to 21.11.3).
- FIX: Fixed wrong initialization in STM32L1xx ADC driver (bug #1247)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed wrong HSI48 support on STM32L0xx (bug #1246)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed wrong DMA definitions for STM32L0xx I2C3 peripheral (bug #1245)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed wrong path in STM32L053 ADC demo makefile (bug #1244)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed missing semicolon in STM32 OTGv1 driver (bug #1243)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed HSI48 not enabled for STM32L496/4A6 (bug #1242)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed problem in STM32 gpt_lld_polled_delay() implementation (bug #1241)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed invalid delay loop in STM32G0/WL ADCv5 driver (bug #1240)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed STM32_MCOSEL setting problem (bug #1239).
- FIX: Fixed problems with cache in STM32 SDMMC drivers (bug #1238).
- FIX: Fixed missing clock enables for some GPIOS on some STM32L4s (bug #1237)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed old bugs in serial driver header (bug #1236)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed virtual timers lockup uder rare conditions (bug #1235)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed STM32 RTCv2 locks for a second (bug #1234)
       (backported to 20.3.5)(backported to 21.11.3).
- FIX: Fixed CAN support broken on STM32F413 (bug #1232).
- FIX: Re-opened and fixed bug #1100
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed wrong buffers toggling in STM32 USBv1 isochronous mode (bug #1232)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed STM32 RTCv2 registers synchronization errata (bug #1231)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed STM32 ADCv1 and ADCv5 do not allow prescaler divide value of 1
       (bug #1230)(backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed missing check on STM32 SPIv2 DMA settings for SPI1 (bug #1229)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed ARMv6-M port Keil compiler fail (bug #1228)
       (backported to 21.11.2).
- FIX: Fixed invalid handling of lwIP NETIF_FLAG_LINK_UP flag (bug #1227)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed missing TIM16/17 errata handling for STM32G0xx (bug #1226)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed missing ADC errata handling for STM32G0xx (bug #1225)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed problem in the HAL I2C fallback driver (bug #1224)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed GPIOH clock not enabled on STM32L432 (bug #1223)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed invalid cumulative time stat in RT (bug #1222)
       (backported to 21.11.2).
- FIX: Fixed incorrect type cast in TIME_I2US() (bug #1221)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed missing clock disable for STM32 OCTOSPI2 (bug #1220)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed wrong condition in STM32 sio_lld_read() function (bug #1219)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed STM32 Ethernet driver causes system hang after 2^31 packets
       sent/received (bug #1218)(backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed clock re-initialization problem in STM32 USARTv2 and USARTv3
       drivers (bug #1217)(backported to 21.11.2).
- FIX: Fixed assertion on initialization of STM32H7xx (bug #1216)
       (backported to 21.11.2).
- FIX: Fixed Virtual Timers failure in a specific condition (bug #1215)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed invalid STM32_OTG_STEPPING for STM32F40_41xxx (bug #1214)
       (backported to 21.11.2).
- FIX: Fixed SPIv2 driver compile fails when SPI_USE_SYNCHRONIZATION is FALSE
       (bug #1213)(backported to 21.11.2).
- FIX: Fixed invalid state transition in SNOR flash driver (bug #1212)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed missing exit condition in sioSynchronizeRX() and
       sioSynchronizeTX() (bug #1211)(backported to 21.11.2).
- FIX: Fixed missing check in chobjcaches.h (bug #1210)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed misspelled chTraceSuspendI() function name (bug #1209)
       (backported to 21.11.2).
- FIX: Fixed RT testbuild application broken (bug #1208)
       (backported to 20.3.5)(backported to 21.11.2).
- FIX: Fixed PWR_CR2_USV not set in STM32L4+ mcuconf.h file (bug #1207)
       (backported to 21.11.1).
- FIX: Fixed USB not enabled on STM32F103x6 (bug #1206)
       (backported to 20.3.5)(backported to 21.11.1).
- FIX: Fixed RT test suite build fails when CH_CFG_USE_TIMESTAMP is FALSE
       (bug #1205)(backported to 21.11.1).
- FIX: Fixed wrong number of CAN filters for STM32L496/9A6 (bug #1204)
       (backported to 20.3.5)(backported to 21.11.1).
- FIX: Fixed DMA stream not disabled in STM32 QUADSPIv1 driver (bug #1203)
       (backported to 20.3.5)(backported to 21.11.1).
- FIX: Fixed I2C4 DMA streams for STM32L496 (bug #1202)
       (backported to 20.3.5)(backported to 21.11.1).
- FIX: Fixed STM32_SDMMC2_NUMBER on STM32H7 (bug #1201)
       (backported to 20.3.5)(backported to 21.11.1).
- FIX: Fixed STM32G0B1 demo application hangs debuggers (bug #1200)
       (backported to 21.11.1).
