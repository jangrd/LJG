//  Lib Jan Grdanjski implements
//  LJG_MetaVec
//
//  A type-agnostic vector with hidden metadata (count, capacity).
//  Exists to preserve square bracket indexing syntax.
//
// USAGE:
//
//  LJG_MetaVec_Init(PTR, SIZE)
//  DESC:   Initializes vector for type *PTR at PTR, of size SIZE.
//          Overrides errno with 0 by default
//  ERROR:  On malloc fail errno is set to ENOMEM
//
//  LJG_MetaVec_Len(METAVEC)
//  DESC:   Returns vector length as size_t
//  
//  LJG_MetaVec_Push(METAVEC, VALUE)
//  DESC:   Adds VALUE to end of METAVEC (and resizes memory if needed)
//          Overrides errno with 0 by default
//  ERROR:  On realloc fail errno is set to ENOMEM
//
//  LJG_MetaVec_Free(METAVEC)
//  DESC:   Frees memory for METAVEC
//
// EXAMPLE:
//
//      float* data;
//      LJG_MetaVec_Init(data, 32);
//
//      LJG_MetaVec_Push(data, 3.14);
//      LJG_MetaVec_Push(data, 3.14159);
//
//      for (size_t i = 0; i < LJG_MetaVec_Len(data); i++) {
//          printf("Value at %zu: %f\n", i, data[i]);
//      }
//
// SOURCES:
//  - Tsoding - "Insane Shadow Data Trick in C"
//    https://gist.github.com/rexim/48d7087bfc8ba2c28a8b0b3aa3183558
//  - https://github.com/nothings/stb
//

#ifndef __LJG_METAVEC_H__
#define __LJG_METAVEC_H__

#include <stdlib.h>
#include <errno.h>

typedef struct {
    size_t count;
    size_t capacity;
} _LJG_MetaVec_Header;

#define LJG_MetaVec_Init(metavec, size) \
    do { \
        errno = 0; \
        _LJG_MetaVec_Header* header = (_LJG_MetaVec_Header*)malloc(sizeof(_LJG_MetaVec_Header) + size * sizeof(*(metavec))); \
        if (header != NULL) { \
            header->count = 0; \
            header->capacity = size; \
            (metavec) = (void*)(header + 1); \
        } else errno = ENOMEM; \
    } while (0)

#define LJG_MetaVec_Push(metavec, value) \
    do { \
        errno = 0; \
        _LJG_MetaVec_Header* header = (_LJG_MetaVec_Header*)(metavec) - 1; \
        if (header->count >= header->capacity) { \
            _LJG_MetaVec_Header* new = realloc(header, sizeof(_LJG_MetaVec_Header) + header->capacity*2 * sizeof(*(metavec))); \
            if (new != NULL) { \
                header = new; \
                header->capacity *= 2; \
            } else { \
                errno = ENOMEM; \
                break; \
            } \
            (metavec) = (void*)(header + 1); \
        } \
        (metavec)[header->count++] = (value); \
    } while (0)

#define LJG_MetaVec_Len(metavec) ((_LJG_MetaVec_Header*)(metavec) - 1)->count

#define LJG_MetaVec_Free(metavec) free((_LJG_MetaVec_Header*)metavec - 1)

#endif // __LJG_MEGAVEC_H__
