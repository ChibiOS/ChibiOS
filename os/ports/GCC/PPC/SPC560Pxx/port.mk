# List of the ChibiOS/RT SPC560Pxx port files.
PORTSRC = ${CHIBIOS}/os/ports/GCC/PPC/chcore.c

PORTASM = ${CHIBIOS}/os/ports/GCC/PPC/bam.s \
          ${CHIBIOS}/os/ports/GCC/PPC/crt0.s \
          ${CHIBIOS}/os/ports/GCC/PPC/SPC560Pxx/ivor.s \
          ${CHIBIOS}/os/ports/GCC/PPC/SPC560Pxx/vectors.s

PORTINC = ${CHIBIOS}/os/ports/GCC/PPC \
          ${CHIBIOS}/os/ports/GCC/PPC/SPC560Pxx

PORTLD  = ${CHIBIOS}/os/ports/GCC/PPC/SPC560Pxx/ld
