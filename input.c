#include "input.h"
#include "object.h"
#include "globals.h"
#include <raylib.h>


void read_input() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Ball o;
        o.pos = GetMousePosition();

        if (CheckCollisionPointRec(o.pos, g_bounds))
            ball_add(o);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Ball o;
        o.pos = GetMousePosition();

        if (CheckCollisionPointRec(o.pos, g_bounds))
            for (int i = 0; i < 10; ++i)
                ball_add(o);
    }

    if (IsKeyPressed(KEY_H)) {
        g_display_help = !g_display_help;
    }

    // toggle quad tree usage
    if (IsKeyPressed(KEY_SPACE)) {
        g_use_quadtree = !g_use_quadtree;
    }

    // list toggle (vs array)
    if (IsKeyPressed(KEY_L)) {
        g_nearby_items_is_lst = !g_nearby_items_is_lst;
    }

    // compare
    if (IsKeyPressed(KEY_C)) {
        // dont go into visualize mode if less than 2 balls
        if (g_item_count < 2) {
            g_visualize_compare = false;
            g_step = false;
        }
        else {
            g_visualize_compare = !g_visualize_compare;
            // Fix tree positions in case quadtree was disabled
            if (g_visualize_compare && !g_use_quadtree) {
                for (size_t i = 0; i < g_item_count; ++i) {
                    qt_relocate_item(&g_items[i]);
                }
            }
        }
    }

    // step through compare
    if (IsKeyPressed(KEY_S)) {
        if (g_visualize_compare)
            g_step = !g_step;
    }
}
