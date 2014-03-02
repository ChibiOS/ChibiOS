# List of all the LPC43xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/LPC43xx/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/pal_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/serial_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/gpt_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/spi_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/lpc43xx_dma.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/mac_lld.c \
              ${CHIBIOS}/os/hal/platforms/LPC43xx/dac_lld.c
         
# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/LPC43xx
