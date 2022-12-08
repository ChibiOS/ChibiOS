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
  |  |  +--ports/               - RTOS ports usable by both RT and NIL.
  |  |  +--startup/             - Startup support.
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
  |  |  |  |  +--serial_nor/    - HAL managed flash storage driver.
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

*** 20.3.5 ***
- FIX: Fixed wrong DMA definitions for STM32L0xx I2C3 peripheral (bug #1245).
- FIX: Fixed wrong path in STM32L053 ADC demo makefile (bug #1244).
- FIX: Fixed missing semicolon in STM32 OTGv1 driver (bug #1243).
- FIX: Fixed HSI48 not enabled for STM32L496/​4A6 (bug #1242).
- FIX: Fixed problem in STM32 gpt_lld_polled_delay() implementation (bug #1241).
- FIX: Fixed invalid delay loop in STM32G0/WL ADCv5 driver (bug #1240).
- FIX: Fixed missing clock enables for some GPIOS on some STM32L4s (bug #1237).
- FIX: Fixed old bugs in serial driver header (bug #1236).
- FIX: Fixed virtual timers lockup uder rare conditions (bug #1235).
- FIX: Fixed STM32 RTCv2 locks for a second (bug #1234).
- FIX: Some MISRA-related fixes.
- FIX: Re-opened and fixed bug #1100.
- FIX: Fixed wrong buffers toggling in STM32 USBv1 isochronous mode (bug #1232).
- FIX: Fixed STM32 RTCv2 registers synchronization errata (bug #1231).
- FIX: Fixed STM32 ADCv1 and ADCv5 do not allow prescaler divide value of 1
       (bug #1230).
- FIX: Fixed missing check on STM32 SPIv2 DMA settings for SPI1 (bug #1229).
- FIX: Fixed invalid handling of lwIP NETIF_FLAG_LINK_UP flag (bug #1227).
- FIX: Fixed missing TIM16/17 errata handling for STM32G0xx (bug #1226).
- FIX: Fixed missing ADC errata handling for STM32G0xx (bug #1225).
- FIX: Fixed problem in the HAL I2C fallback driver (bug #1224).
- FIX: Fixed GPIOH clock not enabled on STM32L432 (bug #1223).
- FIX: Fixed incorrect type cast in TIME_I2US() (bug #1221).
- FIX: Fixed missing clock disable for STM32 OCTOSPI2 (bug #1220).
- FIX: Fixed STM32 Ethernet driver causes system hang after 2^31 packets
       sent/received (bug #1218).
- FIX: Fixed Virtual Timers failure in a specific condition (bug #1215).
- FIX: Fixed invalid state transition in SNOR flash driver (bug #1212).
- FIX: Fixed missing check in chobjcaches.h (bug #1210).
- FIX: Fixed RT testbuild application broken (bug #1208).
- FIX: Fixed USB not enabled on STM32F103x6 (bug #1206).
- FIX: Fixed wrong number of CAN filters for STM32L496/9A6 (bug #1204).
- FIX: Fixed DMA stream not disabled in STM32 QUADSPIv1 driver (bug #1203).
- FIX: Fixed I2C4 DMA streams for STM32L496 (bug #1202).
- FIX: Fixed STM32_SDMMC2_NUMBER on STM32H7 (bug #1201).

*** 20.3.4 ***
- NEW: Made STM32H7 non-cacheable memory option also shareable.
- NEW: STM32 improved ADCv4 driver (H7) back-ported from trunk.
- NEW: New "VT Storm" test application for RT.
- NEW: More efficient virtual timers implementation for RT, it has been
       back-ported from RT7.
- NEW: Improved PWR settings for STM32G4.
- NEW: Improved boost settings for STM32G4.
- NEW: Files mcuconf.h for STM32F746, F767, L432, L452, L476, L496 received
       the missing setting STM32_WSPI_QUADSPI1_PRESCALER_VALUE.
- FIX: Fixed function nvicSetSystemHandlerPriority() failing with CM0+ cores
       (bug #1199).
- FIX: Fixed several problems in STM32L4xx PLL and GPIO settings (bug #1198).
- FIX: Fixed ARMCMx port, MPU not enabled when PORT_ENABLE_GUARD_PAGES is set
       (bug #1195).
- FIX: Fixed removed incorrect ADCv4 checks for MONEN (bug #1194)
- FIX: Fixed hang in spi_lld_ignore() in SPIv3 on H7 (bug #1193)
- FIX: Fixed invalid RCC LPUART1 clock enable on STM32G0xx (bug #1189).
- FIX: Fixed wrong configuration in testex LSM6DSL demos (bug #1184).
- FIX: Fixed STM32 ADCv3 differences in headers (bug #1182).
- FIX: Fixed DMAv1 compile fail on STM32L011 (bug #1181).
- FIX: Fixed error in STM32_ADCCLK_MIN for STM32F37x/hal_lld.h (bug #1180).
- FIX: Fixed direct calls to RT in STM32 RTCv2 and RTCv3 (bug #1179).
- FIX: Fixed invalid DMAMUX settings in DMAv1 for some devices (bug #1176).
- FIX: Fixed problem with RC initialization mask of the GPIO for the 
       STM32L4x3 (bug #1174).
- FIX: Fixed problem with N25Q driver (bug #1173).       
- FIX: Fixed missing constant in ADuCM36x hal_lld.c (bug #1166).
- FIX: Fixed invalid STM32 TIM21/TIM22 debug freeze setting (bug #1164).
- FIX: Fixed I2S-related definitions missing in STM32F3xx registry (bug #1162).
- FIX: Fixed STM32 SDMMCv2 driver invalid initial clock settings (bug #1160).
- FIX: Fixed wrong wait states calculation in STM32G4xx, insufficient
       boost settings (bug #1159).
- FIX: Fixed warning in STM32 ADCv4 (bug #1158).
- FIX: Fixed wrong check on HAL_USE_RTC in STM32G4 clock initialization
       (bug #1157).
- FIX: Fixed wrong checks related to PLLSAI2 on L4 and L4+ (bug #1156).
- FIX: Fixed STM32G431 DMA defines error (bug #1155).
- FIX: Fixed errors in STM32L4xx registry (bug #1154).
- FIX: Fixed QUADSPI errata fix applied to all platforms (bug #1153).
- FIX: Fixed (again) LPUART1 support for STM32H7xx (bug #1113).
- FIX: Fixed wrong behavior in Serial-USB driver (bug #1151).
- FIX: Fixed L0x2 series DAC not allocated in registry (bug #1150).
- FIX: Fixed inconsistent naming of SAI DMAMUX defines for STM32H7 (bug #1149).
- FIX: Fixed TIM register layout difference for STM32G4 series (bug #1148).
- FIX: Fixed STM32 QUADSPI driver problem when used with DMAv2 (bug #1147).
- FIX: Fixed incorrect IRQ vector for PVM (bug #1146).

*** 20.3.3 ***
- NEW: RT, NIL: Renamed _THREADS_QUEUE_DECL to THREADS_QUEUE_DECL for
       consistency.
- FIX: Fixed missing STM32F765 from registry (bug #1145).
- FIX: Fixed wrong macro check on STM32 SPIv3 (bug #1144).
- FIX: Fixed wrong check on STM32 TIM9 timer (bug #1143).
- FIX: Fixed FAT time problem in RTC driver (bug #1142).
- FIX: Fixed Heap allocation of aligned FIFO objects in chFactory (bug #1141).
- FIX: Fixed chsnprintf() sign mode/filler mode conflict (bug #1140).
- FIX: Fixed GCC 10 causes warning in factory module (bug #1139).
- FIX: Fixed C strict aliasing rules violation causes test cases to fail
       (bug #1138).
- FIX: Fixed STM32H7xx Missing CRC RCC macros (bug #1137).
- FIX: Fixed STM32L0x wrong ISR names for USART 4 and 5 (bug #1136).
- FIX: Fixed OTG_FS error on STM32H7 (bug #1135).
- FIX: Fixed compile error of STM32 CRYPv1 driver when DMAMU is present
       (bug #1134).
- FIX: Fixed moved define into hal_wspi_lld.c (bug #1133).
- FIX: Fixed various bugs in MDMAv1 driver (bug #1132).
- FIX: Fixed wrong check on LSI on all STM32 platforms (bug #1131).
- FIX: Fixed missing EFL driver in platform_l432.mk (bug #1130).
- FIX: Fixed wrong check in STM32 ST driver (bug #1129).
- FIX: Fixed OSAL does not allow 64 bits resolution values (bug #1128).
- FIX: Fixed wrong SDMMC RCC macros for STM32H7xx (bug #1127).
- FIX: Fixed STM32 ADCv3 hanging on initialization (bug #1126).
- FIX: Fixed I2S-related problems in STM32F4xx registry (bug #1124).
- FIX: Fixed STM32 EXTIv1 driver unable to enable/disable fixed lines
       (bug #1123).
- FIX: Fixed incorrect STM32 iWDG initialization in windowed mode (bug #1122).
- FIX: Fixed ignored HSIDIV setting on STM32G0xx (bug #1121)

*** 20.3.2 ***
- NEW: Support for 3 analog watchdogs in ADCv3 (STM32F3, L4, L4+, G4).
- NEW: Support for 3 analog watchdogs in ADCv5 (STM32G0).
- NEW: Updated FatFS to version 0.14.
- NEW: Added a new setting to STM32 USBv1 allowing for some clock deviation
       from 48MHz. Renamed setting USB_HOST_WAKEUP_DURATION to
       STM32_USB_HOST_WAKEUP_DURATION for consistency.
- FIX: Fixed STM32 QSPI errata workaround (bug #1116).
- FIX: Fixed wrong condition in STM32 BDMAv1 driver (bug #1115).
- FIX: Fixed HSI48 not getting enabled on STM32H7 (bug #1114).
- FIX: Fixed LPUART1 support for STM32H7xx (bug #1113).
- FIX: Fixed wrong sector count in EFL driver for L4+ dual bank configuration 
       (bug #1112).
- FIX: Fixed wrong preprocessor checks in STM32 TIMv1 ICU driver (bug #1111).
- FIX: Fixed wrong revisions handling in STM32H743 HAL (bug #1110).
- FIX: Fixed missing STM32_I2C_BDMA_REQUIRED definition in I2Cv3 driver
       (bug #1109).
- FIX: Fixed wrong definitions in SPC563M board files (bug #1108).
- FIX: Fixed cortex-M vectors table alignment problem (bug #1107).
- FIX: Fixed extra condition in MAC driver macWaitTransmitDescriptor() function
       (bug #1106).
- FIX: Fixed schedule anomaly when CH_CFG_TIME_QUANTUM is greater than zero
       (bug #1105).
- FIX: Fixed Virtual Timers corner case (bug #1104).
- FIX: Fixed GCC6 problem breaks Cortex-M0 port (bug #985).
- FIX: Fixed a wrong management of the SPI TX buffer in the ADUCM port 
       (bug #1103).
- FIX: Fixed STM32F4 EFL sector bug (bug #1102).
- FIX: Fixed differences in STM32 EXTI (bug #1101).
- FIX: Fixed STM32 DACv1 driver regressed because DMA changes (bug #1100).
- FIX: Fixed STM32L0 missing LPUART IRQ initialization (bug #1099).
- FIX: Fixed invalid EXTI definitions for STM32L0xx (bug #1098).
- FIX: Fixed compilation error in file nvic.c (bug #1097).
- FIX: Fixed STM32_DMAx_CH8_HANDLER not defined for DMAv1 (bug #1096).
- FIX: Fixed STM32 EXTI2 and EXTI4 not triggering a callback (bug #1095).
- FIX: Fixed STM32G4 demos compile fails if smart mode is disabled (bug #1094).
- FIX: Fixed failure in chSemReset() function when counter is equal to MAXINT
       (bug #1093).
- FIX: Fixed error in EXTIv1 ISRs (bug #1077).

*** 20.3.1 ***
- NEW: STM32 ICU driver now allows to setup the ARR register in the
       configuration structure, the default value should be 0xFFFFFFFFU.
- NEW: Updated debug tools to be independent from the toolchain position:
       they now rely on the environment variable CHIBISTUDIO.
- NEW: Added dynamic reconfiguration API to lwIP bindings.
- FIX: Fixed swapped definition in ST_STM32F746G_DISCOVERY board files
       (bug #1092).
- FIX: Fixed missing symbols in GCC scatter files (bug #1091).
- FIX: Fixed wrong SAI1 clock selection for STM32G4xx (bug #1090).
- FIX: Fixed STM32H7xx ADC problem in dual mode (bug #1089).
- FIX: Fixed invalid CHSEL DMA setting in STM32 UART drivers (bug #1088).
- FIX: Fixed sector count incorrect in STM32G07/8 EFL driver (bug #1085).
- FIX: Fixed sector size incorrect in STM32F413 EFL driver (bug #1084).
- FIX: Fixed wrong arguments for the cacheBufferInvalidate in the STM32 SPI 
       demo (bug #1086).
- FIX: Fixed race condition in HAL MAC driver (bug #1083).
- FIX: Fixed STM32H7 compile fails for I2C4 (bug #1082).
- FIX: Fixed early interrupts enable in ARMv7-M port (bug #1081).
- FIX: Fixed I2CD4 interrupt vectors are swapped versus I2CD1-I2CD3 (bug #1080).
- FIX: Fixed incorrect clock check when using PLLSAI1R in ADCv3 (bug #1079).
- FIX: Fixed missing checks in TIM6 and TIM7 STM32 mini drivers (bug #1078).
- FIX: Fixed problem in chMtxUnlockAllS() (bug #1076).

