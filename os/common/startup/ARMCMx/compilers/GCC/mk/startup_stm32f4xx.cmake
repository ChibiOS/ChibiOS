set(CHIBIOS_STARTUP_DIR ${CHIBIOS_ROOT_DIR}/common/startup/ARMCMx/compilers/GCC)
# Source files in the directory
# Carefully copying what is written in startup_stm32f4.mk in ChibiOS project.
set(CHIBIOS_STARTUP_SRC
    ${CHIBIOS_STARTUP_DIR}/crt1.c
    )

# Assembly files
set(CHIBIOS_STARTUP_ASM
    ${CHIBIOS_STARTUP_DIR}/crt0_v7m.S
    ${CHIBIOS_STARTUP_DIR}/vectors.S
    )

# Include directories
# Note CHIBIOS_ROOT_DIR is ChibiOS/os folder
set(CHIBIOS_STARTUP_INCLUDE_DIR
    ${CHIBIOS_ROOT_DIR}/common/portability/GCC
    ${CHIBIOS_STARTUP_DIR}
    ${CHIBIOS_ROOT_DIR}/common/startup/ARMCMx/devices/STM32F4xx
    ${CHIBIOS_ROOT_DIR}/common/ext/ARM/CMSIS/Core/Include
    ${CHIBIOS_ROOT_DIR}/common/ext/ST/STM32F4xx
    )

# Linker script directory
set(CHIBIOS_STARTUP_LINKER_DIR
    ${CHIBIOS_ROOT_DIR}/common/startup/ARMCMx/compilers/GCC/ld)