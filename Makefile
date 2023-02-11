
##	Direcotrys
O=bin
B=boot
K=kern

OF=$(O)/*.bin
DF=disk.img

##	Makefile target
all: bootl kernl diski run

bootl:
	$(MAKE) -C boot

kernl:
	$(MAKE) -C kern
	
diski:
	dd if=/dev/zero of=$(DF) bs=512 count=2880
	mkfs.fat -F 12 -n "PELLY" $(DF)
	dd if=$(O)/boot16.bin of=$(DF) conv=notrunc
	mcopy -i $(DF) $(O)/kern16.bin "::kern16.bin"
	mcopy -i $(DF) $(O)/test.txt "::test.txt"

run:
	qemu-system-i386 -fda $(DF)

clean:
	rm $(OF)
	rm $(DF)
