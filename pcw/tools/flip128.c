
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct block
{
	unsigned char data[128];
	struct block *next;
} BLOCK;

BLOCK *head = NULL;

int main(int argc, char **argv)
{
	FILE *fpi, *fpo;

	if (argc < 2)
	{
		fpi = stdin;
	}
	else
	{
		fpi = fopen(argv[1], "rb");
		if (!fpi)
		{
			perror(argv[1]);
			return EXIT_FAILURE;
		}
	}
	if (argc < 3)
	{
		fpo = stdout;	
	}
	else
	{
		fpo = fopen(argv[2], "wb");
		if (!fpo)
		{
			if (fpi != stdin) fclose(fpi);
			perror(argv[2]);
			return EXIT_FAILURE;
		}
	}
	while (1)
	{
		unsigned char buf[128];
		BLOCK *blk;

		if (fread(buf, 1, sizeof(buf), fpi) < sizeof(buf))
		{
			break;
		}
		blk = malloc(sizeof(BLOCK));
		if (!blk)
		{
			fprintf(stderr, "Out of memory\n");
			return EXIT_FAILURE;
		}
		memcpy(blk->data, buf, sizeof(buf));
		blk->next = head;
		head = blk;
	}
	while (head != NULL)
	{
		BLOCK *b = head->next;

		fwrite(head->data, 1, 128, fpo);
		free(head);
		head = b;
	}
	if (fpo != stdout) fclose(fpo);
	if (fpi != stdin) fclose(fpi);

	return EXIT_SUCCESS;
}
