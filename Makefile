##	C compiler (WCC)
CC=/usr/bin/watcom/binl/wcc

##	Assembler (NASM)
AS=nasm
AF=-f bin

##	Direcotrys
O=bin
B=boot
K=kern

OF=$(O)/*.bin
DF=disk.img

##	Makefile target
all: bootl kernl diski run

bootl:
	$(AS) $(AF) $(B)/boot16.S -o $(O)/boot16.bin

kernl:
	$(AS) $(AF) $(K)/kern16.S -o $(O)/kern16.bin

diski:
	dd if=/dev/zero of=$(DF) bs=512 count=2880
	mkfs.fat -F 12 -n "PELLY" $(DF)
	dd if=$(O)/boot16.bin of=$(DF) conv=notrunc
	mcopy -i $(DF) $(O)/kern16.bin "::kern16.bin"

run:
	qemu-system-i386 -fda $(DF)

clean:
	rm $(OF)
	rm $(DF)
