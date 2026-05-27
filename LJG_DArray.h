//  Lib Jan Grdanjski implements
//  LJG_DArray
//
//  A type-agnostic dynamic array with hidden metadata (count, capacity).
//  Exists to preserve square bracket indexing syntax.
//
// USAGE:
//
//  LJG_DArray_Init(darr, size)
//  DESC:   Allocates array for type *darr, initial capacity size.
//  ERROR:  On malloc fail errno is set to ENOMEM
//
//  LJG_DArray_Reset(darr, size)
//  DESC:   Resets count to 0 and resizes capacity to size.
//  ERROR:  On realloc fail errno is set to ENOMEM, count still reset to 0
//
//  LJG_DArray_PushRight(darr, value)
//  DESC:   Inserts value at the end. Alias for Insert at Len.
//  ERROR:  On realloc fail errno is set to ENOMEM
//
//  LJG_DArray_PushLeft(darr, value)
//  DESC:   Inserts value at index 0, shifting all elements right.
//  ERROR:  On realloc fail errno is set to ENOMEM
//
//  LJG_DArray_Insert(darr, value, idx)
//  DESC:   Inserts value at idx, shifting elements from idx onwards right.
//          Doubles capacity when full.
//  ERROR:  On realloc fail errno is set to ENOMEM
//
//  LJG_DArray_PopRight(darr, out)
//  DESC:   Removes last element and stores it in out.
//          Caller must ensure Len > 0 before calling.
//
//  LJG_DArray_PopLeft(darr, out)
//  DESC:   Removes first element and stores it in out, shifting all elements left.
//          Caller must ensure Len > 0 before calling.
//
//  LJG_DArray_Remove(darr, idx)
//  DESC:   Removes element at idx, shifting elements after it left.
//          Caller must ensure idx < Len before calling.
//
//  LJG_DArray_Len(darr)
//  DESC:   Returns count as size_t.
//
//  LJG_DArray_Capacity(darr)
//  DESC:   Returns current capacity as size_t.
//
//  LJG_DArray_Free(darr)
//  DESC:   Frees the allocation.
//
// SOURCES:
//  - Tsoding - "Insane Shadow Data Trick in C"
//    https://gist.github.com/rexim/48d7087bfc8ba2c28a8b0b3aa3183558
//  - https://github.com/nothings/stb
//

#ifndef LJG_DARRAY_H
#define LJG_DARRAY_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    size_t count;
    size_t capacity;
} LJG_DArray_Header;

#define LJG_DArray_Init(darr, size) \
    do { \
        errno = 0; \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)malloc(sizeof(LJG_DArray_Header) + (size) * sizeof(*(darr))); \
        if (_ljg_h != NULL) { \
            _ljg_h->count    = 0; \
            _ljg_h->capacity = (size); \
            (darr) = (void*)(_ljg_h + 1); \
        } else errno = ENOMEM; \
    } while (0)

#define LJG_DArray_Reset(darr, size) \
    do { \
        errno = 0; \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)(darr) - 1; \
        LJG_DArray_Header* _ljg_new = realloc(_ljg_h, sizeof(LJG_DArray_Header) + (size) * sizeof(*(darr))); \
        if (_ljg_new != NULL) { \
            _ljg_h = _ljg_new; \
            _ljg_h->capacity = (size); \
            (darr) = (void*)(_ljg_h + 1); \
        } else errno = ENOMEM; \
        _ljg_h->count = 0; \
    } while (0)

#define LJG_DArray_PushRight(darr, value) LJG_DArray_Insert(darr, value, LJG_DArray_Len(darr))

#define LJG_DArray_PushLeft(darr, value)  LJG_DArray_Insert(darr, value, 0)

#define LJG_DArray_Insert(darr, value, idx) \
    do { \
        errno = 0; \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)(darr) - 1; \
        if (_ljg_h->count >= _ljg_h->capacity) { \
            LJG_DArray_Header* _ljg_new = realloc(_ljg_h, sizeof(LJG_DArray_Header) + _ljg_h->capacity * 2 * sizeof(*(darr))); \
            if (_ljg_new != NULL) { \
                _ljg_h = _ljg_new; \
                _ljg_h->capacity *= 2; \
                (darr) = (void*)(_ljg_h + 1); \
            } else { \
                errno = ENOMEM; \
                break; \
            } \
        } \
        memmove((darr) + (idx) + 1, (darr) + (idx), (_ljg_h->count - (idx)) * sizeof(*(darr))); \
        (darr)[(idx)] = (value); \
        _ljg_h->count++; \
    } while (0)

#define LJG_DArray_PopRight(darr, out) \
    do { \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)(darr) - 1; \
        (out) = (darr)[--_ljg_h->count]; \
    } while (0)

#define LJG_DArray_PopLeft(darr, out) \
    do { \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)(darr) - 1; \
        (out) = (darr)[0]; \
        memmove((darr), (darr) + 1, (_ljg_h->count - 1) * sizeof(*(darr))); \
        _ljg_h->count--; \
    } while (0)

#define LJG_DArray_Remove(darr, idx) \
    do { \
        LJG_DArray_Header* _ljg_h = (LJG_DArray_Header*)(darr) - 1; \
        memmove((darr) + (idx), (darr) + (idx) + 1, (_ljg_h->count - (idx) - 1) * sizeof(*(darr))); \
        _ljg_h->count--; \
    } while (0)

#define LJG_DArray_Len(darr)      ((LJG_DArray_Header*)(darr) - 1)->count
#define LJG_DArray_Capacity(darr) ((LJG_DArray_Header*)(darr) - 1)->capacity
#define LJG_DArray_Free(darr)     free((LJG_DArray_Header*)(darr) - 1)

#endif // LJG_DARRAY_H
