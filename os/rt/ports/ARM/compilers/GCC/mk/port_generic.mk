# List of the ChibiOS/RT ARM generic port files.
PORTSRC = ${CHIBIOS}/os/rt/ports/ARM/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARM/compilers/GCC/vectors.s \
          $(CHIBIOS)/os/common/ports/ARM/compilers/GCC/crt0.s \
          $(CHIBIOS)/os/rt/ports/ARM/compilers/GCC/chcoreasm.s

PORTINC = ${CHIBIOS}/os/rt/ports/ARM \
          ${CHIBIOS}/os/rt/ports/ARM/compilers/GCC
