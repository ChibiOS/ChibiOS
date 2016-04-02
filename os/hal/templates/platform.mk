# List of all the template platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/templates/hal_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_adc_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_can_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_dac_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_ext_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_gpt_lld.c \
 			  ${CHIBIOS}/os/hal/templates/hal_i2c_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_icu_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_mac_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_pal_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_pwm_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_sdc_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_serial_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_spi_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_uart_lld.c \
              ${CHIBIOS}/os/hal/templates/hal_usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/templates
