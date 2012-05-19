# List of all the STM32F1xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32F0xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32F0xx/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2/pal_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32F0xx \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2
