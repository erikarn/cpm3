#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int
write_region(const char *buf, int start, int size, const char *file)
{
	int fd;

	fd = open(file, O_CREAT | O_WRONLY, 0644);
	if (fd < 0) {
		warn("%s: open %s", __func__, file);
		return (0);
	}

	if (write(fd, buf + start, size) != size) {
		close(fd);
		fprintf(stderr, "Couldn't write the full buffer?\n");
		return (0);
	}

	close(fd);
	return (1);
}

static void
trim_trailing_crlf(char *str)
{
	int l;

	l = strlen(str);
	if (l == 0) {
		return;
	}

	/* Yes this isn't as efficient as pointer iteration, sue me */
	l--;
	while (l != 0) {
		if (str[l] == '\r' || str[l] == '\n') {
			str[l] = 0;
		} else {
			break;
		}
		l--;
	}

}

void
read_iterate_config(const char *buf, int size, const char *confname)
{
	FILE *fp;
	int st, le, ret;
	char lbuf[256];
	char dfile[256];

	fp = fopen(confname, "r");
	if (fp == NULL) {
		warn("%s: fopen %s failed", __func__, confname);
	}

	while (!feof(fp)) {
		if (fgets(lbuf, 256, fp) == NULL) {
			break;
		}
		trim_trailing_crlf(lbuf);
		//printf("read: %s\n", lbuf);
		ret = sscanf(lbuf, "%x,%x,%s", &st, &le, dfile);
		if (ret != 3) {
			continue;
		}

		/* We parsed something */
		printf("got %d, %d bytes, dest file %s\n", st, le, dfile);

		if (st + le > size) {
			printf("note: ran off end of file (%s)\n", lbuf);
			continue;
		}

		write_region(buf, st, le, dfile);
	}

	fclose(fp);
}

int
main(int argc, const char *argv[])
{
	struct stat sb;
	char *buf = NULL;
	int fd = -1, ret, size;

	/* Read buffer */
	fd = open("J15CPM3.EMT", O_RDONLY);
	if (fd < 0) {
		err(1, "fopen");
	}

	ret = fstat(fd, &sb);
	if (ret < 0) {
		err(1, "fstat");
	}

	size = sb.st_size;

	buf = calloc(size, sizeof(char));
	if (buf == NULL) {
		err(1, "calloc");
	}

	if (read(fd, buf, size) != size) {
		fprintf(stderr, "Couldn't read file in one read!\n");
		exit(1);
	}
	close(fd); fd = -1;

	/* Next, read in the config file with offsets */
	read_iterate_config(buf, size, "file-memmap.txt");

	/* Done! */
	free(buf);
	exit(0);
}
