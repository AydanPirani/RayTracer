#ifndef __VECTOR_H_INCLUDED__
#define __VECTOR_H_INCLUDED__
#define _USE_MATH_DEFINES
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <limits>
#define inf std::numeric_limits<double>::infinity()

class Vector {
 public:
  double x, y, z;
  Vector(double a, double b, double c);

  void operator+=(const Vector);
  void operator-=(const Vector);
  void operator*=(const double);
  void operator*=(const float);
  void operator*=(const int);
  void operator/=(const double);
  void operator/=(const float);
  void operator/=(const int);

  inline Vector operator-(const Vector rhs) const {
    return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  inline Vector operator+(const Vector rhs) const {
    return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  inline Vector operator*(const Vector a) const {
    return Vector(x * a.x, y * a.y, z * a.z);
  }
  inline Vector operator*(const double rhs) const {
    return Vector(x * rhs, y * rhs, z * rhs);
  }
  inline Vector operator*(const float rhs) const {
    return Vector(x * rhs, y * rhs, z * rhs);
  }
  inline Vector operator*(const int rhs) const {
    return Vector(x * rhs, y * rhs, z * rhs);
  }
  inline Vector operator/(const double rhs) const {
    return Vector(x / rhs, y / rhs, z / rhs);
  }
  inline Vector operator/(const float rhs) const {
    return Vector(x / rhs, y / rhs, z / rhs);
  }
  inline Vector operator/(const int rhs) const {
    return Vector(x / rhs, y / rhs, z / rhs);
  }
  inline bool operator==(const Vector &other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  inline Vector cross(const Vector &a) const {
    return Vector(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
  }
  double mag2() const { return x * x + y * y + z * z; }
  double mag() const { return sqrt(x * x + y * y + z * z); }
  inline double dot(const Vector &a) const { return x * a.x + y * a.y + z * a.z; }
  inline Vector normalize() const {
    double m = mag();
    return Vector(x / m, y / m, z / m);
  }

  friend std::ostream &operator<<(std::ostream &out, const Vector &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }
};

inline Vector operator-(const Vector b) {
  return Vector(-b.x, -b.y, -b.z);
}
inline Vector operator+(const Vector b) {
  return b;
}
inline Vector operator*(const int a, const Vector b) {
  return Vector(a * b.x, a * b.y, a * b.z);
}
inline Vector operator*(const double a, const Vector b) {
  return Vector(a * b.x, a * b.y, a * b.z);
}
inline Vector operator*(const float a, const Vector b) {
  return Vector(a * b.x, a * b.y, a * b.z);
}
inline Vector operator/(const int a, const Vector b) {
  return Vector(a / b.x, a / b.y, a / b.z);
}
inline Vector operator/(const double a, const Vector b) {
  return Vector(a / b.x, a / b.y, a / b.z);
}
inline Vector operator/(const float a, const Vector b) {
  return Vector(a / b.x, a / b.y, a / b.z);
}

Vector solveScalers(const Vector &v1, const Vector &v2, const Vector &v3, const Vector &C);

class Ray {
 public:
  Vector point, vector;
  Ray(const Vector &po, const Vector &ve);
};

#endif
