##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32L476-Discovery ===============================
	+@make --no-print-directory -f ./make/stm32l476_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F302R8-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32f302r8_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC_Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F767_Nucleo144 ===============================
	+@make --no-print-directory -f ./make/stm32f767zi_nucleo144.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32l476_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f302r8_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f767zi_nucleo144.make clean
	@echo

#
##############################################################################
