#include "autonoma.h"

#include "camera.h"
#include "shape.h"

Autonoma::Autonoma(const Camera& c) : camera(c) {
  depth = 10;
  skybox = BLACK;
}

Autonoma::Autonoma(const Camera& c, Texture* tex) : camera(c) {
  depth = 10;
  skybox = tex;
}

void Autonoma::addShape(Shape* r) { shapes.push_back(r); }

void Autonoma::addLight(Light* r) { lights.push_back(r); }

void getLight(double* tColor, Autonoma* aut, const Vector& point,
              const Vector& norm, unsigned char flip) {
  tColor[0] = tColor[1] = tColor[2] = 0.;
  const double normMag = norm.mag();

  for (auto& t : aut->lights) {
    double lightColor[3];
    lightColor[0] = t->color[0];
    lightColor[1] = t->color[1];
    lightColor[2] = t->color[2];
    Vector ra = t->center - point;

    bool hit = false;
    for (int i = 0; !hit && i < aut->shapes.size(); i++) {
      hit = aut->shapes[i]->getLightIntersection(Ray(point + ra * .01, ra), lightColor);
    }
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