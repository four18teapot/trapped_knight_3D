#include "ll.h"

#include <stdlib.h>
#include <assert.h>

LL* LL_new(Vec3D data) {
  LL* new = malloc(sizeof(LL));
  new->data = data;
  new->next = NULL;
  return new;
}

void LL_push(LL *head, Vec3D data) {

  assert(head);

  while(head->next)
      head = head->next;
    
  head->next = LL_new(data);

}

bool LL_contains(LL *head, Vec3D data) {
  assert(head);

  while(head->next) {
    if(vec_eq(data, head->data)) return true;
    head = head->next;
  }

  return false;
}
