#include "physics.h"
#include "darray.h"
#include "defines.h"
#include "globals.h"
#include "quadtree.h"
#include <math.h>

void collision_simple(ObjectItem* items, size_t objects_count, size_t currIdx) {
    Ball* a = &items[currIdx].obj;

    for (size_t i = currIdx + 1; i < objects_count; ++i) {
        Ball* b = &items[i].obj;
        ++g_collision_check_count;
        if (CheckCollisionCircles(a->pos, a->r, b->pos, b->r)) {
            a->colliding = true;
            b->colliding = true;
        }
    }
}

void collision_simple_visualize(ObjectItem* items, size_t objects_count, size_t currIdx, List* lines) {
    Ball* a = &items[currIdx].obj;

    for (size_t i = currIdx + 1; i < objects_count; ++i) {
        Ball* b = &items[i].obj;
        ++g_collision_check_count;

        CollisionCheckLine* line = malloc(sizeof(CollisionCheckLine));
        line->line = (Vector4) {a->pos.x, a->pos.y, b->pos.x, b->pos.y};
        line->colliding = false;
        list_insert(lines, line);

        if (CheckCollisionCircles(a->pos, a->r, b->pos, b->r)) {
            a->colliding = true;
            b->colliding = true;
            line->colliding = true;
        }
    }
}

void collision_qt_lst(ObjectItem* a) {
    List nearby_items;
    list_init(&nearby_items);

    qt_search_lst(a, &nearby_items);

    ListNode* it = nearby_items.begin;
    while (it) {
        ++g_collision_check_count;

        Ball* b = it->object;

        if (CheckCollisionCircles(a->obj.pos, a->obj.r, b->pos, b->r)) {
            a->obj.colliding = true;
            b->colliding = true;
        }

        it = it->next;
    }

    list_free_nodes(&nearby_items);
}

void collision_qt_visualize(ObjectItem* a, List* lines) {
    List nearby_items;
    list_init(&nearby_items);

    qt_search_lst(a, &nearby_items);

    ListNode* it = nearby_items.begin;
    while (it) {
        ++g_collision_check_count;

        Ball* b = it->object;

        CollisionCheckLine* line = malloc(sizeof(CollisionCheckLine));
        line->line = (Vector4) {a->obj.pos.x, a->obj.pos.y, b->pos.x, b->pos.y};
        line->colliding = false;
        list_insert(lines, line);

        if (CheckCollisionCircles(a->obj.pos, a->obj.r, b->pos, b->r)) {
            a->obj.colliding = true;
            b->colliding = true;
            line->colliding = true;
        }

        it = it->next;
    }

    list_free_nodes(&nearby_items);
}

void collision_qt_visualize_step(ObjectItem* a, List* lines) {
    List nearby_items;
    list_init(&nearby_items);

    qt_search_lst_step(a, &nearby_items);

    ListNode* it = nearby_items.begin;
    while (it) {
        ++g_collision_check_count;

        Ball* b = it->object;

        CollisionCheckLine* line = malloc(sizeof(CollisionCheckLine));
        line->line = (Vector4) {a->obj.pos.x, a->obj.pos.y, b->pos.x, b->pos.y};
        line->colliding = false;
        list_insert(lines, line);

        if (CheckCollisionCircles(a->obj.pos, a->obj.r, b->pos, b->r)) {
            a->obj.colliding = true;
            b->colliding = true;
            line->colliding = true;
        }

        it = it->next;
    }

    list_free_nodes(&nearby_items);
}

void collision_qt_arr(ObjectItem* a) {
    DArray nearby_items = darray_init(64);

    qt_search_arr(a, &nearby_items);

    for (size_t i = 0; i < nearby_items.size; ++i) {
        ++g_collision_check_count;

        Ball* b = darray_at(&nearby_items, i);

        if (CheckCollisionCircles(a->obj.pos, a->obj.r, b->pos, b->r)) {
            a->obj.colliding = true;
            b->colliding = true;
        }
    }
    darray_deinit(&nearby_items);
}

void reset_collision(ObjectItem* items, size_t objects_count) {
    for (size_t i = 0; i < objects_count; ++i) {
        items[i].obj.colliding = false;
    }
}

void bounce_off_bounds(Ball* o) {
    Rectangle* bounds = &g_bounds;

    if (bounds->x + bounds->width < o->pos.x + o->r) {
        o->pos.x = bounds->x + bounds->width - o->r;
        o->v.x = -fabs(o->v.x);
    } else if (bounds->x > o->pos.x - o->r) {
        o->pos.x = bounds->x + o->r;
        o->v.x = fabs(o->v.x);
    }

    if (bounds->y + bounds->height < o->pos.y + o->r) {
        o->pos.y = bounds->y + bounds->height - o->r;
        o->v.y = -fabs(o->v.y);
    } else if (bounds->y > o->pos.y - o->r) {
        o->pos.y = bounds->y + o->r;
        o->v.y = fabs(o->v.y);
    }
}

void simulate_physics() {
    reset_collision(g_items, g_item_count);
    g_collision_check_count = 0;

    for (size_t i = 0; i < g_item_count; ++i) {
        Ball* o = &g_items[i].obj;

        o->pos.x += o->v.x;
        o->pos.y += o->v.y;

        bounce_off_bounds(o);

        if (g_use_quadtree) {
            if (g_nearby_items_is_lst)
                collision_qt_lst(&g_items[i]);
            else
                collision_qt_arr(&g_items[i]);

            qt_relocate_item(&g_items[i]);
        } else {
            collision_simple(g_items, g_item_count, i);
        }
    }
}

void simulate_compare() {
    g_collision_check_count = 0;
    //  take first ball and draw lines for every comparisson for collision
    reset_collision(g_items, g_item_count);

    // handle step
    if (g_step) {
        if (g_step_timer > STEP_TIME) {
            g_step_timer = 0;
            g_step_index = (g_step_index + 1) % (QUADTREE_MAX_DEPTH + 1);
        }
        else {
            g_step_timer += GetFrameTime();
        }
    }

    // clear list before visualization
    list_free_nodes(&g_collision_check_lines);

    if (g_use_quadtree) {
        if (g_step)
            collision_qt_visualize_step(&g_items[0], &g_collision_check_lines);
        else
            collision_qt_visualize(&g_items[0], &g_collision_check_lines);
    } else {
        collision_simple_visualize(g_items, g_item_count, 0, &g_collision_check_lines);
    }
}
