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

char *append_char(char *str, char c) {
  size_t length = strlen(str);
  str = REALLOC(str, sizeof(char) * (length + 2));
  str[length] = c;
  str[length + 1] = '\0';
  return str;
}

bool starts_with(const char *haystack, const char *needle) {
  return strstr(haystack, needle) == haystack;
}

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

bool str_equals(const char *str, const char *begin, const char *end) {
  size_t n;

  if (end) {
    n = (size_t)(end - begin);
  } else {
    n = strlen(begin) + 1;
  }

  return strncmp(str, begin, n) == 0;
}

char *clone_str(const char *begin, const char *end) {
  char *result;
  size_t n;

  if (end) {
    n = (size_t)(end - begin);
  } else {
    n = strlen(begin) + 1;
  }

  result = MALLOC(n + 1);
  strncpy(result, begin, n);
  result[n] = '\0';
  return result;
}

size_t length_str(const char *str) { return strlen(str); }

char *append_str(char *a, const char *b) {
  a = REALLOC(a, length_str(a) + length_str(b) + 1);
  return strcat(a, b);
}

char *copy_str(char *dest, const char *begin, const char *end) {
  size_t n;

  if (end) {
    n = (size_t)(end - begin);
  } else {
    n = strlen(begin) + 1;
  }

  strncpy(dest, begin, n);
  dest[n] = '\0';
  return dest;
}

// vim: set ft=c ts=2 sw=2 et:
