#ifndef __PLANE_H__
#define __PLANE_H__

#include "shape.h"

class Plane : public Shape {
 public:
  Vector normal, right, up;
  double d, xsin, xcos, ysin, ycos, zsin, zcos;
  Plane(const Vector& c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  double getIntersection(const Ray& ray);
  bool getLightIntersection(const Ray& ray, double* toFill);
  void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth);
  Vector getNormal(const Vector& point);
  unsigned char reversible();
  void setAngles(const double yaw, const double pitch, const double roll);
  void setYaw(const double d);
  void setPitch(const double d);
  void setRoll(const double d);
};

#endif
