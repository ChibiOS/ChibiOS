*****************************************************************************
*** Files Organization                                                    ***
*****************************************************************************

--{root}                - ChibiOS/RT directory.
  +--readme.txt         - This file.
  +--documentation.html - Shortcut to the web documentation page.
  +--todo.txt           - Current plan (development/unstable versions only).
  +--license.txt        - GPL license text.
  +--exception.txt      - GPL exception text (stable releases only).
  +--boards/            - Board support files.
  +--demos/             - Demo projects.
  +--docs/              - Documentation.
  |  +--html/           - Local HTML documentation (after rebuild).
  |  +--reports/        - Test reports.
  |  +--src/            - Documentation source files (required for rebuild).
  |  +--rsc/            - Documentation resource files (required for rebuild).
  |  +--Doxyfile        - Doxygen project file (required for rebuild).
  |  +--index.html      - Local documentation access (after rebuild).
  +--ext/               - External libraries, not part of ChibiOS/RT.
  +--os/                - ChibiOS/RT files.
  |  +--hal/            - Hardware Abstraction Layer.
  |  |  +--include/     - HAL high level headers.
  |  |  +--src/         - HAL high level source.
  |  |  +--platforms/   - HAL low level drivers implementations.
  |  |  |  +--AT91SAM7/ - Drivers for AT91SAM7 platform.
  |  |  |  +--AVR/      - Drivers for AVR platform.
  |  |  |  +--LPC11xx/  - Drivers for LPC11xx platform.
  |  |  |  +--LPC13xx/  - Drivers for LPC13xx platform.
  |  |  |  +--LPC214x/  - Drivers for LPC214x platform.
  |  |  |  +--MSP430/   - Drivers for MSP430 platform.
  |  |  |  +--SPC56x/   - Drivers for SPC56x/MPC563xx platforms.
  |  |  |  +--STM32/    - Drivers for STM32 platform (common).
  |  |  |  +--STM32F1xx/- Drivers for STM32F1xx platform.
  |  |  |  +--STM32F2xx/- Drivers for STM32F2xx platform.
  |  |  |  +--STM32F4xx/- Drivers for STM32F4xx platform.
  |  |  |  +--STM32L1xx/- Drivers for STM32L1xx platform.
  |  |  |  +--STM8L/    - Drivers for STM8L platform.
  |  |  |  +--STM8S/    - Drivers for STM8S platform.
  |  |  |  +--Posix/    - Drivers for x86 Linux/OSX simulator platform.
  |  |  |  +--Win32/    - Drivers for x86 Win32 simulator platform.
  |  |  +--templates/   - Driver template files.
  |  |     +--meta/     - Driver meta templates.
  |  +--ports/          - Port files for the various architectures.
  |  |  +--GCC/         - Ports for the GCC compiler.
  |  |  |  +--ARM/      - Port files for ARM7 and ARM9 architectures.
  |  |  |  +--ARMCMx/   - Port files for ARMCMx architectures (ARMv6/7-M).
  |  |  |  +--PPC/      - Port files for PowerPC architecture.
  |  |  |  +--AVR/      - Port files for AVR architecture.
  |  |  |  +--MSP430/   - Port files for MSP430 architecture.
  |  |  |  +--SIMIA32/  - Port files for SIMIA32 simulator architecture.
  |  |  +--IAR/         - Ports for the IAR compiler.
  |  |  |  +--ARMCMx/   - Port files for ARMCMx architectures (ARMv6/7-M).
  |  |  +--RVCT/        - Ports for the Keil RVCT compiler.
  |  |  |  +--ARMCMx/   - Port files for ARMCMx architectures (ARMv6/7-M).
  |  |  +--cosmic/      - Ports for the Cosmic compiler.
  |  |  |  +--STM8/     - Port files for STM8 architecture.
  |  |  +--RC/          - Ports for the Raisonance compiler.
  |  |     +--STM8/     - Port files for STM8 architecture.
  |  +--kernel/         - Kernel portable files.
  |  |  +--include/     - Kernel headers.
  |  |  +--src/         - Kernel source.
  |  |  +--templates/   - Kernel port template files.
  |  +--various/        - Various portable support files.
  +--test/              - Kernel test suite source code.
  |  +--coverage/       - Code coverage project.
  +--testhal/           - HAL integration test demos.
  |  +--LPC11xx/        - LPC11xx HAL demos.
  |  +--LPC13xx/        - LPC11xx HAL demos.
  |  +--STM32F1xx/      - STM32F1xx HAL demos.
  |  +--STM32F4xx/      - STM32F4xx HAL demos (valid for STM32F2xx too).
  |  +--STM32L1xx/      - STM32L1xx HAL demos.
  |  +--STM8S/          - STM8S HAL demos.
  +--tools              - Various tools.
     +--eclipse         - Eclipse enhancements.

*****************************************************************************
*** Releases                                                              ***
*****************************************************************************

*** 2.5.0 ***
- FIX: Fixed MMC over SPI driver performs an unnecessary SPI read (bug
  3486930)(backported to 2.2.9 and 2.4.1). Also removed unnecessary CRC7
  calculation.
- FIX: Fixed Realtime counter initialization in STM32 HALs (bug 3485500)
  (backported to 2.4.1).
- FIX: Fixed PPC port broken when CH_DBG_SYSTEM_STATE_CHECK is activated
  (bug 3485667)(backported to 2.4.1).
- FIX: Fixed missing PLL3 check in STM32F107 HAL (bug 3485278)(backported
  to 2.4.1).
- FIX: Fixed ADC maximum frequency limit in STM32F2/F4 ADC drivers (bug
  3484947)(backported to 2.4.1).
- FIX: Fixed various minor documentation errors (bug 3484942)(backported
  to 2.4.1).
- NEW: Updated debug plugin 1.0.8 (backported to 2.4.0).
- NEW: Added more accurate UBRR calculation in AVR serial driver (backported
  to 2.4.0).
- NEW: Revision of the round-robin scheduling, now threads do not lose their
  time slice when preempted. Each thread has its own time slices counter.
  TODO: Seek optimizations.
- NEW: Modified the Virtual Timers management, now the callback is invoked
  not in lock mode. This change reduces the interrupt jitter caused by
  multiple timers used at same time.
- NEW: Added board files and demo for Olimex LPC-P1343 (contributed by
  Johnny Halfmoon).

*** 2.3.5 ***
- FIX: Fixed RTC compile problem on STM32F103 (bug 3468445).
- FIX: Fixed PWM with TIM1 and TIM8 broken in STM32 HAL (bug 3458947).
- FIX: Fixed SYSCFG clock not started in STM32L1/F4 HALs (bug 3449139).
- FIX: Fixed wrong definitions in STM32L-Discovery board file (bug 3449076).
- OPT: Improved the exception exit code in the GCC Cortex-Mx ports.
- NEW: Added a DMA stress test application for the STM32F4 in order to assess
  robustness of the whole HAL.
- NEW: Added a Time Measurement driver to the HAL, this generic driver uses
  the realtime counters abstracted in the HAL driver.
- NEW: Improved the STM32F1xx HAL driver, it now has the same features and
  configuration options of the newer STM32s.
- NEW: MMC over SPI driver improved to handle high capacity cards, by
  Matthias Blaicher.
- NEW: Added PVD support to the HAL of all STM32s, by Barthess.
- NEW: Added to the HAL driver the handling of an abstract realtime free
  running counter, added the capability to all the STM32 HALs.
- NEW: Modified ARM and ARMCMx build rules to allow parallel build. Now the
  log outputs one dummy compilation command in order to allow paths discovery
  by Eclipse.
- NEW: Added an utility module to access LIS302DL MEMS using a SPI.
- NEW: Updated STM32F2xx support by inheriting the work done on the STM32F4xx,
  the whole thing is untested because lack of hardware.
- NEW: Files nvic.c and nvic.h moved under ./os/ports/common/ARMCMx, removed
  the duplicated instances under the GCC, IAR and Keil ports. Function names
  prefixes changed from "NVIC" to "nvic" because style conventions.
- NEW: Added voltage regulator initialization to the STM32F4xx HAL.
- NEW: Added a linker script that demonstrates how to put stacks and other
  critical structures in the CCM RAM instead normal RAM.
- NEW: Added experimental support for the Cortex-M4 FPU (default when the
  FPU is present but can be disabled).
- NEW: Improved I2C driver model and STM32 implementation by Barthess.
- CHANGE: Removed the option to change the stack alignment in the GCC
  Cortex-Mx ports, now alignment is always 64 bits.
- CHANGE: Modified the function palSetGroupMode() to have an offset parameter
  in order to make it similar to other functions operating on groups.
- CHANGE: Increased main and process default stack sizes from 0x100 to 0x200
  in LPC1114 and LPC1343 linker scripts.

*** 2.3.4 ***
- FIX: Fixed Extra initialization in STM32 SPI driver (bug 3436127)
  (backported to 2.2.8).
- FIX: Fixed DMA priority setting error in STM32 UART driver (bug 3436125)
  (backported to 2.2.8).
- FIX: Fixed DMA priority setting error in STM32 SPI driver (bug 3436124)
  (backported to 2.2.8).
- FIX: Fixed broken support for UART5 in STM32 serial driver (bug 3434094)
  (backported to 2.2.8).
- FIX: Fixed broken TIM8 support in STM32 PWM driver (bug 3418620).
- FIX: Fixed halconf.h file corrupted in some STM32 demos (bug 3418626).
- NEW: Added an unified registers file for STM32: stm32.h. This file includes
  the appropriate vendor file then adds its own additional definitions.
- NEW: Added demo for the ST STM32F4-Discovery kit.
- NEW: STM32F4xx ADC driver implementation.
- NEW: Added initialization of the NVIC VTOR register to all Cortex-Mx (v7M)
  ports. Also added a port option CORTEX_VTOR_INIT to enforce a different
  default value into the register.
- NEW: Removed the warning about the "untested M4 platform", now it is
  tested and officially supported.
- NEW: Reorganized the STM32F1xx hal_lld_xxx.h files in order to distribute
  the capability macros into the appropriate file (previously those were all
  in the common hal_lld.h).
- NEW: Added HAL, Serial, ADC, EXT, GPT, ICU, PWM, SPI and UART support for
  the STM32F4xx sub-family.
  TODO: Add CAN and SDC, the drivers need to be ported and tested.
- NEW: Added handling of USART6 to the STM32 serial driver.
- NEW: Added USE_COPT setting to all makefiles, contributed by Mabl.
- NEW: Added EXT driver implementation for AT91SAM7x, contributed by Florian.
  TODO: Test application missing.
- NEW: Updated USB driver model and STM32 implementation and fixed several
  problems.
  - Changed the API to move buffer copy operations out of critical zones.
  - Added usbConnectBus() and usbDisconnectBus() functions.
  - Fixed problems with incorrect assertions.
- NEW Updated the SERIAL_USB driver to match the new USB API, also fixed
  some problems.
  - Fixed incorrect use of input queues, the change required a change in
    input queues too.
- NEW: Added a macro THD_STATE_NAMES to chthreads.h. This macro is an
  initializer for string arrays containing thread state names.
- NEW: Added memory copy functionality to the STM32 DMA driver.
- NEW: Implemented new makefile system for ARM GCC ports, now objects,
  listings and output files are generated into a "build" directory and not
  together with sources, also implemented a simplified output log mode.
  Now makefiles and load script files are requirements and trigger a
  rebuild if touched.
- NEW: Updated AVR demos to use the new PAL driver.
- NEW: Added Keil build files to the STM32L-Discovery demo.
- CHANGE: Now the callback associated to input queues is invoked before
  reading each character. Previously it was invoked only before going
  to sleep into the THD_STATE_WTQUEUE state.
- CHANGE: Moved the STM32 DMA helper drivers files under the sub-family
  specific directories because documentation issues.

*** 2.3.3 ***
- FIX: Fixed missing UART5 definition in STM32 HAL (bug 3411774)(backported
  to 2.2.8).
- FIX: Fixed uninitialized variable in STM32 PWM and ICU drivers (bug 3413558).
- FIX: Fixed wrong parameter passed to the DMA error hook in STM32 ADC driver,
  the DMA error hook has been removed entirely in the new ADC driver model
  (bug 3413214).
- FIX: The function chThdExit() triggers an error on shell return when the
  system state checker is enabled (bug 3411207)(backported to 2.2.8).
- FIX: Some ARMCMx makefiles refer the file rules.mk in the ARM7 port (bug
  3411180)(backported to 2.2.8).
- FIX: Fixed wrong check on CH_DBG_ENABLE_STACK_CHECK setting (bug 3387671)
  (backported to 2.2.7).
- FIX: Fixed wrong APB1 frequency check (bug 3361039)(backported to 2.2.7).
- FIX: Fixed missing state in shell demos (bug 3351556)(backported to 2.2.7).
- FIX: Fixed race condition in Cortex-Mx ports (bug 3317500)(backported
  to 2.2.6).
- FIX: Fixed wrong macro check in STM32 UART driver (bug 3311999)(backported
  to 2.2.6).
- FIX: Fixed wrong macro definition in ARMv6-M architecture files (bug
  3310084).
- FIX: Fixed race condition in output queues (bug 3303908)(backported
  to 2.2.4).
- FIX: Fixed CH_USE_HEAP and CH_USE_MALLOC_HEAP conflict (bug 3303841)
  (backported to 2.2.4).
- FIX: Fixed timeout problem in the lwIP interface layer (bug 3302420)
  (backported to 2.2.4).
- NEW: Added AVR implementation of the PAL driver contributed by Leszek.
- NEW: STM32L ADC driver implementation.
- NEW: Improved ADC driver model, now it is possible to handle error
  conditions during the conversion process.
- NEW: STM32L1xx sub-family support, all STM32 drivers adapted and re-tested
  on the new platform except ADC that will need a specific implementation. 
- NEW: Added new API chThdExitS() in order to allow atomic operations on
  thread exit (backported to 2.2.8).
- NEW: New EXT driver model and STM32 implementation.
- NEW: New I2C driver model and STM32 implementation.
  (evaluate the option to change the API to a synchronous model)
- NEW: New RTC driver model and STM32 implementation.
  (API and functionality review)
- NEW: Improved MAC driver model, it now follows the same template of other
  drivers.
  TODO: implement macStop() in AT91SAM7X implementation.
- NEW: New RCC helper driver for STM32F1xx and STM32L1xx, it simplifies
  the use of the RCC resources and hides most differences found among the
  various STM32 sub-families.
- NEW: New DMA helper driver for STM32, it simplifies the use of the DMA
  resources and hides most differences with the new enhanced DMA units
  found in the STM32F2xx sub-family.
- NEW: Now an error is generated at compile time when trying to enable the
  options CH_DBG_ENABLE_STACK_CHECK on ports that do not support it.
- NEW: Added a kernel-only Cortex-Mx demo as reference project for users not
  interested in the HAL but just want to use the ChibiOS/RT kernel.
  The demo is named ARMCM3-GENERIC-KERNEL and is defaulted to the STM32, in
  order to use it on other families or on the ARM Cortex-M0 just change the
  inclusion paths in the makefile.
- NEW: Integrated new FatFs version 0.8b.
- NEW: Added a new hook THREAD_CONTEXT_SWITCH_HOOK() that allows to insert
  code just before a context switch. For example this hook could be used
  in oder to implement advanced power management schemes.
- NEW: Added a new debug option CH_DBG_SYSTEM_STATE_CHECK that ensures the
  correct API call protocol. If an API is invoked out of the correct context
  then the kernel panics with a debug message.
- NEW: The ARMCMx startup file (crt0.c) now is able to fill the stack areas
  with a filler (default behavior). This is required in order to easily assess
  the stack usage at runtime.
- NEW: Added the new CMSIS 2.1 headers, now CMSIS resides into a shared
  location: ./os/ports/common/ARMCMx/CMSIS. Old CMSIS files have been
  removed from the various platforms.
- NEW: Removed all the ch.ld files from the ARMCMx demos, now the makefiles
  point to common ld files under the various ports. Less duplication and
  easier maintenance.
- NEW: Improved stack checking and reorganized memory map for the Cortex-Mx
  demos. Now stacks are allocated at the start of the RAM, an overflow of the
  exception stack now triggers an exception (it could go unnoticed before).
  The process stack is organized to be checked on context switch like other
  threads. Now all threads have an explicit stack boundary pointer.
- NEW: Added debug plugin for Eclipse under ./tools/eclipse (backported to
  2.2.7).
- NEW: The debug macros chDbgCheck() and chDbgAssert() now can be externally
  redefined. The macro chDbgCheck() no more includes the line number in the
  description because incompatibility with the Cosmic compiler (backported to
  2.2.7).
- NEW: Added provisional support for STM32F2xx. Because of this some
  directories related to the STM32 have been renamed, your makefiles may
  require adjustments.
- NEW: Added a custom rule to the various rules.mk files, now it is possible
  to add an user rule into the Makefiles.
- NEW: Improvements to the trace buffer, now it stores a full thread pointer
  and event time, changed names to debug variables by adding the "dbg_"
  prefix (backported to 2.2.7).
- NEW: Added a new functionality to the registry subsystem, now it is possible
  to associate a name to the threads using chRegSetThreadName. The main and
  idle threads have their name assigned by default (backported to 2.2.7).
- NEW: Added TIM8 support to the STM32 GPT, ICU and PWM drivers.
- NEW: Updated the STM32 header file to the latest version 3.5.0 and fixed
  it in order to correct several bugs related to the XL family.
- NEW: Added a chprintf() function to ./os/various, it can print on any
  BaseChannel.
- NEW: Improved the mini shell, enhanced info command, optimizations and
  removed the shellPrint() and shellPrintLine() functions, now it uses
  chprintf() for output.
- NEW: lwIP 1.4.0 has been integrated, this new version does not require
  custom hooks into the Thread structure and is thus much lighter.
- NEW: FatFs demo for the STM32F103ZG using the SDC driver.
- NEW: Now the STM32 SDC driver supports unaligned buffers transparently.
  Optimized the driver for single block read and write operations. Optimized
  the driver state machine.
- NEW: Finished the reorganization of the Cortex-Mx ports, now also the
  IAR and RVCT ports support the new Compact mode.
- NEW: Added to the ARMv6-M sub-port an option to use the PendSV exception
  instead of NMI for preemption (backported to 2.2.5).
- NEW: Reorganization of the Cortex-Mx ports in order to reduced code and
  comments duplication in the various headers (backported to 2.2.5).
- NEW: Improved the ARMv7-M sub-port now there are two modes: Compact and
  Advanced.
  The advanced mode is equivalent to the previous versions, the compact mode
  is new and makes the kernel *much* smaller and generally faster but does
  not support fast interrupts (backported to 2.2.5).
- NEW: Now the port layer exports info regarding the compiler and the port
  options. The info are printed into the test reports. Date and time also
  added.
- CHANGE: Removed the option CH_USE_NESTED_LOCK, lwIP no more requires it and
  it would have conflicted with CH_DBG_SYSTEM_STATE_CHECK which is far more
  useful.
- CHANGE: Renamed the scheduler functions chSchIsRescRequiredExI() to
  chSchIsPreemptionRequired(), chSchDoRescheduleI() to chSchDoReschedule(),
  chSysSwitchI() to chSysSwitch(). All those functions were special cases
  and not regular I-class APIs.
- CHANGE: Renamed the macros IDLE_THREAD_STACK_SIZE and INT_REQUIRED_STACK
  to PORT_IDLE_THREAD_STACK_SIZE and PORT_INT_REQUIRED_STACK for consistency.
- CHANGE: Removed the "old" Cortex-M3 port from the code, the current port
  has no drawbacks and the old port is now just a maintenance cost.
- CHANGE: Removed the CH_CURRP_REGISTER_CACHE option, it is GCC-specific so
  it does not belong to the kernel options. The feature will be eventually
  reimplemented as a port-specific option.
- CHANGE: chiQGetFullI() and chOQGetFullI() become macros. The queues
  subsystem has been optimized and is no more dependent on semaphores.
  Note that the queues callbacks invocation policy has been slightly
  changed, see the documentation (backported to 2.2.4).

*** 2.3.2 ***
- FIX: Fixed invalid BRR() macro in AVR serial driver (bug 3299306)(backported
  to 2.2.4).
- FIX: Fixed missing IRQ vectors amicable names for STM32 XL devices (bug
  3298889)(backported to 2.2.4).
- FIX: Fixed wrong identifier in AVR serial driver (bug 3292084)(backported
  to 2.2.4).
- FIX: Fixed wrong macro check for STM32 XL devices (bug 3291898)(backported
  to 2.2.4).
- FIX: Fixed SPI driver restart in STM32 SPI driver implementation, also
  applied the same fix to the STM8S SPI driver (bug 3288758)(backported to
  2.2.4).
- FIX: Fixed missing state transition in ADC driver (bug 3288149)(backported
  to 2.2.4).
- FIX: Fixed missing state transition in SPI driver (bug 3288112)(backported
  to 2.2.4).
- FIX: Fixed spurious characters generated by Serial over USB driver (bug
  3276379).
- NEW: Added an option to the kernel to not spawn the Idle Thread from within
  chSysInit(), this way the application can spawn a custom idle thread or
  even use the main() thread as idle thread (backported to 2.2.4).
- NEW: Added new SDC driver model, Secure Digital Card.
- NEW: SDC driver implementation for STM32.
- NEW: Updated the STM32 header file to the latest version 3.4.0, had to
  fix a bug regarding the STM32 XL sub-family.
- NEW: New unified GCC startup file for Cortex-Mx processors, it is written
  in C instead of asm and supports constructors/destructors. Improved the
  Cortex-Mx linker scripts in all the GCC demos.
- NEW: Now it is possible to share DMA channels in the STM32 HAL thanks
  to a centralized manager. Channels are allocated when the driver is
  started and released when it is stopped.
- NEW: Added an STM32 C++ demo for the GNU compiler.
- NEW: Added an STM32F103ZG demo for the STM3210E-EVAL evaluation board.
- OPT: STM32 PWM driver implementation simplified.
- CHANGE: Now pwmChangePeriod() does not implicitly disable the active
  PWM channels.
- CHANGE: Renamed the chSemSetCounterI() function to chSemAddCounterI() and
  changed its behavior.
- CHANGE: Modified the STM32 USB-CDC test demo to spawn a shell over the USB
  serial connection.

*** 2.3.1 ***
- FIX: Fixed insufficient idle thread stack in Cortex-M0-GCC port (bug 3226671)
  (backported to 2.2.3).
- FIX: Fixed stack checking in Cortex-M0-GCC port (bug 3226657)(backported
  to 2.2.3).
- FIX: Fixed wrong checks in PAL driver (bug 3224681)(backported to 2.2.3).
- FIX: Fixed wrong checks in I/O Queues (bug 3219197)(backported to 2.2.3).
- FIX: Fixed invalid assertion in adcConvert() (bug 3205410)(backported
  to 2.2.3).
- NEW: Improvements to the PWM driver model:
  - Easier configuration similar to the GPT driver initializations, macros
    are no more required.
  - Added a new function that allows to change the PWM period on the fly,
    even from within callbacks. Formerly it was required to stop and restart
    the driver.
  - Improved driver documentation.
- NEW: Added advanced mode to the STM32 PWM driver (TIM1 only).
- NEW: Added new ICU driver model, Input Capture Unit.
- NEW: ICU driver implementation for STM32.
- NEW: Implemented stack checking in the Cortex-Mx RVCT port (backported
  to 2.2.3).
- NEW: Added support for PLL3 in STM32 HAL driver. Note, the format of the
  mcuconf.h file is changed for STM32F105/STM32F107 devices.
- NEW: Added board files for the Olimex STM32-P107.
- NEW: Improved setup packets handling in the USB driver through a specific
  callback.
- NEW: Improvements to the PAL driver and various implementation in order
  to make them more parenthesis friendly.
- OPT: Simplified Serial over USB driver configuration.
- CHANGE: Renamed the demo ARMCM3-STM32F107-GCC in ARMCM3-STM32F107 and added
  IAR and Keil projects.
- CHANGE: Now the ARMCM3-STM32F107 demo targets the board Olimex STM32-P107
  as default.
- CHANGE: Removed all the prefixes from the structure/union field names
  in the HAL subsystem.
- CHANGE: Updated the documentation to use Doxygen 1.7.4 which produces a much
  more readable output. Also modified the documentation layout to put functions
  and variables ahead of everything else in the group pages.
  Doxygen version below 1.7.4 cannot be used anymore because differences in
  templates. Note that now there are two Doxygen projects, one for generating
  the CHM file the other for plain HTML.

*** 2.3.0 ***
- FIX: Fixed race condition in CM0 ports, the fix also improves the
  ISR latency (bug 3193062)(backported to 2.2.2).
- FIX: Fixed Cortex-Mx linker scripts alignment of __heap_base__, the
  correct alignment is now enforced at runtime into core_init() in order
  to make the OS integration easier (bug 3191112)(backported to 2.2.2).
- FIX: Fixed error in function chCoreAllocI() function documentation (bug
  3191107)(backported to 2.2.2).
- FIX: Fixed minor problem with memory pools (bug 3190512)(backported to
  2.2.2).
- FIX: Stack overflow in CM0 ports when nearing interrupts saturation (bug
  3187105)(backported to 2.2.1).
- FIX: Fixed error in _BSEMAPHORE_DATA macro (bug 3184139)(backported to
  2.2.1).
- FIX: Error in MAC driver (bug 3179783)(backported to 2.2.1).
- FIX: Fixed wrong serial driver macros (bug 3173336)(backported to 2.2.1).
- NEW: Improved preemption implementation for the Cortex-M0, now it uses
  the NMI vector in order to restore the original context. The change makes
  IRQ handling faster and also saves some RAM/ROM space. The GCC port code
  now does not inline the epilogue code in each ISR saving significant ROM
  space for each interrupt handler in the system (backported to 2.2.3).
- NEW: Added "IRQ STORM" long duration tests for the STM32, LPC11xx and
  LPC11xx. The test demonstrates the system stability in a thread-intensive,
  progressively CPU-saturating, IRQ-intensive long duration test.
- NEW: Added two new functions to the events subsystem: chEvtBroadcastFlags()
  and chEvtBroadcastFlagsI(). The old chEvtBroadcast() and chEvtBroadcastI()
  become macros. The new functions allow to add the same flags to all the
  registered listener threads.
- NEW: Added I-Class functions to the MailBoxes subsystem, now it is
  possible to use them as a transport layer between ISRs and Threads
  (backported to 2.2.2).
- NEW: Added new USB driver model, probably it will evolve in next
  releases.
- NEW: USB driver implementation for STM32.
- NEW: Added "serial over USB" driver, it implements a Communication
  Device Class exposing it as a normal serial driver to applications,
  probably it will evolve in next releases.
- NEW: Added STM32 USB CDC loopback test application.
- NEW: Added new GPT driver model, General Purpose Timer. The driver
  allows to access the available timers in an abstract way.
- NEW: GTP driver implementation for STM32, LPC13xx and LPC11xx.
- NEW: Added STM32 GPT test application.
- NEW: Implemented new event IO_TRANSMISSION_END in the generic serial
  driver. This event marks the physical transmission end of a data stream.
- NEW: Implemented the new IO_TRANSMISSION_END event in the STM32 serial
  driver.
- NEW: Added explicit reset of all peripherals inside the STM32 HAL driver.
  Removed the separate resets on initialization from the various other
  drivers saving significant space.
- OPT: Removed TIMx reset on system startup in the STM32 PWM driver, the
  timers are already reset on driver startup.
- CHANGE: The functions chEvtSignal() and chEvtSignalI() have been renamed
  to chEvtSignalFlags() and chEvtSignalFlagsI() for consistency.
- CHANGE: Swapped the numeric values of the TIME_IMMEDIATE and TIME_INFINITE
  constants. Fixed the relative documentation in various places (backported
  to 2.2.2).
- Many documentation improvements.

*** 2.1.8 ***
- FIX: Fixed error in STM32 ADC driver macro names (bug 3160306)(backported
  to 2.0.11).
- FIX: Fixed IAR Cortex-Mx port memory organization problem (bug 3158776).
- FIX: Fixed STM32F103 demo's incorrect clock settings (bug 3153746).
- NEW: Added OTG clock setting to the STM32 HAL.
- NEW: Added stack check support to the IAR Cortex-Mx port.
- CHANGE: Removed .uvopt files from the Keil projects.

*** 2.1.7 ***
- FIX: Fixed various errors in the HAL documentation (bug 3153591).
- FIX: Fixed error in chIOGetxxxxxEventSource() macros (bug 3153550)
  (backported to 2.0.10)
- FIX: Fixed error in STM32 unbuffered UART driver (bug 3153437).
- FIX: Fixed wrong macro check in LPC214x driver (bug 3152510).
- FIX: Fixed switch condition error in STM32 PWM driver (bug 3152482)
  (backported to 2.0.10).
- FIX: Fixed error in output queues static initializer (bug 3149141)
  (backported to 2.0.9).
- FIX: Fixed extra notifications in input queues (bug 3148525)(backported
  to 2.0.9).
- NEW: New ARM Cortex-Mx port for RVCT compiler.
- NEW: Integrated the various Cortex-Mx GCC, IAR and RVCT demos in unified
  demos with multiple project files, the code is exactly the same. Renamed
  the directories removing the compiler suffix.
- NEW: Added an USB clock configuration in the STM32 HAL driver (LD, MD, HD).
- NEW: New semaphore API chSemSetCounterI().
- NEW: New queue APIs chIQGetFullI() and chOQGetFullI().
- CHANGE: Serial drivers now have a single event source instead of three,
  the event source is located in the superclass, this allows to create
  alternative implementations of the serial driver while keeping compatibility,
  the change also allowed to save 8/4 RAM bytes per serial driver.
- CHANGE: Modified the ADC and CAN drivers to allow a NULL pointer for
  the configuration structure if it is not required by the implementation.
- CHANGE: Modified the MMC_SPI driver to *require* a NULL as pointer to
  the configuration.
- CHANGE: Removed enforced inlining for the chSchReadyI() function when
  the CH_OPTIMIZE_SPEED is enabled. Now the matter is left to the compiler
  specific settings. This change is meant to increase compatibility with
  compilers that have a different inlining semantic than GCC when not in
  C99 mode.
- CHANGE: Changed the declaration of the main() function in all demos
  without arguments. Removed the clearing of arguments from the startup
  files and saved some space.
- CHANGE: Queues callbacks now have as parameter a pointer to the queue,
  there were no parameters previously.
- Documentation related fixes.

*** 2.1.6 ***
- FIX: Fixed error in sdPutTimeout() macro (bug 3138763)(backported in 2.0.9).
- NEW: New ARM Cortex-Mx port for IAR compiler.
- NEW: Now the STM32 CAN driver puts the lower half word of the ESR
  register in the upper half word of the can status word for easier
  debug.
- CHANGE: Changes in the board files organization, now the board
  initialization is invoked from within halInit() after all the device
  drivers have been initialized. Now applications are required to
  explicitly invoke halInit() and chSysInit() from within their main().
- CHANGE: Removed the CMSIS files from the ARMCMx port, added the headers
  into the various HAL platforms requiring them. The change is required
  because the port layer must not have vendor specific dependencies and
  there is the possibility that the various vendors would use different
  CMSIS versions now that CMSIS 2.x has been released.
- CHANGE: Modified the start of the ADC in the STM32 ADC driver, now it is
  no more required to specify ADC_CR2_EXTSEL_SWSTART and ADC_CR2_CONT
  in the CR2 register configuration. Also reordered the start sequence
  in order to allows a longer stabilization time for the ADC.
- Documentation improvements.

*** 2.1.5 ***
- FIX: Fixed references to non-existing SSP1 device in LPC13xx SPI device
  driver (bug 3127926).
- FIX: Fixed broken SPI synchronous API (bug 3127921).
- FIX: Fixed missing vector.c files in LPC11xx and LPC13xx ports (bug 3124849).
- FIX: Fixed pwmDisableChannel() now working in STM32 PWM driver (bug 3121246).
- FIX: Fixed problem with PWM channel callbacks (bug 3120785).
- NEW: Added support for TIM5 in the STM32 PWM driver.
- NEW: Added to the ARM port the option to externally redefine the port IRQ
  macros in order to accomodate different implementations without have to
  change the port layer.
- CHANGE: Modified the STM32_PWM_PWMx_IRQ_PRIORITY macros in the STM32
  PWM driver (and all the STM32 mcuconf.h files) and renamed them in
  STM32_PWM_TIMx_IRQ_PRIORITY for consistency.
- Documentation related fixes.

*** 2.1.4 ***
- FIX: Fixed failed memory recovery by registry scan, improved the related
  test case (bug 3116888)(backported to 2.0.8).
- FIX: Fixed failure in STM32 ADC driver when a linear buffer mode is used
  (bug 3114696).
- FIX: Fixed PWM channels going to ACTIVE state when the pulse width is
  set to zero in the STM32 PWM driver (bug 3114481)(backported to 2.0.8).
- FIX: Fixed PWM channels return to IDLE state in STM32 PWM driver (bug
  3114467)(backported to 2.0.8).
- FIX: Fixed wrong initializer macros in STM32 PWM driver (bug 3114319).
- FIX: Fixed syntax error in STM32 PWM driver(bug 3114266).
- FIX: Fixed typo in board name (bug 3113574)(backported to 2.0.7).
- FIX: Fixed defective event wait functions with timeout (bug 3113443)
  (backported to 2.0.7).
- NEW: More improvements to the ADC and SPI drivers, now synchronous
  operations can also have callbacks, optimized ISR code paths.
- NEW: Added to the STM32 ADC driver the macros for easy handling of the
  sampling time for each channel.
- NEW: Greatly simplified the STM32 PWM driver implementation.
- NEW: Added new macro PWM_FRACTION_TO_WIDTH() to the PWM driver model.
- NEW: ARM7 port reorganization following the same pattern of the ARMCMx
  one, on now the port is also supports the ARM9 architecture (but not
  tested yet hardware). Removed the dependencies between the port layer
  and the vendors-provided headers.
- NEW: Initial STM8L support: HAL, PAL and Serial drivers.
- NEW: Added demo for the ST STM8L-Discovery kit.
- NEW: Added support for the STM32 Value Line to the HAL.
- NEW: Added demo for the ST STM32VL-Discovery kit.
- OPT: STM32 SPI driver implementation improved, reduced latency when
  starting a transfer.
- CHANGE: Improved the STM32 HAL to support multiple sub-families, at compile
  time now it is possible to test the presence of any single peripheral into
  the specified STM32 device.
- CHANGE: Separated dynamic threads code into dedicated files: chdynamic.c
  and chdynamic.h.
- CHANGE: Moved the declaration of the variable pal_default_config from
  hal_lld.c to the various board.c. That structure is more meaningful in
  the board layer because it is related to the board initialization.
- CHANGE: Removed the various assembler files for the STM32 interrupt
  vectors, now it is a single vectors.c file.
- CHANGE: Deleted ./os/ports/GCC/ARM.
- CHANGE: Renamed ./os/ports/GCC/ARM7 in ./os/ports/GCC/ARM.
- CHANGE: Renamed ./os/hal/platforms/STM8 in ./os/hal/platforms/STM8S.
- CHANGE: Renamed ./testhal/STM8 in ./testhal/STM8S.
- Added SPI driver to the STM8S documentation, it was missing.
- Improved documentation for the ARM and ARMCMx ports.

*** 2.1.3 ***
- FIX: Fixed broken STM32 synchronous UART driver (bug 3100946).
- FIX: Broken C++ wrapper (bug 3100925).
- FIX: Broken lwIP/uIP demos (bug 3100901).
- FIX: DMA not initialized under some conditions (bug 3099701).
- FIX: Restored the RIDE7 build files in the STM32F103 demo, note the RIDE7
  build files are not supported so this has not been tracked as a bug.
- NEW: Added an SPI driver to the STM8 platform support.
- NEW: Added a simple STM8 SPI demo under ./testhal/STM8/SPI.
- NEW: Divided the file STM32/vectors.s in several files, one for each
  STM32 sub-family. This has been done in order to remove the preprocessor
  directives from assembler files and to support all the various STM32
  sub-families.
- CHANGE: Renamed ./os/ports/GCC/ARMCMx/STM32F10x in STM32.
- CHANGE: Divided the file ARMCMx/crt0.s in crt0_v6m.s and crt0_v7m.s in
  order to remove the preprocessor directives from assembler files.
- CHANGE: Renamed the HAL settings macro names, removed the CH_ prefix
  because it is reserved for the kernel namespace.
      NOTE: ****** Make sure to use a mcuconf.h file taken from ******
            ****** this version in your project.                ******

*** 2.1.2 ***
- FIX: Fixed typo in memstreams.h (bug 3089567)(backported to 2.0.6).
- FIX: Fixed wrong macro check in LPC214x and AT91SAM7 serial drivers (bug
  3088776)(backported to 2.0.6).
- FIX: Fixed non functioning option SPI_USE_MUTUAL_EXCLUSION=FALSE (bug
  3084764)(backported to 2.0.6).
- FIX: Fixed wrong macro check in STM32 serial support (bug 3078891)(backported
  to 2.0.6).
- FIX: Fixed non functioning option CH_USE_NESTED_LOCKS (bug 3075544)
  (backported to 2.0.6).
- FIX: Incorrect AT91SAM7X initialization, thanks Leszek (bug 3075354)
  (backported to 2.0.5).
- FIX: Fixed race condition in function chSchGoSleepTimeoutS(), thanks Bal�zs
  (bug 3074984)(backported to 2.0.5).
- FIX: Fixed race condition in threads creation (bug 3069854)(backported
  to 2.0.5).
- FIX: Fixed broken CH_DBG_ENABLE_STACK_CHECK option in legacy CM3 port (bug
  3064274)(backported to 2.0.5).
- FIX: Fixed CAN_USE_SLEEP_MODE setting (bug 3064204)(backported to 2.0.5).
- FIX: Fixed potential issue with GCC reorganizing instructions around "asm
  volatile" statements (bug 3058731)(backported in 2.0.4).
- FIX: Fixed reduced ARM7 performance with GCC 4.5.x (bug 3056866)(backported
  to 2.0.4).
- FIX: Fixed crash of the Posix simulator under Ubuntu 10.4 (bug 3055329)
  (backported to 2.0.3).
- FIX: Fixed incorrect PLL2 setting in STM32 HAL (bug 3044770)(backported
  in 2.0.3).
- FIX: Fixed wrong check on STM32_HCLK (bug 3044758)(backported to 2.0.3).
- FIX: Fixed wrong condition check in STM32 PWM driver (bug 3041414)
  (backported in 2.0.3).
- FIX: Corrupted IRQ stack in Cortex-Mx port (bug 3041117)(backported to
  2.0.3).
- FIX: Fixed a documentation error regarding the ADC driver function
  adcStartConversion() (bug 3039890)(backported to 2.0.3).
- NEW: New I2C driver model (not complete and no implementations yet).
- NEW: New SPI driver model, the new model supports both synchronous and
  asynchronous operations and, in general, simplifies the implementation of the
  low level driver. The new driver also specifies a simplified polled exchange
  API, useful for efficiently sending small amount of data over high speed
  connections. The driver state diagram changed slightly changed so be careful.
- NEW: New ADC driver model, the new model supports both synchronous and
  asynchronous operations and, in general, simplifies the implementation of the
  low level driver. The driver state diagram changed slightly changed so be
  careful.
- NEW: Improved PWM driver model, added several macros that helps to setup
  configuration structures and to specify pulse widths also as percentages or
  degrees using a fixed point notation. Added new pwmEnableChannelI() and
  pwmDisableChannelI() APIs in order to allow channel reprogramming from
  within callbacks or interrupt handlers, the new APIs are implemented as
  macros so there is no footprint overhead.
- NEW: Added driver fields and initialization hooks for the callback-based
  drivers. The hooks are named XXX_DRIVER_EXT_FIELDS and
  XXX_DRIVER_EXT_INIT_HOOK().
- NEW: Added to the UART driver the capability to return the number of
  not yet transferred frames when stopping an operation.
- NEW: Added more compile-time checks to the various STM32 device drivers.
- NEW: Improved LPC214x SPI driver, now it uses IRQs, it implements the
  new SPI device driver model.
- NEW: Improved AT91SAM7 SPI driver, now it uses IRQs and DMAs, it implements
  the new SPI device driver model.
- NEW: New LPC11xx SPI driver, updated the LPCXpresso demo to use the SPI.
- NEW: New LPC13xx SPI driver, updated the LPCXpresso demo to use the SPI.
- NEW: Added a simple STM32 ADC demo under ./testhal/STM32/ADC.
- NEW: Added a simple STM32 CAN demo under ./testhal/STM32/CAN.
- NEW: Added a simple STM32 PWM demo under ./testhal/STM32/PWM.
- NEW: Added a simple STM32 SPI demo under ./testhal/STM32/SPI.
- NEW: More assertions added to the kernel for improved bug fixing.
- NEW: New kernel hooks: SYSTEM_TICK_EVENT_HOOK(), SYSTEM_HALT_HOOK().
- NEW: Added board files for the Olimex STM32-H103.
- NEW: New kernel APIs chSysGetIdleThread() and chThdGetTicks(), the new
  APIs are simple macros so there is no footprint overhead.
- NEW: Added a generic BaseFileStream interface for future File System
  implementations or integrations (untested and not sure if it will stay or
  change).
- NEW: Added to the documentation more notes about interrupt handlers in
  the ARM7 port.
- NEW: Modified some tests in order to bring back code coverage to 100%
  in all modules except chdebug.c. Added a test case covering binary
  semaphores.
- OPT: The fix to the bug 3075544 considerably improved the threads creation
  benchmarks score.
- OPT: Speed optimizations to the STM32 SPI driver, greatly improved latency.
- OPT: Speed optimizations to the STM32 ADC driver.
- CHANGE: The API chThdInit() has been renamed to chThdCreateI() in order to
  make clear it is usable from interrupt handlers.
- CHANGE: The mailboxes macros chMBSize(), chMBGetEmpty(), chMBGetFull(),
  chMBPeek() have been renamed to chMBSizeI(), chMBGetFreeCountI(),
  chMBGetUsedCountI(), chMBPeekI().
- CHANGE: The queue APIs chQSize(), chQSpace(), chIQIsEmpty(), chIQIsFull(),
  chOQIsEmpty(), chOQIsFull() have been renamed to chQSizeI(), chQSpaceI(),
  chIQIsEmptyI(), chIQIsFullI(), chOQIsEmptyI(), chOQIsFullI().
- CHANGE: The event APIs chEvtPend() and chEvtClear() have been renamed
  to chEvtAddFlags() and chEvtClearFlags() for consistency and correct
  English. Changed the macro chEvtIsListening() in chEvtIsListeningI().
- CHANGE: Added a parameter to the PWM driver callbacks, the pointer to the
  driver itself.
- CHANGE: Added a parameter to the UART driver callbacks, the pointer to the
  driver itself.
- CHANGE: In the UART driver now an error does not automatically brings the
  receiver in UART_RX_IDLE state, the receive operation continues unless
  explicitly stopped in the error callback handler. This considerably
  simplifies the low level driver code.
- CHANGE: Removed a spurious trailing ; after the WORKING_AREA() macro
  declaration in several ports. The ; should go after the macro invocation.
- CHANGE: Modified the STM32 ADC driver to use the new DMA infrastructure.
- CHANGE: Modified the STM32 SPI driver to use the new DMA infrastructure.
- CHANGE: Added DMA cleanup code to the STM32 dmaInit() function.
- CHANGE: Simplified preprocessor conditions in the STM32 serial driver.
- CHANGE: Renamed most of the STM32 HAL settings macro names in order to
  make names more consistent.
      NOTE: ****** Make sure to use a mcuconf.h file taken from ******
            ****** this version in your project.                ******
- CHANGE: Renamed the existing system hooks by adding the _HOOK suffix to
  the name in order to make names more consistent.
      NOTE: ****** Make sure to use a chconf.h file taken from  ******
            ****** this version in your project.                ******
- CHANGE: Extensive documentation improvements, fixed terminology in the
  events related documentation and articles.
- CHANGE: The documentation is no more included in the distribution, the
  file ./documentation.html redirects to the online documentation page
  that contains *much* better documents.
  Note that it is still possible to generate the local documentation using
  Doxygen, the procedure is very simple and described in ./docs/readme.txt.

*** 2.1.1 ***
- FIX: Fixed insufficient stack size for idle thread (bug 3033624)(backported
  to 2.0.3).
- FIX: Fixed misspelled word in some chioch.h and chstreams.h macros (bug
  3031534)(backported to 2.0.3).
- FIX: Fixed wrong macro check in the STM32 SPI driver (bug 3028562)(backported
  to 2.0.3).
- FIX: Fixed invalid context restore in MSP430 port (bug 3027975)(backported
  to 2.0.2).
- FIX: Fixed STM32 vectors file (bug 3026528)(backported to 2.0.2).
- FIX: Fixed race condition in STM32 SPI driver (bug 3025854)(backported to
  2.0.2).
- FIX: Fixed H_LOCK and H_UNLOCK redefined with CH_USE_MALLOC_HEAP (bug
  3025549)(backported to 2.0.2).
- FIX: Added option to enforce the stack alignment to 32 or 64 bits in the
  Cortex-Mx port (bug 3025133)(backported to 2.0.2).
- NEW: Centralized DMA macros in the STM32 HAL.
- NEW: New UART device driver model, this device driver allows unbuffered,
  callback driven access to UART-type devices.
- NEW: UART device driver for STM32 and UART demo application under
  ./testhal/STM32/UART.
- NEW: Added friendly interrupt vectors names to the STM32 HAL (change request
  3023944).
- NEW: Added support for SPI3 in the STM32 HAL.
- CHANGE: Redeclared the IRQ handlers in the various STM32 drivers using the
  new friendly vector names.
- CHANGE: Moved the STM32 HAL stess test application under
  ./testhal/STM32/_stress_test.
- CHANGE: Removed the option -mabi=apcs-gnu from all the Cortex-Mx demos. The
  option is not compatible with the 64 bits stack alignment now default in
  the Cortex-Mx port. Note that the 64 bits alignment has a cost both as
  performance and as space but it is the "standard".
- OPT: Small speed optimization in the STM32 SPI driver.
- OPT: Optimized DMA clearing in STM32 ADC and SPI drivers, there was no need
  to read/modify/write the IFCR DMA register, it is write only.
- Fixed various documentation errors.

*** 2.1.0 ***
- FIX: Fixed notification order in input queues (bug 3020708)(backported in
  2.0.1).
- FIX: Fixed non functional CH_CURRP_REGISTER_CACHE option in the Cortex-M3
  port (bug 3020702)(backported in 2.0.1).
- FIX: Fixed non functional CH_DBG_ENABLE_STACK_CHECK option in the Cortex-M3
  caused by GCC 4.5.0, the fix also improves the context switch performance
  because GCC 4.5.0 apparently was generating useless instructions within the
  very critical context switch code (bug 3019738)(backported in 2.0.1).
- FIX: Fixed insufficient stack space assigned to the idle thread in
  Cortex-M3 port (bug 3019594)(backported in 2.0.1).
- FIX: Fixed missing check in chIQReadTimeout() and chIQWriteTimeout() (bug
  3019158)(backported in 2.0.1).
- FIX: Fixed instability in Mutexes subsystem (bug 3019099)(backported
  in 2.0.1).
- FIX: Fixed broken AVR port (bug 3016619)(backported in 2.0.0).
- FIX: Fixed assertion in adcStop() (bug 3015109)(backported in 2.0.0).
- NEW: Merged the new unified STM8 port, now the port supports both the
  Cosmic and Raisonance compilers.
- NEW: Added an STM8-DISCOVERY demo, the demo can be compiled with both the
  Cosmic and Raisonance compilers under the STDV IDE. Performance reports
  have been added for both compilers.
  NOTE: The Raisonance compiler seems to have problems in debugging under
        the STDV IDE, if you need to debug using the Raisonance compiler
        consider using the RIDE7 IDE instead.
        Compiling and executing using the "release" mode under STDV works fine.
- NEW: Added timers clock macros to the STM32 clock tree HAL driver (backported
  in 2.0.1).
- NEW: Added Binary Semaphores among the synchronization primitives. The new
  subsystem is entirely implemented as macros over the existing and proven
  Counting Semaphores thus takes no space (experimental).
- NEW: Added a simplified SPI driver for AT91SAM7 devices, contributed by
  Alexander (experimental).
- NEW: Added FatFs demos for AT91SAM7S and AT91SAM7X, contributed by
  Alexander (experimental).
- OPT: Simplified the test suite code, now it is smaller.
- Reorganized the documentation, now the description of the device drivers
  implementation is under the HAL module instead of the Ports module.

*** 1.5.9 ***
- FIX: Fixed STM8 baud rate setup error (bug 3010990).
- FIX: Fixed STM8 UART3 driver (bug 3009145).
- NEW: Added a STM8_ENABLE_WFI_IDLE option to the STM8 port, the default
  is FALSE.
- OPT: Small size optimization in the semaphores subsystem.
- OPT: Improvements in the STM8 port, the code is now smaller and generally
  faster, also saved few bytes of RAM.
- Added explicit casts in chevents.h, chqueues.h, chqueues.c in order to
  silence some warnings on a specific compiler.
- Added a section in chconf.h where redefine the port-related configuration
  parameters, also added notes into the documentation about this.

*** 1.5.8 ***
- FIX: Fixed missing files from ST library zip file (bug 3006629).
- NEW: Added a demo for the AT91SAM7S256 and board files for the Olimex
  SAM7-P256. The demo has been contributed by Alexander Kozaruk.
- Fixed some wrong paths into the demos readme files.
- Formatting fixes to the port templates.
- Added notes about copyright assignment to the documentation. Fixed some
  problems in the licensing FAQ page.
- Added documentation article about events in ChibiOS/RT.

*** 1.5.7 ***
- FIX: Fixed missing SPI driver reinitialization in the MMC driver (bug
  3005628)(backported in 1.4.4).
- FIX: Fixed wrong inclusion order of board.h and halconf.h into hal.h
  (bug 3005041)(backported in 1.4.4).
- FIX: Fixed wrong GPIO ports size in the STM8 PAL driver (bug 3001528).
- NEW: Improved clock initialization for the STM32, now it is possible to
  configure the clock using any clock source and any HSE frequency.
- NEW: The STM32 clock tree parameters and checks are now calculated into
  a separate file in order to support multiple clock trees for different
  sub-families of the STM32 platform.
- NEW: Added separated clock trees for the STM32 LD/MD/HD sub-family and
  the CL sub-family. Now the selection of the sub-family is done in the
  board.h file, there is no more the need to put -DSTM32F10X_xx into
  the makefile.
- NEW: Added the palSetBusMode() capability to the STM8 PAL driver.
- NEW: Added the palSetBusMode() capability to the LPC11xx and LPC13xx
  PAL drivers.
- NEW: Updated the STM32 FW Library files to latest version 3.3.0.
- CHANGE: Renamed the STM32, STM8, LPC214x, AT91SAM7x, MSP430 and simulators
  PAL configuration structures to PALConfig, it is better to have the same
  name for this structure in all ports (like for all the other drivers).
- CHANGE: Modified the linker scripts in the ARM demos in order to increase
  compatibility with the CodeSourcery toolchain.
- Tested the STM8 port with the latest RKit-STM8_2.28.10.0092. It works but
  the new compiler shows a slight performance regression except in one
  test case.
- Added credits page to the documentation, improved the article regarding
  timings.
- Performed another documentation revision cycle, fixed more bad English and
  few errors.

*** 1.5.6 ***
- FIX: Fixed centralized ARM makefile (bug 2992747)(backported in 1.4.3).
- FIX: Fixed write problems in MMC_SPI driver (bug 2991714)(backported in
  1.4.3).
- FIX: Fixed wrong macros in chioch.h (bug 2989468).
- FIX: Fixed wrong macro check in serial.h (bug 2989459)(backported in 1.4.3).
- NEW: Added a ROMCONST macro in chtypes.h, this macro must be used for
  constant that must be placed in code space, it should not be assumed that
  the "const" keyword does that. Note that this macro is not used to place
  constants in different address spaces (AVR) because it is assumed that a
  pointer to a ROMCONST variable is compatible with a normal pointer.
- NEW: AT91SAM7 HAL support for the DGBU UART peripheral, as SD3.
- NEW: Introduced a new macro CH_FAST_IRQ_HANDLER() for the declaration of
  fast interrupt handlers on those architectures that support them.
- OPT: Internal optimization in the serial driver, it now is a bit smaller
  and uses less RAM (all architectures).
- CHANGE: Modified the STM32 FatFs demo, now it spawns a command shell or
  the serial port SD2, type "help" for the available commands. More commands
  can be easily added.
- CHANGE: Renamed the chCoreFree() function in chCoreStatus() because it
  might be mistaken for a function that frees memory.
- CHANGE: All ARM demos makefiles now defaults to arm-none-eabi- GNU
  toolchains because the latest YAGARTO now uses that setting. It is still
  possible to use arm-elf- toolchains by editing the TRGT variable in the
  makefiles.
- Various documentation fixes, added new articles covering debugging under
  ChibiOS/RT, OS integration, OS stop and restart. Updated the article about
  interrupt handlers to cover also fast interrupt sources, updated the article
  about the OS porting.
- Long overdue test code cleanup and documentation.
- Added new test cases, now the coverage is again up to 100% except for the
  debug module that would require triggering system terminating tests (panics),
  the uncovered code is minimal, extremely simple and working anyway.
- ChibiOS/RT has been successfully verified with the latest GCC 4.5.0.

*** 1.5.5 ***
- FIX: Removed some "dead" code in the old ARMv7-M files (there are new
  ones, see below).
- NEW: LPC13xx support, drivers (Serial, PAL, HAL), demo and reports.
- NEW: Added statistic info to the lwIP demo.
- CHANGE: Renamed LPC111x port and platform in LPC11xx, minor fixes to the
  platform header files.
- CHANGE: Small documentation fixes and improvements.
- OPT: New Cortex-M3 port code, *huge* performance improvements in all the
  context switching related benchmarks (up to 18% depending on the benchmark).
  The new code does no more require the use of the PendSV vector that is
  thus available to the user, it also saves four RAM bytes for each thread
  in the system. The old code is still available as a fall back option while
  the new one is being hardened by peer review and time, the two ports are
  perfectly interchangeable.

*** 1.5.4 ***
- FIX: Fixed broken CH_CURRP_REGISTER_CACHE option in the ARM7 port (bug
  2973365).
- FIX: Fixed missing memory recovery on thread reference release in
  chRegNextThread() (bug 2971878).
- FIX: Fixed wrong thread state macro in STM32/spi_lld.c (bug 2968142).
- NEW: New unified ARM Cortex-Mx port, this port supports both the ARMv6M
  and ARMv7-M architecture (Cortex-M0/M1/M3/M4 so far). The new port also
  allow to easily add to new Cortex-M implementations by simply adding a
  parameters file (cmparams.h).
- NEW: Embedded Artists LPCxpresso Base Board support files added.
- NEW: LPC111x support, drivers (Serial, PAL, HAL) and demo.
- NEW: The port layer now can "capture" the implementation of individual
  scheduler API functions in order to provide architecture-optimized
  versions. This is done because further scheduler optimizations are
  becoming increasingly pointless without considering architecture and
  compiler related constraints.
- NEW: Added support for the STM8 large memory model to the STM8 port. Now
  the assembler port code is totally inlined and the chcoreasm.asm file has
  been removed.
- NEW: Added RIDE7 project files to the STM32 demo under a ./ride7
  subdirectory, this should make things easier for RIDE7 users. The normal
  makefile is still available of course.
- NEW: New articles and guides in the documentation.
- NEW: Documentation improvements, now the description goes on top of each
  page, doxygen defaulted it in the middle, not exactly the best for
  readability. Improved many descriptions of the various subsystems. Fixed
  a misplaced page (STM8 port).
- OPT: Optimization on the interface between scheduler and port layer, now
  the kernel is even smaller and the context switch performance improved
  quite a bit on all the supported architectures.
- OPT: Simplified the implementation of chSchYieldS() and made it a macro.
  The previous implementation was probably overkill and took too much space
  even if a bit faster.
- CHANGE: Modified the Cortex-M3 port to be a more generic Cortex-Mx port,
  changes were required to the startup code and the port code because the
  reduced instruction set of the Cortex-M0 (there are two code paths now,
  both optimized).
- CHANGE: Modified the SysTick initialization for STM32 to use the system
  clock rather than the external clock.
- CHANGE: Exiting from a chCondWaitTimeout() because a timeout now does not
  re-acquire the mutex, ownership is lost.
- CHANGE: The module documentation has been moved from the kernel.dox file
  to the various source code files in order to make it easier to maintain
  and double as source comments.
- CHANGE: Updated CMSIS files to version 1.3 and fixed the warnings in there,
  again...

*** 1.5.3 ***
- FIX: Removed C99-style variables declarations (bug 2964418)(backported
  in 1.4.2).
- FIX: Fixed missing reschedule in chEvtSignal() (bug 2961208)(backported
  in 1.4.2).
- NEW: Added STM8 port and demo, the demo targets the Raisonance REva board
  with STM8S208RB piggyback.
- NEW: Enhanced the kernel size report to cover more cases.
- NEW: Improvements to the documentation.
- OPT: Minor optimizations in the "compact" code path.

*** 1.5.2 ***
- FIX: Fixed wrong UART deinitialization sequence in LPC214x serial driver
  (bug 2953985)(backported in 1.4.1).
- FIX: Fixed wrong PINSEL2 offset into lpc214x.h (bug 2953981)(backported
  in 1.4.1).
- FIX: Fixed invalid UART-related macro in the LPC214x HAL (bug 2953195)
  (backported in 1.4.1).
- FIX: Impossible to enforce alignment greater of a pointer size for heap/core
  objects (bug 2952961).
- FIX: Wrong prototype in template file chcore.c (bug 2951529)(backported
  in 1.4.1).
- NEW: Added an experimental PowerPC port targeting the SPC563M/MPC563xM
  ST/Freescale automotive SOCs. The port passed the whole test suite but it
  will be developed further in next releases.
- NEW: Added core variant name macro in chcore.h and platform name in
  hal_lld.h, the info are printed in the test report and from the "info"
  shell command.
- NEW: Added BOARD_NAME macro to the various board.h files.
- NEW: Added a MemoryStream class under ./os/various.
- CHANGE: Removed an instance of a structure without name from test.h for
  increased portability of the test suite.
- NEW: Added Mac OS-X support for the simulator. The Linux simulator has
  been renamed to Posix simulator in order to include this change in a
  single project.
- NEW: New articles, sections and various improvements to the documentation.
- CHANGE: Renamed the kernel header files to match the names of their source
  files. The change was also required in order to make the names less
  "generic" and less likely to match names in external libraries.

*** 1.5.1 ***
- FIX: Fixed insufficient stack space for the idle thread in the ARMCM3 port
  when compiling without optimizations (bug 2946233)(backported in 1.4.1).
- FIX: Fixed wrong notes on function chThdResume() (bug 2943160)(backported
  in 1.4.1).
- NEW: Implemented the concept of thread references, this mechanism ensures
  that a dynamic thread's memory is not freed while some other thread still
  owns a reference to the thread. Static threads are not affected by the new
  mechanism. Two new APIs have been added: chThdAddRef() and chThdRelease().
- NEW: Now more than one thread can be waiting in chThdWait() as long they
  own a reference.
- NEW: Implemented a new threads registry subsystem, the registry allows to
  enumerate the active threads at runtime and/or from a debugger. This is
  a preparatory step for a dedicated ChibiOS/RT debugger.
- NEW: New chCoreFree() API that returns the core memory left.
- NEW: Added to the simulators shell demos two new commands: threads and mem,
  that show the currently active threads (using the new registry) and the
  memory allocators state.
- CHANGE: Doxygen tags cleanup in all the system code, comments are better
  looking now.
- CHANGE: Documentation improvements.

*** 1.5.0 ***
- FIX: Fixed missing dependencies check for CH_USE_DYNAMIC (bug 2942757)
  (backported in 1.4.1).
- FIX: Fixed swapped thread states descriptions (bug 2938445)(backported in
  1.4.1).
- FIX_ Fixed C99-style variable declaration (bug 2938444)(backported in 1.4.1).
- FIX: Fixed parameter check in sdStart() function (bug 2932922)(backported in
  1.4.0).
- FIX: Fixed missing platform.mk file in MSP430 port (bug 2933735)(backported
  in 1.4.0).
- CHANGE: Removed the unnamed union from the thread and heaps structures,
  some compilers do not support this non standard construct.
- CHANGE: Removed the empty structures from the streams/channels headers.
- CHANGE: Modified the thread-related constant macros to have a THD_ prefix.
- CHANGE: Modified NULL assignments to function pointers to use an explicit
  cast because some compilers issue warnings without the cast.
- OPT: Speed/size optimization to the events subsystem.
- OPT: Speed/size optimization to the mutexes subsystem.
- OPT: Speed/size optimization to the condvars subsystem.
- OPT: Speed/size optimization to the synchronous messages subsystem.
- NEW: Added support for STM32/HD/CL UART4 and UART5, thanks Egon for the
  patch.

*** 1.3.8 ***
- FIX: Fixed dequeuing in lifo_remove() function (bug 2928142).
- FIX: Fixed spurious character generation in MSP430 serial driver (bug
  2926838).
- NEW: Introduced an abstract streams interface BaseSequentialStream.
- NEW: Added timeout specification to the I/O queues read/write primitives.
- NEW: Added support for HD and CL STM32 devices in the vectors table.
- CHANGE: Modified the BaseChannel interface in order to make it a
  BaseSequentialStream descendant.
- CHANGE: Updated the serial driver model in order to expose the
  BaseSequentialStream methods.
- CHANGE: The behavior of the read/write primitives is changed, now the
  functions are synchronous and do not return until the specified number of
  bytes have been transferred or a timeout occurs, the old behavior can be
  replicated by specifying TIME_IMMEDIATE as timeout. Another difference is
  that specifying zero as bytes number is like specifying the largest size_t
  plus one, zero was an illegal value before.
- CHANGE: Simplified the LPC214x driver by removing the option to not use the
  FIFO preload feature. Setting LPC214x_UART_FIFO_PRELOAD to 1 results in
  the same behavior.
- Documentation fixes and improvements, testing strategy explained.
- Added article about waking up threads from IRQ handlers.

*** 1.3.7 ***
- FIX: Fixed duplicated definition of SPI_USE_MUTUAL_EXCLUSION (bug 2922495).
- FIX: Fixed coverage tool hanging during execution (bug 2921120).
- FIX: Fixed Linux simulator startup message (bug 2921012).
- FIX: Fixed section separators comments into the HAL-related files. Now all
  the files should use the same style.
- NEW: Improved HAL configuration file.
- NEW: Introduced a new, per-project, MCU configuration file mcuconf.h that
  contains all the drivers related settings.
- NEW: Readability improvements to the channels code.
- NEW: Serial driver model improvements, added states management and checks,
  added a new SD_NOISE_ERROR error event.
- NEW: Improvements and optimizations in the various serial driver
  implementations.
- Documentation fixes and improvements.

*** 1.3.6 ***
- FIX: Fixed missing STM32 PWM low level driver error in platform.mk by
  adding the driver files (bug 2913560).
- NEW: The Linux simulator now works again, also supports the serial
  drivers over TCP/IP and has a command line interface like the Win32
  simulator.
- NEW: STM32 PWM driver implementation.
- NEW: LPC214x SPI driver implementation (SSP only, polled mode, no IRQ), this
  driver replaces the old, not HAL compatible, SSP driver.
- NEW: LPC214x FatFS demo added, LPC214x minimal demo removed, LPC214x "normal"
  demo reduced to work like all the other generic demos.
- NEW: Added custom mode settings to the STM32 PAL driver:
  - PAL_MODE_STM32_ALTERNATE_PUSHPULL
  - PAL_MODE_STM32_ALTERNATE_OPENDRAIN
- NEW: Included all the board-specific files into a new directories structure
  under ./boards, this allows to not duplicate the board files into each demo.
- CHANGE: Changes to the PWM driver model, made it simpler.
- CHANGE: The STM32 device drivers now no more configure the I/O pins on
  initialization. Pins must be configured in board.h, the change was required
  in order to support the STM32 AFIO remapping feature.
- CHANGE: Removed the mmcsd.c driver, it is replaced by the generic MMC_SPI
  driver present into the HAL.
- CHANGE: Updated the GPL exception text in the documentation, this should be
  the final text for the stable version 1.4.x.

*** 1.3.5 ***
- FIX: Fixed problem with memory core allocator (bug 2912528).
- FIX: Fixed problem with CH_USE_MEMCORE option (bug 2912522).
- FIX: Fixed some problems in the MMC_SPI driver (bugs 2901084 and 2901172).
- NEW: Added a command line shell component, modified the Win32 simulator
  in order to use the new shell. The shell can be used under any architecture,
  it just requires an I/O channel. Custom commands can be easily added.
- NEW: Unified the initialization of the various drivers from a single HAL
  driver. The single drivers can be enabled or disabled from a HAL
  configuration file halconf.h.
- NEW: New CAN driver model.
- NEW: New PWM driver model.
- NEW: STM32 ADC driver implementation with DMA support.
- NEW: STM32 CAN driver implementation.
- NEW: Extended the support to all the SAM7X and SAM7S devices thanks to
  code contributed by Liam Staskawicz.
- NEW: Improvements to the AT91SAM7 startup code contributed by Liam.
- NEW: Added test report for MSP430 running from the external high speed
  oscillator.
- NEW: HAL stress test for STM32 added, the demo is located under
  ./testhal/STM32. The demo code also shows how to use the ADC, CAN and SPI
  drivers.
- CHANGE: Removed the MII from the standard drivers, now it is part of the
  AT91SAM7 support, the header mii.h is still part of the HAL.
- CHANGE: In the STM32 drivers now the DMA errors are handled by hook macros
  rather than by events. The default action is to halt the system but users
  are able to override this and define custom handling.
- CHANGE: In the Cortex-M3 port, modified the NVICEnableVector() function
  to make it clear pending interrupts.
- CHANGE: Minor changes to the ADC driver model.

*** 1.3.4 ***
- FIX: Fixed bug in STM32 PAL port driver (bug 2897636).
- FIX: Fixed problem with ARM-CM3 context switch when compiled at level
  -O0 (bug 2890382).
- FIX: Fixed wrong conditional in chschd.c (bug 2888836).
- FIX: Fixed wrong macro in chheap.c (bug 2888833).
- FIX: Fixed AIC initialization in AT91SAM7X support (bug 2888583).
- NEW: New SPI (master) driver model.
- NEW: SPI driver for STM32 implementing the new SPI driver model.
- NEW: New ADC (streaming capable) driver model.
- NEW: Generic MMC (over SPI) driver.
- NEW: Added a STM32 demo that integrates the MMC driver and the FatFs
  file system.
- NEW: Implemented I/O redirection on a serial driver into syscalls.c, now
  it is possible (but not recommended) to use printf()/scanf() etc. An usage
  example is in the new MMC/FatFs demo. Note the extra -D... into the Makefile.
- CHANGE: Moved the STM32 firmware library under ./ext, this way there is no
  need to duplicate it in each demo program.
- CHANGE: Moved the file stm32f10x.h from the demos to the platforms support
  directory.

*** 1.3.3 ***
- FIX: Fixed bug in the LPC2148 PAL driver (bug 2881380).
- FIX: Fixed bug in the AT91SAM7X PAL driver (bug 2879933).
- NEW: New MAC and MII driver models and implementations for the AT91SAM7X.
  Removed the old EMAC driver, updated the uIP WEB demo to use the new
  driver model.
- NEW: Added a simple lwIP demo (web server) for the AT91SAM7X.
- NEW: Centralized core memory manager. This simple allocator implements a
  sbrk()-like API: chCoreAlloc(). The other allocators now use this manager
  in order to get memory blocks.
- NEW: The heap allocator has been modified, now it is possible to have
  multiple heaps. The default heap gets its memory from the new core memory
  manager.
- NEW: Now memory pools can optionally get new objects automatically from the
  core memory manager.
- NEW: Added newlib interface file syscalls.c under ./os/various for use with
  the newest YAGARTO releases. The file provides bindings between the C
  runtime and the core memory manager.
- CHANGE: Because the changes in the allocators some API prototypes changed:
  chHeapAlloc(), chHeapStatus(), chThdCreateFromHeap().
- CHANGE: Because the changes in the allocators some configuration options
  changed and some were removed, see the new template chconf.h file.
- CHANGE: renamed ./demos/ARM7-AT91SAM7X-WEB-GCC in ARM7-AT91SAM7X-UIP-GCC.
- FIX: Added the most restrictive GCC warning option to the makefiles (-Wextra)
  and fixed some warnings in the code, mostly unused function parameters.

*** 1.3.2 ***
- FIX: Fixed GCC 4.4.x aliasing warnings (bug 2846336)(backported in stable
  branch).
- FIX: Modified linker scripts for GCC 4.4.x (bug 2846302)(backported in stable
  branch).
- FIX: Fixed the CH_OPTIMIZE_SPEED option in the CM3 port (bug 2846278)
  (backported in stable branch).
- FIX: Fixed GCC 4.4.x related problems in CM3 port (bug 2846162)(backported
  in stable branch).
- FIX: Fixed LPC214x UART problem (bug 2841088)(backported in stable branch).
- NEW: Improved the Cortex-M3 preemption code, now less interrupt-related
  jitter is generated and all benchmarks scores improved a bit.
- NEW: Added new APIs chSchDoYieldS() and chThdYield().
- MEW: Added new benchmark about RAM footprint.
- MEW: Added new benchmark about round robin rescheduling.
- NEW: Reorganized and rationalized the distribution tree and the
  documentation.
- NEW: Enhanced serial driver. The driver now supports speed change at runtime
  and low power stop mode.
- NEW: Serial driver removed from the kernel and added to the I/O subsystems
  together with PAL. Note that the related API names changed their prefix from
  chFDDxxx to sdxxx because of this.
- NEW: Added standard CMSIS 1.2.0 support to the Cortex-M3 port. The kernel
  does not use it (the OS uses its own optimized code) but the functionality
  is available to the applications. The CMSIS files were patched in order
  to correct some warnings.
- NEW: Updated the STM32 port to use the newest ST firmware library files
  (version 3.1.0). Note that now the ST drivers are included in the STM32
  demo directory.
- NEW: Improved makefiles and makefile fragments, now the paths are not fixed,
  the fragments can be included also from projects outside the ChibiOS/RT files
  structure by simply defining the variable ${CHIBIOS}.
- CHANGE: Removed the CH_USE_SERIAL_FULLDUPLEX configuration option because
  the serial driver is no more part of the kernel.
- CHANGE: Reorganized the PAL and Serial identifiers now IOPORT1..N and
  SD1..N rather than IOPORT_A..Z and COM1..N, some of the old names were
  conflicting with some AVR libraries.

*** 1.3.1 ***
- FIX: Removed mention of an obsolete option from the documentation (bug
  2799507).
- NEW: Abstract digital I/O ports driver (PAL), this driver defines a common
  interface for digital I/O operations, this should help to create more
  portable applications and, in general, make easier to work with ChibiOS/RT
  on multiple architectures.
- NEW: Port drivers for STM32, LPC214x, AT91SAM7X and MSP430, cleaned up the
  initialization code in board.c. All the demos now use PAL for I/O. AVR is
  not supported because its "sparse" registers layout, it would not be
  efficient enough for my taste.
- Modified the STM32 demo to use the bit definitions in the ST header file,
  removed the bit definitions in board.h and stm32_serial.h.
- Documentation section reorganization and fixes.
- Changed the STM32 demo stack sizes, it was incorrectly adjusted in version
  1.3.0 but it did not create problems (not a bug).

*** 1.3.0 ***
- FIX: Fixed regression in MinGW demo (bug 2745153)(backported in stable
  branch).
- FIX: Fixed problem with the timeout constant TIME_IMMEDIATE, added a
  specific test in the test suite (bug 2755170)(backported in stable branch).
- FIX: Fixed a problem in semaphores test case #2 (bug  2755195)(backported
  in stable branch).
- FIX: Removed unused list functions (bug 2755230)(backported in stable
  branch).
- FIX: Added license notice to several files (bug 2772160)(backported in
  stable branch).
- FIX: Found new instances of the obsolete function chSysGetTime() in the
  C++ wrapper and in the WEB demo (bug 2772237)(backported in stable branch).
- FIX: Fixed macro in test.h (bug 2781176)(backported in stable branch).
- FIX: Fixed sequence assertion in test.c (bug 2789377)(backported in stable
  branch).
- FIX: Fixed test_cpu_pulse() incorrect behavior (bug 2789383)(backported in
  stable branch).
- FIX: Fixed missing volatile modifier for p_time field in Thread structure
  (bug 2789501)(backported in stable branch).
- FIX: Fixed C99-style variable declarations (bug 2792919)(backported in
  stable branch).
- FIX: Fixed instance of obsolete CH_USE_TERMINATE option in the C++ wrapper
  (bug 2796065)(backported in stable branch).
- FIX: Insufficient stack allocated to the C++ LPC2148 demo (bug 2796069)
  (backported in stable branch).
- FIX: Fixed errors in events test case (bug 2796081)(backported in stable
  branch).
- NEW: Abstract I/O Channels mechanism introduced. This mechanism allows to
  access I/O resources through a standard interface and hides implementation
  details. The existing serial drivers were modified to offer a standard
  channel interface to the applications (the old APIs are retained as macros).
- NEW: The I/O queues code was improved, now there are 2 separate structures:
  InputQueue and OutputQueue. There are some changes in the queue APIs
  in order to make them more symmetrical and functional. Improved the queues
  documentation. Some of the changes were needed in order to support the new
  channels mechanism as a backend for queued serial drivers.
- NEW: Static initializers macros introduced for most kernel objects. The
  static initializers allow to not have to chXXXInit() any object and save some
  code space. The initialization functions are retained in order to allow
  initialization of dynamic objects and re-initializations.
- NEW: Added more test cases in order to improve the test suite code coverage
  (it was 74% in version 1.2.0, it is now close to 100%).
- NEW: Added test cases for the improved queues and serial drivers.
- NEW: Added a code coverage analysis application under ./tests/coverage.
- NEW: Added the test suite documentation to the general documentation.
- NEW: Added a new "naked" context switch benchmark that better defines the
  real context switch time, previous benchmarks introduced too much overhead
  to the measurement. The STM32 performs the context switch in under 1.48uS.
- NEW: Improved priority inheritance test cases.
- NEW: Added architecture name strings to the port code.
- NEW: Linux x86 simulator demo added. It is still work in progress.
- CHANGE: Removed the half duplex queues and half duplex serial drivers because
  it was never extensively tested. The code is still available but not as part
  of the kernel.
- CHANGE: Removed the chMsgSendWithEvent() function. It is rarely used and
  the functionality can be re-created with a compound atomic operation. Also
  removed the CH_USE_MESSAGES_EVENT configuration option.
- CHANGE: Modified the test suite assertions in order to save RAM on the AVR
  targets. The test suite now uses much less string space.
- CHANGE: Removed the CH_USE_SERIAL_HALFDUPLEX, CH_USE_QUEUES_TIMEOUT,
  CH_USE_QUEUES_HALFDUPLEX, CH_USE_SEMAPHORES_TIMEOUT configuration options.
- CHANGE: Made CH_DBG_THREADS_PROFILING default to TRUE in all demos because
  the changes to the function test_cpu_pulse().
- CHANGE: Increased main stack size to 1KiB for all the ARMx demos, 2KiB for
  the C++ LPC2148 demo. This should make things easier for unexperienced
  users.

*** 1.2.0 ***
- Added license exception text to the 1.2.0 branch.
- FIX: Fixed serious AVR regression (bug 2731578).
- FIX: Fixed build failure when CH_USE_MUTEXES=FALSE (bug 2730706).
- FIX: Removed reference to an obsolete function (bug 2731661).
- Full test cycle and test reports updated.
- Small fixes to the documentation.

*** 1.1.3unstable ***
- FIX: Fixed makefile in STM32 demo, this bug was reported fixed in
  version 1.1.2 but it was still there (bug 2686451).
- FIX: Fixed makefile in MSP430 demo (bug 2700690).
- FIX: Fixed thumb mode build error in AT91SAM7X demos (bug 2700695).

*** 1.1.2unstable ***
- FIX: Fixed priority inheritance problem with condvars (bug 2674756) and
  added a specific test case to the test suite (backported in stable branch).
- FIX: Fixed a problem in time ranges (bug 2680425)(backported in stable
  branch).
- FIX: Build error with option CH_DBG_FILL_THREADS (bug 2683965).
- FIX: Fixed a wrong parameter check in chVTSetI() and chThdSleep()
  (bug 2679155).
- FIX: Build error with options CH_USE_NESTED_LOCKS && !CH_OPTIMIZE_SPEED
  (bug 2678928).
- FIX: Removed unused chSysPuts() macro (bug 2672678).
- FIX: Renamed function chSysInTimeWindow() as chTimeIsWithin() and renamed
  the macro chSysGetTime() in chTimeNow(), the old names are still recognized
  but marked as deprecated (fixes the bug 2678953 but goes a bit further by
  introducing a new API category "Time").
- FIX: Fixed makefile problems in the AT91SAM7X256 and STM32 demos (bugs
  2686347 and 2686451).
- FIX: Fixed AT91SAM7X256 EMAC driver (bug 2686349).
- FIX: Fixed small some errors in the documentation (bug 2692510).
- OPT: Small optimization to the Cortex-M3 thread startup code, improved thread
  related performance scores and smaller code.
- OPT: Alternative, non-inlined and more compact, implementations for
  port_lock() and port_unlock() in the Cortex-M3 port when CH_OPTIMIZE_SPEED
  is FALSE.
- OPT: Improved ready list and priority ordered lists code, some space saved,
  better context switch performance.
- Modified the test thread function to return the global test result flag.
- Removed testcond.c|h and moved the test cases into testmtx.c. Mutexes and
  condvars have to be tested together.
- Added architecture diagram to the documentation.
- Removed from the documentation some references to long gone functions...

*** 1.1.1unstable ***
- FIX: Fixed a problem into the STACK_ALIGN() macro (backported in stable
  branch).
- FIX: Fixed a problem with a wrong declaration of the PLL structure in the
  file lpc214x.h (backported in stable branch).
- FIX: Fixed build problem with the C++ demo (bug 2687489).
- FIX: Removed EMAC driver from the AT91SAM7X256 demo makefile (bug 2686347).
- FIX: Removed C++ wrapper from the STM32 demo makefile (bug 2686451).
- FIX: Fixed a problem with some event APIs not showing in the documentation
  (backported in stable branch).
- FIX: Fixed wrong assertions in chThdWait() and chHeapFree() (backported in
  stable branch).
- FIX: Fixed @file tag in sam7x_serial.c (bug 2788573) (backported in stable
  branch).
- FIX: Fixed a small problem in the chcore.c template file.
- NEW: Mailboxes (asynchronous messages) subsystem and test cases added.
- NEW: Most APIs with a timeout specification now accept the constant
  TIME_IMMEDIATE (-1) that triggers an immediate timeout when trying to enter
  a sleep state.
- NEW: Mode flexible debug configuration options, removed the old CH_USE_DEBUG
  and CH_USE_TRACE. Replaced with CH_DBG_ENABLE_CHECKS, SCH_DBG_ENABLE_ASSERTS,
  CH_DBG_ENABLE_TRACE and CH_DBG_FILL_THREADS.
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
- CHANGE: Changed the chSemFastWaitS() macro in chSemFastWaitI() and
  chSemGetCounter() in chSemGetCounterI().
- CHANGE: Removed the port_puts() function from the port templates. It was not
  implemented on all ports.
- Improvements to the test suite, added a new level of indirection that allows
  to make tests depend on the configuration options without have to put #ifs
  into the test main module. New benchmarks about semaphores and mutexes.

*** 1.1.0unstable ***
- FIX: Modified the default value for the STM32 HSI setup it was 1, it should
  be 0x10 (backported in stable branch).
- FIX: Removed an obsolete constant (P_SUSPENDED) from thread.h (backported in
  stable branch).
- FIX: Removed unused field mp_grow in the MemoryPool structure (backported in
  stable branch).
- NEW: Added to the STM32 demo makefile an option to build ChibiOS/RT with the
  full STM32 FWLib 2.03. **NOTE**, except for the makefile option, the
  library is not used by the OS nor required (backported in stable branch).
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
  can be overridden externally, for example from within the Makefile.
  The options are no mode a simple define but a define with an assigned
  TRUE/FALSE value within an #ifndef block.
- NEW: Idle thread hook macro added to the configuration file.
- NEW: Changed the ARM7 and Cortex-M3 startup files, now the action when
  the main() function returns can be overridden by redefining the symbol
  MainExitHandler.
- OPT: Improved ARM7 thumb port code, thanks to some GCC tricks involving
  registers usage now the kernel is much smaller, faster and most OS APIs
  use less RAM in stack frames (note, this is an ARM7 thumb mode specific
  optimization).
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
- OPT: Improved reschedule time by reordering the sequence of operations,
  now during enqueuing the ready list contains one less element. This change
  also slightly improves the interrupt latency.
- OPT: Optimization to the chSemReset(), reversed the order of dequeuing.
- FIX: Fixed a bug in the chThdSetPriority() API.
- FIX: Modified the structure names into nvic.h in order to not make them
  collide with external libraries.
- Added a benchmark to the test suit that measures the mass reschedule
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
  of software loops into the benchmarks in order to have more stable results.
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
- Some fixes to the doxygen documentation.
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
- Various fixes to the doxygen documentation.

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
- Port for Atmel AT91SAM7X256 introduced, the port should be usable also on
  SAM7S and SAM7XC but no tests were performed. Other SAM7 processors should
  also be usable with limited changes.
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
  serving messages in FIFO order can exist at the same time in the system.
  This feature can be enabled or disabled by toggling the option
  CH_USE_MESSAGES_PRIORITY into the chconf.h file (disabled by default, old
  behavior).
  Note: This option brings a small overhead when sending a message regardless
  if in FIFO or priority order, if you don't need priority ordering for your
  messages it is better to keep the feature disabled in chconf.h.
- Added to the ARM demos load scripts the capability to load code in RAM
  instead flash, the function must be marked as:
    __attribute__((section(".ramtext")))
  The option -mlong-calls should be specified in the makefile too or the
  function declared with the "long-call" attribute.
- Fixed the MSVC demo project files.
- Fixed some syntax incompatibilities between GCC and MSVC into chmtx.c.

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
- The GCC option -falign-functions=16 is now default in the Makefile, it is
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
- Added missing chThdSuspend() declaration in threads.h.

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
- Added a generic events generator timer module to the library code.
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
- Moved some documentation and images from the web site into the Doxygen
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
