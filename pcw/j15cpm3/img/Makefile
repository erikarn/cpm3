
all: j15cpm3.emt

BINFILES=	bootstrap-1.bin bootstrap-2.bin xbios-jumpblock-init.bin
BINFILES+=	xbios-4.bin
BINFILES+=	fid-loader.bin zero-page.bin dot-matrix-printer.bin charset.bin
BINFILES+=	ccp.bin bios-resident-rev.bin bios-banked-rev.bin
BINFILES+=	bdos-banked-rev.bin bdos-resident-rev.bin

bdos-banked-rev.bin: bdos-banked.bin
	./rev bdos-banked.bin bdos-banked-rev.bin

bios-banked-rev.bin: bios-banked.bin
	./rev bios-banked.bin bios-banked-rev.bin

bdos-resident-rev.bin: bdos-resident.bin
	./rev bdos-resident.bin bdos-resident-rev.bin

bios-resident-rev.bin: bios-resident.bin
	./rev bios-resident.bin bios-resident-rev.bin

bios-resident.bin: bios-resident.z80
	z80asm -i bios-resident.z80 -o bios-resident.bin

bootstrap-1.bin: bootstrap-1.z80
	z80asm -i bootstrap-1.z80 -o bootstrap-1.bin

bootstrap-2.bin: bootstrap-2.z80
	z80asm -i bootstrap-2.z80 -o bootstrap-2.bin

j15cpm3.emt: $(BINFILES)


# TODO: assembling emt file
# TODO: verifying how big each chunk is so things don't overflow
