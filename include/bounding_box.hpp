#ifndef INCLUDE_BOUNDING_BOX_HPP_
#define INCLUDE_BOUNDING_BOX_HPP_

#include <vector>

#include "include/point3f.hpp"
#include "include/triangle.hpp"
#include "include/vertex.hpp"

class BoundingBox {
 public:
  // Origin point - (min_x, min_y, min_z);
  BoundingBox(const Point3f& origin, const Point3f& size);

  BoundingBox(const Vertex& v1, const Vertex& v2, const Vertex& v3);

  bool IsIntersects(const Point3f& ray_point, const Point3f& ray) const;

  BoundingBox& operator +=(const Point3f& p);

 private:
  Point3f origin_;
  Point3f size_;
};

#endif  // INCLUDE_BOUNDING_BOX_HPP_
