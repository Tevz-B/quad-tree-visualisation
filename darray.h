#pragma once

#include "object.h"
#include <stdlib.h>

typedef struct DArray {
    Ball** data;
    size_t size;
    size_t capacity;
} DArray;

DArray darray_init(size_t capacity);
void darray_deinit(DArray* arr);
void darray_upsize(DArray* arr);
void darray_push(DArray* arr, Ball* el);
Ball* darray_at(DArray* arr, size_t i);
