#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <vector>

#include "Textures/colortexture.h"
#include "Textures/texture.h"
#include "camera.h"
#include "vector.h"

class Light {
 public:
  double* color;
  unsigned char* getColor(unsigned char a, unsigned char b, unsigned char c);
  Vector center;
  Light(const Vector& cente, double* colo);

  ~Light();
};

#endif
