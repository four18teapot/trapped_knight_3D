#include <assert.h>
#include <stdio.h>
#include "hashset.h"
#include "vec3D.h"

hash FNV(const byte* pBuffer, const byte* const pBufferEnd)
{
    const hash MagicPrime = 0x00000100000001b3;
    hash       Hash       = 0xcbf29ce484222325;

    for(; pBuffer < pBufferEnd; pBuffer++)
        Hash = (Hash ^ *pBuffer) * MagicPrime;
    
    return Hash;
}

hash hash_vec(Vec3D key)
{
    byte* pBuffer = (byte*) &key;
    byte* pBufferEnd = pBuffer + sizeof(Vec3D);

    return FNV(pBuffer, pBufferEnd);
}

// Linear probing
size_t probe(HashSet *set, Vec3D key)
{
  size_t index = hash_vec(key) % set->capacity;
  while(set->buckets[index].flag == SET_FLAG_OCCUPIED && !vec_eq(set->buckets[index].entry, key))
    index++;

  return index;
}  

bool set_has(HashSet *set, Vec3D key)
{
  size_t index = probe(set, key);
  return (set->buckets[index].flag == SET_FLAG_OCCUPIED) && vec_eq(set->buckets[index].entry, key);
}

void set_put(HashSet *set, Vec3D key)
{ 
  size_t index = probe(set, key);
  set->buckets[index].entry = key;
  set->buckets[index].flag = SET_FLAG_OCCUPIED;
  set->filled++;
}
