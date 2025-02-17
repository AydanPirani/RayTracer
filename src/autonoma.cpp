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

Autonoma::~Autonoma() {
  for (auto& l : lights) {
    delete l;
  }

  for (auto& s : shapes) {
    delete s;
  }
}
