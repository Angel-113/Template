#include "../../headers/memory/slab.h"
#include <stdlib.h>

#define STD_INIT_CAPACITY 5
#define STD_MINIMUM_BLOCK_SIZE WORD

/*
 * If the idea behind this slab is to connect systems that use
 * "huge" sections of memory, then, the std init capacity shouldn't be
 * neither too large or too small.
 */

struct buffer_t {
  size_t current;
  size_t size;
  size_t capacity;
  unsigned char *buffer;
} buffer_t;

typedef struct slab_t {
  size_t size;
  size_t buffers;
  void *free_list;
  struct buffer_t *buffer;
} slab_t __attribute__((aligned(WORD)));

static void *free_list_push(void *free_list, void *ptr);
static void *free_list_pop(void *free_list);

extern inline slab_t *init_slab(size_t block_size, size_t slab_size) {
  slab_t *slab = NULL;
  mem_alloc_namespace_t namespace = init_namespace();

  size_t size = !slab_size ? block_size * STD_INIT_CAPACITY + sizeof(slab_t)
                           : ROUND_UP_PW_OF2(slab_size, __page_size);

  slab = namespace.allocate_chunk(&size);

  if (!slab)
    exit(EXIT_FAILURE);

  slab->size = block_size;
  slab->buffer->buffer = (unsigned char *)slab + sizeof(slab_t);
  slab->buffer->size = size;
  slab->buffer->capacity = (slab->buffer->size - sizeof(slab_t)) / block_size;
  slab->buffer->current = 0;

  slab->free_list = NULL;

  destroy_namespace(&namespace);

  return slab;
}

extern inline void destroy_slab(slab_t *slab) {
  mem_alloc_namespace_t namespace = init_namespace();
  while (slab->buffers-- >= 1) {
    namespace.deallocate_chunk(&slab->buffer[slab->buffers],
                               slab->buffer[slab->buffers].size);
  }
  namespace.deallocate_chunk(slab, slab->buffer[0].size);
  destroy_namespace(&namespace);
}

extern void *slab_allocate(slab_t *slab) {
  if (!slab)
    return NULL;

  if (slab->free_list)
    return free_list_pop(slab->free_list);

  void *ptr = NULL;
  size_t current = slab->buffer[slab->buffers].current;
  size_t capacity = slab->buffer[slab->buffers].capacity;

  if (!(current < capacity - 1)) { /* Allocate new buffer */
    mem_alloc_namespace_t namespace = init_namespace();

    size_t size = STD_INIT_CAPACITY * slab->size + sizeof(struct buffer_t);
    struct buffer_t *new_buffer = namespace.allocate_chunk(&size);

    if (!new_buffer)
      exit(EXIT_FAILURE);

    new_buffer->capacity = (size - sizeof(struct buffer_t)) / slab->size;
    new_buffer->size = size;
    new_buffer->current = 0;

    struct buffer_t *current_new = &slab->buffer[++slab->buffers];
    *current_new = *new_buffer;

    destroy_namespace(&namespace);
  }

  ptr = (void *)((unsigned char *)&slab->buffer[slab->buffers].buffer[current]);
  current++;

  return ptr;
}

extern void slab_deallocate(slab_t *slab, void *ptr) {
  if (!slab->free_list) {
    /* free_list = ptr and *ptr = 0 that translates to NULL if seen as a ptr  */
    slab->free_list = ptr;
    unsigned long int *aux = ptr;
    *aux = 0;
  } else
    free_list_push(slab->free_list, ptr);
}

static void *free_list_pop(void *free_list) {
  if (!free_list)
    return NULL;

  void *ptr = free_list;
  unsigned long int *aux = free_list;

  if (*aux == 0)
    free_list = NULL;
  else
    free_list = (void *)(*aux);

  return ptr;
}

static void *free_list_push(void *free_list, void *ptr) {
  if (!free_list) {
    free_list = ptr;
    *((unsigned long int *)free_list) = 0;
    return free_list;
  }

  unsigned long int address = (unsigned long int)free_list;
  unsigned long int *aux = ptr;

  *aux = address;
  free_list = ptr;

  return free_list;
}
