#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

typedef struct _LJG_Stack_Header {
    size_t capacity;
    size_t top;
} _LJG_Stack_Header;

#define LJG_Stack_Init(ptr, size) \
    do { \
        errno = 0; \
        _LJG_Stack_Header* _ljg_header = malloc(sizeof(_LJG_Stack_Header) + size * sizeof(*(ptr))); \
        if (_ljg_header == NULL) { \
            errno = 1; \
            break; \
        } \
        _ljg_header->capacity = size; \
        _ljg_header->top = 0; \
        (ptr) = (void*)(_ljg_header + 1); \
    } while(0)

#define LJG_Stack_Free(ptr) free((_LJG_Stack_Header*)ptr - 1)

#define LJG_Stack_Push(ptr, value) \
    do { \
        errno = 0; \
        _LJG_Stack_Header* _ljg_header = (_LJG_Stack_Header*)(ptr) - 1; \
        if (_ljg_header->top >= _ljg_header->capacity) { \
            errno = 1; \
            break; \
        } \
        (ptr)[++_ljg_header->top] = (value); \
    } while(0)

#define LJG_Stack_Pop(ptr) (ptr)[((_LJG_Stack_Header*)(ptr) - 1)->top--]

int main() {
    float* stack = NULL; // LJG_Stack
    LJG_Stack_Init(stack, 1024);

    for (size_t i = 0; i < 100; i++) {
        LJG_Stack_Push(stack, i*2);
    }

    for (size_t i = 0; i < 50; i++) {
        printf("%.2f\n", LJG_Stack_Pop(stack));
    }

    LJG_Stack_Free(stack);
}
