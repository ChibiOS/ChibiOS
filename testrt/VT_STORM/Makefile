##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC-Nucleo64 SYSTICKv2 ====================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64_v2.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64_v2.make clean
	@echo

#
##############################################################################
