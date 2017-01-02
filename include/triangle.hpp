#ifndef INCLUDE_TRIANGLE_HPP_
#define INCLUDE_TRIANGLE_HPP_

#include "include/point3f.hpp"
#include "include/vertex.hpp"

class Triangle {
 public:
  Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

  bool IsIntersects(const Point3f& start_point, const Point3f& ray);

 private:
  Vertex v1_, v2_, v3_;
};

#endif  // INCLUDE_TRIANGLE_HPP_
