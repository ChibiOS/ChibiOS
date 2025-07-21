# List of all the board related files.
BOARDSRC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO64_C031C6/board.c

# Required include directories
BOARDINC = $(CHIBIOS)/os/hal/boards/ST_NUCLEO64_C031C6

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
