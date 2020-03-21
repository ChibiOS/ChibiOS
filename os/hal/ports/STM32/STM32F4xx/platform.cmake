# Set the type of MCU used.
set(MCU cortex-m4)

if(NOT CHIBIOS_HAL_DIR)
    message(STATUS "CHIBIOS_HAL_DIR not found. Did you include hal.cmake file?")
    set(CHIBIOS_HAL_DIR ${CHIBIOS_ROOT_DIR}/hal)
endif(NOT CHIBIOS_HAL_DIR)

# List of all the STM32F4XX platform files
# Can be found in platform.mk
set(CHIBIOS_PLATFORM_SRC
    ${CHIBIOS_HAL_DIR}/ports/common/ARMCMx/nvic.c
    ${CHIBIOS_HAL_DIR}/ports/STM32/STM32F4xx/stm32_isr.c
    ${CHIBIOS_HAL_DIR}/ports/STM32/STM32F4xx/hal_lld.c
    # LLD related source files ADCV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/ADCv2/hal_adc_lld.c
    # CANv1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/CANv1/hal_can_lld.c
    # DACV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/DACv1/hal_dac_lld.c
    # DMAV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/DMAv2/stm32_dma.c
    # EXTIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/EXTIv1/hal_ext_lld.c
    # GPIOV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/GPIOv2/hal_pal_lld.c
    # I2CV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/I2Cv1/hal_i2c_lld.c
    # MACV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/MACv1/hal_mac_lld.c
    # OTGV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/OTGv1/hal_usb_lld.c
    # QUADSPIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/QUADSPIv1/hal_qspi_lld.c
    # RTCV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/RTCv2/hal_rtc_lld.c
    # SDIOV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SDIOv1/hal_sdc_lld.c
    # I2S
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SPIv1/hal_i2s_lld.c
    # SPIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SPIv1/hal_spi_lld.c
    # ST
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1/hal_st_lld.c
    # GPT
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1/hal_gpt_lld.c
    # ICU
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1/hal_icu_lld.c
    # PWM
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1/hal_pwm_lld.c
    # Serial
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/USARTv1/hal_serial_lld.c
    # UART
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/USARTv1/hal_uart_lld.c
    # WDGV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/xWDGv1/hal_wdg_lld.c
    )

# Includes
set(CHIBIOS_PLATFORM_INCLUDE_DIR
    ${CHIBIOS_HAL_DIR}/ports/common/ARMCMx
    ${CHIBIOS_HAL_DIR}/ports/STM32/STM32F4xx
    # LLD related source files ADCV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/ADCv2
    # CANv1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/CANv1
    # DACV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/DACv1
    # DMAV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/DMAv2
    # EXTIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/EXTIv1
    # GPIOV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/GPIOv2
    # I2CV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/I2Cv1
    # MACV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/MACv1
    # OTGV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/OTGv1
    # QUADSPIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/QUADSPIv1
    # RTCV2
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/RTCv2
    # SDIOV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SDIOv1
    # I2S
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SPIv1
    # SPIV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/SPIv1
    # ST
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1
    # GPT
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1
    # ICU
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1
    # PWM
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/TIMv1
    # Serial
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/USARTv1
    # UART
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/USARTv1
    # WDGV1
    ${CHIBIOS_HAL_DIR}/ports/STM32/LLD/xWDGv1
    )

# Include ARMv7m port files
include(${CHIBIOS_ROOT_DIR}/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.cmake)

# Include STM32F4xx startup and CMSIS files
include(${CHIBIOS_ROOT_DIR}/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.cmake)

