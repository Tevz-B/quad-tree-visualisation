#pragma once

typedef struct ListNode ListNode;

struct ListNode {
    ListNode* next;
    ListNode* prev;
    void* object;
};

typedef struct List {
    ListNode* begin;
    ListNode* end;
} List;

// List* list_new();
void list_init(List* list);
ListNode* list_insert(List* list, void* object);
// only remove node from list, no free
void list_remove_node(List* list, ListNode* node);
// clear and free list
void list_free_nodes(List* list);

