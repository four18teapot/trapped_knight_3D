#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include "vec3D.h"
#include "hashset.h"
#include "constants.h"

int main() {
    HashSet set;
    set.capacity = 100000000;
    set.filled   = 0;
    set.buckets  = calloc(set.capacity, sizeof(SetEntry));

    printf("Created HashSet with %ld entries\n", set.capacity);

    size_t jumps  = 0;
    Vec3D current = {.x=0,.y=0,.z=0};

    clock_t t;
    t = clock();
    while(true)
    {
        set_put(&set, current);
        bool found      = false;
        long min_dist   = LLONG_MAX;
        Vec3D min_vec   = {.x=0,.y=0,.z=0};
        for(int m = 0; m < KNIGHT_MOVES_LEN; m++)
        {
            if(!set_has(&set, vec_add(current, KNIGHT_MOVES[m])))
            {
                if(vec_abs(vec_add(current, KNIGHT_MOVES[m])) < min_dist)
                {
                    min_dist = vec_abs(vec_add(current, KNIGHT_MOVES[m]));
                    min_vec  = vec_add(current, KNIGHT_MOVES[m]);
                    found    = true;
                }
            }
        }

        if(!found)
        {
            printf("No more knight moves at %ld jumps.\n", jumps);
            break;
        }

        current = min_vec;
        if(jumps % 1000000 == 0)
        {
            double time_taken = ((double)(clock() - t))/CLOCKS_PER_SEC;
	    t = clock();
	    printf("%ld %f (%d, %d, %d)\n", jumps, time_taken, current.x, current.y, current.z);
        }
        jumps++;
    }
}
