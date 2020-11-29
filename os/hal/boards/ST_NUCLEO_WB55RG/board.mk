# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO_WB55RG/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO_WB55RG

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
