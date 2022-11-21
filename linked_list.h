#pragma once

// generic list item
typedef struct ListItem {
  struct ListItem* prev;
  struct ListItem* next;
} ListItem;

// head of list
typedef struct ListHead {
  ListItem* first;
  ListItem* last;
  int size;
} ListHead;

// initializes the head
void List_init(ListHead* head);

// retrieves an element in the list
// returns NULL if item not in list
ListItem* List_find(ListHead* head, char* filename);//ListItem* item

// inserts an element after previous in the list
// returns NULL on failure
ListItem* List_insert(ListHead* head, ListItem* previous, ListItem* item);

// detaches (without deleting) an element to the
// list
ListItem* List_detach(ListHead* head, ListItem* item);



void List_print(ListHead* head);
