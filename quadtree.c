#include "quadtree.h"
#include "defines.h"
#include "globals.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


QTNode* new_node() {
    QTNode* node = malloc(sizeof(QTNode));
    for (int i = 0; i < 4; ++i) {
        node->children[i] = 0;
    }
    list_init(&node->container);
    return node;
}

void print_node(QTNode* node) {
    printf("pos:%.2f,%.2f size:%.2f,%.2f\n", node->area.x, node->area.y,
           node->area.width, node->area.height);
}

void qt_init(QuadTree* qt) {
    QTNode* root = new_node();
    root->parent = 0;
    root->depth = 0;
    root->area = g_bounds;
    qt->root = root;
}

void qt_build_node(QTNode* node) {
    if (node->depth >= QUADTREE_MAX_DEPTH) return;
    Rectangle rec;
    for (int i = 0; i < 4; ++i) {
        float w = node->area.width / 2;
        float h = node->area.height / 2;
        rec.width = w;
        rec.height = h;
        rec.x = (i & 1) * w + node->area.x;
        rec.y = ((i & 2) >> 1) * h + node->area.y;


        QTNode* n = new_node();
        n->parent = node;
        n->area = rec;
        n->depth = node->depth + 1;
        node->children[i] = n;
    }

    for (int i = 0; i < 4; ++i) {
        qt_build_node(node->children[i]);
    }
}

void qt_build(QuadTree* qt) {
    qt_init(qt);
    qt_build_node(qt->root);
}

ObjectLoc insert_qt(QTNode * node, Ball* object) {
    // early exit - max depth
    if (node->depth >= QUADTREE_MAX_DEPTH) {
        ListNode* ret = list_insert(&node->container, object);
        return (ObjectLoc){.node = node, .iter = ret};
    }

    int collision_count = 0;
    int chosen_child = -1;
    for (int i = 0; i < 4; ++i) {
        QTNode* n = node->children[i];
        if (CheckCollisionCircleRec(object->pos, object->r, n->area)) {
            collision_count++;
            chosen_child = i;
        }
    }
    assert(collision_count > 0);
    assert(chosen_child != -1);

    // if mutliple collisions insert here otherwise insert deeper
    if (collision_count > 1) {
        ListNode* ret = list_insert(&node->container, object);
        return (ObjectLoc){.node = node, .iter = ret};
    } else {
        return insert_qt(node->children[chosen_child], object);
    }
}

ObjectLoc insert_qt_dynamic(QTNode* node, Ball* object) {
    // early exit - max depth
    if (node->depth >= QUADTREE_MAX_DEPTH) {
        ListNode* ret = list_insert(&node->container, object);
        return (ObjectLoc){.node = node, .iter = ret};
    }

    Rectangle rects[4];
    for (int i = 0; i < 4; ++i) {
        float w = node->area.width / 2;
        float h = node->area.height / 2;
        rects[i].width = w;
        rects[i].height = h;
        rects[i].x = (i & 1) * w + node->area.x;
        rects[i].y = ((i & 2) >> 1) * h + node->area.y;
    }

    // check for collisions
    int collision_count = 0;
    int chosen_child = -1;
    for (int i = 0; i < 4; ++i) {
        if (CheckCollisionCircleRec(object->pos, object->r, rects[i])) {
            collision_count++;
            chosen_child = i;
        }
    }
    assert(collision_count > 0);
    assert(chosen_child != -1);

    // if mutliple collisions insert here otherwise insert deeper
    if (collision_count > 1) {
        // insert in current node
        ListNode* ret = list_insert(&node->container, object);
        return (ObjectLoc){.node = node, .iter = ret};
    } else { // insert in child
        // ensure children
        if (node->children[0] == 0) {
            for (int i = 0; i < 4; ++i) {
                QTNode* n = new_node();
                n->parent = node;
                n->area = rects[i];
                n->depth = node->depth + 1;
                node->children[i] = n;
            }
        }
        return insert_qt_dynamic(node->children[chosen_child], object);
    }
}

void qt_insert_object(ObjectItem* item) {
    item->loc = insert_qt(g_qt.root, &item->obj);
}

void qt_relocate_item(ObjectItem* item) {
    list_remove_node(&item->loc.node->container, item->loc.iter);
    qt_insert_object(item);
}

void search_node_arr(QTNode* node, DArray* nearby_items, Ball* target) {
    // ignore children, children will collide with us later, only check parents
    if (node->parent)
        search_node_arr(node->parent, nearby_items, target);

    // insert items from node container to resulting array
    ListNode* it = node->container.begin;
    while(it) {
        if (it->object != target) {
            darray_push(nearby_items, (Ball*)it->object);
        }

        it = it->next;
    }
}

void qt_search_arr(ObjectItem* item, DArray* nearby_items) {
    search_node_arr(item->loc.node, nearby_items, &item->obj);
}


void search_node_lst(QTNode* node, List* nearby_items, Ball* target) {
    // insert items from node container to resulting array
    ListNode* it = node->container.begin;
    while(it) {
        if (it->object != target) {
            list_insert(nearby_items, it->object);
        }

        it = it->next;
    }

    // ignore children, children will collide with us later, only check parents
    if (node->parent)
        search_node_lst(node->parent, nearby_items, target);
}

void search_node_lst_step(QTNode* node, List* nearby_items, Ball* target, int* curr_step) {
    // insert items from node container to resulting array
    ListNode* it = node->container.begin;
    while(it) {
        if (it->object != target) {
            list_insert(nearby_items, it->object);
        }

        it = it->next;
    }

    // When stepping, stop if reached current index
    if (g_step && *curr_step == g_step_index)
        return;
    ++*curr_step;

    // ignore children, children will collide with us later, only check parents
    if (node->parent)
        search_node_lst_step(node->parent, nearby_items, target, curr_step);
}

void qt_search_lst(ObjectItem* item, List* nearby_items) {
    search_node_lst(item->loc.node, nearby_items, &item->obj);
}

void qt_search_lst_step(ObjectItem* item, List* nearby_items) {
    int curr_step = 0;
    search_node_lst_step(item->loc.node, nearby_items, &item->obj, &curr_step);
}
