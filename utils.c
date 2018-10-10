#include "utils.h"
#include "error.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void *checked_malloc(size_t size) {
  void *ptr = malloc(size);
  PANIC_IF(ptr == NULL);
  return ptr;
}

char *read_from_file(char *pathname) {
  int fd;
  struct stat statbuf;

  /* open the file */
  fd = open(pathname, O_RDONLY);
  if (fd == -1) {
    return NULL;
  }

  /* get size of the file */
  if (fstat(fd, &statbuf) == -1) {
    int e = errno;
    WARN_IF(close(fd) == -1, "%s", pathname);
    errno = e;
    return NULL;
  }
  if (!S_ISREG(statbuf.st_mode)) {
    return NULL;
  }
  size_t size = (size_t)statbuf.st_size;

  /* read source file */
  char *buf = checked_malloc(sizeof(char) * (size + 1));
  ssize_t how_many_read = read(fd, buf, size);
  ERROR_IF(how_many_read == -1, "%s", pathname);
  ERROR_IF(how_many_read != (ssize_t)size,
           "size mismatch (read() == %ld, size == %ld)", how_many_read, size);
  buf[size] = '\0';

  WARN_IF(close(fd) == -1, "%s", pathname);
  return buf;
}

// vim: set ft=c ts=2 sw=2 et:
