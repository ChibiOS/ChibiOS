# List of all the STM32F3xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32F3xx/stm32_dma.c \
              ${CHIBIOS}/os/hal/platforms/STM32F3xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/icu_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/SPIv2/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/USARTv2/serial_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32F3xx \
              ${CHIBIOS}/os/hal/platforms/STM32 \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/SPIv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/USARTv2
