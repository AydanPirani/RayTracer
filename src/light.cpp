#include "light.h"

#include "camera.h"
#include "shape.h"

Light::Light(const Vector& cente, double* colo) : center(cente), color(colo) {}
Light::~Light() { delete color; }
