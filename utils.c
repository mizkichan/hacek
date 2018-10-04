#include "utils.h"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

char *read_from_file(char *pathname) {
  int fd;
  struct stat statbuf;

  /* open the file */
  fd = open(pathname, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, pathname,
            strerror(errno));
    return NULL;
  }

  /* get size of the file */
  if (fstat(fd, &statbuf) != 0 || !S_ISREG(statbuf.st_mode)) {
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, pathname,
            strerror(errno));
    close(fd);
    return NULL;
  }
  size_t size = (size_t)statbuf.st_size;

  /* read source file */
  char *buf = malloc(sizeof(char) * (size + 1));
  ssize_t result = read(fd, buf, size);
  if (result != (ssize_t)size) {
    if (result == -1) {
      fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));
    } else {
      fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, "size mismatch");
    }
    close(fd);
    return NULL;
  }
  buf[size] = '\0';

  close(fd);
  return buf;
}

// vim: set ft=c ts=2 sw=2 et:
