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
	int fd, ret;

	fd = open(file, O_CREAT | O_WRONLY, 0644);
	if (fd < 0) {
		warn("%s: open %s", __func__, file);
		return (0);
	}

	if ((ret = write(fd, buf + start, size)) != size) {
		close(fd);
		fprintf(stderr, "Couldn't write the full buffer (%d != %d)?\n", ret, size);
		return (0);
	}

	close(fd);
	return (1);
}

void
iterate_file(const char *buf, int size, const char *dst)
{
	int i = size - 128;

	int fd;

	fd = open(dst, O_CREAT | O_WRONLY, 0644);
	if (fd < 0) {
		warn("%s: open %s", __func__, dst);
	}

	while (i >= 0) {
		if (write(fd, buf + i, 128) != 128) {
			close(fd);
			fprintf(stderr, "Couldn't write the full buffer?\n");
		}
		i = i - 128;
	}
	close(fd);
}

int
main(int argc, const char *argv[])
{
	struct stat sb;
	char *buf = NULL;
	int fd = -1, ret, size;

	/* Read buffer */
	fd = open(argv[1], O_RDONLY);
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

	/* Needs to be a multiple of 128 bytes */
	if (size % 128 != 0) {
		fprintf(stderr, "File size isn't a multiple of 128 bytes!\n");
		exit(1);
	}

	/* Iterate backwards through the file */
	iterate_file(buf, size, argv[2]);

	/* Done! */
	free(buf);
	exit(0);
}
