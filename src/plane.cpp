#include "plane.h"

Plane::Plane(const Vector& c, Texture* t, double ya, double pi, double ro, double tx, double ty) : Shape(c, t, ya, pi, ro), normal(c), right(c), up(c) {
  textureX = tx;
  textureY = ty;
  setAngles(yaw, pitch, roll);
  normalMap = NULL;
  mapX = textureX;
  mapY = textureY;
}

void Plane::setAngles(const double a, const double b, const double c) {
  yaw = a;
  pitch = b;
  roll = c;
  xcos = cos(yaw);
  xsin = sin(yaw);
  ycos = cos(pitch);
  ysin = sin(pitch);
  zcos = cos(roll);
  zsin = sin(roll);
  normal.x = xsin * ycos * zcos + ysin * zsin;
  normal.y = ysin * zcos - xsin * ycos * zsin;
  normal.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  right.z = -xsin;
  d = -normal.dot(center);
}

void Plane::setYaw(const double a) {
  yaw = a;
  xcos = cos(yaw);
  xsin = sin(yaw);

  normal.x = xsin * ycos * zcos + ysin * zsin;
  normal.y = ysin * zcos - xsin * ycos * zsin;
  normal.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  right.z = -xsin;
  d = -normal.dot(center);
}

void Plane::setPitch(const double b) {
  pitch = b;
  ycos = cos(pitch);
  ysin = sin(pitch);
  normal.x = xsin * ycos * zcos + ysin * zsin;
  normal.y = ysin * zcos - xsin * ycos * zsin;
  normal.z = xcos * ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  d = -normal.dot(center);
}

void Plane::setRoll(const double c) {
  roll = c;
  zcos = cos(roll);
  zsin = sin(roll);
  normal.x = xsin * ycos * zcos + ysin * zsin;
  normal.y = ysin * zcos - xsin * ycos * zsin;
  //   vect.z = xcos*ycos;
  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  // up.z = -xcos*ysin;
  right.x = xcos * zcos;
  right.y = -xcos * zsin;
  // right.z = -xsin;
  d = -normal.dot(center);
}

double Plane::getIntersection(const Ray& ray) {
  const double t = ray.vector.dot(normal);
  const double norm = normal.dot(ray.point) + d;
  const double r = -norm / t;
  return (r > 0) ? r : inf;
}

bool Plane::getLightIntersection(const Ray& ray, double* fill) {
  const double t = ray.vector.dot(normal);
  const double norm = normal.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.) return false;

  if (texture->opacity > 1 - 1E-6) return true;
  Vector dist = solveScalers(right, up, normal, ray.point - center);
  unsigned char temp[4];
  double amb, op, ref;
  texture->getColor(temp, &amb, &op, &ref, fix(dist.x / textureX - .5), fix(dist.y / textureY - .5));
  if (op > 1 - 1E-6) return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

void Plane::getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth) {
  Vector dist = solveScalers(right, up, normal, ray.point - center);
  texture->getColor(toFill, am, op, ref, fix(dist.x / textureX - .5), fix(dist.y / textureY - .5));
}
unsigned char Plane::reversible() {
  return 1;
}

Vector Plane::getNormal(const Vector& point) {
  if (normalMap == NULL)
    return normal;
  else {
    Vector dist = solveScalers(right, up, normal, point - center);
    double am, ref, op;
    unsigned char norm[3];
    normalMap->getColor(norm, &am, &op, &ref, fix(dist.x / mapX - .5 + mapOffX), fix(dist.y / mapY - .5 + mapOffY));
    Vector ret = ((norm[0] - 128) * right + (norm[1] - 128) * up + norm[2] * normal).normalize();
    return ret;
  }
}

const AABB Plane::getAABB() {
  AABB aabb;
  Vector p = -d * normal;
  // use 0.001 for thickness
  aabb.expand(p + right * INFINITY + up * INFINITY + normal * 0.001);
  aabb.expand(p + right * -INFINITY + up * -INFINITY + normal * -0.001);
  return aabb;
}
