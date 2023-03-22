#!/bin/sh

# global for everything
SYMFILE="xbios-jumpblock-init.sym"

# this needs to be per file being disassembled
BLKFILE="xbios-jumpblock-init.blk"

/home/adrian/local/bin/z80dasm -v -g 0x0080 -t -a -l -o xbios-jumpblock-init.txt -S ${SYMFILE} -b ${BLKFILE} xbios-jumpblock-init.bin

/home/adrian/local/bin/z80dasm -g 0x3c62 -v -a -l -S ${SYMFILE} -o xbios-4.txt xbios-4.bin
