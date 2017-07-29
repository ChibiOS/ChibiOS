# List of the ChibiOS/RT ARMCAx-TZ generic port files.
PORTSRC = ${CHIBIOS}/os/common/ports/ARMCAx-TZ/chcore.c

PORTASM = $(CHIBIOS)/os/common/ports/ARMCAx-TZ/compilers/GCC/chcoreasm.S

PORTINC = ${CHIBIOS}/os/common/ports/ARMCAx-TZ \
          ${CHIBIOS}/os/common/ports/ARMCAx-TZ/compilers/GCC
