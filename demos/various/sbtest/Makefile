CROSS?=arm-none-eabi-
CC=$(CROSS)gcc
LD=$(CROSS)gcc
STRIP=$(CROSS)strip
SIZE=$(CROSS)size

SRC=main.c start.c

OPT?=0

CFLAGS=-mcpu=cortex-m3 -mthumb -O$(OPT) -ggdb3 \
	-mword-relocations -mlong-calls -fno-common

LDFLAGS=-r -Bsymbolic -nostartfiles \
	-mcpu=cortex-m3 -mthumb -mlong-calls -fno-common \
	-T sandbox.ld

OBJS=$(SRC:.c=.o)
DEPS=$(SRC:.c=.d)

all: app.elf

-include $(DEPS)

%.o: %.c $(MAKEFILE_LIST)
	@echo " CC $<"
	@$(CC) -MMD $(CFLAGS) -o $@ -c $<

app.elf: $(OBJS)
	@echo " LINK $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	@$(STRIP) -g -o app-striped.elf $@
	@$(SIZE) --common $@
	@echo " Creating list..."
	@$(CROSS)objdump -hpxdSsrt app.elf > app.elf.list
	@$(CROSS)readelf -atSln app.elf > app.elf.read

.PHONY: clean all list

clean:
	@echo " CLEAN"
	@rm -fR $(OBJS) $(DEPS) *.elf
	@rm -f app.elf.list app.elf.read oocd.log

list:
	@echo " Creating list..."
	@$(CROSS)objdump -hpxdSsrt app.elf > app.elf.list
	@$(CROSS)readelf -atSln app.elf > app.elf.read
