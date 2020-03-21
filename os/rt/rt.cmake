get_filename_component(CHIBIOS_RT_DIR
    ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)

set(CHIBIOS_RT_SRC
    ${CHIBIOS_RT_DIR}/src/chsys.c
    ${CHIBIOS_RT_DIR}/src/chdebug.c
    ${CHIBIOS_RT_DIR}/src/chtrace.c
    ${CHIBIOS_RT_DIR}/src/chvt.c
    ${CHIBIOS_RT_DIR}/src/chschd.c
    ${CHIBIOS_RT_DIR}/src/chthreads.c
    ${CHIBIOS_RT_DIR}/src/chtm.c
    ${CHIBIOS_RT_DIR}/src/chstats.c
    ${CHIBIOS_RT_DIR}/src/chregistry.c
    ${CHIBIOS_RT_DIR}/src/chsem.c
    ${CHIBIOS_RT_DIR}/src/chmtx.c
    ${CHIBIOS_RT_DIR}/src/chcond.c
    ${CHIBIOS_RT_DIR}/src/chevents.c
    ${CHIBIOS_RT_DIR}/src/chmsg.c
    ${CHIBIOS_RT_DIR}/src/chdynamic.c
    ${CHIBIOS_ROOT_DIR}/common/oslib/src/chmboxes.c
    ${CHIBIOS_ROOT_DIR}/common/oslib/src/chmemcore.c
    ${CHIBIOS_ROOT_DIR}/common/oslib/src/chheap.c
    ${CHIBIOS_ROOT_DIR}/common/oslib/src/chmempools.c
    ${CHIBIOS_ROOT_DIR}/common/oslib/src/chfactory.c
)

set(CHIBIOS_RT_INCLUDE_DIR 
    ${CHIBIOS_RT_DIR}/include
    ${CHIBIOS_ROOT_DIR}/common/oslib/include)