#include "triangle.h"

Triangle::Triangle(const Vector& c, const Vector& b, const Vector& a, Texture* t) : Plane(Vector(0, 0, 0), t, 0., 0., 0., 0., 0.) {
  center = c;
  Vector righta = (b - c);
  textureX = righta.mag();
  right = righta / textureX;
  normal = right.cross(b - a).normalize();

  xsin = -right.z;
  if (xsin < -1.)
    xsin = -1;
  else if (xsin > 1.)
    xsin = 1.;
  yaw = asin(xsin);
  xcos = sqrt(1. - xsin * xsin);

  zcos = right.x / xcos;
  zsin = -right.y / xcos;
  if (zsin < -1.)
    zsin = -1;
  else if (zsin > 1.)
    zsin = 1.;
  if (zcos < -1.)
    zcos = -1;
  else if (zcos > 1.)
    zcos = 1.;
  roll = asin(zsin);

  ycos = normal.z / xcos;
  if (ycos < -1.)
    ycos = -1;
  else if (ycos > 1.)
    ycos = 1.;
  pitch = acos(ycos);
  ysin = sqrt(1 - ycos * ycos);

  up.x = -xsin * ysin * zcos + ycos * zsin;
  up.y = ycos * zcos + xsin * ysin * zsin;
  up.z = -xcos * ysin;
  Vector temp = normal.cross(right);
  Vector np = solveScalers(right, up, normal, a - c);
  textureY = np.y;
  thirdX = np.x;

  d = -normal.dot(center);
  centroid = center + (right * (textureX + thirdX) + up * textureY) / 3.0;
}

double Triangle::getIntersection(const Ray& ray) {
  double time = Plane::getIntersection(ray);
  if (time == inf)
    return time;
  Vector dist = solveScalers(right, up, normal, ray.point + ray.vector * time - center);
  unsigned char tmp = (thirdX - dist.x) * textureY + (thirdX - textureX) * (dist.y - textureY) < 0.0;
  return ((tmp != (textureX * dist.y < 0.0)) || (tmp != (dist.x * textureY - thirdX * dist.y < 0.0))) ? inf : time;
}

bool Triangle::getLightIntersection(const Ray& ray, double* fill) {
  const double t = ray.vector.dot(normal);
  const double norm = normal.dot(ray.point) + d;
  const double r = -norm / t;
  if (r <= 0. || r >= 1.) return false;
  Vector dist = solveScalers(right, up, normal, ray.point + ray.vector * r - center);

  unsigned char tmp = (thirdX - dist.x) * textureY + (thirdX - textureX) * (dist.y - textureY) < 0.0;
  if ((tmp != (textureX * dist.y < 0.0)) || (tmp != (dist.x * textureY - thirdX * dist.y < 0.0))) return false;

  if (texture->opacity > 1 - 1E-6) return true;
  unsigned char temp[4];
  double amb, op, ref;
  texture->getColor(temp, &amb, &op, &ref, fix(dist.x / textureX - .5), fix(dist.y / textureY - .5));
  if (op > 1 - 1E-6) return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

const AABB Triangle::getAABB() {
  AABB aabb;
  aabb.expand(center);                                   // c
  aabb.expand(center + right * textureX);                // b
  aabb.expand(center + right * thirdX + up * textureY);  // a
  return aabb;
}

const Vector Triangle::getCentroid() {
  return centroid;
}