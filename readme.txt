*****************************************************************************
*** Directories structure                                                 ***
*****************************************************************************

./readme.txt           - This file.
./license.txt          - GPL3 license file.
./src/                 - ChibiOS/RT portable kernel source files.
./src/include/         - ChibiOS/RT include files.
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
LPC214x-GCC         - ChibiOS/RT port for ARM7 LPC2148, the demo targets the
                      Olimex LPC-P2148 board. This port can be easily modified
                      for any processor into the LPC2000 family or other
                      boards. The demo can be compiled using YAGARTO or any
                      other GCC-based ARM toolchain.

*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

*** 0.3.1 ***
- Test program added to the demos. Telnet the MinGW and MSVS demos and type
  "test" at the "ch>" prompt. On the LPC214x demo the test is activated by
  pressing both the board. The test performs integrity tests on the main
  ChibiOS/RT functionalities.
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
