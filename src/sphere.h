#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "shape.h"

class Sphere : public Shape {
 public:
  double radius;
  Sphere(const Vector& c, Texture* t, double ya, double pi, double ro, double radius);
  double getIntersection(const Ray& ray);
  bool getLightIntersection(const Ray& ray, double* fill);
  void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth);
  Vector getNormal(const Vector& point);
  unsigned char reversible();

  const AABB getAABB();
};
#endif
