#ifndef __LJG_METAVEC_H__
#define __LJG_METAVEC_H__

#include <stdlib.h>

typedef struct {
    size_t count;
    size_t capacity;
} LJG_MetaVec_Header;

#define LJG_MetaVec_Push(metavec, value) \
    do { \
        if (metavec == NULL) { \
            LJG_MetaVec_Header* header = (LJG_MetaVec_Header*)malloc(sizeof(*metavec) * size + sizeof(LJG_MetaVec_Header)); \
            if (header != NULL) { \
                header->count = 0; \
                header->capacity = size \
                metavec = (void*)(header + 1); \
            } \
        } \
        Header* header = (Header*)(metavec) - 1 \
        if (header->count >= header->capacity) { \
            header->capacity *= 1.5; \
            header = realloc(header, sizeof(*metavec) * header->capacity + sizeof(LJG_MetaVec_Header)); \
            metavec = (void*)(header + 1); \
        } \
    } while(0)

#define LJG_MetaVec_Len(metavec) ((LJG_MetaVec_Header*)(metavec) - 1)->count

#endif // __LJG_MEGAVEC_H__
