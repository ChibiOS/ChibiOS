##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32G474RE-Nucleo64 Alternate ====================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64_alt.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L552ZE-Nucleo144 =============================
	+@make --no-print-directory -f ./make/stm32l552ze_nucleo144.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L552ZE-Nucleo144 Alternate ===================
	+@make --no-print-directory -f ./make/stm32l552ze_nucleo144_alt.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64_alt.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32l552ze_nucleo144.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32l552ze_nucleo144_alt.make clean
	@echo

#
##############################################################################
