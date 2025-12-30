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
- NEW: Added EFL driver for simulator and a simulator MFS test application.
- NEW: Added simulator support for x84-64 on Posix/Linux.
- NEW: Added CHN_TRANSMISSION_END event to hal_serial_usb.
- NEW: Added XSHELL_EXIT_HOOK to xshell.
- NEW: Added STM32U0xx support.
- NEW: Flash-level mutual exclusion option added to MFS. Added the ability to
       include a mfsconf.h file if MFS_USE_MFSCONF is defined at project
       level.
- NEW: Memory areas functions in OSLIB addressed for portability.
- NEW: Implemented better chThdSleepUntil() in NIL using the same logic used
       in the RT implementation.
- NEW: Function chSftIntegrityCheckI() rewritten to be much more efficient in
       performing lists integrity checks.
- NEW: Added to RT a dedicated functional safety module, the function
       chSysIntegrityCheckI() has been moved in this module and renamed to
       chSftIntegrityCheckI().
- NEW: I2Cv4 support added to STM32C0xx, STM32G0xx, STM32G4xx, STM32L4xx+,
       STM32U3xx families. This driver only requires one DMA channel vs two
       of the old implementations.
- NEW: Added STM32U3xx support.
- NEW: Added multicore memory classes modifiers to RT in order to support
       NUMA architectures and non-coherent cache architectures.
- NEW: Added MPU initialization settings in ARMv7-M, ARv7-M-ALT, ARMv8-ML-ALT
       ports. Settings allow to statically initialize any region.
- NEW: Improved interrupts processing in ARMv7-M-ALT and ARMv8-M-ML-ALT
       ports. Specifically, saved few cycles on the context switch code path.
- NEW: Added missing context switch hook in ARMv7-M-ALT and ARMv8-M-ML-ALT
       ports.
- NEW: Added chRegGarbageCollect() function to registry for simplified
       dynamic threads management.
- NEW: Added integration demos for VFS+LittleFS/FatFS+XSHELL, now also available on STM32U0 Nucleo-64.
- NEW: Added faster context switch modes to ARMv7-M and ARMv8-M ports. The
       new modes allow to avoid saving FP context for threads that do not
       use the FPU.
- NEW: Added new ARMv8-M port aligned with features of then ARMv7-M port for
       sandboxing.
- NEW: Added XShell support for change prompt, multi-command per line, line 
       editing, user definable extra fields, init and execution hooks.
- NEW: Improved LFS support, now it is possible to make an LFS file system
       at arbitrary positions in flash.
- NEW: New XShell specific for the new RT, it leverages the new thread
       dispose feature.
- NEW: New RT threads spawning API decoupling the thread stack from the
       thread_t structure as required in NUMA multicore devices. Old
       "create" API is still present and supported, long term it could be
       removed.
- NEW: Added capability to associate a "dispose" functions to threads,
       dynamic API modified to use this mechanism.
- NEW: Recursive locks in RT and NIL made optional, only enabled if the
       underlying port supports the capability.
- NEW: Now RT virtual timers can recalculate the value of CH_CFG_ST_TIMEDELTA
       at runtime and continue using the recalculated value. This has two
       consequences: 1) The value is recalculated once 2) it is possible
       to use the new API chVTGetCurrentDelta() and update the static
       setting in order to avoid recalculation.
- NEW: OSLIB release methods now return the value of the reference counter.
- NEW: Added a "waend" field to the thread structure in RT for debug
       convenience.
- NEW: Added a para-virtualized XHAL port for use in sandboxes.
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
- NEW: Added a new MEM_IS_VALID_FUNCTION() macro to RT and NIL.
- NEW: Changed SB configuration options names to be prefixed with SB_CFG_.
- NEW: Added a new CH_CFG_HARDENING_LEVEL option to RT.
- NEW: Added a chXXXDispose() function to all objects in NIL.
- NEW: Added a chXXXDispose() function to all objects in RT.
- NEW: Added MEM_NATURAL_ALIGN macro to RT and NIL.
- NEW: Added static initializer for virtual timers in RT.
- NEW: Added new function chHeapIntegrityCheck().
- NEW: Function chCoreGetStatusX() changed to return a memory region object
       instead of a simple size.
- NEW: RT and NIL upgraded to support the enhanced OSLIB.
- NEW: Memory areas/pointers checker functions added to OSLIB.
