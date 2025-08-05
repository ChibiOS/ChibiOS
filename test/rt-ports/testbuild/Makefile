##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for ARMv7-M ===========================================
	+@make --no-print-directory -f ./make/ARMv7-M.make all
	@echo ====================================================================
	@echo
	@echo === Building for ARMv7-M-ALT =======================================
	+@make --no-print-directory -f ./make/ARMv7-M-ALT.make all
	@echo ====================================================================
	@echo
	@echo === Building for ARMv8-M-ML ========================================
	+@make --no-print-directory -f ./make/ARMv8-M-ML.make all
	@echo ====================================================================
	@echo
	@echo === Building for ARMv8-M-ML ========================================
	+@make --no-print-directory -f ./make/ARMv8-M-ML-ALT.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/ARMv7-M.make clean
	@echo
	+@make --no-print-directory -f ./make/ARMv7-M-ALT.make clean
	@echo
	+@make --no-print-directory -f ./make/ARMv8-M-ML.make clean
	@echo
	+@make --no-print-directory -f ./make/ARMv8-M-ML-ALT.make clean
	@echo

#
##############################################################################
