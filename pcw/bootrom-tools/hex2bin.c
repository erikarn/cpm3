
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


char linebuf[80];
char ex[4];

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

	if (p) p++;
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
				ex[n] = 0;
				sscanf(ex, "%x", &n);
				putchar(n);
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

int main()
{
	while (fgets(linebuf, 80, stdin))
	{
		parsehex(linebuf);
	}
	return 0;
}
