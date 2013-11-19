# List of the ChibiOS/RT e200z4 SPC56ELxx port files.
PORTSRC = ${CHIBIOS}/os/rt/ports/e200/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/e200/devices/SPC56ELxx/boot.s \
          $(CHIBIOS)/os/common/ports/e200/compilers/GCC/vectors.s \
          $(CHIBIOS)/os/common/ports/e200/compilers/GCC/ivor.s \
          $(CHIBIOS)/os/common/ports/e200/compilers/GCC/crt0.s

PORTINC = ${CHIBIOS}/os/common/ports/e200/devices/SPC56ELxx \
          ${CHIBIOS}/os/rt/ports/e200 \
          ${CHIBIOS}/os/rt/ports/e200/compilers/GCC

PORTLD  = ${CHIBIOS}/os/common/ports/e200/compilers/GCC/ld
