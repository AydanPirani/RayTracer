#ifndef __BVH_H__
#define __BVH_H__

#define THRESHOLD 5

#include <vector>

#include "autonoma.h"

// source for a lot of this code: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/

enum class Axis {
  X_AXIS,
  Y_AXIS,
  Z_AXIS
};

struct AABB {
  Vector aabbMin = Vector(INFINITY, INFINITY, INFINITY);
  Vector aabbMax = Vector(-INFINITY, -INFINITY, -INFINITY);

  void expand(const AABB& other);
  void expand(const Vector& point);
  bool intersects(const Ray& ray) const;
};

struct BVHNode {
  BVHNode *leftChild, *rightChild;
  AABB aabb;
  std::vector<Shape*> _shapes;

  void updateBoundingBox(int leftIdx, int rightIdx, std::vector<int>& indices, const std::vector<Shape*>& shapes);
  Axis getLargestAxis() const;
  double getIntersection(const Ray& ray, Shape*& outHit) const;
  bool getLightIntersection(const Ray& ray, double* fill) const;
};

class BVH {
  BVHNode* nodePool = nullptr;
  BVHNode* getNextNode(int size);
  void sortIndicesByCentroid(int leftIdx, int rightIdx, std::vector<int>& indices, const std::vector<Shape*>& shapes, Axis axis);
  BVHNode* createBVH(int leftIdx, int rightIdx, std::vector<int>& indices, const std::vector<Shape*>& shapes);

 public:
  BVH(const std::vector<Shape*>& shapes);
  BVHNode* rootNode;
  double getIntersection(const Ray& ray, Shape*& outHit) const;
  bool getLightIntersection(const Ray& ray, double* fill) const;
};

#endif