/* SLL.h */

#ifndef SLL_H
#define SLL_H

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>

typedef struct MenuNode{
  char text[17];
  struct MenuNode* next;
} MenuNode;

typedef struct{
  MenuNode* head;
  MenuNode* tail;
  size_t size;
} SinglyLinkedListMenu;

SinglyLinkedListMenu* sll_create();
size_t sll_size(SinglyLinkedListMenu* list);
bool sll_is_empty(SinglyLinkedListMenu* list);
void sll_push_back(SinglyLinkedListMenu* list, MenuNode menu);
MenuNode sll_pop_front(SinglyLinkedListMenu* list);
void sll_clear(SinglyLinkedListMenu* list);
MenuNode* sll_get_at(SinglyLinkedListMenu* list, size_t index);

#endif // SLL_H