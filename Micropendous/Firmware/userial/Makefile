# BOARD = USERIAL_BOARD or BUMBLEB_BOARD
BOARD=USERIAL_BOARD

ifeq ($(BOARD), USERIAL_BOARD)
MCU = at90usb647
F_CPU = 16000000UL
F_CLOCK = 16000000UL
BOARD_ID = USERIAL_BOARD
else ifeq ($(BOARD), BUMBLEB_BOARD)
MCU = at90usb162
F_CPU = 16000000UL
F_CLOCK = 16000000UL
BOARD_ID = BUMBLEB_BOARD
else
$(error Unknown board.)
endif

CC = avr-gcc
CFLAGS  = -I.
CFLAGS += -std=gnu99 -W -Wall -Wstrict-prototypes -Wundef
CFLAGS += -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -finline-limit=20
CFLAGS += -fno-split-wide-types -fno-tree-scev-cprop
CFLAGS += -ffreestanding -Os -g -gdwarf-2
CFLAGS += -DF_CPU=$(F_CPU) -mmcu=$(MCU) -DBOARD=$(BOARD_ID)
CFLAGS += -DF_CLOCK=$(F_CLOCK)
PFLAGS  = -Werror #-pedantic
#LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,--relax -Wl,--gc-sections
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size


LUFA_OPTS  = -D NO_STREAM_CALLBACKS
LUFA_OPTS += -D USB_DEVICE_ONLY
LUFA_OPTS += -D FIXED_CONTROL_ENDPOINT_SIZE=8
LUFA_OPTS += -D FIXED_NUM_CONFIGURATIONS=1
LUFA_OPTS += -D USE_FLASH_DESCRIPTORS
LUFA_OPTS += -D USE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)"
CFLAGS += $(LUFA_OPTS)

lufasrc = \
	LUFA/Drivers/USB/HighLevel/ConfigDescriptor.c \
	LUFA/Drivers/USB/HighLevel/Events.c \
	LUFA/Drivers/USB/HighLevel/USBInterrupt.c \
	LUFA/Drivers/USB/HighLevel/USBTask.c \
	LUFA/Drivers/USB/LowLevel/DevChapter9.c \
	LUFA/Drivers/USB/LowLevel/Endpoint.c \
	LUFA/Drivers/USB/LowLevel/HostChapter9.c \
	LUFA/Drivers/USB/LowLevel/Host.c \
	LUFA/Drivers/USB/LowLevel/LowLevel.c \
	LUFA/Drivers/USB/LowLevel/Pipe.c


target = $(shell sed -ne '/define *PROJECT_NAME /{s/.*"\(.*\)".*/\1/p; q;}' version.h)
src = $(wildcard *.c) $(lufasrc)
asm = $(wildcard *.S)
hdr = $(wildcard *.h)
obj = $(src:.c=.o) $(asm:.S=.o)
elf = $(target).elf
sre = $(target).srec
hex = $(target).hex
lss = $(target).lss
map = $(target).map
tag = tags

ver = $(shell sed -ne '/define *VERSION_STRING /{s/.*"\(.*\)".*/\1/p; q;}' version.h)
dst = $(target)-firmware-v$(ver)



all: $(elf) $(lss) $(hex)
	$(SIZE) $(elf)

$(elf): $(obj) $(src) $(hdr)
	$(CC) $(CFLAGS) $(LDFLAGS) $(obj) -Wl,-Map,$(map) -o $(elf)

LUFA/%.o: LUFA/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.c version.h
	$(CC) -c $(CFLAGS) $(PFLAGS) -o $@ $<

%.o: %.S version.h
	$(CC) -c $(CFLAGS) $(PFLAGS) -o $@ $<

cdc_descriptors.o: cdc_descriptors.c
	$(CC) $(filter-out -pedantic, $(CFLAGS)) -c -o $@ $^

%.sre: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.lss: %.elf
	$(OBJDUMP) -D $< > $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@


.PHONY: doc
doc:
	[ -d doc ] || mkdir doc
	doxygen

.PHONY: tags
tags:
	ctags -o $(tag) -R $(src) $(hdr)

.PHONY: clean
clean:
	$(RM) $(elf) $(obj) $(sre) $(lss) $(map) $(hex) $(tag)
	$(RM) -rf "$(dst)".zip "$(dst)"
	$(RM) -rf doc/doxygen
	$(MAKE) -C doc clean

.PHONY: program
program: $(hex)
	avrdude -p at90usb647 -P usb -c dragon_jtag -U flash:w:$(hex)

.PHONY: bootload
bootload: $(hex)
	avrdude -p at90usb647 -P /dev/ttyACM0 -c avr109 -U flash:w:$(hex)

.PHONY: readfuses
readfuses:
	hibyte=`mktemp`; \
	lobyte=`mktemp`; \
	avrdude -p at90usb647 -c dragon_jtag -P usb -U hfuse:r:$$hibyte:h -U lfuse:r:$$lobyte:h; \
    echo -n "hi: "; cat $$hibyte; \
    echo -n "lo: "; cat $$lobyte; \
	$(RM) $$hibyte $$lobyte

.PHONY: dist
dist: $(elf) $(hex)
	svn export . "$(dst)"
	cp $^ "$(dst)"
	$(RM) -rf "$(dst)"/contrib "$(dst)"/test $(lss)
	zip -r "$(dst)".zip "$(dst)"
	$(RM) -rf "$(dst)"
