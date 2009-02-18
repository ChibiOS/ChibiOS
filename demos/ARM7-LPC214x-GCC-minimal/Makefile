##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -mabi=apcs-gnu -falign-functions=16
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = no
endif

# Enable register caching optimization (read documentation).
ifeq ($(USE_CURRP_CACHING),)
  USE_CURRP_CACHING = no
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
#include ../../test/test.mk

# Sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
SRC = ../../ports/ARM7/chcore.c \
      ../../ports/ARM7-LPC214x/vic.c  \
      ${KERNSRC} \
      board.c main.c

# Sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ASRC =

# Sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TSRC =

# List ASM source files here
ASMSRC = ../../ports/ARM7/crt0.s \
         ../../ports/ARM7-LPC214x/vectors.s \
         ../../ports/ARM7/chcoreasm.s

INCDIR = $(KERNINC) $(TESTINC) \
         ../../src/lib \
         ../../ports/ARM7 \
         ../../ports/ARM7-LPC214x

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = arm7tdmi

TRGT = arm-elf-
CC   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

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

include ../../ports/ARM7/rules.mk
