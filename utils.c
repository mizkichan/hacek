#include "alloc.h"
#include "error.h"
#include "utils.h"
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char *read_from_file(char *pathname) {
  int fd;
  struct stat statbuf;
  size_t size;
  char *buf;
  ssize_t how_many_read;

  /* open the file */
  fd = open(pathname, O_RDONLY);
  if (fd < 0) {
    return NULL;
  }

  /* get size of the file */
  if (fstat(fd, &statbuf) < 0) {
    int e = errno;
    WARN_IF(close(fd) < 0, "%s", pathname);
    errno = e;
    return NULL;
  }
  if (!S_ISREG(statbuf.st_mode)) {
    return NULL;
  }
  size = (size_t)statbuf.st_size;

  /* read source file */
  buf = MALLOC(sizeof(char) * (size + 1));
  how_many_read = read(fd, buf, size);
  ERROR_IF(how_many_read < 0, "%s", pathname);
  ERROR_IF(how_many_read != (ssize_t)size,
           "size mismatch (read() == %ld, size == %ld)", how_many_read,
           (ssize_t)size);
  buf[size] = '\0';

  WARN_IF(close(fd) < 0, "%s", pathname);
  return buf;
}

char *append_str(char *str, char c) {
  size_t length = strlen(str);
  str = REALLOC(str, sizeof(char) * (length + 2));
  str[length] = c;
  str[length + 1] = '\0';
  return str;
}

char *clone_str_range(const char *const begin, const char *const end) {
  size_t n = (size_t)(end - begin);
  char *cloned = MALLOC(n + 1);
  strncpy(cloned, begin, n);
  cloned[n] = '\0';
  return cloned;
}

bool starts_with(const char *haystack, const char *needle) {
  return strstr(haystack, needle) == haystack;
}

bool str_equals(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

char *remove_str(char *str, char c) {
  size_t diff = 0, i = 0;

  do {
    while (str[i + diff] == c) {
      ++diff;
    }
    str[i] = str[i + diff];
  } while (str[i++ + diff]);

  return str;
}

void erase_str(char *begin, char *end) {
  while (*end) {
    *begin = *end;
    ++begin;
    ++end;
  }
  *begin = '\0';
}

char *search_str(const char *haystack, const char *needle) {
  return strstr(haystack, needle);
}

char *search_char(const char *haystack, char needle) {
  return strchr(haystack, needle);
}

// vim: set ft=c ts=2 sw=2 et:
