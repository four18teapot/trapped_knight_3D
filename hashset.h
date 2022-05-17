#ifndef HASHSET_H_
#define HASHSET_H_

#include <stdbool.h>
#include <stddef.h>
#include "ll.h"
#include "vec3D.h"

typedef struct {
    size_t size;
    LL** buckets;
} HashSet;

typedef size_t hash;
typedef char byte;

hash FNV(const byte* pBuffer, const byte* const pBufferEnd);

hash hash_vec(Vec3D a);

bool is_replacable(HashSet *set, Vec3D data);

bool set_has(HashSet *set, Vec3D data);

void set_put(HashSet *set, Vec3D data);

bool LL_push_replace(HashSet *set, LL *head, Vec3D data);

#endif
