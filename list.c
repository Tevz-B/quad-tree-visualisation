#include "list.h"
#include <stdlib.h>

ListNode* list_new() {
    ListNode* list = malloc(sizeof(ListNode));
    list->next = 0;
    list->prev = 0;
    list->object = 0;
    return list;
}

void list_init(List* list) {
    list->begin = 0;
    list->end = 0;
}

ListNode* insert_lst(ListNode* node, void* object) {
    node->next = malloc(sizeof(ListNode));
    node->next->next = 0;
    node->next->prev = node;
    node->next->object = object;
    return node->next;
}

ListNode* list_insert(List* list, void* object) {
    if (list->end) {
        ListNode* new_node = insert_lst(list->end, object);
        list->end = new_node;
        return new_node;
    }

    // empty list
    list->begin = list_new();
    list->end = list->begin;
    list->begin->object = object;
    return list->begin;
}

void list_remove_node(List* list, ListNode* node) {

    if (node->prev)
        node->prev->next = node->next;

    if (node->next)
        node->next->prev = node->prev;

    if (list->end == node)
        list->end = node->prev;

    if (list->begin == node)
        list->begin = node->next;

    node->prev = 0;
    node->next = 0;
    node->object = 0;

    free(node);
}

void list_free_node(ListNode* list) {
    if (list->next)
        list_free_node(list->next);

    list->next = 0;
    list->prev = 0;
    list->object = 0;

    free(list); // set ptr to 0 after
}

void list_free_nodes(List* list) {
    if (list->begin)
        list_free_node(list->begin);

    list->begin = 0;
    list->end = 0;
}

