#include "linked_list.h"
#include "fat.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void List_init(ListHead* head) {
  head->first=0;
  head->last=0;
  head->size=0;
}

ListItem* List_find(ListHead* head, char* filename){
  // linear scanning of list
  ListItem* aux=head->first;
  while(aux){
    if (!strcmp(((FileHandle*)aux)->ffb->fcb.name,filename)) //if (aux==item)
      return aux; //return item;
    aux=aux->next;
  }
  return 0; //return NULL;
}

ListItem* List_insert(ListHead* head, ListItem* prev, ListItem* item) {
  if (item->next || item->prev){
    printf("qua problema\n"); //return NULL;
    return 0;
  }
  
#ifdef _LIST_DEBUG_
  // we check that the element is not in the list
  ListItem* instance=List_find(head, ((FileHandle*)item)->ffb->fcb.name);//item
  assert(!instance);

  // we check that the previous is in the list

  if (prev) {
    ListItem* prev_instance=List_find(head, ((FileHandle*)prev)->ffb->fcb.name);//prev
    assert(prev_instance);
  }
  // we check that the previous is in the list
#endif

  ListItem* next= prev ? prev->next : head->first;
  if (prev) {
    item->prev=prev;
    prev->next=item;
  }
  if (next) {
    item->next=next;
    next->prev=item;
  }

  if (!prev)
    head->first=item;
  
  if(!next)
    head->last=item;
 
  ++head->size;

  return item;
}

ListItem* List_detach(ListHead* head, ListItem* item) {

#ifdef _LIST_DEBUG_
  // we check that the element is in the list
  ListItem* instance=List_find(head,((FileHandle*)item)->ffb->fcb.name);//item
  assert(instance);
#endif

  ListItem* prev=item->prev;
  ListItem* next=item->next;
  if (prev){
    prev->next=next;
  }
  if(next){
    next->prev=prev;
  }
  if (item==head->first)
    head->first=next;
  if (item==head->last)
    head->last=prev;
  head->size--;
  item->next=item->prev=0;//NULL
  return item;
}

void List_print(ListHead* head) {
  ListItem* aux=head->first;
  while(aux) {
    printf("Nome file: %s\n",((FileHandle*)aux)->ffb->fcb.name); //printf("%d ", element->info);
    aux=aux->next;
  }
}

