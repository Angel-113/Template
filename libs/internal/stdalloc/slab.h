#ifndef SLAB_H
#define SLAB_H

#include "memory.h"

typedef struct slab_t slab_t;

extern slab_t *init_slab(size_t block_size, size_t slab_size);
extern void destroy_slab(slab_t *slab);
extern void *slab_allocate(slab_t *slab);
extern void slab_deallocate(slab_t *slab, void *ptr);

#endif
