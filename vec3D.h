#ifndef Vec3D_H_
#define Vec3D_H_

#include <stdbool.h>

typedef struct {
  short x, y, z;
} Vec3D;

Vec3D vec_add(Vec3D a, Vec3D b);

bool vec_eq(Vec3D a, Vec3D b);

long vec_abs(Vec3D a);

#endif
