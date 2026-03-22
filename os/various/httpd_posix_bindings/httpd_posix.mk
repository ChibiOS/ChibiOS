# List of files for HTTPD-to-POSIX bindings.
HTTPDPOSIXSRC = $(CHIBIOS)/os/various/httpd_posix_bindings/httpd_posix.c

HTTPDPOSIXINC = $(CHIBIOS)/os/various/httpd_posix_bindings

# Shared variables
ALLCSRC += $(HTTPDPOSIXSRC)
ALLINC  += $(HTTPDPOSIXINC)
