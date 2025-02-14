#ifndef __AUTONOMA_H__
#define __AUTONOMA_H__
#include <vector>

#include "Textures/colortexture.h"
#include "Textures/texture.h"
#include "camera.h"
#include "light.h"
#include "vector.h"
struct LightNode {
  Light *data;
  LightNode *prev, *next;
};

class Shape;
struct ShapeNode {
  Shape *data;
  ShapeNode *prev, *next;
};

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
};

void getLight(double *toFill, Autonoma *aut, const Vector &point, const Vector &norm, unsigned char r);

#endif