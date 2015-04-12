# List of all the STM32F2xx/STM32F4xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F4xx/stm32_dma.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F4xx/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F4xx/adc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F4xx/ext_lld_isr.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/can_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/ext_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/mac_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/sdc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/DACv1/dac_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv2/pal_lld.c \
 			  ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv1/i2c_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/OTGv1/usb_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv2/rtc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv1/i2s_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv1/spi_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/gpt_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/icu_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/pwm_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/st_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1/serial_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1/uart_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/common/ARMCMx \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F4xx \
              ${CHIBIOS}/os/hal/ports/STM32/LLD \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/DACv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/OTGv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/FSMCv1
