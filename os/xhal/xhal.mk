# XHAL subsystem build.

# Dependencies.
include $(CHIBIOS)/os/common/oop/oop.mk

ifeq ($(USE_SMART_BUILD),yes)

# Configuration files directory
ifeq ($(XHALCONFDIR),)
  ifeq ($(CONFDIR),)
    XHALCONFDIR = .
  else
    XHALCONFDIR := $(CONFDIR)
  endif
endif

XHALCONF := $(strip $(shell cat $(XHALCONFDIR)/xhalconf.h | grep -E "\#define"))

# Required files.
XHALSRC := $(CHIBIOS)/os/xhal/src/hal.c \
           $(CHIBIOS)/os/xhal/src/hal_safety.c \
           $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
           $(CHIBIOS)/os/xhal/src/hal_cb_driver.c \
           $(CHIBIOS)/os/xhal/src/hal_st.c \
           $(CHIBIOS)/os/xhal/src/hal_buffered_serial.c \
           $(CHIBIOS)/os/xhal/src/hal_queues.c
ifneq ($(findstring HAL_USE_ETH TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_eth.c
endif
ifneq ($(findstring HAL_USE_GPT TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_gpt.c
endif
ifneq ($(findstring HAL_USE_I2C TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_i2c.c
endif
ifneq ($(findstring HAL_USE_ICU TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_icu.c
endif
ifneq ($(findstring HAL_USE_PWM TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_pwm.c
endif
ifneq ($(findstring HAL_USE_PAL TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_pal.c
endif
ifneq ($(findstring HAL_USE_ADC TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_adc.c
endif
ifneq ($(findstring HAL_USE_CAN TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_can.c
endif
ifneq ($(findstring HAL_USE_DAC TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_dac.c
endif
ifneq ($(findstring HAL_USE_RTC TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_rtc.c
endif
ifneq ($(findstring HAL_USE_SDC TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_sdc.c
endif
ifneq ($(findstring HAL_USE_SIO TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_sio.c
endif
ifneq ($(findstring HAL_USE_SPI TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_spi.c
endif
ifneq ($(findstring HAL_USE_USB TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_usb.c \
           $(CHIBIOS)/os/xhal/src/hal_usb_service.c \
           $(CHIBIOS)/os/xhal/src/hal_usb_binder.c
endif
ifneq ($(findstring HAL_USE_WSPI TRUE,$(XHALCONF)),)
XHALSRC += $(CHIBIOS)/os/xhal/src/hal_wspi.c
endif
else
XHALSRC = $(CHIBIOS)/os/xhal/src/hal.c \
          $(CHIBIOS)/os/xhal/src/hal_safety.c \
          $(CHIBIOS)/os/xhal/src/hal_base_driver.c \
          $(CHIBIOS)/os/xhal/src/hal_cb_driver.c \
          $(CHIBIOS)/os/xhal/src/hal_st.c \
          $(CHIBIOS)/os/xhal/src/hal_buffered_serial.c \
          $(CHIBIOS)/os/xhal/src/hal_queues.c \
          $(CHIBIOS)/os/xhal/src/hal_adc.c \
          $(CHIBIOS)/os/xhal/src/hal_can.c \
          $(CHIBIOS)/os/xhal/src/hal_dac.c \
          $(CHIBIOS)/os/xhal/src/hal_eth.c \
          $(CHIBIOS)/os/xhal/src/hal_gpt.c \
          $(CHIBIOS)/os/xhal/src/hal_i2c.c \
          $(CHIBIOS)/os/xhal/src/hal_icu.c \
          $(CHIBIOS)/os/xhal/src/hal_pal.c \
          $(CHIBIOS)/os/xhal/src/hal_pwm.c \
          $(CHIBIOS)/os/xhal/src/hal_rtc.c \
          $(CHIBIOS)/os/xhal/src/hal_sdc.c \
          $(CHIBIOS)/os/xhal/src/hal_sio.c \
          $(CHIBIOS)/os/xhal/src/hal_spi.c \
          $(CHIBIOS)/os/xhal/src/hal_usb.c \
          $(CHIBIOS)/os/xhal/src/hal_usb_service.c \
          $(CHIBIOS)/os/xhal/src/hal_usb_binder.c \
          $(CHIBIOS)/os/xhal/src/hal_wspi.c
endif

# Required include directories
XHALINC = $(CHIBIOS)/os/xhal/include

# Shared variables
ALLCSRC += $(XHALSRC)
ALLINC  += $(XHALINC)
