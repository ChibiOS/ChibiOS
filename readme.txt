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
                         generic and architecture indipendent.
./src/templates/       - ChibiOS/RT non portable source templates, new ports
                         are started by copying the templates into a new
                         directory under ./demos/.
./ports/               - Architecture/compiler specific portable files.
./demos/               - Demo programs for specific archtectures/boards.
./test/                - Test code, used by some demos.
./docs/Doxifile        - Doxigen project file.
./docs/html/index.html - ChibiOS/RT documentation (after running doxigen).
                         The documentation is also available on the project
                         web page: http://chibios.sourceforge.net/

Current ports under ./demos:

Win32-MinGW         - ChibiOS/RT simulator and demo into a WIN32 process,
                      MinGW version.
Win32-MSVS          - ChibiOS/RT simulator and demo into a WIN32 process,
                      Visual Studio 7 or any later version should work.
ARM7-LPC214x-GCC    - ChibiOS/RT port for ARM7 LPC2148, the demo targets the
                      Olimex LPC-P2148 board. This port can be easily modified
                      for any processor into the LPC2000 family or other
                      boards. The demo can be compiled using YAGARTO or any
                      other GCC-based ARM toolchain.
AVR-AT90CANx-GCC    - Port on AVR AT90CAN128, not complete yet.

*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

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
- Renamed makefiles to Makefiles, upper case M.

*** 0.1.0 ***
- First alpha release
