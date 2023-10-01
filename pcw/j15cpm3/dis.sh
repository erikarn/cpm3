#!/bin/sh

# global for everything
SYMFILE="xbios-jumpblock-init.sym"

# this needs to be per file being disassembled
BLKFILE="xbios-jumpblock-init.blk"

BLKFILE_XBIOS="xbios-4.blk"

/home/adrian/local/bin/z80dasm -v -g 0x0080 -t -a -l -o xbios-jumpblock-init.txt -S ${SYMFILE} -b ${BLKFILE} bin/xbios-jumpblock-init.bin

BLKFILE="xbios-4.blk"

/home/adrian/local/bin/z80dasm -g 0x3c62 -t -v -a -l -S ${SYMFILE} -b ${BLKFILE_XBIOS} -o xbios-4.txt bin/xbios-4.bin
