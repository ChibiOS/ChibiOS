# List of all the STM32F1xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F1xx/stm32_dma.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F1xx/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F1xx/adc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F1xx/ext_lld_isr.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/can_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/ext_lld.c \
			  ${CHIBIOS}/os/hal/ports/STM32/LLD/mac_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/sdc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv1/pal_lld.c \
			  ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv1/i2c_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv1/rtc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv1/spi_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/gpt_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/icu_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/pwm_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1/serial_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1/uart_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/OTGv1/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/common/ARMCMx \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F1xx \
              ${CHIBIOS}/os/hal/ports/STM32/LLD \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/OTGv1
