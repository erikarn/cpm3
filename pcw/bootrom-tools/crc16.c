
#include <stdio.h>
#include "crcsubs.h"

byte buf[512];

void crc16(const char *name, FILE *fp)
{
	int c;

	CRC_Clear();

	while ( (c = fgetc(fp)) != EOF)
	{
		CRC_Update(c);	
	}
	printf("%04x %s\n", CRC_Done(), name);
}

int main(int argc, char **argv)
{
	int n;

	CRC_Init(buf);

	if (argc < 2)
	{
		crc16("stdin", stdin);
		return 0;
	}

	for (n = 1; n < argc; n++)
	{
		FILE *fp = fopen(argv[n], "rb");
		if (!fp)
		{
			perror(argv[n]);
		}
		else
		{
			crc16(argv[n], fp);
			fclose(fp);
		}

	}
	return 0;
}
