
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *buf;
unsigned char spr_header[256];
unsigned base = 0;
unsigned len = 0;
unsigned bios_page = 0;
unsigned resbdos_page = 0;

#define ARG1 4
#define ARG2 5

void syntax(const char *progname)
{
	fprintf(stderr, "Syntax: %s base_address bios_address resbdos_address "
			"{infile {outfile}}\n\n"
			"Enter addresses in the form 0xnnnn\n",
				progname);
}


int main(int argc, char **argv)
{
	FILE *fpi, *fpo;
	unsigned addr;
	int ch, mask;
	unsigned bios;
	unsigned resbdos;

	if (argc < 3 
	||  !sscanf(argv[1], "0x%x", &base)
	||  !sscanf(argv[2], "0x%x", &bios)
	||  !sscanf(argv[3], "0x%x", &resbdos))
	{
		syntax(argv[0]);
		return EXIT_FAILURE;
	}
	if (argc >= ARG1 + 1) 
	{
		fpi = fopen(argv[ARG1], "rb");
		if (!fpi) { perror(argv[ARG1]); return EXIT_FAILURE; }
	}
	else
	{
		fpi = stdin;
	}
	if (argc >= ARG2 + 1) 
	{
		fpo = fopen(argv[ARG2], "wb");
		if (!fpo) { perror(argv[ARG2]); return EXIT_FAILURE; }
	}
	else
	{
		fpo = stdout;
	}
	if (fread(spr_header, 1, sizeof(spr_header), fpi) != sizeof(spr_header))
	{
		fprintf(stderr, "SPR header not read\n");
		return EXIT_FAILURE;
	}
	resbdos_page = resbdos >> 8;
	bios_page = bios >> 8;
	len = spr_header[1] + 256 * spr_header[2];
	buf = malloc(len);
	if (!buf)
	{
		fprintf(stderr, "malloc() failed\n");
		return EXIT_FAILURE;
	}
	if (fread(buf, 1, len, fpi) != len)
	{
		fprintf(stderr, "SPR body not read\n");
		return EXIT_FAILURE;
	}
	mask = 0;
	for (addr = 0; addr < len; addr++)
	{
		if (!mask)
		{
			ch = fgetc(fpi);
			mask = 0x80;			
			if (ch == EOF)
			{
				fprintf(stderr, "SPR relocation map not read\n");
				return EXIT_FAILURE;
			}
		}
/*		printf("%04x: %02x", addr, buf[addr]); */
		if (ch & mask)
		{
			switch(buf[addr])
			{
				case 0xFF:
				buf[addr] = bios_page;
			/*	printf(" -> %02x\n", buf[addr]); */
				break;

				case 0xFC:
				buf[addr] = resbdos_page;
				break;

				case 0xFB:
				buf[addr] = bios_page - 1;
			/*	printf(" -> %02x\n", buf[addr]); */
				break;

				case 0xFD:
				buf[addr] = bios_page - 6;
			/*	printf(" -> %02x\n", buf[addr]); */
				break;

				default:
				buf[addr] += (base >> 8);
			/*	printf(" -> %02x\n", buf[addr]); */
			}
		}
/*		else	putchar('\n'); */
		mask = (mask >> 1) & 0xFF;
	}
	if (fwrite(buf, 1, len, fpo) != len)
	{
		fprintf(stderr, "SPR body not written\n");
		return EXIT_FAILURE;
	}
	if (fpo != stdout) fclose(fpo);
	if (fpi != stdin) fclose(fpi);
	free(buf);
	return EXIT_SUCCESS;
}
