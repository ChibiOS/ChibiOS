# List of the ChibiOS/RT e200 generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/e200/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/e200/compilers/GCC/ivor.s \
          $(CHIBIOS)/os/common/ports/e200/compilers/GCC/chcoreasm.s

PORTINC = $(CHIBIOS)/os/common/ports/e200 \
          $(CHIBIOS)/os/common/ports/e200/compilers/GCC
