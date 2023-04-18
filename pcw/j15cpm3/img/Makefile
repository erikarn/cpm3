
all: j15cpm3.emt

BINFILES=	bootstrap-1.bin bootstrap-2.bin xbios-jumpblock-init.bin
BINFILES+=	xbios-4.bin
BINFILES+=	fid-loader.bin zero-page.bin dot-matrix-printer.bin charset.bin
BINFILES+=	ccp.bin bios-resident-rev.bin bios-banked-rev.bin
BINFILES+=	bdos-banked-rev.bin bdos-resident-rev.bin

# Binaries needing reversing in the CP/M load style.
bdos-banked-rev.bin: bdos-banked.bin
	./rev bdos-banked.bin bdos-banked-rev.bin

bios-banked-rev.bin: bios-banked.bin
	./rev bios-banked.bin bios-banked-rev.bin

bdos-resident-rev.bin: bdos-resident.bin
	./rev bdos-resident.bin bdos-resident-rev.bin

bios-resident-rev.bin: bios-resident.bin
	./rev bios-resident.bin bios-resident-rev.bin

# Things I have disassembled!

bios-resident.bin: bios-resident.z80
	z80asm -i bios-resident.z80 -o bios-resident.bin

bootstrap-1.bin: bootstrap-1.z80
	z80asm -i bootstrap-1.z80 -o bootstrap-1.bin

bootstrap-2.bin: bootstrap-2.z80
	z80asm -i bootstrap-2.z80 -o bootstrap-2.bin

fid-loader.bin: fid-loader.z80
	z80asm -i fid-loader.z80 -o fid-loader.bin

j15cpm3.emt: $(BINFILES)

bdos-banked.bin: bnkbdos3.spr
	./sprlocate 0x8c00 0xfc00 0x8c00 $< > $@

bdos-resident.bin: resbdos3.spr
	./sprlocate 0xf600 0xfc00 0x8c00 $< > $@

# TODO: assembling emt file
# TODO: verifying how big each chunk is so things don't overflow

# From John - taking the CP/M 3 build .SPR files and relocating them
# for the PCW:
#
# resbdos.bin:    resbdos3.spr sprlocate flip128
#         ./sprlocate 0xf600 0xfc00 0x8c00 $< | ./flip128 > $@
#
# bnkbdos.bin:    bnkbdos3.spr sprlocate flip128
#         ./sprlocate 0x8c00 0xfc00 0x8c00 $< | ./flip128 > $@
#
# Need to triple check the offset/sizes though, so be careful.
