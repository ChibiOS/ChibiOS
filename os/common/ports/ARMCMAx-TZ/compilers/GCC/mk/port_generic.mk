# List of the ChibiOS/RT ARMCMAx-TZ generic port files.
PORTSRC = ${CHIBIOS}/os/common/ports/ARMCMAx-TZ/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARMCMAx-TZ/compilers/GCC/chcoreasm.S

PORTINC = ${CHIBIOS}/os/common/ports/ARMCMAx-TZ \
          ${CHIBIOS}/os/common/ports/ARMCMAx-TZ/compilers/GCC
