#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int get_file_size(char *pathname) {
  struct stat statbuf;

  if (stat(pathname, &statbuf) != 0 || !S_ISREG(statbuf.st_mode)) {
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, pathname,
            strerror(errno));
    return -1;
  }

  return (int)statbuf.st_size;
}

bool read_from_file(char *pathname, size_t count, char *buffer) {
  int fd;

  /* open the file */
  fd = open(pathname, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, pathname,
            strerror(errno));
  }

  /* read source file */
  read(fd, buffer, count);

  /* close the file */
  close(fd);

  return true;
}

// vim: set ft=c ts=2 sw=2 et:
