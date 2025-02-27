#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "plane.h"

class Triangle : public Plane {
 public:
  double thirdX;
  Triangle(const Vector& c, const Vector& b, const Vector& a, Texture* t);
  double getIntersection(const Ray& ray);
  bool getLightIntersection(const Ray& ray, double* fill);
  Vector centroid = Vector(0, 0, 0);

  const AABB getAABB();
  const Vector getCentroid();
  const bool useBVH() { return true; }
};

#endif
