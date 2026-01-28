/* SLL.cpp */

#include "SLL.h"

SinglyLinkedListMenu* sll_create()
{
    /* Allocate memory */
    SinglyLinkedListMenu* list = (SinglyLinkedListMenu*)malloc(sizeof(SinglyLinkedListMenu));
    if (list == NULL) return NULL;

    /* Init vals*/
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    /* Return list */
    return list;
}

size_t sll_size(SinglyLinkedListMenu* list)
{
    return list->size;
}

bool sll_is_empty(SinglyLinkedListMenu* list)
{
    return (list->head == NULL);
}

void sll_push_back(SinglyLinkedListMenu* list, MenuNode menu)
{

    MenuNode* new_node = (MenuNode*)malloc(sizeof(MenuNode));
    if (new_node == NULL) return;

    strncpy(new_node->text, menu.text, sizeof(new_node->text) - 1);
    new_node->text[sizeof(new_node->text) - 1] = '\0';
    new_node->next = NULL;

    if (sll_is_empty(list))
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
}

MenuNode sll_pop_front(SinglyLinkedListMenu* list)
{
    MenuNode empty = {0};
    if (sll_is_empty(list)) return empty;

    MenuNode removed = {0};  
    strncpy(removed.text, list->head->text, sizeof(removed.text) - 1);  
    removed.text[sizeof(removed.text) - 1] = '\0';  
    removed.next = NULL;

    MenuNode* old_head = list->head;

    list->head = old_head->next;
    free(old_head);

    list->size--;

    if (sll_is_empty(list)) list->tail = NULL;

    return removed;
}

void sll_clear(SinglyLinkedListMenu* list)
{
    MenuNode* to_clear = list->head;
    MenuNode* next_to_clear = NULL;

    while (to_clear != NULL)
    {
        next_to_clear = to_clear->next;
        free(to_clear);
        to_clear = next_to_clear;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

MenuNode* sll_get_at(SinglyLinkedListMenu* list, size_t index)
{
    if (list == NULL || index >= list->size) return NULL;

    MenuNode* current = list->head;
    for (size_t i = 0; i < index; i++) 
    {
        current = current->next;
    }

    return current;
}