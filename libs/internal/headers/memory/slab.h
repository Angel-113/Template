#ifndef SLAB_H
#define SLAB_H

#include "memory.h"

typedef struct slab_t slab_t;

extern inline slab_t *init_slab(size_t block_size, size_t slab_size)
    __attribute__((constructor(202)));
extern inline void destroy_slab(slab_t *slab) __attribute__((destructor(202)));

extern void *slab_allocate(slab_t *slab);
extern void slab_deallocate(slab_t *slab, void *ptr);
extern void *slab_allocate_aligned(slab_t *slab, size_t blocks,
                                   unsigned long int alignment);
extern void slab_deallocate_aligned(slab_t *slab, void *ptr, size_t blocks,
                                    unsigned long int alignment);

#endif
