#!/bin/sh

# Note: disassemble is from pcw/tools/disassemble.c in this
# git repository.

../../tools/disassemble ../orig/J15CPM3.EMT ../orig/file-memmap.txt
../../tools/rev bdos-banked-rev.bin bdos-banked.bin
../../tools/rev bdos-resident-rev.bin bdos-resident.bin
../../tools/rev bios-banked-rev.bin bios-banked.bin
../../tools/rev bios-resident-rev.bin bios-resident.bin

