#include <assert.h>
#include <stdlib.h>

#include "defines.h"
#include "globals.h"
#include "object.h"
#include "quadtree.h"

void ball_add(Ball object) {
    assert(g_item_count + 1 < MAX_BALLS);

    object.v.x = (drand48() - 0.5) * 30;
    object.v.y = (drand48() - 0.5) * 30;

    object.r = 4.0;

    object.colliding = false;

    g_items[g_item_count].obj = object;
    qt_insert_object(&g_items[g_item_count]);
    ++g_item_count;
}
