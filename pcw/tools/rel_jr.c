#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

/*
 * This takes a hex address and the JR/DJNZ relative offset,
 * and returns which address it WOULD jump to.
 *
 * The Z80 adds the offset to the PC that's pointing to the
 * instruction after the JR/DJNZ.  However, to simplify things,
 * take the PC of the JR/DJNZ itself.
 */
int
main(int argc, const char *argv[])
{
	uint16_t pc, dst_pc;
	int8_t rel;

	pc = strtoul(argv[1], NULL, 0);
	rel = strtoul(argv[2], NULL, 0);

	dst_pc = pc + 2 + rel;

	printf("0x%x + 0x%x -> 0x%x\n",
	    pc, rel, dst_pc);

	exit(0);
}
