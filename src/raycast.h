#ifndef __RAYCAST_H__
#define __RAYCAST_H__
#include <vector>

#include "autonoma.h"
#include "bvh.h"
#include "shape.h"
#include "vector.h"

void getLight(double* tColor, Autonoma* aut, const BVH* bvh, const Vector& point,
              const Vector& norm, unsigned char flip) {
  tColor[0] = tColor[1] = tColor[2] = 0.;
  const double normMag = norm.mag();

  for (auto& t : aut->lights) {
    double lightColor[3];
    lightColor[0] = t->color[0];
    lightColor[1] = t->color[1];
    lightColor[2] = t->color[2];
    Vector ra = t->center - point;

    bool hit = bvh->getLightIntersection(Ray(point + ra * .01, ra), lightColor);
    if (hit) continue;

    double perc = (norm.dot(ra) / (ra.mag() * normMag));
    if (flip && perc < 0) perc = -perc;
    if (perc > 0) {
      tColor[0] = std::min(tColor[0] + perc * lightColor[0], 1.);
      tColor[1] = std::min(tColor[1] + perc * lightColor[0], 1.);
      tColor[2] = std::min(tColor[2] + perc * lightColor[0], 1.);
    }
  }
}

void calcColor(unsigned char* toFill, Autonoma* c, const BVH* bvh, const Ray& ray,
               unsigned int depth) {
  Shape* curShape = nullptr;
  double curTime = bvh->getIntersection(ray, curShape);

  if (curTime == inf) {
    double opacity, reflection, ambient;
    Vector temp = ray.vector.normalize();
    const double x = temp.x;
    const double z = temp.z;
    const double me = abs(temp.y);
    const double angle = atan2(z, x);
    c->skybox->getColor(toFill, &ambient, &opacity, &reflection,
                        fix(angle / M_TWO_PI), fix(me));
    return;
  }

  Vector intersect = curTime * ray.vector + ray.point;
  double opacity, reflection, ambient;
  curShape->getColor(toFill, &ambient, &opacity, &reflection, c,
                     Ray(intersect, ray.vector), depth);

  double lightData[3];
  getLight(lightData, c, bvh, intersect, curShape->getNormal(intersect),
           curShape->reversible());
  toFill[0] =
      (unsigned char)(toFill[0] * (ambient + lightData[0] * (1 - ambient)));
  toFill[1] =
      (unsigned char)(toFill[1] * (ambient + lightData[1] * (1 - ambient)));
  toFill[2] =
      (unsigned char)(toFill[2] * (ambient + lightData[2] * (1 - ambient)));
  if (depth < c->depth && (opacity < 1 - 1e-6 || reflection > 1e-6)) {
    unsigned char col[4];
    if (opacity < 1 - 1e-6) {
      const Ray nextRay = Ray(intersect + ray.vector * 1E-4, ray.vector);
      calcColor(col, c, bvh, nextRay, depth + 1);
      toFill[0] = (unsigned char)(toFill[0] * opacity + col[0] * (1 - opacity));
      toFill[1] = (unsigned char)(toFill[1] * opacity + col[1] * (1 - opacity));
      toFill[2] = (unsigned char)(toFill[2] * opacity + col[2] * (1 - opacity));
    }
    if (reflection > 1e-6) {
      Vector norm = curShape->getNormal(intersect).normalize();
      Vector vec = ray.vector - 2 * norm * (norm.dot(ray.vector));
      const Ray nextRay = Ray(intersect + vec * 1E-4, vec);
      calcColor(col, c, bvh, nextRay, depth + 1);

      toFill[0] =
          (unsigned char)(toFill[0] * (1 - reflection) + col[0] * (reflection));
      toFill[1] =
          (unsigned char)(toFill[1] * (1 - reflection) + col[1] * (reflection));
      toFill[2] =
          (unsigned char)(toFill[2] * (1 - reflection) + col[2] * (reflection));
    }
  }
}
#endif