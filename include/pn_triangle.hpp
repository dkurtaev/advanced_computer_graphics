#ifndef INCLUDE_PN_TRIANGLE_HPP_
#define INCLUDE_PN_TRIANGLE_HPP_

#include <vector>

#include "include/vertex.hpp"
#include "include/color.hpp"
#include "include/triangle.hpp"
#include "include/point3f.hpp"
#include "include/bounding_box.hpp"

class PNTriangle {
 public:
  PNTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
             const Color& color, int lod = 0);

  ~PNTriangle();

  bool IsIntersects(const Point3f& ray_point, const Point3f& ray) const;

  void GetTriangles(std::vector<Triangle*>* tris);

  void Move(const Point3f& delta);

 private:
  std::vector<Triangle*> tris_;
  BoundingBox bbox_;
};

#endif  // INCLUDE_PN_TRIANGLE_HPP_
