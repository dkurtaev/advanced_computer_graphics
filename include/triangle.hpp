#ifndef INCLUDE_TRIANGLE_HPP_
#define INCLUDE_TRIANGLE_HPP_

#include <stdint.h>

#include "include/point3f.hpp"
#include "include/vertex.hpp"

class Triangle {
 public:
  Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
           const Point3f& color);

  bool IsIntersects(const Point3f& start_point, const Point3f& ray,
                    Point3f* intersection) const;

  void GetColor(uint8_t* r, uint8_t* g, uint8_t* b) const;

 private:
  Point3f Project(const Point3f& point, const Point3f& dir) const;

  Vertex v1_, v2_, v3_;
  Point3f color_;
};

#endif  // INCLUDE_TRIANGLE_HPP_
