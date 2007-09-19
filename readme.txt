Directories structure:

./readme.txt        - This file.
./license.txt       - GPL3 license file.
./src/              - ChibiOS/RT portable kernel source files.
./src/include/      - ChibiOS/RT include files.
./src/templates/    - ChibiOS/RT non portable source templates, new ports are
                      started by copying the templates into a new directory
                      under ./demos/.
./ports/            - Architecture/compiler specific portable files.
./demos/            - Demo programs for specific archtectures/boards.
./docs/doxifile     - Doxigen project file.
./docs/index.html   - ChibiOS/RT documentation (after running doxigen).

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
