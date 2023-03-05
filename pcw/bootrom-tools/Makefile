
CC=gcc
CFLAGS=-Wall -g

ZIPFILES=Makefile README.txt crcsubs.h crc16.c crcsubs.c hex2bin.c hexcrc.c \
	dump8.dsk dump9.dsk dump.zsm boot8.bin boot9.bin font.inc crcsubs.inc

OBJECTS1=crc16.o crcsubs.o 
OBJECTS2=hexcrc.o crcsubs.o

all:	dump8.dsk dump9.dsk \
	crc16 hex2bin hexcrc \
#	8512rom.bin 9512rom.bin

crc16:	$(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS1)

hexcrc:	$(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS2)

%.bin:	%.txt hex2bin
	./hex2bin < $< > tmp.bin
	dd if=tmp.bin of=$@ bs=1 count=256 skip=2
	./crc16 < $@	

dump8.dsk:	dump8.raw
	dsktrans $< $@

dump9.dsk:	dump9.raw
	dsktrans $< $@

dump9.raw:	dump.bin boot9.bin
	dd if=/dev/zero of=tmp1 bs=512 count=8
	dd if=/dev/zero of=tmp2 bs=512 count=360
	cat	boot9.bin tmp1 dump.bin tmp2 > tmp3
	dd if=tmp3 of=$@ bs=512 count=360
	rm	tmp1 tmp2 tmp3

dump8.raw:	dump.bin boot8.bin
	dd if=/dev/zero of=tmp1 bs=512 count=8
	dd if=/dev/zero of=tmp2 bs=512 count=360
	cat	boot8.bin tmp1 dump.bin tmp2 > tmp3
	dd if=tmp3 of=$@ bs=512 count=360
	rm	tmp1 tmp2 tmp3

dump.bin:	dump.zsm
	z80asm -fb -l dump.zsm

zip:	$(ZIPFILES)
	zip bootdump.zip $(ZIPFILES)
