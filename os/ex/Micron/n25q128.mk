# List of all the N25Q128 device files.
N25Q128SRC := $(CHIBIOS)/os/hal/lib/peripherals/flash/hal_flash.c \
              $(CHIBIOS)/os/ex/Micron/n25q128.c

# Required include directories
N25Q128INC := $(CHIBIOS)/os/hal/lib/peripherals/flash \
              $(CHIBIOS)/os/ex/Micron