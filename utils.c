#include "utils.h"
#include "error.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

void *checked_malloc(size_t size) {
  void *ptr = malloc(size);
  PANIC_IF(ptr == NULL);
  return ptr;
}

void *checked_realloc(void *ptr, size_t size) {
  assert(size > 0);
  ptr = realloc(ptr, size);
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

void *push_back(void *vec, size_t vec_length, void *item, size_t item_size) {
  vec = checked_realloc(vec, item_size * (vec_length + 1));
  uintptr_t vec_addr = (uintptr_t)vec;
  memcpy((void *)(vec_addr + item_size * vec_length), item, item_size);
  return vec;
}

void *push_back_char(void *vec, size_t vec_length, char byte) {
  return push_back(vec, vec_length, &byte, sizeof(char));
}

char *clone_str_range(const char *begin, const char *const end) {
  char *buf = checked_malloc((uintptr_t)(end - begin) + 1);
  char *ptr = buf;
  while (begin != end) {
    *ptr = *begin;
    ++ptr;
    ++begin;
  }
  *ptr = '\0';
  return buf;
}

void erase(void *const end, void *const erase_begin, void *const erase_end) {
  assert(erase_begin!= NULL);
  assert(erase_end!= NULL);

  uintptr_t erase_begin_addr = (uintptr_t)erase_begin;
  uintptr_t erase_end_addr = (uintptr_t)erase_end;
  uintptr_t end_addr = (uintptr_t)end;

  assert(erase_begin_addr< erase_end_addr);
  size_t diff = erase_end_addr - erase_begin_addr;

  while (erase_begin_addr < end_addr - diff) {
    *(char *)erase_begin_addr = *((char *)erase_begin_addr + diff);
    ++erase_begin_addr;
  }
}

char *append_str(char *head, char *tail) {
  size_t len_head = strlen(head), len_tail = strlen(tail);
  head = checked_realloc(head, sizeof(char) * (len_head + len_tail + 1));
  strcat(head, tail);
  return head;
}

// vim: set ft=c ts=2 sw=2 et:
