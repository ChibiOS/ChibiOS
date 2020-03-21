# wrappers. Take a look at chcpp.mk
get_filename_component(CHIBIOS_WRAPPER_DIR
    ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(CHIBIOS_WRAPPER_SRC
    ${CHIBIOS_WRAPPER_DIR}/chp.cpp
    ${CHIBIOS_WRAPPER_DIR}/syscalls_cpp.cpp)

set(CHIBIOS_WRAPPER_INCLUDE_DIR ${CHIBIOS_WRAPPER_DIR})