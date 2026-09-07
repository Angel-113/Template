#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ROUND_UP_ALIGNMENT(X, ALIGNMENT)                                       \
  (((X) + (ALIGNMENT) + 1) / (ALIGNMENT)) * (ALIGNMENT)

#define ROUND_UP_PW_OF2(X, ALIGNMENT)                                          \
  ((X) + (ALIGNMENT) + 1) & ~((ALIGNMENT) - 1)

#define _2MB (21 << MAP_HUGE_SHIFT)
#define _1GB (30 << MAP_HUGE_SHIFT)

int __page_size = 0;

inline void *get_allocate() __attribute__((constructor(101)));
inline void *get_deallocate() __attribute__((constructor(101)));

static void *allocate(size_t *size);
static void deallocate(void *ptr, size_t size);
static inline int get_page_size(void) __attribute__((constructor(101)));

inline void *get_allocate() { return &allocate; }
inline void *get_deallocate() { return &deallocate; }
static inline int get_page_size(void) { return sysconf(_SC_PAGESIZE); }

static void *allocate(size_t *size) {
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  int fd = 0;
  off_t off = 0;

  void *ptr = NULL;

  unsigned int alignment =
      *size < _2MB ? __page_size : (*size < _1GB ? _2MB : _1GB);
  *size = alignment & (alignment - 1) ? ROUND_UP_PW_OF2(*size, alignment)
                                      : ROUND_UP_ALIGNMENT(*size, alignment);

  ptr = mmap(NULL, *size, prot, flags, fd, off);

  if (!ptr) {
    int current_errno = errno;
    fprintf(stderr, "mmap returned null ptr: %s\n", strerror(current_errno));
    printf("mmap returned null ptr: %s\n", strerror(current_errno));
  }

  return ptr;
}

static void deallocate(void *ptr, size_t size) { munmap(ptr, size); }
