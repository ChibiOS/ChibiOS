# List of all the STM32 platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32/stm32_dma.c \
              ${CHIBIOS}/os/hal/platforms/STM32/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/can_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/spi_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32
