##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for STM32G071RB-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g071rb_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32G474RE-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32G474RE-Nucleo64 Alternate ====================
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64_alt.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32L476-Discovery ===============================
	+@make --no-print-directory -f ./make/stm32l476_discovery.make all
	@echo ====================================================================
	@echo
	@echo === Building for STM32WL55JC-Nucleo64 ==============================
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/stm32g071rb_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32g474re_nucleo64_alt.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32l476_discovery.make clean
	@echo
	+@make --no-print-directory -f ./make/stm32wl55jc_nucleo64.make clean
	@echo
#
##############################################################################
