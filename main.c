#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#include "vec3D.h"
#include "hashset.h"
#include "constants.h"

#define TEST_ITERATIONS    5
#define TEST_SAMPLE_POINTS 100
#define TEST_HASH_SET_SIZE 1000000
#define TEST_JUMP_AMOUNT   1000000
#define TEST_SAMPLE_FREQUENCY TEST_JUMP_AMOUNT/TEST_SAMPLE_POINTS
#define TEST_FILE_NAME "linear_sum.test"
#define TEST_NAME "Open Adressing; Quadratic Probing; Sum Hash"

double time_table_tests[TEST_ITERATIONS][TEST_SAMPLE_POINTS];

void test_single(size_t i_test) {
  size_t i_sample = 0;
  
  HashSet set;
  set.capacity = TEST_HASH_SET_SIZE;
  set.filled = 0;
  set.buckets = (SetEntry*)calloc(set.capacity, sizeof(SetEntry));
  
  size_t jumps = 0;
  Vec3D current = {.x=0,.y=0,.z=0};

  clock_t t;
  t = clock();
  while(jumps < TEST_JUMP_AMOUNT) {
    set_put(&set, current);
    bool found = false;
    long min_dist = LLONG_MAX;
    Vec3D min_vec;

    for(int m = 0; m < KNIGHT_MOVES_LEN; m++) {
      if(!set_has(&set, vec_add(current, KNIGHT_MOVES[m]))) {
	if(vec_abs(vec_add(current, KNIGHT_MOVES[m])) < min_dist) {
	  min_dist = vec_abs(vec_add(current, KNIGHT_MOVES[m]));
	  min_vec = vec_add(current, KNIGHT_MOVES[m]);
	  found = true;
	}
      }
    }

    if(!found) {
      printf("While testing: Knight tour solved. (shouldn't happen).");
    }

    current = min_vec;

    if((jumps % (TEST_JUMP_AMOUNT / TEST_SAMPLE_POINTS)) == 0) {
      double time_taken = ((double)(clock() - t))/CLOCKS_PER_SEC;
      time_table_tests[i_test][i_sample] = time_taken;
      i_sample += 1;
    }

    jumps++;
  }
  printf("Trying to free set.buckets: %p\n", (void *) set.buckets);
}

void test(void) {
  for(size_t i = 0; i < TEST_ITERATIONS; i++) {
    printf("Test %ld/%d\n", i + 1, TEST_ITERATIONS);
    test_single(i);
  }

  double average_times[TEST_SAMPLE_POINTS] = {0};
  for(size_t p = 0; p < TEST_SAMPLE_POINTS; p++) {
    for(size_t i = 0; i < TEST_ITERATIONS; i++) {
      average_times[p] += time_table_tests[i][p];
    }
    average_times[p] /= TEST_ITERATIONS;
  }

  FILE *testlog;
  char filename[2048] = "test_results/";
  testlog = fopen(strcat(filename, TEST_FILE_NAME), "w+");

  fprintf(testlog, "TEST_NAME: %s\n", TEST_NAME);
  fprintf(testlog, "TEST_ITERATIONS: %d\n", TEST_ITERATIONS);
  fprintf(testlog, "TEST_JUMPS: %d\n", TEST_JUMP_AMOUNT);
  fprintf(testlog, "TEST_HASHSET_SIZE: %d\n\n", TEST_HASH_SET_SIZE);
  
  for(size_t p = 0; p < TEST_SAMPLE_POINTS; p++) {
    fprintf(testlog, "%ld %f\n", p * TEST_SAMPLE_FREQUENCY, average_times[p]);
  }

  fclose(testlog);
}

int main(void) {
  test();
  /*
  HashSet set;
  set.capacity = TEST_HASH_SET_SIZE;
  set.filled   = 0;
  set.buckets  = (SetEntry*) calloc(set.capacity, sizeof(SetEntry));

  printf("Created HashSet with %ld entries\n", set.capacity);

  size_t jumps  = 0;
  Vec3D current = {.x=0,.y=0,.z=0};

  clock_t t;
  t = clock();
  while(true) {
    set_put(&set, current);
    bool found = false;
    long min_dist = LLONG_MAX;
    Vec3D min_vec;
    for(int m = 0; m < KNIGHT_MOVES_LEN; m++) {
      if(!set_has(&set, vec_add(current, KNIGHT_MOVES[m]))) {
        if(vec_abs(vec_add(current, KNIGHT_MOVES[m])) < min_dist) {
          min_dist = vec_abs(vec_add(current, KNIGHT_MOVES[m]));
          min_vec = vec_add(current, KNIGHT_MOVES[m]);
          found = true;
        }
      }
    }

    if(!found) {
      printf("No more knight moves at %ld jumps.\n", jumps);
      break;
    }

    current = min_vec;
    if(jumps % 1000000 == 0) {
      double time_taken = ((double)(clock() - t))/CLOCKS_PER_SEC;
      t = clock();
      printf("%ld %f (%d, %d, %d)\n", jumps, time_taken, current.x, current.y, current.z);
    }

    jumps++;
  }
  */
}
