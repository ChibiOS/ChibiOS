##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -mabi=apcs-gnu -falign-functions=16
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(LINK_GC),)
  USE_LINK_GC = yes
endif

# Enable register caching optimization (read documentation).
ifeq ($(USE_CURRP_CACHING),)
  USE_CURRP_CACHING = no
endif

# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = ch

# Define linker script file here
LDSCRIPT= ch.ld

# Imported source files
include ../../src/kernel.mk
include ../../test/test.mk

# Sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
SRC = ../../ports/ARMCM3/chcore.c \
      ../../ports/ARMCM3/nvic.c \
      ../../ports/ARMCM3-STM32F103/stm32_serial.c \
      ${KERNSRC} \
      ${TESTSRC} \
      ../../src/lib/evtimer.c \
      board.c main.c

# List ASM source files here
ASMSRC =  ../../ports/ARMCM3/crt0.s \
          ../../ports/ARMCM3-STM32F103/vectors.s

INCDIR = $(KERNINC) $(TESTINC) \
         ../../src/lib \
         ../../ports/ARMCM3 \
         ../../ports/ARMCM3-STM32F103 \
         ./stm32lib/inc
#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m3

#TRGT = arm-none-eabi-
#TRGT = arm-eabi-
TRGT = arm-elf-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# Define warning options here
WARN = -Wall -Wstrict-prototypes

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################

##############################################################################
# Start of user section
#

# Define project name here
PROJECT = ch

# Define linker script file here
LDSCRIPT= ch.ld

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

#
# End of user defines
##############################################################################

include ../../ports/ARMCM3/rules.mk
