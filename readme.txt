*****************************************************************************
*** Directories structure                                                 ***
*****************************************************************************

./readme.txt           - This file.
./license.txt          - GPL3 license file.
./src/                 - ChibiOS/RT portable kernel source files.
./src/include/         - ChibiOS/RT include files.
./src/lib/             - ChibiOS/RT library code that can be included into
                         user applications but is not part of the core system.
                         The code in this directory is meant to be portable,
                         generic and architecture independent.
./src/templates/       - ChibiOS/RT non portable source templates, new ports
                         are started by copying the templates into a new
                         directory under ./demos/ and/or ./ports/.
./ports/               - Architecture specific portable files.
./demos/               - Demo programs for specific architectures/boards.
./ext/                 - External libraries or other code not part of
                         ChibiOS/RT but used in the demo applications.
./test/                - Test code, used by some demos.
./docs/src             - Documentation sources.
./docs/rsc             - Documentation resources.
./docs/Doxyfile        - Doxygen project file.
./docs/index.html      - ChibiOS/RT documentation.
                         The documentation is also available on the project
                         web page: http://chibios.sourceforge.net/
./docs/reports         - Test reports on the various targets.

*****************************************************************************
*** Current ports/demos                                                   ***
*****************************************************************************
Current ports under ./demos:

ARM7-LPC214x-GCC       - ChibiOS/RT port for ARM7 LPC2148, the demo targets
                         the Olimex LPC-P2148 board. This port can be easily
                         modified for any processor into the LPC2000 family or
                         other boards. The demo can be compiled using YAGARTO
                         or any other GCC-based ARM toolchain. Full demo.
ARM7-LPC214x-GCC-min   - Minimal demo for LPC214X.
ARM7-LPC214x-G++       - Yet another LPC214X demo but this one is done using
                         G++ in order to provide a C++ template project to the
                         ChibiOS/RT users.
ARM7-AT91SAM7X-GCC     - Port for Atmel AT91SAM7X256. The demo program targets
                         the Olimex SAM7-EX256 board.
ARM7-AT91SAM7X-WEB-GCC - Port for Atmel AT91SAM7X256. The web demo program
                         targets the Olimex SAM7-EX256 board.
ARMCM3-ST32F103-GCC    - ARM Cortex-M3 port. The demo targets the Olimex
                         STM32-P103 board.
AVR-AVRmega128-GCC     - Port on AVRmega128. A special thanks to Vladimir for
                         the work done on the AVR port. The demo program
                         targets the Olimex AVR-MT-128 mini terminal board.
AVR-AT90CANx-GCC       - Port on AVR AT90CAN128, not tested on hardware yet. It
                         ran correctly in the simulator.
MSP430-MSP430x1611-GCC - Port on Texas Instruments MSP430F1611, the demo
                         targets the Olimex MSP430-P1611 board.
Win32-MinGW            - ChibiOS/RT simulator and demo into a WIN32 process,
                         MinGW version.

*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

*** 1.2.1 ***
- FIX: Fixed regression in MinGW demo (bug 2745153).
- FIX: Fixed problem with the timeout constant TIME_IMMEDIATE (bug 2755170).
- FIX: Fixed a problem in semaphores test case #2 (bug  2755195).
- FIX: Removed usused list functions (bug 2755230).
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
