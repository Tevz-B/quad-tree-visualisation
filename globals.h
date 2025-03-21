#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "args.h"
#include "object.h"
#include "quadtree.h"

extern QuadTree g_qt;

extern Args g_args;
extern Rectangle g_bounds;
extern List g_collision_check_lines;

extern ObjectItem g_items[];
extern size_t g_item_count;

extern size_t g_collision_check_count;
extern int g_step_index;
extern float g_step_timer;
extern bool g_display_help;
extern bool g_step;
extern bool g_visualize_compare;
extern bool g_nearby_items_is_lst;
extern bool g_use_quadtree;
