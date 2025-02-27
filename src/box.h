#ifndef __BOX_H__
#define __BOX_H__
#include "plane.h"

class Box : public Plane {
 public:
  Box(const Vector& c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  Box(const Vector& c, Texture* t, double ya, double pi, double ro, double tx);
  double getIntersection(const Ray& ray);
  bool getLightIntersection(const Ray& ray, double* fill);

  const AABB getAABB();
};

#endif
