# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/RP_PICO2_RP2350/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/os/hal/boards/RP_PICO2_RP2350

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
