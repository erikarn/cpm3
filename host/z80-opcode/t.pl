#!/usr/local/bin/perl -w

use strict;

# argtype from this list is:
# ^ = two byte
# $ = one byte

while (<>) {
	chomp;
	my (@a) = split(/	/);

	printf "\t{ .opcode = 0x%s, .str = \"%s\", .numbytes = %d, .argtype = %d, },\n",
	     $a[1], $a[3], $a[4], 0;
}

