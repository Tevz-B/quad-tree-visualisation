#include "globals.h"
#include "defines.h"

QuadTree g_qt = (QuadTree) {.root = 0};

Args g_args;
List g_collision_check_lines = (List) {.begin = 0, .end = 0};

ObjectItem g_items[MAX_BALLS];
size_t g_item_count = 0;

size_t g_collision_check_count = 0;
int g_step_index = 0;
float g_step_timer = 0.0;
bool g_display_help = false;
bool g_step = false;
bool g_visualize_compare = false;
bool g_nearby_items_is_lst = false;
bool g_use_quadtree = true;

Rectangle g_bounds = (Rectangle){
    .x = SCREEN_PADDING,
    .y = SCREEN_TOP_PADDING,
    .height = BOUNDS_HEIGHT,
    .width = BOUNDS_WIDTH,
};
