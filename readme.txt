*****************************************************************************
*** Files Organization                                                    ***
*****************************************************************************

--{root}                - ChibiOS/RT directory.
  +--readme.txt         - This file.
  +--todo.txt           - Current plan (development/unstable versions only).
  +--license.txt        - GPL license text.
  +--exception.txt      - GPL exception text (stable releases only).
  +--boards/            - Board support files.
  +--demos/             - Demo projects.
  +--docs/              - Documentation.
  |  +--html/           - HTML documentation.
  |  +--reports/        - Test reports.
  |  +--src/            - Documentation source files (required for rebuild).
  |  +--rsc/            - Documentation resource files (required for rebuild).
  |  +--index.html      - Documentation access.
  +--ext/               - External libraries, not part of ChibiOS/RT.
  +--os/                - ChibiOS/RT files.
  |  +--hal/            - Hardware Abstraction Layer.
  |  |  +--include/     - HAL high level headers.
  |  |  +--src/         - HAL high level source.
  |  |  +--platforms/   - HAL low level drivers implementations.
  |  |  |  +--AT91SAM7/ - Drivers for AT91SAM7 platform.
  |  |  |  +--AVR/      - Drivers for AVR platform.
  |  |  |  +--LPC214x/  - Drivers for LPC214x platform.
  |  |  |  +--MSP430/   - Drivers for MSP430 platform.
  |  |  |  +--STM32/    - Drivers for STM32 platform.
  |  |  |  +--Linux/    - Drivers for x86 Linux simulator platform.
  |  |  |  +--LPC214x/  - Drivers for x86 Win32 simulator platform.
  |  |  +--templates/   - Driver template files.
  |  |     +--meta/     - Driver meta templates.
  |  +--ports/          - Port files for the various architectures.
  |  |  +--GCC/         - Ports for the GCC compiler.
  |  |     +--ARM/      - Port files for generic ARM architecture.
  |  |     +--ARM7/     - Port files for ARM7 architecture.
  |  |     +--ARMCM3/   - Port files for ARMCM3 architecture.
  |  |     +--AVR/      - Port files for AVR architecture.
  |  |     +--MSP430/   - Port files for MSP430 architecture.
  |  |     +--SIMIA32/  - Port files for SIMIA32 simulator architecture.
  |  +--kernel/         - Kernel portable files.
  |  |  +--include/     - Kernel headers.
  |  |  +--src/         - Kernel source.
  |  |  +--templates/   - Kernel port template files.
  |  +--various/        - Various portable support files.
  +--test/              - Kernel test suite source code.
  |  +--coverage/       - Code coverage project.
  +--testhal/           - HAL integration test demos.
 
*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

*** 1.4.0 ***
- Full test cycle and test reports updated.
- NEW: Reorganized and rationalized the distribution tree and the
  documentation.
- NEW: Abstract Streams and I/O Channels mechanisms introduced.
- NEW: Added a new core memory manager.
- NEW: Improved Heap and Pools allocators.
- NEW: The I/O queues code has been improved, now there are 2 separate
  structures: InputQueue and OutputQueue.
- NEW: Added timeout specification to the I/O queues read/write primitives.
- NEW: Static initializers macros introduced for most kernel objects.
- NEW: Added new APIs chSchDoYieldS() and chThdYield().
- NEW: Improved and simplified kernel configuration files.
- MEW: Added new benchmarks and test cases.
- NEW: Added more test cases in order to improve the test suite code coverage
  (it was 74% in version 1.2.0, it is now close to 100%).
- NEW: Added a code coverage analysis application under ./tests/coverage.
- NEW: Added the test suite documentation to the general documentation.
- NEW: Linux x86 simulator demo added.
- NEW: Improved the Cortex-M3 preemption code.
- NEW: Added standard CMSIS 1.2.0 support to the Cortex-M3 port.
- NEW: Added support for the ST firmware library to the STM32 port.
- NEW: Added support for HD and CL STM32 devices.
- NEW: Improvements to the AT91SAM7 support.
- NEW: Improved makefiles and makefile fragments, now the paths are not fixed.
- NEW: Unified the initialization of the various drivers from a single HAL
  driver. The single drivers can be enabled or disabled from a HAL
  configuration file halconf.h.
- NEW: Hardware Abstraction Layer (HAL) with support for PAL, ADC, CAN, MAC,
  MMC/SD, PWM, Serial, SPI drivers. Added driver implementations to the
  various platforms.
- NEW: Added support for uIP, lwIP, FatFS external libraries, added demos.
- Many Many improvements and minor features.

*** 1.2.4 ***
- FIX: Fixed GCC 4.4.x aliasing warnings (bug 2846336).
- FIX: Modified linker scripts for GCC 4.4.x (bug 2846302).
- FIX: Fixed the CH_OPTIMIZE_SPEED option in the CM3 port (bug 2846278).
- FIX: Fixed GCC 4.4.x related problems in CM3 port (bug 2846162).
- FIX: Fixed LPC214x UART problem (bug 2841088).

*** 1.2.3 ***
- FIX: Fixed C99-style variable declarations (bug 2792919).
- FIX: Fixed instance of obsolete CH_USE_TERMINATE option in the C++ wrapper
  (bug 2796065).
- FIX: Insufficient stack allocated to the C++ LPC2148 demo (bug 2796069).
- FIX: Fixed errors in events test case (bug 2796081).
- CHANGE: Increased main stack size to 1KiB for all the ARMx demos, 2KiB for
  the C++ LPC2148 demo. This should make things easier for unexperienced
  users.

*** 1.2.2 ***
- FIX: Fixed macro in test.h (bug 2781176).
- FIX: Fixed @file tag in sam7x_serial.c (bug 2788573).
- FIX: Fixed sequence assertion in test.c (bug 2789377).
- FIX: Fixed test_cpu_pulse() incorrect behavior (bug 2789383).
- FIX: Fixed missing volatile modifier for p_time field in Thread structure
  (bug 2789501).
- CHANGE: Made the option CH_DBG_THREADS_PROFILING default to TRUE because it
  is now required in order to execute the whole test suite. Note that this
  option is very light so there is no real overhead in the system.
- Added a (harmless) workaround to the Cortex-M3 startup file in order to
  make the RIDE7 demo compile on an unmodified distribution.

*** 1.2.1 ***
- FIX: Fixed regression in MinGW demo (bug 2745153).
- FIX: Fixed problem with the timeout constant TIME_IMMEDIATE (bug 2755170).
- FIX: Fixed a problem in semaphores test case #2 (bug  2755195).
- FIX: Removed unused list functions (bug 2755230).
- FIX: Added the exception notes into the source headers (bug 2772129).
- FIX: Added license notice to several files (bug 2772160).
- FIX: Found new instances of the obsolete function chSysGetTime() in the
  C++ wrapper and in the WEB demo (bug 2772237).

*** 1.2.0 ***
- Full test cycle and test reports updated.
- NEW: Better separation between the port code and the system APIs, now an
  architecture-specific "driver" contains all the port related code.
  Port functions/macros are no more directly exposed as APIs to the user code.
- NEW: Added a configuration option to enable nested system locks/unlocks.
- NEW: Improved the interrupt handlers related code. Now interrupts are
  handled in a very similar way in every architecture. See the "Concepts"
  section and the "Writing interrupt handlers under ChibiOS/RT" article in the
  documentation.
- NEW: Added the chEvtSignal() and chEvtSignalI() APIs that allows direct
  thread signaling, much more efficient that chEvtBroadcast() when the target
  is a known single thread.
- NEW: Added a configuration option that enables the priority enqueuing on
  semaphores. It is defaulted to off because usually semaphores are used for
  I/O related tasks without hard realtime requirements.
- NEW: Now the all the options in chconf.h and the various driver headers
  can be overridden externally, as example from within the Makefile.
  The options are no mode a simple define but a define with an assigned
  TRUE/FALSE value within an #ifndef block.
- NEW: Idle thread hook macro added to the configuration file.
- NEW: Changed the ARM7 and Cortex-M3 startup files, now the action when
  the main() function returns can be overridden by redefining the symbol
  MainExitHandler.
- NEW: Mailboxes (asynchronous messages) subsystem and test cases added.
- NEW: Most APIs with a timeout specification now accept the constant
  TIME_IMMEDIATE (-1) that triggers an immediate timeout when trying to enter
  a sleep state.
- NEW: Mode flexible debug configuration options, removed the old CH_USE_DEBUG
  and CH_USE_TRACE. Replaced with CH_DBG_ENABLE_CHECKS, SCH_DBG_ENABLE_ASSERTS,
  CH_DBG_ENABLE_TRACE  and CH_DBG_FILL_THREADS.
- NEW: Added a debug option CH_DBG_THREADS_PROFILING for threads profiling.
  A field into the Thread structure counts the consumed time. The information
  is not used into the kernel, it is meant for debugging.
- NEW: Added a debug option CH_DBG_ENABLE_STACK_CHECK for stack overflow
  checking. The check is not performed in the kernel but in the port code.
  Currently only the ARM7 and ARMCM3 ports implements it.
- NEW: Unified makefiles for ARM7, ARMCM3 MSP430 projects, the new makefiles
  share a common part making them easier to maintain. Also reorganized the
  demo-specific part of the makefile, now it is easier to configure and the
  option can be overridden from outside.
- OPT: Improved ARM7 thumb port code, thanks to some GCC tricks involving
  registers usage now the kernel is much smaller, faster and most OS APIs
  use less RAM in stack frames (note, this is an ARM7 thumb mode specific
  optimization).
- OPT: Small optimization to the Cortex-M3 thread startup code, improved thread
  related performance scores and smaller code.
- OPT: Alternative, non-inlined and more compact, implementations for
  port_lock() and port_unlock() in the Cortex-M3 port when CH_OPTIMIZE_SPEED
  is FALSE.
- OPT: Improved ready list and priority ordered lists code, some space saved,
  better context switch performance.
- CHANGE: Now the API chThdSetPriority() returns the old priority instead
  of void.
- CHANGE: Modified the signature of the chMsgSendWithEvent() API, it now uses
  a more efficient event signaling method.
- CHANGE: Removed the field p_tid from the Thread structure and the related
  code, this improved the thread creation scores (~2%) and saves some RAM.
  The trace buffer field cse_tid is now populated with a simple hash of the
  thread pointer as thread identifier.
- CHANGE: Renamed the macros chSysIRQEnter() and chSysIRQExit() in
  CH_IRQ_PROLOGUE() and CH_IRQ_EPILOGUE() in order to make very clear that
  those are not functions but inlined code. Also introduced a new macro
  CH_IRQ_HANDLER that should be used when declaring an interrupt handler.
- CHANGE: Renamed several internal initialization functions by removing the
  "ch" prefix because could not be considered system APIs.
- CHANGE: Changed the chSemFastWaitS() macro in chSemFastWaitI() and
  chSemGetCounter() in chSemGetCounterI().
- Improved ARM7 and Cortex-M3 support, new configuration options.
- Introduced the concept of interrupt classes, see the documentation.
- Introduced the concept of system states, see the documentation.
- Huge improvements to the documentation.
- Articles and notes previously in the wiki now merged in the general
  documentation and updated, the wiki entries are obsolete and will be removed.
- New application notes and articles added.
- Added kernel size metrics to the test reports.
- Removed the inclusion graph from the documentation because the little
  info they add and the size of all the images. It is possible to configure
  Doxygen to have them again (and more graph types).
- Improvements to the test suite, added a new level of indirection that allows
  to make tests depend on the configuration options without have to put #ifs
  into the test main module. New benchmarks about semaphores and mutexes.
- Modified the test thread function to return the global test result flag.
- Removed testcond.c|h and moved the test cases into testmtx.c. Mutexes and
  condvars have to be tested together.
- Added architecture diagram to the documentation.

*** 1.0.2 ***
- FIX: Fixed priority inheritance problem with condvars (bug 2674756).
- FIX: Fixed a problem in time ranges (bug 2680425).
- Replaced ./docs/index.html with a direct shortcut to the documentation.

*** 1.0.1 ***
- NEW: Added to the STM32 demo makefile an option to build ChibiOS/RT with the
  full STM32 FWLib 2.03.
  Note that, except for the compile option, the library is not used by the
  OS nor supported.
- FIX: Fixed a problem into the STACK_ALIGN() macro.
- FIX: Fixed a problem with a wrong declaration of the PLL structure in the
  file lpc214x.h.
- FIX: Modified the default value for the STM32 HSI setup it was 1, it should
  be 0x10.
- FIX: Removed an obsolete constant (P_SUSPENDED) from thread.h.
- FIX: Removed unused field mp_grow in the MemoryPool structure.
- FIX: Fixed wrong assertions in chThdWait() and chHeapFree().
- FIX: Fixed a problem with some event APIs not showing in the documentation.

*** 1.0.0 ***
- License switch, added GPL exception, see exception.txt.
- Full test cycle and test reports updated.
- Renamed some occurrences of "Conditional Variable" in "Condition Variable" in
  the documentation.
- FIX: Fixed some images in the documentation because problems when seen in
  Internet Explorer.
