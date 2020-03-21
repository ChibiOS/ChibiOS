# Streams. Take a look at streams.mk file in Chibios
get_filename_component(CHIBIOS_STREAMS_DIR
    ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(CHIBIOS_STREAMS_SRC
    ${CHIBIOS_STREAMS_DIR}/chprintf.c
    ${CHIBIOS_STREAMS_DIR}/memstreams.c
    ${CHIBIOS_STREAMS_DIR}/nullstreams.c
)

set(CHIBIOS_STREAMS_INCLUDE_DIR ${CHIBIOS_STREAMS_DIR})