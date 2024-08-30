
all: j15cpm3.emt

clean:
	$(RM) *emt

# Bin files that we generate from source
BINFILES=	bootstrap-1.bin bootstrap-2.bin fid-loader.bin
BINFILES+=	bios-resident-rev.bin
BINFILES+=	zero-page.bin
BINFILES+=	charset.bin

# Bin files that are binary data
BINFILES+=	dot-matrix-printer.bin
BINFILES+=	padding-1.bin padding-2.bin

# Bin files that we currently don't have reverse engineered
# source code for.

BINFILES+=	xbios-jumpblock-init.bin
BINFILES+=	xbios-4.bin
BINFILES+=	bios-banked-rev.bin

# Bin files from a CP/M 3 build, thrown through the
# relocation / reordering process
#
# Note that ccp.bin, resbdos3.spr and bnkbdos3.spr should be
# the output from a CPM/3 build; on my PCW image disassembly
# they're ccp.bin, bdos-resident.bin and bdos-banked.bin.
# (And I've already unflipped them to make disassembly easier,
# hence the trip through 'rev' here.)
#
BINFILES+=	ccp.bin
BINFILES+=	bdos-banked-rev.bin bdos-resident-rev.bin

# Binaries needing reversing in the CP/M load style.
bdos-banked-rev.bin: bdos-banked.bin
	../../tools/rev bdos-banked.bin bdos-banked-rev.bin

bdos-resident-rev.bin: bdos-resident.bin
	../../tools/rev bdos-resident.bin bdos-resident-rev.bin

bios-banked-rev.bin: ../bin/bios-banked.bin
	../../tools/rev ../bin/bios-banked.bin bios-banked-rev.bin

bios-resident-rev.bin: bios-resident.bin
	../../tools/rev bios-resident.bin bios-resident-rev.bin

# Things I have disassembled!

bios-resident.bin: ../src/bios-resident.z80
	z80asm -i ../src/bios-resident.z80 -o bios-resident.bin

bootstrap-1.bin: ../src/bootstrap-1.z80
	z80asm -i ../src/bootstrap-1.z80 -o bootstrap-1.bin

bootstrap-2.bin: ../src/bootstrap-2.z80
	z80asm -i ../src/bootstrap-2.z80 -o bootstrap-2.bin

fid-loader.bin: ../src/fid-loader.z80
	z80asm -i ../src/fid-loader.z80 -o fid-loader.bin

zero-page.bin: ../src/zero-page.z80
	z80asm -i ../src/zero-page.z80 -o zero-page.bin

charset.bin: ../src/charset.z80
	z80asm -i ../src/charset.z80 -o charset.bin

# TODO: assembling emt file
# TODO: verifying how big each chunk is so things don't overflow
j15cpm3.emt: $(BINFILES)
	../../tools/assemble ../orig/file-memmap.txt j15cpm3.emt

# Eventually these should come from a CPM/3 build!
bdos-banked.bin: bnkbdos3.spr
	../../tools/sprlocate 0x8c00 0xfc00 0x8c00 $< > $@

bdos-resident.bin: resbdos3.spr
	../../tools/sprlocate 0xf600 0xfc00 0x8c00 $< > $@


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
