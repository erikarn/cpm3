#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int
write_region(int dest_fd, int start, int size, const char *file)
{
	struct stat sb;
	int fd = -1, ret;
	char *src_buf = NULL;
	size_t src_len;

	fd = open(file, O_RDONLY);
	if (fd < 0) {
		warn("%s: open %s", __func__, file);
		goto error;
	}

	ret = fstat(fd, &sb);
	if (ret < 0) {
		warn("%s: fstat %s", __func__, file);
		goto error;
	}
	src_len = sb.st_size;

	src_buf = calloc(1, src_len);
	if (src_buf == NULL) {
		warn("%s: calloc", __func__);
		goto error;
	}

	if (read(fd, src_buf, src_len) != src_len) {
		fprintf(stderr, "Couldn't read the full buffer?\n");
		goto error;
	}

	if (src_len != size) {
		fprintf(stderr,
		    "%s: filesize for %s mismatch (got %d, needed %d\n",
		    __func__,
		    file,
		    (int) src_len,
		    (int) size);
		goto error;
	}

	if (lseek(dest_fd, start, SEEK_SET) != start) {
		warn("%s: seek", __func__);
		goto error;
	}

	if (write(dest_fd, src_buf, size) != size) {
		fprintf(stderr, "Couldn't write the full buffer?\n");
		goto error;
	}

	close(fd);
	free(src_buf);
	return (1);
error:
	if (src_buf != NULL)
		free(src_buf);
	if (fd != -1)
		close(fd);
	return (0);
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
read_iterate_config(int dest_fd, const char *confname)
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

		write_region(dest_fd, st, le, dfile);
	}

	fclose(fp);
}

int
main(int argc, const char *argv[])
{
	int fd;

	if (argc < 3) {
		printf("Usage: %s <config file> <dest file>\n", argv[0]);
		exit(127);
	}

	fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		err(1, "open");
	}
	/* Read in the config file with offsets */
	/* This will do the bulk of the real work. */
	read_iterate_config(fd, argv[1]);

	/* Done! */
	close(fd);
	exit(0);
}
