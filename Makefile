##	C compiler (WCC)
CC=/usr/bin/watcom/binl/wcc

##	Assembler (NASM)
AS=nasm
AF=-f bin

##	Direcotrys
O=bin
B=boot

OF=$(O)/*.bin
DF=disk.img

##	Makefile target
all: bootl diski run

bootl:
	$(AS) $(AF) $(B)/boot16.S -o $(O)/boot16.bin

kernl:

diski:
	dd if=/dev/zero of=$(DF) bs=512 count=2880
	dd if=$(O)/boot16.bin of=$(DF) conv=notrunc

run:
	qemu-system-i386 -fda $(DF)

clean:
	rm $(OF)
	rm $(DF)
