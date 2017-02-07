# List of the ChibiOS/RT e200 generic port files.
PORTSRC = $(CHIBIOS)/os/common/ports/e200/chcore.c
          
PORTASM = $(CHIBIOS)/os/common/ports/e200/compilers/HighTec/ivor.S \
          $(CHIBIOS)/os/common/ports/e200/compilers/HighTec/chcoreasm.S

PORTINC = $(CHIBIOS)/os/common/ports/e200 \
          $(CHIBIOS)/os/common/ports/e200/compilers/HighTec
