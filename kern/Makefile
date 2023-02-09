
BUILD_DIR?=bin/
ASM?=nasm
ASMFLAGS?=-f obj
CC16?=/usr/bin/watcom/binl/wcc
CFLAGS16?=-4 -d3 -s -wx -ms -zl -zq -za99 # -oneatxzh
LD16?=/usr/bin/watcom/binl/wlink

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kern16/c/%.obj, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kern16/asm/%.obj, $(SOURCES_ASM))

.PHONY: all kern16 clean always

all: kern16

kern16: $(BUILD_DIR)/kern16.bin

$(BUILD_DIR)/kern16.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD16) NAME ../bin/kern16.bin FILE \{ $(OBJECTS_ASM) $(OBJECTS_C) \} OPTION MAP=$(BUILD_DIR)/kern16.map @linker.lnk

$(BUILD_DIR)/kern16/c/%.obj: %.c always
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/kern16/asm/%.obj: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/kern16/c
	mkdir -p $(BUILD_DIR)/kern16/asm

clean:
	rm -f $(BUILD_DIR)/kern16.bin
