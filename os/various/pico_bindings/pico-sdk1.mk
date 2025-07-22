# Pico-SDK files.
PICOSDKROOT   := $(CHIBIOS)/ext/pico-sdk

PICOSDKSRC     = $(PICOSDKROOT)/src/rp2_common/hardware_clocks/clocks.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_pll/pll.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_ticks/ticks.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_watchdog/watchdog.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_xosc/xosc.c \
                 $(PICOSDKROOT)/src/rp2_common/pico_runtime_init/runtime_init_clocks.c \
                 $(PICOSDKROOT)/src/rp2_common/pico_runtime_init/runtime_init.c

PICOSDKINC     = $(CHIBIOS)/os/various/pico_bindings/dumb/include \
                 $(PICOSDKROOT)/src/common/boot_picobin_headers/include \
                 $(PICOSDKROOT)/src/common/boot_picoboot_headers/include \
                 $(PICOSDKROOT)/src/common/pico_base_headers/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_base/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_boot_lock/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_claim/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_clocks/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_gpio/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_irq/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_pll/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_resets/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_sync_spin_lock/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_sync/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_ticks/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_watchdog/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_xosc/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_bootrom/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_compiler/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_panic/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_sections/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_runtime_init/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_runtime/include \
                 $(PICOSDKROOT)/src/rp2040/hardware_regs/include \
                 $(PICOSDKROOT)/src/rp2040/hardware_structs/include \
                 $(PICOSDKROOT)/src/rp2040/pico_platform/include \
                 $(PICOSDKROOT)/src/boards/include

# Shared variables
ALLCSRC += $(PICOSDKSRC)
ALLINC  += $(PICOSDKINC)
