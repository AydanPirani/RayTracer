#include "bvh.h"

#include <algorithm>
#include <numeric>

#include "shape.h"

BVH::BVH(const std::vector<Shape*>& shapes) {
  std::vector<int> indices;
  for (int i = 0; i < shapes.size(); i++) {
    if (shapes[i]->useBVH()) {
      indices.push_back(i);
    } else {
      notBvh.push_back(shapes[i]);
    }
  }

#pragma omp parallel num_threads(4)
  {
#pragma omp single
    {
      rootNode = createBVH(0, indices.size(), indices, shapes);
    }
  }
}

BVHNode* BVH::createBVH(int leftIdx, int rightIdx, std::vector<int>& indices,
                        const std::vector<Shape*>& shapes) {
  if ((rightIdx - leftIdx) <= THRESHOLD) {
    BVHNode* leafNode = getNextNode(shapes.size());
    leafNode->updateBoundingBox(leftIdx, rightIdx, indices, shapes);
    leafNode->leftChild = nullptr;
    leafNode->rightChild = nullptr;
    return leafNode;
  }

  BVHNode* node = getNextNode(shapes.size());
  node->updateBoundingBox(leftIdx, rightIdx, indices, shapes);

  int midpointIdx = leftIdx + (rightIdx - leftIdx) / 2;
  Axis axis = node->getLargestAxis();
  std::nth_element(indices.begin() + leftIdx, indices.begin() + midpointIdx,
                   indices.begin() + rightIdx, [&](int idxA, int idxB) {
                     const Vector centroidA = shapes[idxA]->getCentroid();
                     const Vector centroidB = shapes[idxB]->getCentroid();
                     switch (axis) {
                       case Axis::X_AXIS:
                         return centroidA.x < centroidB.x;
                       case Axis::Y_AXIS:
                         return centroidA.y < centroidB.y;
                       case Axis::Z_AXIS:
                         return centroidA.z < centroidB.z;
                       default:
                         return false;
                     }
                   });
#pragma omp taskgroup
  {
#pragma omp task shared(node, indices, shapes)
    node->leftChild = createBVH(leftIdx, midpointIdx, indices, shapes);
#pragma omp task shared(node, indices, shapes)
    node->rightChild = createBVH(midpointIdx, rightIdx, indices, shapes);
  }
  return node;
}

Axis BVHNode::getLargestAxis() const {
  double xSize = aabb.aabbMax.x - aabb.aabbMin.x;
  double ySize = aabb.aabbMax.y - aabb.aabbMin.y;
  double zSize = aabb.aabbMax.z - aabb.aabbMin.z;

  if (xSize >= ySize && xSize >= zSize) {
    return Axis::X_AXIS;
  } else if (ySize >= zSize) {
    return Axis::Y_AXIS;
  } else {
    return Axis::Z_AXIS;
  }
}

BVHNode* BVH::getNextNode(int size) {
  return new BVHNode();
}

double BVH::getIntersection(const Ray& ray, Shape*& outHit) const {
  double notBvhRv = inf;
  Shape* maybeOut = nullptr;
  for (auto shape : notBvh) {
    const double hitTime = shape->getIntersection(ray);
    if (hitTime < notBvhRv) {
      maybeOut = shape;
      notBvhRv = hitTime;
    }
  }
  double rv = rootNode->getIntersection(ray, outHit);
  if (notBvhRv < rv) {
    rv = notBvhRv;
    outHit = maybeOut;
  }

  return rv;
}

bool BVH::getLightIntersection(const Ray& ray, double* fill) const {
  bool rv = false;
  for (auto shape : notBvh) {
    rv = shape->getLightIntersection(ray, fill);
    if (rv) return true;
  }

  return rootNode->getLightIntersection(ray, fill);
}

bool AABB::intersects(const Ray& ray) const {
  const Vector invDir(1.0f / ray.vector.x, 1.0f / ray.vector.y,
                      1.0f / ray.vector.z);
  const Vector t1 = (aabbMin - ray.point) * invDir;
  const Vector t2 = (aabbMax - ray.point) * invDir;

  const Vector tNear(std::min(t1.x, t2.x), std::min(t1.y, t2.y),
                     std::min(t1.z, t2.z));
  const Vector tFar(std::max(t1.x, t2.x), std::max(t1.y, t2.y),
                    std::max(t1.z, t2.z));

  double tMin = std::max(std::max(tNear.x, tNear.y), tNear.z);
  double tMax = std::min(std::min(tFar.x, tFar.y), tFar.z);

  return tMax >= tMin;
}

double BVHNode::getIntersection(const Ray& ray, Shape*& outHit) const {
  double rv = inf;
  if (leftChild == nullptr && rightChild == nullptr) {
    for (auto shape : _shapes) {
      const double hitTime = shape->getIntersection(ray);
      if (hitTime < rv) {
        outHit = shape;
        rv = hitTime;
      }
    }

    return rv;
  }

  if (!aabb.intersects(ray)) {
    return inf;
  }

  Shape* leftOut = nullptr;
  Shape* rightOut = nullptr;
  double left = leftChild->getIntersection(ray, leftOut);
  double right = rightChild->getIntersection(ray, rightOut);

  if (left <= right) {
    outHit = leftOut;
    return left;
  } else {
    outHit = rightOut;
    return right;
  }
}

bool BVHNode::getLightIntersection(const Ray& ray, double* fill) const {
  bool rv = false;
  if (leftChild == nullptr && rightChild == nullptr) {
    for (auto shape : _shapes) {
      rv = shape->getLightIntersection(ray, fill);
      if (rv) return true;
    }
    return rv;
  }

  if (!aabb.intersects(ray)) {
    return false;
  }
  // Check on left
  if (leftChild == nullptr) {
    return rightChild->getLightIntersection(ray, fill);
  }
  // Check on right
  if (rightChild == nullptr) {
    return leftChild->getLightIntersection(ray, fill);
  }
  // N.B.: shortcircuits
  return rightChild->getLightIntersection(ray, fill) ||
         leftChild->getLightIntersection(ray, fill);
}

void BVHNode::updateBoundingBox(int leftIdx, int rightIdx,
                                std::vector<int>& indices,
                                const std::vector<Shape*>& shapes) {
  for (int i = leftIdx; i < rightIdx; i++) {
    aabb.expand(shapes[indices[i]]->getAABB());
    _shapes.push_back(shapes.at(indices[i]));
  }
}

void AABB::expand(const AABB& other) {
  aabbMin.x = std::min(aabbMin.x, other.aabbMin.x);
  aabbMin.y = std::min(aabbMin.y, other.aabbMin.y);
  aabbMin.z = std::min(aabbMin.z, other.aabbMin.z);
  aabbMin.x = std::min(aabbMin.x, other.aabbMax.x);
  aabbMin.y = std::min(aabbMin.y, other.aabbMax.y);
  aabbMin.z = std::min(aabbMin.z, other.aabbMax.z);

  aabbMax.x = std::max(aabbMax.x, other.aabbMin.x);
  aabbMax.y = std::max(aabbMax.y, other.aabbMin.y);
  aabbMax.z = std::max(aabbMax.z, other.aabbMin.z);
  aabbMax.x = std::max(aabbMax.x, other.aabbMax.x);
  aabbMax.y = std::max(aabbMax.y, other.aabbMax.y);
  aabbMax.z = std::max(aabbMax.z, other.aabbMax.z);
}

void AABB::expand(const Vector& point) {
  aabbMin.x = std::min(aabbMin.x, point.x);
  aabbMin.y = std::min(aabbMin.y, point.y);
  aabbMin.z = std::min(aabbMin.z, point.z);

  aabbMax.x = std::max(aabbMax.x, point.x);
  aabbMax.y = std::max(aabbMax.y, point.y);
  aabbMax.z = std::max(aabbMax.z, point.z);
}
