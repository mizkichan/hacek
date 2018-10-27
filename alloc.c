#include "alloc.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Alloc {
  const void *ptr;
  const char *file;
  int line;
};

static struct Alloc *alloc_list = NULL;
static size_t alloc_capacity = 0;

static void check_alloc(void);
static void alloc_add(void *, const char *, int);
static void alloc_grow(void);
static void alloc_remove(const char *, int, void *);

void alloc_init(void) { PANIC_IF(atexit(check_alloc)); }

static void check_alloc(void) {
  for (size_t i = 0; i < alloc_capacity; ++i) {
    WARN_IF(alloc_list[i].ptr, "Memory was not freed (allocated at %s:%d)",
            alloc_list[i].file, alloc_list[i].line);
  }
  free(alloc_list);
}

void *checked_malloc(const char *file, int line, size_t size) {
  void *ptr = malloc(size);
  PANIC_IF(!ptr);
  alloc_add(ptr, file, line);
  return ptr;
}

void *checked_realloc(const char *file, int line, void *ptr, size_t size) {
  void *new_ptr;
  PANIC_IF(size <= 0);

  if (ptr) {
    alloc_remove(file, line, ptr);
  }

  new_ptr = realloc(ptr, size);
  PANIC_IF(!new_ptr);

  alloc_add(new_ptr, file, line);
  return new_ptr;
}

void checked_free(const char *file, int line, void *ptr) {
  alloc_remove(file, line, ptr);
  free(ptr);
}

static void alloc_add(void *ptr, char const *file, int line) {
  size_t i;

  for (i = 0; i < alloc_capacity; ++i) {
    if (alloc_list[i].ptr) {
      continue;
    }

    if (alloc_list[i].ptr == ptr) {
      debug(file, line, 0, true, "The pointer has already been registered.");
    };

    alloc_list[i].ptr = ptr;
    alloc_list[i].file = file;
    alloc_list[i].line = line;
    return;
  }

  alloc_grow();
  alloc_list[i].ptr = ptr;
  alloc_list[i].file = file;
  alloc_list[i].line = line;
}

static void alloc_grow(void) {
  struct Alloc *new_alloc_list;

  if (alloc_capacity == 0) {
    alloc_capacity = 2;
  }

  new_alloc_list =
      realloc(alloc_list, sizeof(struct Alloc) * alloc_capacity * 2);
  PANIC_IF(!new_alloc_list);
  memset((new_alloc_list + alloc_capacity), 0,
         sizeof(struct Alloc) * alloc_capacity);

  alloc_list = new_alloc_list;
  alloc_capacity *= 2;
}

static void alloc_remove(const char *file, int line, void *ptr) {
  PANIC_IF(!ptr);

  if (alloc_list) {
    for (size_t i = 0; i < alloc_capacity; ++i) {
      if (alloc_list[i].ptr == ptr) {
        alloc_list[i].ptr = NULL;
        return;
      }
    }
  }

  debug(file, line, 0, true, "The pointer has never been registered.");
}

// vim: set ft=c ts=2 sw=2 et:
