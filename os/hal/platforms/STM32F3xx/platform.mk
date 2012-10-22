# List of all the STM32F3xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32F3xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2/pal_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32F3xx \
              ${CHIBIOS}/os/hal/platforms/STM32 \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2
