# List of all the LPC17xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/LPC17xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/lpc17xx_dma.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/adc_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/rtc_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/i2c_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/dac_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/mac_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC17xx/can_lld.c
             
         
# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/LPC17xx
