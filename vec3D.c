#include "vec3D.h"
#include <stdbool.h>

Vec3D vec_add(Vec3D a, Vec3D b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}

bool vec_eq(Vec3D a, Vec3D b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

long vec_abs(Vec3D a)
{
  return (a.x * a.x + a.y * a.y + a.z * a.z);
}
