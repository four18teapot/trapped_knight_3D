#ifndef LL_H_
#define LL_H_

#include <stdbool.h>
#include "vec3D.h"

typedef struct node_t {
    Vec3D data;
    struct node_t * next;
} LL;

LL* LL_new(Vec3D data);

void LL_push(LL *head, Vec3D data);

bool LL_contains(LL *head, Vec3D data);

#endif
