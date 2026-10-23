#ifndef BASE_H
#define BASE_H

#include "libs/internal/i8string/i8string.h"
#include <inttypes.h>
#include <stdbool.h>

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;

typedef u8_t *byte_t;
typedef u16_t *dbyte_t;
typedef u32_t *qbyte_t;
typedef u64_t *obyte_t;

typedef float f32_t;  /* minumum precision float type */
typedef double f64_t; /* double precision float type */

extern void __check_floating_point_size(void) __attribute__((constructor(101)));
extern void __check_word_size(void) __attribute__((constructor(101)));

#if DEBUG_TEMPLATE
extern void __unit_testing_internal_libs(void)
    __attribute__((constructor(101)));
extern void __interface_testing_internal_libs(void)
    __attribute__((constructor(101)));
extern void __global_testing_internal_libs(void)
    __attribute__((constructor(101)));
#endif

#endif
