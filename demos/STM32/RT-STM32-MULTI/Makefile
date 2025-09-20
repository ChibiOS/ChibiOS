##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32C011D6-Discovery =============================
	+@make --no-print-directory -f make/stm32c011f6_discovery.make all
	@echo
	@echo === Building for STM32C031C6-Nucleo64 ==============================
	+@make --no-print-directory -f make/stm32c031c6_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32H503RB-Nucleo64 ==============================
	+@make --no-print-directory -f make/stm32h503rb_nucleo64.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f make/stm32c011f6_discovery.make clean
	@echo
	+@make --no-print-directory -f make/stm32c031c6_nucleo64.make clean
	@echo
	+@make --no-print-directory -f make/stm32h503rb_nucleo64.make clean
	@echo
#
##############################################################################
