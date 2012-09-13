# List of the ChibiOS/RT SPC563Mxx port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/PPC/chcore.c

PORTASM = ${CHIBIOS}/os/ports/GCC/PPC/SPC563Mxx/vectors.s \
          ${CHIBIOS}/os/ports/GCC/PPC/SPC563Mxx/ivor.s \
          ${CHIBIOS}/os/ports/GCC/PPC/crt0.s

PORTINC = ${CHIBIOS}/os/ports/GCC/PPC \
          ${CHIBIOS}/os/ports/GCC/PPC/SPC563Mxx

PORTLD  = ${CHIBIOS}/os/ports/GCC/PPC/SPC563Mxx/ld
