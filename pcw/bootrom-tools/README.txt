
  This archive contains some programs which may help you to make dumps of
the Amstrad PCW boot ROM. 

Instructions
~~~~~~~~~~~~

1. Create a specialised boot floppy from one of the two supplied disc images.
  These are:

dump8.dsk:	PCW8512 boot image (should also work on PCW9256, 10)
dump9.dsk:	PCW9512 boot image (should also work on PCW9512+)

  Both of these are single-sided 40-track disk images. You should write 
the correct disc image to a suitable 3" or 3.5" floppy.

  Notes:
  * Only the first two tracks actually need to be written. The following
   command should work under Linux (with LibDsk installed):

	dsktrans -last 1 -otype floppy dump9.dsk /dev/fd3

  * Do not double-step the drive when writing the disk images.

2. Boot from the disc you prepared. If all goes well, you will get a screen
  showing a hex dump of the first 320 bytes of memory. Each line will look 
  something like this:

0000: BE 04 01 F3 83 ED 41 0D 78 05 87 20 F8 31 F0 FF  n+OsvmAJx1p^  C1D2
...
0130: FF 00 FB 00 FF 00 FB 00 FF 00 FF 00 FF 00 FF 00  ^ { ^ { ^ ^   AC03

  and there will be a line at the end reading something like:

CRC of 256 bytes from 0002-0101=4A75

3. Create a text file containing the contents of the first two columns on
  that screen, for addresses up to 010F. For example:

0000: be 04 01 f3 83 ed 41 0d 78 05 87 20 f8 31 f0 ff
...
0100: ee c9 00 00 00 01 00 01 02 00 f3 00 f3 00 f7 21

4. You can verify your typing by passing your text file through hexcrc:

	./hexcrc < 9512rom.txt

   which will compute and display a 16-bit CRC for each 16-byte line. You
can compare this with the CRC printed by the boot sector.

5. Once you're satisfied that all your hex digits are correct, you can 
  convert the text file to binary and extract the 256 bytes that come from 
  the ROM:

	./hex2bin < 9512rom.txt > tmp.bin
	dd if=tmp.bin of=9512rom.bin bs=1 count=256 skip=2

6. And verify that its CRC matches the one computed on the PCW:

	./crc16 <9512rom.bin


