#include "linked_list.h"

/* Helper to insert a new node between two known adjacent nodes */
static void insert_between(LinkedListNode* prev_node, LinkedListNode* next_node, LinkedListNode* new_node)
{
    new_node->prev = prev_node;
    new_node->next = next_node;
    prev_node->next = new_node;
    next_node->prev = new_node;
}

bool linked_list_init(LinkedList* list)
{
    if (list == NULL) {
        return false;
    }

    list->root.next = &list->root;
    list->root.prev = &list->root;
    list->count = 0U;

    return true;
}

bool linked_list_node_init(LinkedListNode* node)
{
    if (node == NULL) {
        return false;
    }

    node->next = NULL;
    node->prev = NULL;

    return true;
}

bool linked_list_node_is_linked(const LinkedListNode* node)
{
    if (node == NULL) {
        return false;
    }

    return (node->next != NULL);
}

bool linked_list_insert_head(LinkedList* list, LinkedListNode* node)
{
    if ((list == NULL) || (node == NULL)) {
        return false;
    }

    if (linked_list_node_is_linked(node) == true) {
        return false;
    }

    linked_list_insert_head_direct(list, node);
    return true;
}

bool linked_list_insert_tail(LinkedList* list, LinkedListNode* node)
{
    if ((list == NULL) || (node == NULL)) {
        return false;
    }

    if (linked_list_node_is_linked(node) == true) {
        return false;
    }

    linked_list_insert_tail_direct(list, node);
    return true;
}

bool linked_list_insert_before(LinkedList* list, LinkedListNode* target, LinkedListNode* node)
{
    if ((list == NULL) || (target == NULL) || (node == NULL)) {
        return false;
    }

    if (linked_list_node_is_linked(node) == true) {
        return false;
    }

    if (linked_list_node_is_linked(target) == false) {
        return false;
    }

    insert_between(target->prev, target, node);
    list->count++;

    return true;
}

bool linked_list_insert_after(LinkedList* list, LinkedListNode* target, LinkedListNode* node)
{
    if ((list == NULL) || (target == NULL) || (node == NULL)) {
        return false;
    }

    if (linked_list_node_is_linked(node) == true) {
        return false;
    }

    if (linked_list_node_is_linked(target) == false) {
        return false;
    }

    insert_between(target, target->next, node);
    list->count++;

    return true;
}

bool linked_list_remove(LinkedList* list, LinkedListNode* node)
{
    if ((list == NULL) || (node == NULL)) {
        return false;
    }

    if (list->count == 0U) {
        return false;
    }

    if ((node->next == NULL) || (node->prev == NULL) || (node == &list->root)) {
        return false;
    }

    linked_list_remove_direct(list, node);
    return true;
}

LinkedListNode* linked_list_next(const LinkedList* list, const LinkedListNode* current)
{
    if ((list == NULL) || (current == NULL)) {
        return NULL;
    }

    if (current->next == &list->root) {
        return NULL;
    }

    return current->next;
}

LinkedListNode* linked_list_prev(const LinkedList* list, const LinkedListNode* current)
{
    if ((list == NULL) || (current == NULL)) {
        return NULL;
    }

    if (current->prev == &list->root) {
        return NULL;
    }

    return current->prev;
}

void linked_list_clear(LinkedList* list)
{
    if (list == NULL) {
        return;
    }

    LinkedListNode* current = list->root.next;

    while (current != &list->root) {
        LinkedListNode* next_node = current->next;
        current->next = NULL;
        current->prev = NULL;
        current = next_node;
    }

    list->root.next = &list->root;
    list->root.prev = &list->root;
    list->count = 0U;
}
