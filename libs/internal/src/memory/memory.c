#include "../../headers/memory/memory.h"

#if defined(__linux__) || defined(__APPLE__)
#include "../../core/memory/mem_alloc.posix.c"
#elif defined(__WIN32) || defined(__WIN64)
#include "../../core/memory/mem_alloc.windows.c"
#else
#error "Unknown or unsupported operating system"
#endif

extern inline mem_alloc_namespace_t init_namespace(void) {
  return (mem_alloc_namespace_t){.allocate_chunk = get_allocate(),
                                 .deallocate_chunk = get_deallocate()};
}

extern inline void
destroy_namespace(mem_alloc_namespace_t *namespace_instance) {
  *namespace_instance = (mem_alloc_namespace_t){0};
  namespace_instance = NULL;
}
