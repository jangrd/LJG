//  Lib Jan Grdanjski implements
//  LJG_DQueue
//
//  A type-agnostic circular-buffer queue with hidden metadata.
//  Wraps on push (head/tail advance mod capacity).
//  Grows only when the buffer is actually full: on resize the
//  circular data is linearized into the new allocation.
//
// USAGE:
//
//  LJG_DQueue_Init(dq_ptr, size)
//  DESC:   Allocates a queue for type *dq_ptr, initial capacity size.
//  ERROR:  On malloc fail errno is set to ENOMEM
//
//  LJG_DQueue_Push(dq_ptr, value)
//  DESC:   Appends value to the back of the queue.
//          Doubles capacity (with linearization) when full.
//  ERROR:  On malloc fail errno is set to ENOMEM
//
//  LJG_DQueue_Pop(dq_ptr, out)
//  DESC:   Removes the front element and stores it in out.
//          Caller must ensure Len > 0 before calling.
//
//  LJG_DQueue_Reset(dq_ptr, size)
//  DESC:   Resets count and head to 0 and resizes capacity to size.
//  ERROR:  On malloc fail errno is set to ENOMEM, count still reset to 0
//
//  LJG_DQueue_Len(dq_ptr)
//  DESC:   Returns the number of elements as size_t.
//
//  LJG_DQueue_Capacity(dq_ptr)
//  DESC:   Returns current capacity as size_t.
//
//  LJG_DQueue_IsEmpty(dq_ptr)
//  DESC:   Returns true when the queue has no elements.
//
//  LJG_DQueue_Free(dq_ptr)
//  DESC:   Frees the queue's memory.
//
// EXAMPLE:
//
//      size_t* dq_ids = NULL;
//      LJG_DQueue_Init(dq_ids, 8);
//
//      LJG_DQueue_Push(dq_ids, 1);
//      LJG_DQueue_Push(dq_ids, 2);
//
//      while (!LJG_DQueue_IsEmpty(dq_ids)) {
//          size_t val;
//          LJG_DQueue_Pop(dq_ids, val);
//          printf("%zu\n", val);
//      }
//
//      LJG_DQueue_Free(dq_ids);
//

#ifndef LJG_DQUEUE_H
#define LJG_DQUEUE_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    size_t count;
    size_t capacity;
    size_t head;
} LJG_DQueue_Header;

#define LJG_DQueue_Init(dq_ptr, size) \
    do { \
        errno = 0; \
        LJG_DQueue_Header* _dq_h = malloc(sizeof(LJG_DQueue_Header) + (size) * sizeof(*(dq_ptr))); \
        if (_dq_h != NULL) { \
            _dq_h->count    = 0; \
            _dq_h->capacity = (size); \
            _dq_h->head     = 0; \
            (dq_ptr) = (void*)(_dq_h + 1); \
        } else errno = ENOMEM; \
    } while (0)

#define LJG_DQueue_Push(dq_ptr, value) \
    do { \
        errno = 0; \
        LJG_DQueue_Header* _dq_h = (LJG_DQueue_Header*)(dq_ptr) - 1; \
        if (_dq_h->count >= _dq_h->capacity) { \
            size_t _dq_cap = _dq_h->capacity * 2; \
            LJG_DQueue_Header* _dq_new = malloc(sizeof(LJG_DQueue_Header) + _dq_cap * sizeof(*(dq_ptr))); \
            if (_dq_new == NULL) { errno = ENOMEM; break; } \
            char* _dq_dst  = (char*)(_dq_new + 1); \
            size_t _dq_esz = sizeof(*(dq_ptr)); \
            size_t _dq_fp  = _dq_h->capacity - _dq_h->head; \
            if (_dq_fp > _dq_h->count) _dq_fp = _dq_h->count; \
            memcpy(_dq_dst, \
                   (char*)(dq_ptr) + _dq_h->head * _dq_esz, \
                   _dq_fp * _dq_esz); \
            if (_dq_h->count > _dq_fp) \
                memcpy(_dq_dst + _dq_fp * _dq_esz, \
                       (char*)(dq_ptr), \
                       (_dq_h->count - _dq_fp) * _dq_esz); \
            _dq_new->head     = 0; \
            _dq_new->count    = _dq_h->count; \
            _dq_new->capacity = _dq_cap; \
            free(_dq_h); \
            _dq_h = _dq_new; \
            (dq_ptr) = (void*)(_dq_h + 1); \
        } \
        size_t _dq_tail = (_dq_h->head + _dq_h->count) % _dq_h->capacity; \
        (dq_ptr)[_dq_tail] = (value); \
        _dq_h->count++; \
    } while (0)

#define LJG_DQueue_Pop(dq_ptr, out) \
    do { \
        LJG_DQueue_Header* _dq_h = (LJG_DQueue_Header*)(dq_ptr) - 1; \
        (out) = (dq_ptr)[_dq_h->head]; \
        _dq_h->head = (_dq_h->head + 1) % _dq_h->capacity; \
        _dq_h->count--; \
    } while (0)

#define LJG_DQueue_Reset(dq_ptr, size) \
    do { \
        errno = 0; \
        LJG_DQueue_Header* _dq_h = (LJG_DQueue_Header*)(dq_ptr) - 1; \
        LJG_DQueue_Header* _dq_new = malloc(sizeof(LJG_DQueue_Header) + (size) * sizeof(*(dq_ptr))); \
        if (_dq_new != NULL) { \
            free(_dq_h); \
            _dq_h = _dq_new; \
            _dq_h->capacity = (size); \
            (dq_ptr) = (void*)(_dq_h + 1); \
        } else errno = ENOMEM; \
        _dq_h->count = 0; \
        _dq_h->head  = 0; \
    } while (0)

#define LJG_DQueue_Len(dq_ptr)      ((LJG_DQueue_Header*)(dq_ptr) - 1)->count
#define LJG_DQueue_Capacity(dq_ptr) ((LJG_DQueue_Header*)(dq_ptr) - 1)->capacity
#define LJG_DQueue_IsEmpty(dq_ptr)  (((LJG_DQueue_Header*)(dq_ptr) - 1)->count == 0)
#define LJG_DQueue_Free(dq_ptr)     free((LJG_DQueue_Header*)(dq_ptr) - 1)

#endif // LJG_DQUEUE_H
