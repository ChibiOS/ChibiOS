# List of all the STM32F30x platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32F30x/stm32_dma.c \
              ${CHIBIOS}/os/hal/platforms/STM32F30x/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32F30x/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32F30x/ext_lld_isr.c \
              ${CHIBIOS}/os/hal/platforms/STM32/can_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/ext_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/icu_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/SPIv2/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/USARTv2/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/USARTv2/uart_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/USBv1/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32F30x \
              ${CHIBIOS}/os/hal/platforms/STM32 \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/SPIv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/USARTv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/USBv1
