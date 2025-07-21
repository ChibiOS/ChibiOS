# Pico-SDK 2 files - currently configured for the RP2350
PICOSDKROOT   := /home/buzz2/Chibi/pico-sdk

# stdio.c lets us call stdio_init_all()
# gpio.c lets us call gpio_init() and gpio_set_dir()
PICOSDKSRC     = $(PICOSDKROOT)/src/rp2_common/hardware_clocks/clocks.c \
                 $(PICOSDKROOT)/src/rp2_common/pico_stdio/stdio.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_gpio/gpio.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_pll/pll.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_watchdog/watchdog.c \
                 $(PICOSDKROOT)/src/rp2_common/hardware_xosc/xosc.c 

ifeq ($(USE_WIFI_VARIANT),yes)
PICOSDKSRC    += $(PICOSDKROOT)/src/rp2_common/pico_cyw43_arch/cyw43_arch.c \
                 $(PICOSDKROOT)/lib/cyw43-driver/src/cyw43_ctrl.c \
                 $(PICOSDKROOT)/lib/cyw43-driver/src/cyw43_ll.c 
endif

PICOSDKINC     = $(CHIBIOS)//os/various/pico_bindings/dumb/include \
                 $(PICOSDKROOT)/src/common/pico_base_headers/include \
                 $(PICOSDKROOT)/src/common/pico_stdlib_headers/include \
                 $(PICOSDKROOT)/src/common/pico_time/include \
                 $(PICOSDKROOT)/src/common/pico_sync/include \
                 $(PICOSDKROOT)/src/rp2350/pico_platform/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_stdio/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_compiler/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_sections/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_platform_panic/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_base/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_clocks/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_runtime_init/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_gpio/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_irq/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_pll/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_resets/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_watchdog/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_xosc/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_ticks/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_timer/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_uart/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_sync/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_sync_spin_lock/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_bootrom/include \
                 $(PICOSDKROOT)/src/rp2_common/boot_bootrom_headers/include \
                 $(PICOSDKROOT)/src/rp2_common/hardware_boot_lock/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_flash/include \
                 $(PICOSDKROOT)/src/rp2350/hardware_regs/include \
                 $(PICOSDKROOT)/src/rp2350/hardware_structs/include \
                 $(PICOSDKROOT)/src/boards/include

ifeq ($(USE_WIFI_VARIANT),yes)
PICOSDKSRC    += $(PICOSDKROOT)/src/rp2_common/pico_async_context/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_cyw43_arch/include \
                 $(PICOSDKROOT)/lib/cyw43-driver/src/ \
                 $(PICOSDKROOT)/lib/cyw43-driver/firmware \
                 $(PICOSDKROOT)/lib/lwip/src/include \
                 $(PICOSDKROOT)/lib/btstack/platform/lwip/port \
                 $(PICOSDKROOT)/src/rp2_common/pico_cyw43_driver/include \
                 $(PICOSDKROOT)/src/rp2_common/pico_unique_id/include 
endif


# Shared variables
ALLCSRC += $(PICOSDKSRC)
ALLINC  += $(PICOSDKINC)
