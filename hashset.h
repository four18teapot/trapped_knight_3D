#ifndef HASHSET_H_
#define HASHSET_H_

#include <stddef.h>
#include "vec3D.h"

#define SET_FLAG_FREE 0
#define SET_FLAG_OCCUPIED 1
#define SET_FLAG_DELETE 2

typedef size_t hash;
typedef char byte;

typedef struct {
  byte flag;
  Vec3D entry;
} SetEntry;

typedef struct {
  size_t capacity;
  size_t filled;
  SetEntry *buckets;
} HashSet;

hash FNV(const byte* pBuffer, const byte* const pBufferEnd);

hash hash_vec(Vec3D key);

bool set_has(HashSet *set, Vec3D key);

void set_put(HashSet *set, Vec3D key);

size_t probe(HashSet *set, Vec3D key);

#endif
