//  Lib Jan Grdanjski implements
//  LJG_DStack
//
//  A type-agnostic growable stack with hidden metadata.
//  Same memory layout as LJG_DArray; push/pop make the
//  LIFO semantics explicit. Doubles capacity on overflow.
//
// USAGE:
//
//  LJG_DStack_Init(ds_ptr, size)
//  DESC:   Allocates a stack for type *ds_ptr, initial capacity size.
//  ERROR:  On malloc fail errno is set to ENOMEM
//
//  LJG_DStack_Push(ds_ptr, value)
//  DESC:   Pushes value onto the top of the stack.
//          Doubles capacity (realloc) when full.
//  ERROR:  On realloc fail errno is set to ENOMEM
//
//  LJG_DStack_Pop(ds_ptr)
//  DESC:   Removes and returns the top element.
//          Caller must ensure Len > 0 before calling.
//
//  LJG_DStack_Reset(ds_ptr, size)
//  DESC:   Resets count to 0 and resizes capacity to size.
//  ERROR:  On realloc fail errno is set to ENOMEM, count still reset to 0
//
//  LJG_DStack_Len(ds_ptr)
//  DESC:   Returns the number of elements as size_t.
//
//  LJG_DStack_Capacity(ds_ptr)
//  DESC:   Returns current capacity as size_t.
//
//  LJG_DStack_IsEmpty(ds_ptr)
//  DESC:   Returns true when the stack has no elements.
//
//  LJG_DStack_Free(ds_ptr)
//  DESC:   Frees the stack's memory.
//
// EXAMPLE:
//
//      int* ds_nums = NULL;
//      LJG_DStack_Init(ds_nums, 8);
//
//      LJG_DStack_Push(ds_nums, 10);
//      LJG_DStack_Push(ds_nums, 20);
//
//      while (!LJG_DStack_IsEmpty(ds_nums))
//          printf("%d\n", LJG_DStack_Pop(ds_nums));
//
//      LJG_DStack_Free(ds_nums);
//

#ifndef LJG_DSTACK_H
#define LJG_DSTACK_H

#include <stdlib.h>
#include <errno.h>

typedef struct {
    size_t count;
    size_t capacity;
} LJG_DStack_Header;

#define LJG_DStack_Init(ds_ptr, size) \
    do { \
        errno = 0; \
        LJG_DStack_Header* _ds_h = malloc(sizeof(LJG_DStack_Header) + (size) * sizeof(*(ds_ptr))); \
        if (_ds_h != NULL) { \
            _ds_h->count    = 0; \
            _ds_h->capacity = (size); \
            (ds_ptr) = (void*)(_ds_h + 1); \
        } else errno = ENOMEM; \
    } while (0)

#define LJG_DStack_Push(ds_ptr, value) \
    do { \
        errno = 0; \
        LJG_DStack_Header* _ds_h = (LJG_DStack_Header*)(ds_ptr) - 1; \
        if (_ds_h->count >= _ds_h->capacity) { \
            LJG_DStack_Header* _ds_new = realloc(_ds_h, \
                sizeof(LJG_DStack_Header) + _ds_h->capacity * 2 * sizeof(*(ds_ptr))); \
            if (_ds_new != NULL) { \
                _ds_h = _ds_new; \
                _ds_h->capacity *= 2; \
                (ds_ptr) = (void*)(_ds_h + 1); \
            } else { errno = ENOMEM; break; } \
        } \
        (ds_ptr)[_ds_h->count++] = (value); \
    } while (0)

#define LJG_DStack_Pop(ds_ptr) \
    ((ds_ptr)[--((LJG_DStack_Header*)(ds_ptr) - 1)->count])

#define LJG_DStack_Reset(ds_ptr, size) \
    do { \
        errno = 0; \
        LJG_DStack_Header* _ds_h = (LJG_DStack_Header*)(ds_ptr) - 1; \
        LJG_DStack_Header* _ds_new = realloc(_ds_h, sizeof(LJG_DStack_Header) + (size) * sizeof(*(ds_ptr))); \
        if (_ds_new != NULL) { \
            _ds_h = _ds_new; \
            _ds_h->capacity = (size); \
            (ds_ptr) = (void*)(_ds_h + 1); \
        } else errno = ENOMEM; \
        _ds_h->count = 0; \
    } while (0)

#define LJG_DStack_Len(ds_ptr)      ((LJG_DStack_Header*)(ds_ptr) - 1)->count
#define LJG_DStack_Capacity(ds_ptr) ((LJG_DStack_Header*)(ds_ptr) - 1)->capacity
#define LJG_DStack_IsEmpty(ds_ptr)  (((LJG_DStack_Header*)(ds_ptr) - 1)->count == 0)
#define LJG_DStack_Free(ds_ptr)     free((LJG_DStack_Header*)(ds_ptr) - 1)

#endif // LJG_DSTACK_H
