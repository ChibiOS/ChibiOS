# List of all the STM32L1xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/STM32L1xx/stm32_dma.c \
              ${CHIBIOS}/os/hal/platforms/STM32L1xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32L1xx/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/ext_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/i2c_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/icu_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/uart_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/STM32/USBv1/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/STM32L1xx \
              ${CHIBIOS}/os/hal/platforms/STM32 \
              ${CHIBIOS}/os/hal/platforms/STM32/GPIOv2 \
              ${CHIBIOS}/os/hal/platforms/STM32/DMAv1 \
              ${CHIBIOS}/os/hal/platforms/STM32/USBv1
