#ifndef MEMORY_H
#define MEMORY_H

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define WORD sizeof(void *)
#define DWORD 2 * WORD
#define QWORD 4 * WORD
#define OWORD 8 * WORD

#define ROUND_UP_ALIGNMENT(X, ALIGNMENT)                                       \
  (((X) + (ALIGNMENT) + 1) / (ALIGNMENT)) * (ALIGNMENT)

#define ROUND_UP_PW_OF2(X, ALIGNMENT)                                          \
  ((X) + (ALIGNMENT) + 1) & ~((ALIGNMENT) - 1)

#define _2MB (21 << MAP_HUGE_SHIFT)
#define _1GB (30 << MAP_HUGE_SHIFT)

typedef struct mem_alloc_namespace_t {
  void *(*allocate_chunk)(size_t *size);
  void (*deallocate_chunk)(void *ptr, size_t size);
} mem_alloc_namespace_t;

extern int __page_size;
extern inline mem_alloc_namespace_t init_namespace();
extern inline void destroy_namespace(mem_alloc_namespace_t *namespace_instance);

#endif
