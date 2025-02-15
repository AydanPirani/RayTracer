#include "sphere.h"

Sphere::Sphere(const Vector& c, Texture* t, double ya, double pi, double ro, double rad) : Shape(c, t, ya, pi, ro) {
  textureX = textureY = 1.;
  normalMap = NULL;
  radius = rad;
}
bool Sphere::getLightIntersection(const Ray& ray, double* fill) {
  const double A = ray.vector.mag2();
  const double B_half = ray.vector.dot(ray.point - center);
  const double C = (ray.point - center).mag2() - radius * radius;
  const double D_quart = B_half * B_half - A * C;
  if (D_quart < 0. || D_quart < B_half * ((B_half >= 0) ? B_half : -B_half))
    return false;

  const double sqrt_D_half = sqrt(D_quart);
  const double root1 = (-B_half - sqrt_D_half) / A;
  const double root2 = (-B_half + sqrt_D_half) / A;
  const double time = (root1 > 0) ? root1 : root2;

  if (time >= 1.) return false;
  const Vector point = ray.point + ray.vector * time;
  const double data2 = (center.y - point.y + radius) / (2 * radius);
  const double data3 = atan2(point.z - center.z, point.x - center.x);
  unsigned char temp[4];
  double amb, op, ref;
  texture->getColor(temp, &amb, &op, &ref,
                    fix((yaw + data2) / M_TWO_PI / textureX),
                    fix((pitch / M_TWO_PI - (data3))) / textureY);
  if (op > 1 - 1E-6) return true;
  fill[0] *= temp[0] / 255.;
  fill[1] *= temp[1] / 255.;
  fill[2] *= temp[2] / 255.;
  return false;
}

double Sphere::getIntersection(const Ray& ray) {
  // Ax^2 + bx + c
  const Vector toRay = ray.point - center;
  const double A = ray.vector.mag2();
  // const double B = 2*ray.vector.dot(toRay);
  const double B_half = ray.vector.dot(toRay);
  const double C = toRay.mag2() - radius * radius;
  const double D_quart = B_half * B_half - A * C;
  if (D_quart < 0)
    return inf;
  else {
    const double sqrt_D_half = sqrt(D_quart);
    const double root1 = (-B_half - sqrt_D_half) / A;
    const double root2 = (-B_half + sqrt_D_half) / A;
    return (root1 > 0) ? (root1) : ((root2 > 0) ? root2 : inf);
  }
}
unsigned char Sphere::reversible() { return 0; }

void Sphere::getColor(unsigned char* toFill, double* amb, double* op,
                      double* ref, Autonoma* r, Ray ray, unsigned int depth) {
  double data3 = (center.y - ray.point.y + radius) / (2 * radius);
  double data2 = atan2(ray.point.z - center.z, ray.point.x - center.x);
  texture->getColor(toFill, amb, op, ref,
                    fix((yaw + data2) / M_TWO_PI / textureX),
                    fix((pitch / M_TWO_PI - (data3)) / textureY));
}
Vector Sphere::getNormal(const Vector& point) {
  Vector vect = point - center;
  /*   A x B = <x, y, z>
  <ay bz- az by,  bz ax - az bx, ax by - bx ay>
  az = 0
  <ay bz,  bz ax, ax by - bx ay >
  bx = 0
  <ay bz,  bz ax, ax by >
  ax = 1
  <ay bz,  bz , by >
  B: <0, z, -y>
  A <-1,x/y,0>
  */
  if (normalMap == NULL) return vect;
  double data3 = (center.y - point.y + radius) / (2 * radius);
  double data2 = atan2(point.z - center.z, point.x - center.x);
  vect = vect.normalize();
  Vector right = Vector(vect.x, vect.z, -vect.y);
  Vector up = Vector(vect.z, vect.y, -vect.x);
  double am, ref, op;
  unsigned char norm[3];
  normalMap->getColor(norm, &am, &op, &ref,
                      fix(((mapOffX + mapOffX) + data2) / M_TWO_PI / mapX),
                      fix(((mapOffY + mapOffY) / M_TWO_PI - data3) / mapY));
  return ((norm[0] - 128) * right + (norm[1] - 128) * up + norm[2] * vect)
      .normalize();
}