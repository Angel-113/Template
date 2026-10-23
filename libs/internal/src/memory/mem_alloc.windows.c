#include <stddef.h>
#include <windows.h>

inline void *get_allocate() __attribute__((constructor(101)));
inline void *get_deallocate() __attribute__((constructor(101)));

static void *allocate(size_t *size);
static void deallocate(void *ptr);

inline void *get_allocate() { return &allocate; }
inline void *get_deallocate() { return &deallocate; }

static void *allocate(size_t *size) {}
