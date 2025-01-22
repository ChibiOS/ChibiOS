##############################################################################
# Multi-project makefile rules
#

all:
	@echo
	@echo === Building for LS-RAMBOX-DEPLOY ==================================
	+@make --no-print-directory -f ./make/ls-rambox-deploy.make
	@echo ====================================================================
	@echo
	@echo === Building for LS-POSIX-X86-debug ================================
	+@make --no-print-directory -f ./make/ls-posix-x86-debug.make all
	@echo ====================================================================
	@echo

clean:
	@echo
	+@make --no-print-directory -f ./make/ls-rambox-deploy.make clean
	@echo
	+@make --no-print-directory -f ./make/ls-posix-x86-debug.make clean
	@echo

#
##############################################################################
