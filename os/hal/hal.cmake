# cmake file for HAL components
get_filename_component(CHIBIOS_HAL_DIR
                        ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
message(STATUS "CHIBIOS_HAL_DIR : " ${CHIBIOS_HAL_DIR})

# Source files
set(CHIBIOS_HAL_SRC
    ${CHIBIOS_HAL_DIR}/src/hal.c
    ${CHIBIOS_HAL_DIR}/src/hal_buffers.c
    ${CHIBIOS_HAL_DIR}/src/hal_queues.c
    ${CHIBIOS_HAL_DIR}/src/hal_mmcsd.c
    ${CHIBIOS_HAL_DIR}/src/hal_adc.c
    ${CHIBIOS_HAL_DIR}/src/hal_can.c
    ${CHIBIOS_HAL_DIR}/src/hal_crypto.c
    ${CHIBIOS_HAL_DIR}/src/hal_dac.c
    ${CHIBIOS_HAL_DIR}/src/hal_ext.c
    ${CHIBIOS_HAL_DIR}/src/hal_gpt.c
    ${CHIBIOS_HAL_DIR}/src/hal_i2c.c
    ${CHIBIOS_HAL_DIR}/src/hal_i2s.c
    ${CHIBIOS_HAL_DIR}/src/hal_icu.c
    ${CHIBIOS_HAL_DIR}/src/hal_mac.c
    ${CHIBIOS_HAL_DIR}/src/hal_mmc_spi.c
    ${CHIBIOS_HAL_DIR}/src/hal_pal.c
    ${CHIBIOS_HAL_DIR}/src/hal_pwm.c
    ${CHIBIOS_HAL_DIR}/src/hal_qspi.c
    ${CHIBIOS_HAL_DIR}/src/hal_rtc.c
    ${CHIBIOS_HAL_DIR}/src/hal_sdc.c
    ${CHIBIOS_HAL_DIR}/src/hal_serial.c
    ${CHIBIOS_HAL_DIR}/src/hal_serial_usb.c
    ${CHIBIOS_HAL_DIR}/src/hal_spi.c
    ${CHIBIOS_HAL_DIR}/src/hal_st.c
    ${CHIBIOS_HAL_DIR}/src/hal_uart.c
    ${CHIBIOS_HAL_DIR}/src/hal_usb.c
    ${CHIBIOS_HAL_DIR}/src/hal_wdg.c
    )

# Include directory
set(CHIBIOS_HAL_INCLUDE_DIR
    ${CHIBIOS_HAL_DIR}/include)