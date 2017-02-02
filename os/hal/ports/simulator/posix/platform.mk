# List of all the Win32 platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/simulator/posix/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/simulator/posix/hal_serial_lld.c \
              ${CHIBIOS}/os/hal/ports/simulator/console.c \
              ${CHIBIOS}/os/hal/ports/simulator/hal_pal_lld.c \
              ${CHIBIOS}/os/hal/ports/simulator/hal_st_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/simulator/posix \
              ${CHIBIOS}/os/hal/ports/simulator
