#ifndef __LJG_VECTOR_H__
#define __LJG_VECTOR_H__

#include <stdlib.h>

#define LJG_OK   0
#define LJG_FAIL 1

typedef struct {
    void *data;
    size_t element_size;
    size_t count;
    size_t capacity;
} LJG_Vector;

#ifndef LJG_IMPLEMENTATION

bool LJG_Vector_Init(LJG_Vector* vector, size_t element_size, size_t init_capacity);
void LJG_Vector_Free(LJG_Vector* vector);
LJG_Vector* LJG_Vector_New(size_t element_size, size_t init_capacity);
void LJG_Vector_Destroy(LJG_Vector* vector);

#else // LJG_IMPLEMENTATION

bool LJG_Vector_Init(LJG_Vector* vector, size_t element_size, size_t init_capacity) {
    vector->element_size = element_size;
    vector->count = 0;

    vector->data = malloc(element_size * init_capacity);
    if (vector->data == NULL) {
        vector->capacity = 0;
        return LJG_FAIL;
    }
    
    vector->capacity = init_capacity;
    return LJG_OK;    
}

void LJG_Vector_Free(LJG_Vector* vector) {
    if (vector == NULL) return;
    if (vector->data == NULL) return;
    free(vector->data);
}

LJG_Vector* LJG_Vector_New(size_t element_size, size_t init_capacity) {
    LJG_Vector* vector = malloc(sizeof(LJG_Vector));
    LJG_Vector_Init(vector, element_size, init_capacity);
    return vector;
}

void LJG_Vector_Destroy(LJG_Vector* vector) {
    if (vector == NULL) return;
    LJG_Vector_Free(vector);
    free(vector);
}

#endif // LJG_IMPLEMENTATION

#endif // __LJG_VECTOR_H__
