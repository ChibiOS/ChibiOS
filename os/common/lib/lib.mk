# List of the ChibiOS common library modules.
CLIBSRC = ${CHIBIOS}/os/common/lib/src/memstreams.c \
          ${CHIBIOS}/os/common/lib/src/nullstreams.c \
          ${CHIBIOS}/os/common/lib/src/chprintf.c \
          ${CHIBIOS}/os/common/lib/src/chscanf.c
 
CLIBINC = ${CHIBIOS}/os/common/lib/include

# Shared variables
ALLCSRC += $(CLIBSRC)
ALLINC  += $(CLIBINC)
