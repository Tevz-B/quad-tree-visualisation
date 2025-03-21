#include "defines.h"
#include "quadtree.h"
#include "globals.h"
#include "renderer.h"

const double UPDATE_INTERVAL = 1.0;

StatsInfo stats;
double prev_time = 0.0;

void update_stats(StatsInfo* stats) {
    // Immediate updates
    stats->use_quad_tree = g_use_quadtree;
    stats->object_count = g_item_count;
    stats->result_is_list = g_nearby_items_is_lst;

    // Interval updates
    double current_time = GetTime();
    if (current_time - prev_time > UPDATE_INTERVAL) {
        prev_time = current_time;
        stats->collision_check_count = g_collision_check_count;
        stats->frame_time = GetFrameTime() * 1000;
    }
}

void draw_qt_node(QTNode* node) {
    DrawRectangleLinesEx(node->area, 0.5, GRAY);
    if (node->children[0])
        for (int i = 0; i < 4; ++i)
            draw_qt_node(node->children[i]);
}

void draw_qt() { draw_qt_node(g_qt.root); }

typedef enum QtDrawMode {
    QDM_DEFAULT = 0,  // Draw only target node + its parents
    QDM_SIBLINGS = 1, // Draw target node siblings as well
    QDM_OTHER = 2,    // Draw all other tree nodes
} QtDrawMode;

void draw_qt_parent_nodes(QTNode* node, QtDrawMode mode, int* curr_step) {
    DrawRectangleLinesEx(node->area, 1.0, SKYBLUE);

    if ((mode & QDM_SIBLINGS) && node->children[0]) {
        for (int i = 0; i < 4; ++i)
            DrawRectangleLinesEx(node->children[i]->area, 1.0, SKYBLUE);
    }

    // When stepping, stop if reached current index
    if (g_step && *curr_step == g_step_index)
        return;
    ++*curr_step;

    if (node->parent)
        draw_qt_parent_nodes(node->parent, mode, curr_step);
}

void draw_qt_visual_demo(ObjectItem* item, QtDrawMode mode) {
    if (mode & QDM_OTHER)
        draw_qt();

    int curr_step = 0;
    draw_qt_parent_nodes(item->loc.node, mode, &curr_step);
}

void render() {
    update_stats(&stats);
    BeginDrawing();

    ClearBackground((Color){ 40, 40, 40, 255 });

    // objects
    for (size_t i = 0; i < g_item_count; ++i) {
        Ball* o = &g_items[i].obj;
        DrawCircle(o->pos.x, o->pos.y, o->r, o->colliding ? RED : LIGHTGRAY);
    }

    // bounds
    DrawRectangleLinesEx(g_bounds, 2.0, RAYWHITE);

    // quad tree
    if (stats.use_quad_tree) {
        if (g_visualize_compare) {
            draw_qt_visual_demo(&g_items[0], QDM_OTHER | QDM_SIBLINGS ); // ensured that at least 2 items present
        }
        else {
            draw_qt();
        }
    }

    if (g_visualize_compare) {
        // collision check lines
        ListNode* it = g_collision_check_lines.begin;
        while(it) {
            CollisionCheckLine* l = it->object;
            DrawLineEx((Vector2){l->line.x, l->line.y}, (Vector2){l->line.z, l->line.w}, 1.5, PINK);
            it = it->next;
        }
    }

    const int font_size = 20;

    // text
    // DrawText(TextFormat("FrameTime: %.2fms, \n"
    //                     "Objects: %zu, \n"
    //                     "Collision Checks: %zu, ",
    //                     stats.frame_time,
    //                     stats.object_count,
    //                     stats.collision_check_count),
    //          120, 10 + font_size, font_size, LIGHTGRAY);
    DrawText(TextFormat("Objects: %zu, \n"
                        "Collision Checks: %zu, \n"
                        "using QuadTree ? %s, \n"
                        "QuadTree search results in %s",
                        stats.object_count,
                        stats.collision_check_count,
                        stats.use_quad_tree ? "TRUE" : "FALSE",
                        stats.result_is_list ? "LIST" : "ARRAY"),
             200, 10, font_size, LIGHTGRAY);

    // DrawText(TextFormat("using QuadTree ? %s, "
    //                     "QuadTree search results in %s",
    //                     stats.use_quad_tree ? "true" : "false",
    //                     stats.result_is_list ? "list" : "array"),
    //          120, 10 + font_size * 2, font_size, LIGHTGRAY);

    DrawFPS(10, 10);
    DrawText(TextFormat("\n%.2fms\nH - Help", stats.frame_time), 10, 10, font_size, DARKGREEN);

    if (g_display_help) {
        const int padding = 48;
        const int text_padding = 8;
        Rectangle help_background = (Rectangle){
            .x = padding,
            .y = padding,
            .width = 640,
            .height = 320,
            // .width = SCREEN_WIDTH - (2 * padding),
            // .height = SCREEN_HEIGHT - (2 * padding),
        };
        DrawRectangleRec(help_background, DARKGRAY);

        DrawText("Controls:\n"
                 "  Left Mouse Button: Spawn single ball\n"
                 "  Right Mouse Button: Spawn mutiple balls\n"
                 "  H: Open [H]elp\n"
                 "  Space: Toggle quad tree usage\n"
                 "  L: Toggle [L]ist vs array for quad tree container\n"
                 "  C: Visualize [C]ompare\n"
                 "  S: [S]tep (while comparing)\n"
                 , text_padding + padding,
                 text_padding + padding, font_size, LIGHTGRAY);
    }

    EndDrawing();
}

