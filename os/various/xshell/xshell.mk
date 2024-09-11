# X-Shell files.
XSHELLSRC = $(CHIBIOS)/os/various/xshell/xshell.c \
            $(CHIBIOS)/os/various/xshell/xshell_cmd.c

XSHELLINC = $(CHIBIOS)/os/various/xshell

# Shared variables
ALLCSRC += $(XSHELLSRC)
ALLINC  += $(XSHELLINC)
