# List of all the AVR platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_pal_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_serial_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_adc_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_ext_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_i2c_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_spi_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_gpt_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_pwm_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_icu_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_usb_lld.c \
              ${CHIBIOS}/os/hal/ports/AVR/MEGA/hal_st_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/AVR/MEGA
