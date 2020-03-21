# fatfs. Take a look at fatfs.mk
# Note CHIBIOS_ROOT_DIR is ChibiOS/os
set(CHIBIOS_FATFS_SRC
    ${CHIBIOS_ROOT_DIR}/various/fatfs_bindings/fatfs_diskio.c
    ${CHIBIOS_ROOT_DIR}/various/fatfs_bindings/fatfs_syscall.c
    ${CHIBIOS_ROOT_DIR}/../ext/fatfs/src/ff.c
    ${CHIBIOS_ROOT_DIR}/../ext/fatfs/src/option/unicode.c
)

set(CHIBIOS_FATFS_INCLUDE_DIR
    ${CHIBIOS_ROOT_DIR}/../ext/fatfs/src)