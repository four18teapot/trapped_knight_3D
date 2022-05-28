#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include "vec3D.h"
#include "ll.h"
#include "hashset.h"
#include "constants.h"

#define FILENAME "yarn2.dat"
#define REPORT_SIZE 1000000

void ll_print(LL* head) {
  while(head) {
    Vec3D d = head->data;
    printf("(%d,%d,%d)->", d.x, d.y, d.z);
    head = head->next;
  }
}

void hashset_print(HashSet* set) {
  for(size_t i = 0; i < set->size; i++) {
    printf("%ld:\t", i);
    ll_print(set->buckets[i]);
    printf("\n");
  }
}


int main(void) {

  /*
  HashSet set;
  set.size = 10;
  set.buckets = (LL**) calloc(set.size, sizeof(LL*));

  Vec3D zero = {.x=0,.y=0,.z=0};

  set_put(&set, zero);
  if(set_has(&set, zero))
     printf("Set has 0,0,0");

  hashset_print(&set);
  */

  
  FILE *out;
  
  out = fopen(FILENAME, "w+");
  
  HashSet set;
  set.size    = 100000000;
  set.buckets = calloc(set.size, sizeof(LL*));

  size_t jumps = 0;
  Vec3D current = {.x=0,.y=0,.z=0};

  clock_t t;
  t = clock();
  while(true) {
    set_put(&set, current);
    bool found = false;
    long min_dist = LLONG_MAX;
    Vec3D min_vec = {.x=0,.y=0,.z=0};
    int availible = 0;
    
    for(int m = 0; m < KNIGHT_MOVES_LEN; m++) {
      Vec3D candidate = vec_add(current, KNIGHT_MOVES[m]);
      if(!set_has(&set, candidate)) {
        availible++;
	long candidate_abs = vec_abs(candidate);
        if(candidate_abs < min_dist) {
          min_dist = candidate_abs;
          min_vec = candidate;
	  found = true;
        }
      }
    }

    fprintf(out, "%d,%d,%d,%d,%ld\n", current.x, current.y, current.z, availible, vec_abs(current));

    if(!found) {
        printf("No more knight moves at %ld jumps.\n", jumps);
        break;
    }

    current = min_vec;
    jumps++;

    if(jumps % REPORT_SIZE == 0) {
        double time_taken = ((double)(clock() - t))/CLOCKS_PER_SEC;
        printf("%ld\t mil.\t %10.2f sec.\t (%d, %d, %d)\n", jumps / REPORT_SIZE, time_taken, current.x, current.y, current.z);
    }
  } 

  fclose(out);
  
}

