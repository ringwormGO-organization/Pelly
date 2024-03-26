
BUILD_DIR=$(abspath bin)
ASM=nasm
ASMFLAGS?=-f obj
CC16?=wcc
CFLAGS16?=-4 -d3 -s -wx -ms -zl -zq -za99 # -oneatxzh
LD16?=wlink

SOURCES_C=$(shell find -type f -name "*.c")
SOURCES_ASM=$(shell find -type f -name "*.asm")
OBJECTS_C=$(abspath $(addprefix $(BUILD_DIR)/kern16/c/, $(SOURCES_C:.c=.o)))
OBJECTS_ASM=$(abspath $(addprefix $(BUILD_DIR)/kern16/asm/, $(SOURCES_ASM:.asm=.asm_o)))

.PHONY: all kern16 clean always

all: kern16

kern16: $(BUILD_DIR)/kern16.bin

$(BUILD_DIR)/kern16.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD16) NAME ../bin/kern16.bin FILE \{ $(OBJECTS_ASM) $(OBJECTS_C) \} OPTION MAP=$(BUILD_DIR)/kern16.map @linker.lnk

$(BUILD_DIR)/kern16/c/%.o: %.c always
	@mkdir -p $(@D)
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/kern16/asm/%.asm_o: %.asm always
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/kern16/c
	mkdir -p $(BUILD_DIR)/kern16/asm

clean:
	rm -f $(BUILD_DIR)/kern16.bin
