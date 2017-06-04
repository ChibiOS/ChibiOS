# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/AVR/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_pal_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_serial_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_adc_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_ext_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_i2c_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_spi_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_gpt_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_pwm_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_icu_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_usb_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/hal_st_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR
