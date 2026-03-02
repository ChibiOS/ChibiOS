ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_PWM TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/RP/LLD/PWMv1/hal_pwm_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/RP/LLD/PWMv1/hal_pwm_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/hal/ports/RP/LLD/PWMv1
