
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

diski:
	cp $(O)/boot16.bin $(DF)
	truncate -s 1440k $(DF)

run:
	qemu-system-i386 -fda $(DF)

clean:
	rm $(OF)
	rm $(DF)
