##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32L476-Discovery ===============================
	+@make --no-print-directory -f ./make/stm32l476_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32F302-Nucleo64 ================================
	+@make --no-print-directory -f ./make/stm32f302_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC_Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32l476_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32f302_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make clean
	@echo

#
##############################################################################
