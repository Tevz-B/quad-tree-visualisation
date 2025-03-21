#pragma once

#include <raylib.h>
#include <stddef.h>
#include <stdbool.h>
#include "list.h"

typedef struct QTNode QTNode;

typedef struct Ball {
    Vector2 pos;
    Vector2 v;
    float r;
    bool colliding;
} Ball;

typedef struct ObjectLoc {
    QTNode* node;
    ListNode* iter;
} ObjectLoc;

typedef struct ObjectItem {
    Ball obj;
    ObjectLoc loc;
} ObjectItem;

typedef struct CollisionCheckLine {
    Vector4 line;
    bool colliding;
} CollisionCheckLine;

void ball_add(Ball object);
