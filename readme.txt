*****************************************************************************
*** Directories structure                                                 ***
*****************************************************************************

./readme.txt        - This file.
./license.txt       - GPL3 license file.
./src/              - ChibiOS/RT portable kernel source files.
./src/include/      - ChibiOS/RT include files.
./src/templates/    - ChibiOS/RT non portable source templates, new ports are
                      started by copying the templates into a new directory
                      under ./demos/.
./ports/            - Architecture/compiler specific portable files.
./demos/            - Demo programs for specific archtectures/boards.
./docs/Doxifile     - Doxigen project file.
./docs/index.html   - ChibiOS/RT documentation (after running doxigen). The
                      documentation is also available on the project web
                      page: http://chibios.sourceforge.net/

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
