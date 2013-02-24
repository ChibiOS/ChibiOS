# List of all the template platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/templates/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/can_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/ext_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/gpt_lld.c \
 			  ${CHIBIOS}/os/hal/platforms/templates/i2c_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/icu_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/mac_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/pwm_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/rtc_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/sdc_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/uart_lld.c \
              ${CHIBIOS}/os/hal/platforms/templates/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/templates
