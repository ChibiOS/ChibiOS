# OSAL related files. Take a look at osal.mk file in ChibiOS
get_filename_component(CHIBIOS_OSAL_DIR
    ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(CHIBIOS_OSAL_SRC ${CHIBIOS_OSAL_DIR}/osal.c)

set(CHIBIOS_OSAL_INCLUDE_DIR ${CHIBIOS_OSAL_DIR})