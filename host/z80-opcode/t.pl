#!/usr/local/bin/perl -w

use strict;

# argtype from this list is:
# ^ = two byte
# $ = one byte

my $arg = $ARGV[1];
my $inst_prefix = $ARGV[2];
my $addin = 0;

if (defined($inst_prefix)) {
	$inst_prefix = $inst_prefix . ",";
	$addin = 2;
} else {
	$inst_prefix = "";
	$addin = 1;
}

while (<>) {
	chomp;
	my (@a) = split(/	/);

	if (!defined $a[$arg+1]) {
		next;
	};

	printf "\t{ .opcode = { %s 0x%s }, .opcode_len = %d, .str = \"%s\", .numbytes = %d, .argtype = %d, },\n",
	     $inst_prefix, $a[1], $addin, $a[$arg], $a[$arg+1], 0;
}

