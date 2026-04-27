# List of the ChibiOS sandbox VHAL files.
SBVHALSRC = $(CHIBIOS)/os/sb/vio/sbvio.c \
            $(CHIBIOS)/os/sb/vio/sbvio_adc.c \
            $(CHIBIOS)/os/sb/vio/sbvio_eth.c \
            $(CHIBIOS)/os/sb/vio/sbvio_gpt.c \
            $(CHIBIOS)/os/sb/vio/sbvio_gpio.c \
            $(CHIBIOS)/os/sb/vio/sbvio_i2c.c \
            $(CHIBIOS)/os/sb/vio/sbvio_spi.c \
            $(CHIBIOS)/os/sb/vio/sbvio_uart.c
 
SBVHALASM =

SBVHALINC = $(CHIBIOS)/os/sb/vio

# Shared variables
ALLXASMSRC += $(SBVHALASM)
ALLCSRC    += $(SBVHALSRC)
ALLINC     += $(SBVHALINC)
