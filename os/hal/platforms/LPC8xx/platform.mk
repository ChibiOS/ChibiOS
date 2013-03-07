# List of all the LPC8xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/LPC8xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC8xx/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC8xx/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC8xx/serial_lld.c 

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/LPC8xx
