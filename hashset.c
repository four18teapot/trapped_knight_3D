#include <assert.h>
#include "hashset.h"
#include "constants.h"

hash FNV(const byte* pBuffer, const byte* const pBufferEnd) {
    const hash MagicPrime = 0x00000100000001b3;
    hash       Hash       = 0xcbf29ce484222325;

    for(; pBuffer < pBufferEnd; pBuffer++)
        Hash = (Hash ^ *pBuffer) * MagicPrime;
    
    return Hash;
}

hash hash_vec(Vec3D a) {
    byte* pBuffer = (byte*) &a;
    byte* pBufferEnd = pBuffer + sizeof(Vec3D);

    return FNV(pBuffer, pBufferEnd);
}

bool set_has(HashSet *set, Vec3D data) {
    assert(set->size > 0);

    size_t i = hash_vec(data) % set->size;

    if(set->buckets[i]) return LL_contains(set->buckets[i], data);
    else return false;

}

bool is_replacable(HashSet *set, Vec3D data) {
    for(int m = 0; m < KNIGHT_MOVES_LEN; m++) {
        if(!set_has(set, vec_add(data, KNIGHT_MOVES[m]))) {
            return false;
        }
    }    
    return true;
}

bool LL_push_replace(HashSet *set, LL* head, Vec3D data) {
    assert(head);

    while(head->next) {
        if(is_replacable(set, head->data)) {
            head->data = data;
            return true;
        }
        head = head->next;
    }

    head->next = LL_new(data);
    return false;
}


void set_put(HashSet *set, Vec3D data) {
    assert(set->size > 0);

    size_t i = hash_vec(data) % set->size;

    if(set->buckets[i]) LL_push(set->buckets[i], data);
    else set->buckets[i] = LL_new(data);
}
