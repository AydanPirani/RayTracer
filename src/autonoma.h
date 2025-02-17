#ifndef __AUTONOMA_H__
#define __AUTONOMA_H__
#include <vector>

#include "Textures/colortexture.h"
#include "Textures/texture.h"
#include "camera.h"
#include "light.h"
#include "vector.h"

class Shape;

class Autonoma {
 public:
  Camera camera;
  Texture *skybox;
  unsigned int depth;
  std::vector<Shape *> shapes;
  std::vector<Light *> lights;
  Autonoma(const Camera &c);
  Autonoma(const Camera &c, Texture *tex);
  void addShape(Shape *s);
  void addLight(Light *s);

  ~Autonoma();
};

#endif