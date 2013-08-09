# List of all the STM32F30x platform files.
PLATFORMSRC = ${CHIBIOS}/os/halnew/platforms/common/ARMCMx/nvic.c \
              ${CHIBIOS}/os/halnew/platforms/STM32F30x/stm32_dma.c \
              ${CHIBIOS}/os/halnew/platforms/STM32F30x/hal_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32F30x/adc_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32F30x/ext_lld_isr.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/can_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/ext_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/gpt_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/icu_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/pwm_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/st_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/GPIOv2/pal_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/I2Cv2/i2c_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/RTCv2/rtc_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/SPIv2/spi_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/USARTv2/serial_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/USARTv2/uart_lld.c \
              ${CHIBIOS}/os/halnew/platforms/STM32/USBv1/usb_lld.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/halnew/platforms/common/ARMCMx \
              ${CHIBIOS}/os/halnew/platforms/STM32F30x \
              ${CHIBIOS}/os/halnew/platforms/STM32 \
              ${CHIBIOS}/os/halnew/platforms/STM32/GPIOv2 \
              ${CHIBIOS}/os/halnew/platforms/STM32/I2Cv2 \
              ${CHIBIOS}/os/halnew/platforms/STM32/RTCv2 \
              ${CHIBIOS}/os/halnew/platforms/STM32/SPIv2 \
              ${CHIBIOS}/os/halnew/platforms/STM32/USARTv2 \
              ${CHIBIOS}/os/halnew/platforms/STM32/USBv1
