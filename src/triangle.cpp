#include "include/triangle.hpp"

#include <iostream>
#include <math.h>

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
                   const Color& color)
    : v1_(v1), v2_(v2), v3_(v3), color_(color) {}

bool Triangle::IsIntersects(const Point3f& start_point, const Point3f& ray,
                            Point3f* intersection, float* u, float* v) const {
  Point3f p1 = v1_.GetPos();
  Point3f p2 = v2_.GetPos();
  Point3f p3 = v3_.GetPos();

  Point3f b = p1 - p3;
  Point3f c = p2 - p3;
  Point3f right_part = start_point - p3;

  float denominator = Point3f::Det(ray, b, c);

  *u = Point3f::Det(ray, right_part, c) / denominator;
  if (*u < 0.0f || 1.0f < *u) return false;

  *v = Point3f::Det(ray, b, right_part) / denominator;
  if (*v < 0.0f || 1.0f < *v) return false;

  float w = *u + *v;
  if (w <= 1.0f) {
    *intersection = *u * p1 + *v * p2 + (1.0f - w) * p3;
    return Point3f::Dot(ray, *intersection - start_point) > 0;
  } else {
    return false;
  }
}

Color Triangle::GetColor() const {
  return color_;
}

Point3f Triangle::GetNormal(float u, float v) {
  Point3f n1 = v1_.GetNormal();
  Point3f n2 = v2_.GetNormal();
  Point3f n3 = v3_.GetNormal();
  return Point3f(n1 * u + n2 * v + n3 * (1.0f - u - v), true);
}
