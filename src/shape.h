#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "autonoma.h"
#include "bvh.h"

class Shape {
 public:
  Shape(const Vector& c, Texture* t, double ya, double pi, double ro);
  double yaw, pitch, roll;
  Vector center;
  Texture* texture;
  double textureX, textureY, mapX, mapY, mapOffX, mapOffY;
  Texture* normalMap;
  virtual double getIntersection(const Ray& ray) = 0;
  virtual bool getLightIntersection(const Ray& ray, double* fill) = 0;
  virtual unsigned char reversible() = 0;
  virtual void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth) = 0;
  virtual Vector getNormal(const Vector& point) = 0;
  virtual void setAngles(double yaw, double pitch, double roll);
  virtual void setYaw(double d);
  virtual void setPitch(double d);
  virtual void setRoll(double d);

  ~Shape();
  virtual const AABB getAABB() = 0;
  virtual const Vector getCentroid();
};

#endif
