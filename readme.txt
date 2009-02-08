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
./docs/Doxyfile        - Doxygen project file.
./docs/html/index.html - ChibiOS/RT documentation.
                         The documentation is also available on the project
                         web page: http://chibios.sourceforge.net/

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
*** Plans                                                                 ***
*****************************************************************************

- NEW: Added to the STM32 demo makefile an option to build ChibiOS/RT with the
  full STM32 FWLib 2.03.
  Note that, except for the compile option, the library is not used by the
  OS nor supported.
- Evaluate other architectures for a possible ChibiOS/RT port. An important
  selection parameter will be the availability of FOSS toolchains. Currently
  we are evaluating the MicroBlaze.
- Creation of a reduced ChibiOS/RT kernel targeted to lesser 8bit micros and
  educational purposes, the name will probably be ChibiOS/SX, we are still
  discussing it.
- File System implementation as side project. Basic requirements: Thread safe,
  concurrent accesses, proper buffers cache.

*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

*** 1.0.1 ***
- FIX: Modified the default value for the STM32 HSI setup it was 1, it should
  be 0x10.
- FIX: Removed an obsolete constant (P_SUSPENDED) from thread.h.
- FIX: Removed unused field mp_grow in the MemoryPool structure.

*** 1.0.0 ***
- License switch, added GPL exception, see exception.txt.
- Full test cycle and test reports updated.
- Renamed some occurrences of "Conditional Variable" in "Condition Variable" in
  the documentation.
- FIX: Fixed some images in the documentation because problems when seen in
  Internet Explorer.

*** 1.0.0rc3 ***
- FIX: Fixed a nasty regression to the timeout unified code that affected
  some APIs since version 0.5.3. See the bug tracker for more details.
  Added a test case about this.
- FIX: Removed the API chThdSuspend() there was a conceptual flaw and I want
  to think about the whole concept again before introducing something similar
  in future. Anyway, it is possible to replicate the functionality using
  chSchGoSleepS().
- Fixed typos here and there.
- Updated the states diagram in the documentation.

*** 1.0.0rc2 ***
- FIX: Removed unused variable "retaddr" from the Cortex-M3 port.
- FIX: The macro THD_WA_SIZE was defined wrongly in the file
  ./src/templates/chcore.h.
- Fixed some errors in the documentation.

*** 1.0.0rc1 ***
- NEW: Added new macros CH_KERNEL_VERSION set to "1.0.0rc1", CH_KERNEL_MAJOR
  set to 1, CH_KERNEL_MINOR set to 0, CH_KERNEL_PATCH set to 0.
  The macros will be updated to reflect the actual kernel version number.
- NEW: Made all the port-specific configuration settings externally
  configurable, see the documentation.
- FIX: Disabled the configuration option CH_USE_MESSAGES_PRIORITY from the
  MSP430 demo, the default for this option should be off.
- FIX: Fixed a bug that prevented the THREAD_EXT_FIELDS to be compiled into
  the Thread structure.
- FIX: Removed some references to deprecated APIs from the test suite.
- FIX: Set the INT_REQUIRED_STACK configuration value for the ARM7 port to a
  safer 0x10, it was previously 0 (correct but trimmed to specific compiler
  settings).
- FIX: Set the INT_REQUIRED_STACK configuration value for the AVR port to a
  safer 32.
- FIX: Fixed the MinGW demo in order to not use any deprecated construct.
- Removed deprecated threads APIs: chThdCreate() and chThdCreateFast().
- Removed deprecated events APIs: chEvtWait(), chEvtWaitTimeout(), chEvtSend(),
  chEvtSendI(), EventMask().
- Removed deprecated configuration feature CH_USE_EXIT_EVENT and the related
  API chThdGetExitEventSource(). The feature can be reimplemented in user code
  as shown by the MinGW demo.
- Removed deprecated macros: WorkingArea(), UserStackSize(), StackAlign().
- Added usage note into the readme of the MinGW demo.
- Added usage notes for programmers to the ARM7 port documentation about
  interrupt handlers.
- Port-specific settings added to the documentation.
- Added source browser to the documentation.
- Fixes and improvements through the documentation.

*** 0.8.3 ***
- NEW: Introduced new API chThdSleepS() as a macro, no real changes in the
  kernel code.
- FIX: The MinGW simulated demo now works again after breaking in 0.8.2
  because the changes to the macro names.
- FIX: Adjusted the test suite stack sizes for the MinGW simulated demo, now
  the demo passes all the tests.
- FIX: Added fflush(stdout) to the MinGW simulation code output in order to
  make the Eclipse console work correctly.
- FIX: Renamed the MinGW demo main source from demo.c to main.c in order to
  follow the pattern of all the other demos.
- FIX:  Added debug switches to the MinGW simulated demo, not it is possible to
  debug the demo (and the kernel) inside Eclipse without a physical board.
- Removed lots of old deprecated constructs from the MinGW simulated demo.
  There are still some, the demo will need some rework before version 1.0.0.
- Updated the C++ wrapper with the latest APIs changes and fixed some bugs.
- Small fixes to the documentation.

*** 0.8.2 ***
- FIX: Included the files that were missing from version 0.8.1 distribution.
- FIX: Duplicated sections in the documentation removed.
- FIX: Minor problem in Cortex-M3 and AVR ports when the kernel is compiled
  using G++.
- NEW: Added chPoolAllocI() and chPoolFreeI() APIs in order to allow the use
  of memory pools from interrupt handlers and timer callbacks.
- CHANGE: Simplified the code for chThdWait(), it is now both smaller and
  faster. Added an important usage note to the documentation of this API.
- CHANGE: The macros WorkingArea(), UserStackSize() and StackAlign() are now
  deprecated and will be removed in version 1.0.0. Use the new equivalents
  WORKING_AREA(), THD_WA_SIZE() and STACK_ALIGN() instead.
- CHANGE: Renamed the default idle thread function from _IdleThread() to
          _idle().
- Added to the LPC2148 and STM32 load scripts the options "ALIGN(16)
  SUBALIGN(16)" to the flash loading section in order to enforce the alignment
  for both the code and read only data. This is done in order to obtain more
  accurate timings from benchmarks, those families have 16 bytes
  prefetch buffers and are very sensitive to alignment changes.
  You can remove those options in order to save some flash space if accurate
  response time is not on top of your priorities, it mainly depends on your
  requirements.

*** 0.8.1 ***
- FIX: Fixed a regression in version 0.8.0, the configuration switch
  CH_USE_EVENTS_TIMEOUT was redefined as CH_USE_EVENT_TIMEOUT and this broke
  the code using events timeouts (the LPC2148 C++ demo).

*** 0.8.0 ***
- NEW: Added condvars mechanism on top of the mutexes subsystem.
- NEW: Improved events subsystems, now it is also possible to use it just as
  "event flags" without have to use event handler callbacks.
  Some new APIs were introduced:
  * chEvtWaitOne()      - Wait for a single event.
  * chEvtWaitAny()      - Wait with OR condition.
  * chEvtWaitAll()      - Wait with AND condition.
  * chEvtDispatch()     - Invokes the event handlers associated to a mask.
  * chEvtPend()         - Quickly self-pends some events.
  * chEvtRegisterMask() - Registers a set of flags on a single source.
  * EVENT_MASK()        - Replaces the old EventMask() macro.
  All the "wait"-type APIs have a timeout-capable variant.
- CHANGE: The old EventMask(), chEvtWait() and chEvtWaitTimeout() APIs are
  now deprecated and will be removed in version 1.0.0.
- CHANGE: Modified chDbgAssert() to syntax check the condition even when the
  CH_USE_DEBUG is disabled, it produces no code but allows to check the
  optional code without have to compile twice.
- FIX: Fixed a warning generated by the chEvtIsListening() macro.
- Added new test cases to the test suite about condvars and the new events
  APIs.
- Added a new benchmark to the test suite (timers set/reset performance).
- Renamed the macro fifo_init() to queue_init() because it is used to init
  both FIFO queues and priority queues.
- Fixes and improvements to the documentation.
- Cleaned demo applications of old events code.

*** 0.7.3 ***
- FIX: Fixed a bug in chThdSleepUntil(), this API is no more a macro now.
- NEW: New chThdSleepSeconds(), chThdSleepMilliseconds() and
  chThdSleepMicroseconds() utility macros.
- CHANGE: Zero is no more a valid time specification for the chVTSetI() API.
- CHANGE: Removed the files chsleep.c and sleep.h.
- CHANGE: Renamed the files chdelta.c and delta.h to chvt.c and vt.h. All the
  system time related functions and macros are now there.
- CHANGE: Renamed the structure DeltaList to VTList, it includes the system
  time counter too now.
- CHANGE: Removed the CH_USE_SYSTEMTIME and CH_USE_VIRTUAL_TIMER configuration
  options in order to make the chconf.h file simpler. The related subsystems
  are almost always required and are now always included.
- Small optimization to the MSP430 serial driver.
- Improvements to the test code, now a failed assert terminates the test case.
- Added dependency informations handling to the MSP430 demo Makefile.
- Removed the performance spreadsheet (it was *very* old) and added a
  directory containing the test reports ./docs/reports. Each report shows the
  results from the latest test run on each target.
- Small fixes to the documentation.

*** 0.7.2 ***
- NEW: Added a serial driver to the MSP430 port, the MSP430 port now has been
  tested on hardware and passes the test suite.
- NEW: Added to the MSP demo program the option to run from the internal DCO
  or from an external xtal. The default is the internal DCO.
- NEW: Added macros to convert from seconds, milliseconds and microseconds to
  system ticks. This improves application code portability among different
  ports.
- CHANGE: Modified the test suite to use the new time conversion macros.
- CHANGE: Modified the CM3 startup file in order to implement an early
  initialization phase: hwinit0, the late initialization phase is now named
  hwinit1. The demo now initializes the PLL before initializing the BSS and
  DATA segments, this greatly optimizes the system start up time.
- NEW: Unified ARM7 startup file, it is shared by the LPC and SAM7 demo
  projects. The new startup file implements early and late initialization
  phases as described above for the CM3 startup file.
  The architecture specific vector tables are now encapsulated into the
  vectors.s files.
- Modified the STM32 demo makefile to use the latest YAGARTO toolchain as
  default (arm-elf-gcc).
- Documentation improvements, added collaboration diagrams and call graphs.
  Added a documentation-related readme under ./docs.

*** 0.7.1 ***
- NEW: New chThdInit() and chThdCreateStatic() APIs now replace the old
  chThdCreate() and chThdCreateFast() that are thus marked as deprecated.
  The new APIs use one less parameter and are faster.
- NEW: New dynamic chThdCreateFromHeap() and chthdCreateFromMemoryPool() APIs.
  The dynamic APIs are only included if the CH_USE_DYNAMIC option is specified
  into the project configuration file.
- NEW: Added an THREAD_EXT_EXIT macro in chconf.h to add finalization code to
  the chThdExit() API.
- CHANGE: chThdCreateFast() is now a macro that uses chThdCreateStatic().
- CHANGE: chThdWait() now releases the memory allocated by
  chThdCreateFromHeap() and chthdCreateFromMemoryPool(). Threads created
  through the static APIs are not affected thus the behavior is backward
  compatible.
- CHANGE: Modified the chThdResume() API to return the resumed thread pointer
  instead of void. This allowed few optimization into the threads creation
  code.
- CHANGE: The chThdGetExitEventSource() API and the CH_USE_EXIT_EVENT
  configuration option and the are now deprecated. Use the THREAD_EXT_EXIT
  finalization macro in order to implement a similar functionality if needed.
- FIX: The chThdCreate() had a regression in 0.7.0, the mode parameter was
  ignored.
- FIX: Removed duplicated call to chHeapInit() into chSysInit().
- FIX: Fixed a syntax error in chheap.c triggered by the CH_USE_DEBUG option.
- Added new test cases to the test suite for the new dynamic APIs.
- Documentation fixes.

*** 0.7.0 ***
- NEW: Memory Heap Allocator functionality added. The allocator implements a
  first-fit strategy but there is an option that allow it to wrap the compiler
  provided malloc() that may implement a different strategy. The heap
  allocator is thread-safe and can use both a mutex or a semaphore as
  internal synchronization primitive.
- NEW: Memory Pools functionality added, this mechanism allows constant-time
  allocation/freeing of constant-size objects. It can be used to dynamically
  allocate kernel objects like Semaphores, Mutexes, Threads etc fully in real
  time, of course it is also possible to manage application-defined objects.
  The pool allocator is thread-safe.
  It is worth remembering that the kernel is still entirely static, it does
  not use the allocation services internally, it is up to the application
  code to use the allocators in order to use dynamic system objects.
  Both the allocators can be disabled and removed from the memory image.
- NEW: Added option macros in chconf.h to add custom fields and initialization
  code to the Thread structure.
- FIX: Corrected the wrong definition of the chThdResumeI() macro.
- FIX: The API chSemWaitTimeout() was missing in the documentation.
- CHANGE: Modified the chMtxUnlock() and chMtxUnlockS() APIs to return the
  pointer to the released mutex instead of void.
- CHANGE: Now the chThdResume() API asserts that the thread is in PRSUSPEND
  state rather than test it.
- CHANGE: Removed the CH_USE_TERMINATE, CH_USE_SLEEP, CH_USE_SUSPEND and
  CH_USE_RESUME configuration options in order to make the chconf.h file
  simpler. The related functions are very small and almost always required.
- CHANGE: The P_MSGBYPRIO thread option has been removed, now the threads
  always serve messages in priority order if the CH_USE_MESSAGES_PRIORITY
  configuration option is active.
- Added new test cases to the test suite.

*** 0.6.10 ***
- FIX: Fixed a case-sensitiveness error in lpc214x_ssp.c, it affected only
  linux/unix users.
- FIX: Fixed a regression introduced in version 0.6.9, the queues benchmark
  test case was missing from the tests list.
- NEW: Added an option to the ARM7 ports, by specifying -DREENTRANT_LOCKS in
  the makefile options makes the chSysLock() and chSysUnlock() become
  reentrant. The code becomes a bit larger and slower, use it only if your
  application really needs to invoke system API under lock.
- NEW: Added an option to the ARM7 and CM3 makefiles to strip any unused code
  and data from the binary file (the default is on).

*** 0.6.9 ***
- NEW: Added an option to exclude the support for the round robin scheduling,
  this can save some extra program space and makes the context switch a bit
  faster if the feature is not required. Threads at the same priority level
  are still supported when the feature is disabled but the scheduling among
  them becomes cooperative.
- OPT: Improved reschedulation time by reordering the sequence of operations,
  now during enqueuing the ready list contains one less element. This change
  also slightly improves the interrupt latency.
- OPT: Optimization to the chSemReset(), reversed the order of dequeuing.
- FIX: Fixed a bug in the chThdSetPriority() API.
- FIX: Modified the structure names into nvic.h in order to not make them
  collide with external libraries.
- Added a benchmark to the test suit that measures the mass reschedulation
  performance.
- Added a test_terminate_threads() function to the test framework.
- Made the Cortex-M3 port preemption code more readable.
- Added a ENABLE_WFI_IDLE option to the chcore.h file in the Cortex-M3 port,
  setting this option to 1 enables the kernel to enter a low power mode when
  executing the idle thread. Be careful however, this option can be not
  compatible with some JTAG probes, it is better to enable it only on final
  builds and not when debugging.

*** 0.6.8 ***
- FIX: Fixed a bug in the priority inheritance mechanism, the bug was only a
  problems when the CH_USE_MESSAGES_PRIORITY was enabled, this option is
  disabled by default in ChibiOS/RT so it should not affect any user.
- CHANGE: The function chEvtSend() and chEvtSendI() are now renamed in
  chEvtBroadcast() and chEvtBroadcastI(), the old names are still available
  but are deprecated.
- Made the default BASEPRI levels (CM3 port) configurable into chcore.h.
- Many improvements to the documentation.
- All the fixes and changes in this release were suggested/submitted by
  Leon Woestenberg (thank you).

*** 0.6.7 ***
- NEW: New chThdCreateFast() API, it is a simplified form of chThdCreate()
  that allows even faster threads creation. The new API does not support
  the "mode" and "arg" parameters (still available in the old API).
- OPT: Removed an unrequired initialization and made other small optimizations
  to the chThdCreate().
- OPT: Improvements to the test framework, now a virtual timer is used instead
  of software loops into the bechmarks in order to have more stable results.
- New benchmark added to the test suite.
- Added the C++ wrapper entries to the documentation.
- Fixed the documentation entry for the chThdCreate() API.
- Removed redundant ifdefs from the ch.h header.

*** 0.6.6 ***
- NEW: Improved test suite, now the suite is divided in modules and the code
  is much easier to understand. The new framework simplifies the inclusion of
  new test cases and makes possible to verify the exact sequence and the
  timing of test events.
- NEW: New API chSysInTimeWindow() that checks if the current system time is
  within the specified time window.
- FIX: Mutex test #1 in the test suite corrected, it failed to... fail.
- FIX: Fixed a problem in the STM32 port USART1 driver.
- FIX: Fixed a problem in the MMC/SD driver in the LPC2148 demo.
- Added the definitions for packed structures to the chtypes.h files.
- Improvements to the makefiles, now each source group has its own .mk include
  file. Now it is no more required to rewrite everything in each makefile.

*** 0.6.5 ***
- NEW: Web server demo for the AT91SAM7X256, the demo integrates the uIP
  stack and its demo applications.
- NEW: EMAC driver added to the AT91SAM7X port.
- FIX: Small fix to the ARM7 startup files. It used a short jump in the reset
  vector and that could fail in some memory configurations.
- Documentation improvements.

*** 0.6.4 ***
- NEW: MSP430 port, the port code compiles correctly but it is not tested yet.
  The port requires the MSPGCC toolchain.
- NEW: Added a CH_ARCHITECTURE_xxx define to the various chcore.h files, it
  allows to write port-dependent code.
- NEW: Added to the documentation the technical notes about the currently
  supported ports.
- FIX: In the ARM7 and ARMCM3 ports chanced the bool_t base type from int8_t
  to int32_t, this produces a bit faster and smaller code.
  It is nowhere required the bool_t type to be one byte sized.
- FIX: Small fixes to the template files, there were some leftovers of the old
  type names.
- FIX: Modified the ARM demos makefiles in order to make them more compatible
  with GCC 4.3.0, it seems the new GCC assumes -mthumb-interworking and
  -mabi=apcs by default, at least the builds I tested did so, now the makefiles
  explicitly assert -mno-thumb-interworking and -mabi=apcs-gnu in order to
  produce better code.
- Added an Ethernet driver for AT91SAM7X EMAC, not complete yet, it will be
  required by a uIP web server demo under ChibiOS/RT coming in some next
  release.

*** 0.6.3 ***
- NEW: ARM Cortex-M3 port completed. The demo program targets the STM32F103
  chip from ST Microelectronics on an Olimex STM32-P103 board.
- FIX: Fixed a minor error in ./ports/ARM7-LPC214x/vic.h, it should not affect
  anything.
- FIX: Minor fix: in chThdCreate() a working area size equal to
  UserStackSize(0) was asserted as an error when in debug mode. It is now
  allowed.
- FIX: Increased the stack size for the threads in the test suite to 128 bytes
  because THUMB/THUMB2 modes seem to use a lot more stack than ARM mode.

*** 0.6.2 ***
- NEW: Added C++ wrapper around the ChibiOS/RT core APIs, now it is possible
  to use the OS in a fully object oriented application. The wrapper offers
  classes that encapsulate threads, semaphores, mutexes, timers etc. Normal C
  APIs are still accessible from C++ code as usual.
- NEW: Added a new LPC2148 demo using the new C++ wrapper, it is a good
  example of C++ used for an embedded application. The demo does not use RTTI
  nor standard libraries so the resulting code is very compact.
- Enhanced the chSemSignalWait() API to return the wakeup message just like
  the other "Wait" semaphore functions do.
- Fixed a minor problem in the ARM7 port, the extctx structure definition was
  missing one field, the effect was to allocate stacks 4 bytes shorter than
  the declared size.
- Fixed a compile time error into the chThdSleepUntil() macro.
- Fixes in various headers to make some macros compatible with both C and C++.
- Fixed a regression in the LPC214x minimal demo that broke interrupt
  handling.
- Some fixes to the doxigen documentation.
- More work done on the ARM-CM3 port but it is still not complete.

*** 0.6.1 ***
- Removed some redundant checks from the scheduler code: improved threads
  flyback time, reduced interrupts service time.
- Nice scheduler speed improvement obtained by removing the 2nd parameter to
  the chSchReadyI() API and manually assigning the message value only where
  is really needed (very few points in the code).
- More space savings and speed improvements obtained by removing the
  -fno-strict-aliasing option from the makefiles, now the kernel compiles
  without any warning even without this option.
- Removed the -falign-functions=16 option from the AT91SAM7X demo makefiles,
  the Atmel chip does not require it, the option is still present on the
  LPC21xx demos. This saves significant program space.
- Started work on ARM Cortex-M3 architecture. The target chip is the STM32F103
  on a Olimex STM32-P103 board.
- Added a threads state diagram to the documentation.
- Various fixes to the doxigen documentation.

*** 0.6.0 ***
- Code refactory, all the old sized-integer definitions like LONG32, UWORD16
  etc are now replaced by the proper definitions provided by the compiler
  into stdint.h.
- Code refactory, the previous system types style using a t_ in front of the
  name has been replaced with the standard trailing _t. The system now uses
  the size_t type defined into stddef.h. Some type names were modified in
  order to not match commonly used type names.
- The above changes have an impact on some API prototypes but we can't help
  it, the changes were required because the type names were a concern for
  some users.
- Implemented a serial driver in the AVR port.
- Implemented a simple HD44780 LCD driver into the AVRmega128 demo.
- Reworked the AVR AT90CAN128 port to share the common AVR code.
- Modified the test suite to be compatible with 8 bit micros.
- MSVC demo dropped, it is still possible to use the MinGW demo as simulator
  in Win32.
- Fixed a minor error in sam7x_serial.h and lpc214x_serial.h.
- The kernel is *unchanged* compared to version 0.5.3 except for the type
  names but the change is important enough to make this a recommended update.

*** 0.5.5 ***
- Added an AVRmega128 port. The previous AT90CANx port is still present but
  it will be redone after the AVRmega128 port is complete because it will
  share most of it. The demo is very simple, it will be expanded in next
  releases.
- Reorganized the code of the two ARM7 ports, now all the common ARM7 code
  is in ./ports/ARM7. This will make maintenance and new ARM7 ports much much
  easier.
- Simplified the directory structure under ./ports.
- Added to the readme a section with our future plans/ideas.
- The kernel is *unchanged* compared to version 0.5.3, just the new port and
  the new demo were added.

*** 0.5.4 ***
- Port for Atmel AT91SAM7X256 introduced, the port should be useable also on
  SAM7S and SAM7XC but no tests were performed. Other SAM7 processors should
  also be useable with limited changes.
  The demo currently just performs basic operations, will be enhanced in next
  ChibiOS/RT releases, see the demo readme.txt file.
- Small fix to the thumb mode IRQ code on the LPC214x port, removed some extra
  code.
- The kernel is *unchanged* compared to version 0.5.3, just the new port and
  the new demo were added.

*** 0.5.3 ***
- Removed the chMsgSendTimeout() API, it was conceptually flawed because,
  after sending a message, the sender *has* to wait for the answer or
  the next sender in queue would receive it instead (the messages server has
  no way to know that the sender is gone because a timeout).
  A workaround would make the messages subsystem much heavier and this is
  not acceptable.
- Removed the test case for chMsgSendTimeout() from the test suite.
- Space saved by reorganizing the timeout code into a single scheduler
  function chSchGoSleepTimeoutS().
- Space optimizations in the semaphores code.
- The API chThdSleepUntil() become a macro saving some more code space.
- Because all the above changes the kernel code (ARM) is over 700 bytes
  smaller.

*** 0.5.2 ***
- Fixed a small problem in the main header file ch.h.
- Small reordering in the fields of the Thread structure in order to optimize
  the space when messages are not used.

*** 0.5.1 ***
- NEW: Priority enqueing for messages can be optionally enabled by specifying
  the P_MSGBYPRIO option when creating a message server thread.
  This change allows the implementation of a priority ceiling protocol into
  message servers threads. Threads serving messages by priority and threads
  serving messages in FIFO orded can exist at the same time in the system.
  This feature can be enabled or disabled by toggling the option
  CH_USE_MESSAGES_PRIORITY into the chconf.h file (disabled by default, old
  behavior).
  Note: This option brings a small overhead when sending a message regardless
  if in FIFO or priority order, if you dont need priority ordering for your
  messages it is better to keep the feature disabled in chconf.h.
- Added to the ARM demos load scripts the capability to load code in RAM
  instead flash, the function must be marked as:
    __attribute__((section(".ramtext")))
  The option -mlong-calls should be specified in the makefile too or the
  function declared with the "long-call" attribute.
- Fixed the MSVC demo project files.
- Fixed some syntax incompatibilites between GCC and MSVC into chmtx.c.

*** 0.5.0 ***
- NEW: Mutexes, the new mechanism provides a complete implementation of the
  "priority inheritance" algorithm as a tool for work around the priority
  inversion problem.
  The Mutexes are not meant to replace the Semaphores that still are the best
  synchronization mechanism between interrupt handlers and high level
  code, something that Mutexes cannot do.
  Soon an article will be added to the wiki describing pro and cons of the
  various mechanisms and the correct use cases.
- RT Semaphores subsystem removed, the Mutexes implements a better solution
  for the same problem.
- Fixed a bug in the round robin scheduling mode, see the bug tracker for
  details and a fix for previous versions.
- More performance improvements to the scheduler. The ready list insertion
  sequence is now reversed, it is scanned starting from the highest priority
  end. This has an important side effect into the chSchWakeupS() that makes
  most of the ready list insertions happen in constant time (extraction is
  done always in constant time).
  The worst case is always proportional to the number of threads in the ready
  list but the normal case is much more often constant than linear. See the
  new benchmarks added to the test suite.
- Added mutexes test cases and new benchmarks to the test suite.
- Modified the test suite in order to have each test case to have the same
  alignment enforced on functions. This is done to reduce MAM/Cache alignment
  effects on the measurement.
- IRQ entry/exit code is now encapsulated into two new macros, see chcore.h
  for details.
- All the asm code previously in chcore2.s is now inline asm code in chcore.c
  (ARM port), chcore2.s removed.
- Moved all the board specific definitions/code into two new files: board.c
  and board.h. Moved all the files no more board-dependent under ports/
  (ARM port).
- Improved the kernel performance in THUMB mode by better exploiting MAM
  locality in some critical functions. The context switch benchmark shows
  5% improved speed.

*** 0.4.5 ***
- Moved the serial IRQ handlers and VIC vectors initialization inside the
  serial drivers. Now the serial-related code is all inside the driver.
- Moved all the other interrupt handlers from chcore2.s into chcore.c as
  inline asm code. The interrupt code now is faster because one less call
  level.
- Fixed a minor problem in chSysHalt() now it disables FIQ too and makes sure
  to keep the processor in the state it had when it was halted.
  Note: This is not a kernel bug but something specific with the ARM port, the
        other ports are not affected.
- Fixed the macro chThdResumeI(), it had a regression.

*** 0.4.4 ***
- Fixed a very important bug in the preemption ARM code, important enough to
  make this update *mandatory*.
  Note: This is not a kernel bug but something specific with the ARM port, the
        other ports are not affected.
- Fixed a nasty bug in the pure THUMB mode threads trampoline code (chcore2.s,
  threadstart), it failed on THUMB threads returning with a "bx" instruction.
  The bug did not affect ARM mode or THUMB with interworking mode.
  Note: This is not a kernel bug but something specific with the ARM port, the
        other ports are not affected.
- Fixed a bug in chIQGetTimeout(), interrupts were not re-enabled when exiting
  the function because a timeout. The problem affected that API only.
- Fixed a potential problem in chSysInit(), it should not affect any past
  application.
- Added a chDbgAssert() API to the debug subsystem.
- Cleaned up the kernel source code using chDbgAssert() instead of a lot of
  "#ifdef CH_USE_DEBUG", it is much more readable now.
- Now the threads working area is filled with a 0x55 when in debug mode, this
  will make easier to track stack usage using a JTAG probe.
- Added an I/O Queues benchmark to the test suite.
- Removed the chSchTimerHandlerI() routine from chschd.c and moved it into
  chinit.c renaming it chSysTimerHandlerI() because it is not part of the
  scheduler.

*** 0.4.3 ***
- Size optimization in the events code, now the chEvtWait() reuses the
  chEvtWaitTimeout() code if it is enabled.
- Size optimization in the semaphores code, now the chSemWaitTimeout() just
  invokes the chSemWaitTimeoutS() inside its system mutex zone. Same thing
  done with chSemWait() and chSemWaitS().
- Size optimization in the queues code.
- Modified the return type of chSemWait() and chSemWaitS() from void to t_msg,
  this allows to understand if the semaphore was signaled or reset without
  have to access the Thread structure.
- Added a threads create/exit/wait benchmark to the test suite, the system
  is capable of 81712 threads started/terminated per second on the reference
  LPC2148 board. The figure is inclusive of two context switch operations
  for each thread.
- Minor improvement in the LPC214x serial driver, unneeded events were
  generated in some rare cases.
- Fixed a chSysInit() documentation error.
- Fixed a chEvtWaitTimeout() documentation error.
- Added a new debug switch: CH_USE_TRACE, previously the trace functionality
  was associated to the CH_USE_DEBUG switch.

*** 0.4.2 ***
- Added a minimal ARM7-LPC demo, you can use this one as template in order to
  create your application. It is easier to add subsystems back to the small
  demo than remove stuff from the large one.
- Introduced support for "pure" THUMB mode, it is activated when all the
  source files are compiled in THUMB mode, the option -mthumb-interworking is
  not used in this scenario and this greatly improves both code size and
  speed.
  It is recommended to either use ARM mode or THUMB mode and not mix them
  unless you know exactly what you are doing. Mixing modes is still supported
  anyway.
- More optimizations in the scheduler, updated the performance spreadsheet.
- Fixed a problem with the thread working area declarations, the alignment to
  4 bytes boundary was not enforced. Now it is defined a new macro
  WorkingArea(name, length) that takes care of both the allocation and the
  alignment.
  Example:
    static WorkingArea(waThread1, 32);
  It is expanded as:
    ULONG32 waThread1[UserStackSpace(32) >> 2];
  Now the demos use the new declaration style.
- Fixed a small problem in sleep functions introduced in 0.4.1.

*** 0.4.1 ***
- Modified the initialization code in order to have a dedicated idle thread in
  the system, now the main() function behaves like a normal thread after
  executing chSysInit() and can use all the ChibiOS/RT APIs (it was required
  to run the idle loop in previous versions).
  Now it is also possible to use ChibiOS/RT with a single main() thread and
  just use it for the I/O capabilities, Virtual Timers and events. Now you
  don't have to use multiple threads if you don't really need to.
- Added a spreadsheet in the documentation that describes the advantages
  and disadvantages of the various optimization options (both GCC options and
  ChibiOS/RT options), very interesting read IMO.
- The GCC option +falign-functions=16 is now default in the Makefile, it is
  required because of the MAM unit into the LPC chips, without this option
  the code performance is less predictable and can change of some % points
  depending on how the code is aligned in the flash memory, unpredictability
  is bad for a RTOS. This option however increases the code size slightly
  because of the alignment gaps.
- Fine tuning in the scheduler code for improved performance, deeper
  inlining and other small changes, about 5% better scheduler performance.
- Increased the default system-mode stack size from 128 to 256 bytes because
  some optimizations and the THUMB mode seem to eat more stack space.
- Included a Makefile in the LPC2148 demo that builds in THUMB mode.
- Const-ified a parameter in the chEvtWait() and chEvtWaitTimeout() APIs.
- Removed the CPU register clearing on thread start, it was not really useful,
  it is better to maximize performance instead.
- Cleaned up the ARM port code. Now it is easier to understand.
- Cleaned up the LPC2148 demo in main.c, it is now well documented and
  explains everything, I assumed too much stuff to be "obvious".

*** 0.4.0 ***
- NEW, added a benchmark functionality to the test suite. The benchmark
  measures the kernel throughput as messages per second and context switches
  per second. The benchmark will be useful for fine tuning the compiler
  options and the kernel itself.
- NEW, implemented a debug subsystem, it supports debug messages and a context
  switch circular trace buffer. The debug code can be enabled/disabled by
  using the CH_USE_DEBUG in chconf.h.
  The trace buffer is meant to be fetched and decoded by an external tool
  (coming soon, it can be accessed using JTAG in the meanwhile).
- Added new API chThdGetPriority() as a macro.
- Implemented panic messages when CH_USE_DEBUG is enabled.
- Added a thread identifier field to the Thread structure, it is used only
  for debug.
- Global variable stime modified as volatile.
- API chSysGetTime() reimplemented as a macro.
- Fixed a regression with the CH_CURRP_REGISTER_CACHE option.
- Fixed a problem in the LPC2148 interrupt handling code, a spurious
  interrupts fix recommended on the NXP data sheet proved to be a very bad
  idea and not about real spurious interrupts also...
- Fixed an harmless warning message in buzzer.c.

*** 0.3.6 ***
- Added SSP (SPI1) and ext.interrupts definitions to the lpc214x.h file.
- Added SSP driver for the LPC2148.
- Added experimental MMC/SD block driver to the LPC2148 demo in order to
  support file systems. The driver features also events generation on card
  insert/remove, hot plugging supported.
- Added missing chThdSuspend() declararion in threads.h.

*** 0.3.5 ***
- Space optimization in events code.
- Changed the behavior of chEvtWaitTimeout() when the timeout parameter is
  set to zero, now it is consistent with all the other syscalls that have a
  timeout option.
- Reorganized all the kernel inline definitions into a single file (inline.h).
- Fixed a minor problem in the interrupt initialization code for the LPC214x
  demo, regrouped the VIC-specific code into vic.c/vic.h.
- Fixed a bug into the LPC2148 serial driver (limited to the serial port 2).
- Implemented HW transmit FIFO preloading in the LPC2148 serial driver in
  order to minimize the number of interrupts generated, it is possible to
  disable the feature and return to the old code which is a bit smaller, see
  the configuration parameters in ./ARM7-LPC214x/GCC/lpc214x_serial.h.
- Some more work done on the AVR port, it is almost complete but not tested
  yet because my JTAG probe broke...

*** 0.3.4 ***
- Fixed a problem in chVTSetI().
- New API, chVTIsArmedI(), it is a macro in delta.h.
- New API, chThdResumeI(), it is a macro in threads.h. This function is just
  an alias for chSchReadyI() but makes the code more readable.
- New API, chThdSuspend(). New switch CH_USE_SUSPEND added to chconf.h.

*** 0.3.3 ***
- Modified the chVTSetI(), now for the "time" parameter can have value zero
  with meaning "infinite". This allows all the APIs with timeout parameters
  to be invoked with no timeout.
- Fixes in the documentation.
- Renamed some APIs in the "Sch" group to have an S suffix instead of I.

*** 0.3.2 ***
- Modified the chSysInit() to give the idle thread absolute priority, the
  priority is then lowered to the minimum value into the chSysPause(). This
  is done in order to ensure that the initializations performed into the
  main() procedure are finished before any thread starts.
- Added chThdSetPriority() new API.
- Added a generic events generator timer modulee to the library code.
- Modified the ARM7-LPC214x-GCC demo to show the use of the event timer.
- Added the "#ifdef __cplusplus" stuff to the header files.
- Removed an obsolete definition in ./src/templates/chtypes.h.

*** 0.3.1 ***
- Test program added to the demos. Telnet the MinGW and MSVS demos and type
  "test" at the "ch>" prompt. On the LPC214x demo the test is activated by
  pressing both the board buttons. The test performs tests on the ChibiOS/RT
  functionalities.
  The test code is also a good example of APIs usage and ChibiOS/RT behavior.
- Fixed bug in chEvtWaitTimeout(), the timeout code performed an useless
  dequeue operation.
- Fixed a bug in chSemWaitTimeoutS() and chSemWaitTimeout(), the semaphore
  counter was not atomically updated on a timeout condition.
- Fixed bug on RT semaphores, the priority queuing was broken.
- Fixed a bug in the MinGW demo, the chThdExit() code was not correctly
  reported to the thread waiting in chThdWait().
- Fixed a function declaration in semaphores.h.
- Lists code moved into chlists.c from various other places optimized and
  reorganized.
- The list of the threads waiting in chThdWait() is now a single link list,
  this saves some space.
- Cleaned the template files code, the files contained some obsolete
  declarations.
- Code optimization in chSemWaitTimeoutS(), chSemWaitTimeout() and
  chSemSignalWait().
- Code optimization in chEvtSend().
- Code optimization in chVTDoTickI().
- Added a Semaphore pointer to the Thread structure, this allows to know on
  which semaphore a thread is waiting on. It takes no space because it is
  located in the union inside the Thread structure. This also allowed a minor
  optimization inside chSemWaitTimeout() and chSemWaitTimeoutS().
- Changed the priority type to unsigned in order to make it compatible
  with a byte value, this is very important for 8 bits architectures.
- Modified the MinGW and MSVS demos to use 1ms ticks instead of 10ms as
  before.

*** 0.3.0 ***
- ChibiOS/RT goes beta.
- Diet for the threads code, some simple APIs become macros.
- Thread Local Storage implemented as a single API: chThdLS().
  The API simply returns a pointer into the thread working area, see the
  documentation on the web site.
- Moved some documentation and images from the web site into the Doxigen
  generated HTMLs.

*** 0.2.1 ***
- Optimizations in the RT semaphores subsystem. The support for this
  subsystem should still be considered experimental and further changes may
  happen in future versions.
- Bug fix in the virtual timers handling code, now the timers can be re-armed
  from within the callback code in order to create periodic virtual timers.
- Modified the t_prio type in the demos to be 32bits wide instead of 16bits,
  this results in a better code in critical sections of the kernel.

*** 0.2.0 ***
- Introduced support for ARM in thumb mode.
- Optimized context switching when thumb-interworking is not required, one
  less instruction.
- Minor fixes to the ARM demo.

*** 0.1.1 ***
- Some fixes into the documentation
- Renamed the makefiles to Makefile, upper case M.

*** 0.1.0 ***
- First alpha release
