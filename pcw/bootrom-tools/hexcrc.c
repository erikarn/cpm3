
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "crcsubs.h"

char linebuf[80];
char ex[4];
int addr;


int ishex(char c)
{
	if (isdigit(c)) return 1;
	if (c >= 'A' && c <= 'F') return 1;
	if (c >= 'a' && c <= 'f') return 1;
	return 0;
}

void parsehex(char *buf)
{
	int n = 0;
	char c;
	char *p = strchr(buf, ':');

	if (p) ++p;
	else p = buf;
	while (*p)
	{
		c = toupper(*p);
		p++;
		if (isspace(c)) continue;
		if (ishex(c))
		{
			ex[n++] = c;
			if (n == 2)
			{
				if ((addr % 16) == 0)
					printf("%04x: ", addr);
				ex[n] = 0;
				sscanf(ex, "%x", &n);
				CRC_Update(n);
				addr++;
				printf("%02x ", n);
				if ((addr % 16) == 0)
				{
					printf(" %04x\n", CRC_Done());
					CRC_Clear();	
				}	
				n = 0;
			}
		}
		else
		{
			fprintf(stderr, "Invalid hex digit: %c\n", c);
			exit(1);
		}
	}
}

byte table[512];

int main()
{
	addr = 0;
	CRC_Init(table);
	while (fgets(linebuf, 80, stdin))
	{
		parsehex(linebuf);
	}
	return 0;
}
