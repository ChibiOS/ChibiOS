ifeq ($(USE_SMART_BUILD),yes)
ifneq ($(findstring HAL_USE_EFL TRUE,$(HALCONF)),)
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/RP/LLD/EFLv1/rp_efl_lld.c
endif
else
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/RP/LLD/EFLv1/rp_efl_lld.c
endif

PLATFORMINC += $(CHIBIOS)/os/hal/ports/RP/LLD/EFLv1
