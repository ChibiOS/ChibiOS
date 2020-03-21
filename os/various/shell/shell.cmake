# shell. Take a look at shell.mk
get_filename_component(CHIBIOS_SHELL_DIR
    ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

# Source files
set(CHIBIOS_SHELL_SRC
    ${CHIBIOS_ROOT_DIR}/various/shell/shell.c
    ${CHIBIOS_ROOT_DIR}/various/shell/shell_cmd.c)

# Includes
set(CHIBIOS_SHELL_INCLUDE_DIR ${CHIBIOS_SHELL_DIR})