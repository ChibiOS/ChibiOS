# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/AVR/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/i2c_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/AVR/icu_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/AVR
