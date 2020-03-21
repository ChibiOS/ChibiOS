# Port sources. These are optional rt files mentioned in Makefile of Chibios
# The same type of definitions can be found in ChibiOS/os/common/ports/ARMCMx/compilers/GCC/mk
set(CHIBIOS_PORT_SRC
    ${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx/chcore.c
    ${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx/chcore_v7m.c)

set(CHIBIOS_PORT_ASM
    ${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.S)

set(CHIBIOS_PORT_INCLUDE_DIR
    ${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx
    ${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx/compilers/GCC)