##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
#   -02 optimization level is too high for easy debugging...  -Og is better, or -O0 or -O1
#  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
  USE_OPT = -Og -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data.
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT = 
endif

# Enable this if you want link time optimizations (LTO).
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x200
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x200
endif

# Enables the use of FPU (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

# FPU-related options.
ifeq ($(USE_FPU_OPT),)
  USE_FPU_OPT = -mfloat-abi=$(USE_FPU) -mfpu=fpv4-sp-d16
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, target, sources and paths
#

# create a boolean that les me choose either the RP2040 or RP2350 define either as "empty" for unused or "yes" for used, no spaces, and no comments on the next 3 lines.
USE_RP2040=
USE_RP2350=yes
# enable wifi for the 'pico 2 W' variant.
USE_WIFI_VARIANT=no
# tell user what we are doing
ifeq ($(USE_RP2040),yes)
  $(info Using RP2040)
endif 
ifeq ($(USE_RP2350),yes)
  $(info Using RP2350)
endif  
ifeq ($(USE_WIFI_VARIANT),yes)
  $(info Using WIFI-VARIANT)
else 
  $(info Using NO-WIFI-VARIANT)
endif

# Define project name here
PROJECT = ch

# Target settings.
ifeq ($(USE_RP2040),yes)
MCU  = cortex-m0plus
endif
ifeq ($(USE_RP2350),yes)
MCU  = cortex-m33 # this is what arduino uses on the 2350 and is needed to get the correct __ARM_ARCH_8M_MAIN__ and thus get the right software spinlock implementation.
endif

# Imported source files and paths.
CHIBIOS  := ../../..
CONFDIR  := ./cfg
BUILDDIR := ./build
DEPDIR   := ./.dep

# Licensing files.
include $(CHIBIOS)/os/license/license.mk
# Startup files.
ifeq ($(USE_RP2040),yes)
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_rp2040.mk
endif
ifeq ($(USE_RP2350),yes)
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_rp2350.mk
endif
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
ifeq ($(USE_RP2040),yes)
include $(CHIBIOS)/os/hal/ports/RP/RP2040/platform.mk
endif
ifeq ($(USE_RP2350),yes)
include $(CHIBIOS)/os/hal/ports/RP/RP2350/platform.mk
endif
ifeq ($(USE_RP2040),yes)
include $(CHIBIOS)/os/hal/boards/RP_PICO_RP2040/board.mk
endif
ifeq ($(USE_RP2350),yes)
include $(CHIBIOS)/os/hal/boards/RP_PICO_RP2350/board.mk
endif
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
ifeq ($(USE_RP2040),yes)
include $(CHIBIOS)/os/common/ports/ARMv6-M-RP2/compilers/GCC/mk/port.mk
endif
ifeq ($(USE_RP2350),yes)
# this points to karls armv8-m-rp2 variant as it seems to be the most up to date, and it boots.
# the -ML variant might be the other choice, and is "mainline", and it seems to compile right-now, but it doesnt boot.
#include $(CHIBIOS)/os/common/ports/ARMv8-M-ML/compilers/GCC/mk/port.mk
include $(CHIBIOS)/os/common/ports/ARMv8-M-RP2/compilers/GCC/mk/port.mk
endif
# Auto-build files in ./source recursively.
include $(CHIBIOS)/tools/mk/autobuild.mk
# Other files (optional).
include $(CHIBIOS)/os/test/test.mk
include $(CHIBIOS)/test/rt/rt_test.mk
include $(CHIBIOS)/test/oslib/oslib_test.mk
#include $(CHIBIOS)/os/various/pico_bindings/pico-sdk1.mk - old v1 sdk
ifeq ($(USE_RP2040),yes)
include $(CHIBIOS)/os/various/pico_bindings/pico-sdk.2040.mk
endif
ifeq ($(USE_RP2350),yes)
include $(CHIBIOS)/os/various/pico_bindings/pico-sdk.2350.mk
endif
include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/shell/shell.mk

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/RP2040_RAM.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(ALLCSRC) \
       $(TESTSRC) \
       main.c c1_main.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC = $(ALLCPPSRC)

# List ASM source files here.
ASMSRC = $(ALLASMSRC)

# List ASM with preprocessor source files here.
ASMXSRC = $(ALLXASMSRC)

# Inclusion directories.
INCDIR = $(CONFDIR) $(ALLINC) $(TESTINC)

# Define C warning options here.
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# Define C++ warning options here.
CPPWARN = -Wall -Wextra -Wundef

#
# Project, target, sources and paths
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
# 2040 / original pico:
ifeq ($(USE_RP2040),yes)
UDEFS = -DCRT0_VTOR_INIT=1 -DCRT0_EXTRA_CORES_NUMBER=1 -DPICO_NO_FPGA_CHECK -DNDEBUG -DPICO_RP2040=1 -DPICO_RP2350A=0 -DPICO_USE_GPIO_COPROCESSOR=0 -DPICO_SECURE=0
endif
ifeq ($(USE_RP2350),yes)
# 2350 / "pico 2":   
UDEFS = -DCRT0_VTOR_INIT=1 -DCRT0_EXTRA_CORES_NUMBER=1 -DPICO_NO_FPGA_CHECK -DNDEBUG -DPICO_RP2350A=1 -DPICO_RP2350=1 -DPICO_RP2040=0 -DPICO_USE_GPIO_COPROCESSOR=0 -DPICO_SECURE=0 -D__DSP_PRESENT=1 
# pico 2 W variant has wifi... but we dont have to enable wifi, u can leave this OFF on the 'w' and just not use its wifi features.
  ifeq ($(USE_WIFI_VARIANT),yes)
    UDEFS += -DUSE_WIFI_VARIANT=1 -DPICO_CYW43_ARCH_POLL=1 -DCYW43_DEFAULT_PIN_WL_HOST_WAKE=24u -DCYW43_DEFAULT_PIN_WL_REG_ON=23u
  endif
endif

# Define ASM defines here
UADEFS = -DCRT0_VTOR_INIT=1 -DCRT0_EXTRA_CORES_NUMBER=1

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

#
# End of user section
##############################################################################

##############################################################################
# Common rules
#

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
include $(RULESPATH)/arm-none-eabi.mk
include $(RULESPATH)/rules.mk

#
# Common rules
##############################################################################

##############################################################################
# Custom rules
#
uf2: $(BUILDDIR)/$(PROJECT).elf
	$(info Creating UF2 file)
	@/home/buzz2/Chibi/picotool/picotool uf2 convert $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).uf2  --family rp2350-arm-s --abs-block
	@ls -l $(BUILDDIR)/$(PROJECT).uf2
	@file $(BUILDDIR)/$(PROJECT).uf2
	$(info UF2 for 2350 created)

upload: uf2
	$(info Uploading UF2 file... please press the BOOTSEL button on the board while connecting the USB cable in the next 5 seconds)
	@sleep 5
	@/home/buzz2/Chibi/picotool/picotool load -v -x $(BUILDDIR)/$(PROJECT).uf2 -f
	$(info UF2 file uploaded)
#@cp $(BUILDDIR)/$(PROJECT).uf2 /media/$(USER)/RPI-RP2
#
# Custom rules
##############################################################################
