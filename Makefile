
##	Direcotrys
O=bin
B=boot
K=kern

override BF=$(shell find ./ -type f -name '*.bin')
override DF=disk.img
override OF= $(shell find ./ -type f -name '*.o')

##	Makefile target
.PHONY: all
all: bootl kernl diski run

bootl:
	$(MAKE) -C boot

kernl:
	$(MAKE) -C kern
	
diski:
	dd if=/dev/zero of=$(DF) bs=512 count=2880
	mkfs.fat -F 12 -n "PELLY" $(DF)
	dd if=$(O)/boot16.bin of=$(DF) conv=notrunc

	mmd -i disk.img "::sys"
	mmd -i disk.img "::doc"

	mcopy -i $(DF) $(O)/kern16.bin "::kern16.bin"
	mcopy -i $(DF) $(O)/kern16.bin "::sys/kern16.bin"
	mcopy -i $(DF) $(O)/test.txt "::doc/test.txt"

run:
	qemu-system-i386 -m 1M -fda $(DF)

.PHONY: clean
clean:
	rm $(BF)
	rm $(DF)
	rm $(OF)
