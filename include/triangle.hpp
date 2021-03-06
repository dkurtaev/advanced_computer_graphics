#ifndef INCLUDE_TRIANGLE_HPP_
#define INCLUDE_TRIANGLE_HPP_

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/color.hpp"

class Triangle {
 public:
  Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
           const Color& color);

  bool IsIntersects(const Point3f& start_point, const Point3f& ray,
                    Point3f* intersection, float* u, float* v,
                    float* distance) const;

  Point3f GetNormal(float u, float v);

  Color GetColor() const;

  void Move(const Point3f& delta);

  void SetColor(const Color& color);

 private:
  Vertex v1_, v2_, v3_;
  Color color_;
};

#endif  // INCLUDE_TRIANGLE_HPP_
