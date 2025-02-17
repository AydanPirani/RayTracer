#include "shape.h"

Shape::Shape(const Vector& c, Texture* t, double ya, double pi, double ro) : center(c), texture(t), yaw(ya), pitch(pi), roll(ro) {}
Shape::~Shape() { }

void Shape::setAngles(double a, double b, double c) {
  yaw = a;
  pitch = b;
  roll = c;
}

void Shape::setYaw(double a) {
  yaw = a;
}

void Shape::setPitch(double b) {
  pitch = b;
}

void Shape::setRoll(double c) {
  roll = c;
}

const Vector Shape::getCentroid() { return center; }