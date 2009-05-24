##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = no
endif

# Enable register caching optimization (read documentation).
# Option not tested on MSP430, DO NOT USE.
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
LDSCRIPT= mspgcc/msp430x1611.x

# Imported source files
include ../../src/kernel.mk
include ../../test/test.mk

# C sources here.
CSRC = ../../ports/MSP430/chcore.c \
       ../../ports/MSP430/msp430_serial.c \
       ${KERNSRC} \
       ${TESTSRC} \
       ../../src/lib/evtimer.c \
       board.c main.c

# C++ sources here.
CPPSRC =

# List ASM source files here
ASMSRC =

INCDIR = $(KERNINC) $(TESTINC) \
         ../../src/lib \
         ../../ports/MSP430

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = msp430x1611

TRGT = msp430-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# Define C warning options here
CWARN = -Wall -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall

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

include ../../ports/MSP430/rules.mk
