#include "utils.h"
#include "defs.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void *checked_malloc(size_t size) {
  void *ptr = malloc(size);
  PANIC_IF(ptr == NULL);
  return ptr;
}

void *checked_realloc(void *ptr, size_t size) {
  ptr = realloc(ptr, size);
  PANIC_IF(ptr == NULL && size > 0);
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
  if (fstat(fd, &statbuf) == -1 || !S_ISREG(statbuf.st_mode)) {
    int e = errno;
    PANIC_IF(close(fd) == -1);
    errno = e;
    return NULL;
  }
  size_t size = (size_t)statbuf.st_size;

  /* read source file */
  char *buf = checked_malloc(sizeof(char) * (size + 1));
  ssize_t how_many_read = read(fd, buf, size);
  PANIC_IF(how_many_read == -1);
  ERROR_IF(how_many_read != (ssize_t)size, "size mismatch");
  buf[size] = '\0';

  close(fd);
  return buf;
}

// vim: set ft=c ts=2 sw=2 et:
