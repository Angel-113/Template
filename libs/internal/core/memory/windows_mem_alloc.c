#include <stddef.h>
#include <windows.h>

inline void *get_allocate() __attribute__((constructor(101)));
inline void *get_deallcoate() __attribute__((constructor(101)));
inline void *get_allocate_aligned() __attribute__((constructor(101)));
inline void *get_deallocatte_aligned() __attribute__((constructor(101)));

static void *allocate();
static void deallocate();
static void *allocate_aligned();
static void deallocate_aligned();

inline void *get_allocate() { return &allocate; }
inline void *get_deallocate() { return &deallocate; }
inline void *get_allocate_aligned() { return &allocate_aligned; }
inline void *get_deallocate_aligned() { return &deallocate_aligned; }
