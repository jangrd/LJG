#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

typedef struct _LJG_Queue_Header {
    size_t size;
    size_t first;
    size_t last;
    size_t current;
} _LJG_Queue_Header;

#define LJG_Queue_Init(ptr, init_size) \
    do { \
        errno = 0; \
        _LJG_Queue_Header* header = (_LJG_Queue_Header*)malloc(sizeof(_LJG_Queue_Header) + init_size * sizeof(*(ptr))); \
        if (header != NULL) { \
            header->size = init_size; \
            header->first = 0; \
            header->last = 0; \
            header->current = 0; \
            (ptr) = (void*)(header + 1); \
        } else errno = ENOMEM; \
    } while (0)

#define LJG_Queue_First(ptr) ((_LJG_Queue_Header*)(ptr) - 1)->first
#define LJG_Queue_Last(ptr) ((_LJG_Queue_Header*)(ptr) - 1)->last

#define LJG_Queue_Push(ptr, value) \
    do { \
        errno = 0; \
        _LJG_Queue_Header* header = (_LJG_Queue_Header*)(ptr) - 1; \
        if (LJG_Queue_First((ptr)) < LJG_Queue_Last((ptr))) { \
            errno = 69420; \
            break; \
        } \
        (ptr)[++(header->last)] = value; \
    } while(0)

#define LJG_Queue_Pop(ptr) (ptr)[(((_LJG_Queue_Header*)(ptr) - 1)->first)++]

// TODO: overflow calculation

int main() {
    size_t* queue; // LJG_Queue
    LJG_Queue_Init(queue, 1024);

    printf("first: %zu\n", LJG_Queue_First(queue));
    printf("last: %zu\n", LJG_Queue_Last(queue));

    for (size_t i = 0; i < 100; i++) {
        LJG_Queue_Push(queue, i*2);
    }
    
    printf("first: %zu\n", LJG_Queue_First(queue));
    printf("last: %zu\n", LJG_Queue_Last(queue));    

    for (size_t i = 0; i < 100; i++) {
        size_t value = LJG_Queue_Pop(queue);
        printf("%zu. value = %zu\n", i, value);
    }

    printf("first: %zu\n", LJG_Queue_First(queue));
    printf("last: %zu\n", LJG_Queue_Last(queue));
}
