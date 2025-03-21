#pragma once

#include <stddef.h>

#include "object.h"

struct Vector2;

typedef struct {
    size_t object_count;
    size_t collision_check_count;
    float frame_time;
    bool use_quad_tree;
    bool result_is_list;
} StatsInfo;

void render();
