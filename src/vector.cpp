#include "vector.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>

Vector::Vector(double a, double b, double c) : x(a), y(b), z(c) {}

// Vector Operators
void Vector::operator-=(const Vector rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
}
void Vector::operator+=(const Vector rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
}
void Vector::operator*=(const double rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator*=(const float rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator*=(const int rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
}
void Vector::operator/=(const double rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}
void Vector::operator/=(const float rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}
void Vector::operator/=(const int rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
}

// Vector operations
__attribute__((access(read_only, 1), access(read_only, 2), access(read_only, 3), access(read_only, 4)))
Vector
solveScalers(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& C) {
  const Vector v1_v2 = v1.cross(v2);
  const Vector v2_v3 = v2.cross(v3);
  const Vector v3_v1 = v3.cross(v1);
  const double denom = v1.dot(v2_v3);

  const double a = C.dot(v2_v3) / denom;
  const double b = C.dot(v3_v1) / denom;
  const double c = C.dot(v1_v2) / denom;
  return Vector(a, b, c);
}

Ray::Ray(const Vector& po, const Vector& ve) : point(po), vector(ve) {}
