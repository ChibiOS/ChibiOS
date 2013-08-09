# List of all the ChibiOS/RT HAL files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing halconf.h.
HALSRC = ${CHIBIOS}/os/halnew/src/hal.c \
         ${CHIBIOS}/os/halnew/src/hal_queues.c \
         ${CHIBIOS}/os/halnew/src/adc.c \
         ${CHIBIOS}/os/halnew/src/can.c \
         ${CHIBIOS}/os/halnew/src/icu.c \
         ${CHIBIOS}/os/halnew/src/pal.c \
         ${CHIBIOS}/os/halnew/src/pwm.c \
         ${CHIBIOS}/os/halnew/src/serial.c \
         ${CHIBIOS}/os/halnew/src/spi.c \
         ${CHIBIOS}/os/halnew/src/st.c \

# Required include directories
HALINC = ${CHIBIOS}/os/halnew/include
