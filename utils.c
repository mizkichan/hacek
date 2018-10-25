#include "error.h"
#include "utils.h"
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void *checked_malloc(size_t size) {
  void *ptr = malloc(size);
  PANIC_IF(!ptr);
  return ptr;
}

void *checked_realloc(void *ptr, size_t size) {
  PANIC_IF(size <= 0);
  ptr = realloc(ptr, size);
  PANIC_IF(!ptr);
  return ptr;
}

char *read_from_file(char *pathname) {
  int fd;
  struct stat statbuf;

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
  size_t size = (size_t)statbuf.st_size;

  /* read source file */
  char *buf = checked_malloc(sizeof(char) * (size + 1));
  ssize_t how_many_read = read(fd, buf, size);
  ERROR_IF(how_many_read < 0, "%s", pathname);
  ERROR_IF(how_many_read != (ssize_t)size,
           "size mismatch (read() == %ld, size == %ld)", how_many_read, size);
  buf[size] = '\0';

  WARN_IF(close(fd) < 0, "%s", pathname);
  return buf;
}

char *append_char(char *str, char c) {
  size_t length = strlen(str);
  str = checked_realloc(str, sizeof(char) * (length + 2));
  str[length] = c;
  str[length + 1] = '\0';
  return str;
}

char *clone_str_range(const char *begin, const char *const end) {
  return strndup(begin, (size_t)(end - begin));
}

char *append_str(char *head, char *tail) {
  size_t len_head = strlen(head), len_tail = strlen(tail);
  head = checked_realloc(head, sizeof(char) * (len_head + len_tail + 1));
  strcat(head, tail);
  return head;
}

bool starts_with(const char *haystack, const char *needle) {
  return strstr(haystack, needle) == haystack;
}

bool str_equals(const char *s1, const char *s2) { return strcmp(s1, s2) == 0; }

void erase_str(char *const str, size_t first, size_t last) {
  const size_t length = strlen(str);
  const size_t diff = last - first;

  for (size_t i = first; i + diff < last + length; ++i) {
    str[i] = str[i + diff];
  }
}

// vim: set ft=c ts=2 sw=2 et:
