##############################################################################
# Compiler settings
#

TRGT = aarch32-
CC   = clang
CPPC = clang++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = clang
CP   = $(TRGT)objcopy
AS   = $(TRGT)as -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

#
# Compiler settings
##############################################################################
