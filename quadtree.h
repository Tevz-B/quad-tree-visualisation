#pragma once

#include <raylib.h>
#include "darray.h"
#include "list.h"
#include "object.h"

/*

 c0 | c1
----+----
 c2 | c3

*/

typedef struct QTNode QTNode;

struct QTNode {
    QTNode* children[4];

    Rectangle area;
    List container;

    QTNode* parent;

    int depth;
};

typedef struct QuadTree {
    QTNode* root;
} QuadTree;

void qt_init(QuadTree* qt);
void qt_build(QuadTree* qt);
void qt_insert_object(ObjectItem* item);
void qt_relocate_item(ObjectItem* item);
void qt_search_arr(ObjectItem* item, DArray* nearby_items);
void qt_search_lst(ObjectItem* item, List* nearby_items);
void qt_search_lst_step(ObjectItem* item, List* nearby_items);
