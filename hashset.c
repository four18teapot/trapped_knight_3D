#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashset.h"
#include "vec3D.h"
#include "constants.h"

hash FNV(const byte* pBuffer, const byte* const pBufferEnd) {
  const hash MagicPrime = 0x00000100000001b3;
  hash       Hash       = 0xcbf29ce484222325;

  for(; pBuffer < pBufferEnd; pBuffer++)
      Hash = (Hash ^ *pBuffer) * MagicPrime;

  return Hash;
}

hash sum(Vec3D key) {
  return key.x + key.y + key.z;
}

hash hash_vec(Vec3D key) {
  byte* pBuffer = (byte*) &key;
  byte* pBufferEnd = pBuffer + sizeof(Vec3D);

  return FNV(pBuffer, pBufferEnd);
}

// Linear probing
size_t probe(HashSet *set, Vec3D key) {
  size_t index = hash_vec(key) % set->capacity;
  while(set->buckets[index].flag == SET_FLAG_OCCUPIED && !vec_eq(set->buckets[index].entry, key))
    index++;

  return index;
}

bool set_has(HashSet *set, Vec3D key) {
  size_t index = probe(set, key);
  return (set->buckets[index].flag == SET_FLAG_OCCUPIED) && vec_eq(set->buckets[index].entry, key);
}

bool is_surrounded(HashSet *set, Vec3D key) {
  for(size_t m = 0; m < KNIGHT_MOVES_LEN; m++)
    if(!set_has(set, vec_add(key, KNIGHT_MOVES[m])))
      return false;
  return true;
}

bool is_enclosed(HashSet *set, Vec3D key) {
  for(size_t m = 0; m < KNIGHT_MOVES_LEN; m++)
    if(!is_surrounded(set, KNIGHT_MOVES[m]))
      return false;
  return true;
}

size_t mark_all_enclosed(HashSet *set) {
  size_t marked = 0;
  for(size_t i = 0; i < set->capacity; i++) {
    if(set->buckets[i].flag == SET_FLAG_OCCUPIED) {
      if(is_enclosed(set, set->buckets[i].entry)) {
	set->buckets[i].flag = SET_FLAG_DELETE;
	set->filled--;
	marked++;
      }
    }
  }
  return marked;
}

size_t realloc_to(HashSet *set, size_t index) {
  assert(index < set->capacity);
  assert(set->buckets[index].flag == SET_FLAG_DELETE);

  size_t pivot = index;
  while(true) {
    switch(set->buckets[(pivot + 1) % set->capacity].flag) {
    case SET_FLAG_FREE:
      set->buckets[index] = set->buckets[pivot];
      set->buckets[pivot].flag = SET_FLAG_FREE;
      return pivot;
      break;
    case SET_FLAG_DELETE:
      realloc_to(set, (pivot + 1) % set->capacity);
      break;
    case SET_FLAG_OCCUPIED:
      break;
    default:
      printf("Unreachable code! Sorry you broke the Universe. Exiting.\n");
      exit(0);
      break;
    }
    pivot = (pivot + 1) % set->capacity;
  }
}

void sweep_all_marked_delete(HashSet *set) {
  for(size_t i = 0; i < set->capacity; i++) {
    if(set->buckets[i].flag == SET_FLAG_DELETE) {
      realloc_to(set, i);
    }
  }
}

void set_put(HashSet *set, Vec3D key) {
  size_t index = probe(set, key);
  set->buckets[index].entry = key;
  set->buckets[index].flag = SET_FLAG_OCCUPIED;
  set->filled++;

  /*
  if(set->filled > set->capacity / 2) {
    printf("50 percent of set occupied, marking entries for deletion...\n");
    printf("%ld elements marked.\n", mark_all_enclosed(set));
    printf("Starting deletion process...\n");
    sweep_all_marked_delete(set);
    printf("Elements sucessfully deleted.\n");
  }
  */
}
