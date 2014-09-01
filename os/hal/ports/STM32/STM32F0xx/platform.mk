# List of all the STM32F0xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/ports/common/ARMCMx/nvic.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F0xx/stm32_dma.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F0xx/hal_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F0xx/adc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F0xx/ext_lld_isr.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/ext_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv2/pal_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv2/i2c_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv2/rtc_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv2/spi_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/gpt_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/icu_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/pwm_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1/st_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv2/serial_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv2/uart_lld.c \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USBv1/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/ports/common/ARMCMx \
              ${CHIBIOS}/os/hal/ports/STM32/STM32F0xx \
              ${CHIBIOS}/os/hal/ports/STM32/LLD \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/GPIOv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/I2Cv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/RTCv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/SPIv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/TIMv1 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USARTv2 \
              ${CHIBIOS}/os/hal/ports/STM32/LLD/USBv1
