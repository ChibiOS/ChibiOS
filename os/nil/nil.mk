# List of all the ChibiOS/NIL kernel files, there is no need to remove the files
# from this list, you can disable parts of the kernel by editing chconf.h.
ifeq ($(USE_SMART_BUILD),yes)
CHCONF := $(strip $(shell cat chconf.h | egrep -e "\#define"))
KERNSRC := ${CHIBIOS}/os/nil/src/ch.c
ifneq ($(findstring CH_CFG_USE_MAILBOXES TRUE,$(CHCONF)),)
KERNSRC += $(CHIBIOS)/os/common/oslib/src/chmboxes.c
endif
ifneq ($(findstring CH_CFG_USE_MEMCORE TRUE,$(CHCONF)),)
KERNSRC += $(CHIBIOS)/os/common/oslib/src/chmemcore.c
endif
ifneq ($(findstring CH_CFG_USE_HEAP TRUE,$(CHCONF)),)
KERNSRC += $(CHIBIOS)/os/common/oslib/src/chheap.c
endif
ifneq ($(findstring CH_CFG_USE_MEMPOOLS TRUE,$(CHCONF)),)
KERNSRC += $(CHIBIOS)/os/common/oslib/src/chmempools.c
endif
ifneq ($(findstring CH_CFG_USE_FACTORY TRUE,$(CHCONF)),)
KERNSRC += $(CHIBIOS)/os/common/oslib/src/chfactory.c
endif
else
KERNSRC := ${CHIBIOS}/os/nil/src/ch.c \
           $(CHIBIOS)/os/common/oslib/src/chmboxes.c \
           $(CHIBIOS)/os/common/oslib/src/chmemcore.c \
           $(CHIBIOS)/os/common/oslib/src/chheap.c \
           $(CHIBIOS)/os/common/oslib/src/chmempools.c \
           $(CHIBIOS)/os/common/oslib/src/chfactory.c
endif

# Required include directories
KERNINC := ${CHIBIOS}/os/nil/include \
           ${CHIBIOS}/os/common/oslib/include

# Shared variables
ALLCSRC += $(KERNSRC)
ALLINC  += $(KERNINC)
