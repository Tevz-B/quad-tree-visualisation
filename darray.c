#include "darray.h"
#include <stdio.h>


DArray darray_init(size_t capacity)
{
    if (capacity == 0) {
        printf("invalid capacity");
        exit(1);
    }

    Ball** data = malloc(capacity * sizeof(Ball*));
    if (data == 0) {
        printf("failed to alloc array");
        exit(1);
    }
    return (DArray){.data = data, .size = 0, .capacity = capacity};
}

void darray_deinit(DArray* arr)
{
    free(arr->data);
    arr->size = 0;
    arr->capacity = 0;
}

void darray_upsize(DArray* arr)
{
    arr->capacity = arr->capacity * 2;
    arr->data = realloc(arr->data, arr->capacity * sizeof(Ball));
    if (arr->data == 0) {
        printf("failed to realloc array");
        exit(1);
    }
}

void darray_push(DArray* arr, Ball* el)
{
    if (arr->capacity < arr->size + 1) {
        darray_upsize(arr);
    }
    arr->data[arr->size++] = el;
}

Ball* darray_at(DArray* arr, size_t i)
{
    return arr->data[i];
}

