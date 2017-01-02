#include "include/triangle.hpp"

#include <iostream>

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
                   const Point3f& color)
    : v1_(v1), v2_(v2), v3_(v3), color_(color) {}

bool Triangle::IsIntersects(const Point3f& start_point, const Point3f& ray,
                            Point3f* intersection) {
  // Let a = p1 - start_point
  //     b = p2 - start_point
  //     c = p3 - start_point
  // Solve linear system:
  // ray = c1 * a + c2 * b + c3 * c
  // If all c1, c2, c3 is in [0, 1] -> ray intersectst triangle because
  // ray is inside cone.
  Point3f p1 = v1_.GetPos();
  Point3f p2 = v2_.GetPos();
  Point3f p3 = v3_.GetPos();
  Point3f a = Point3f(p1 - start_point, true);
  Point3f b = Point3f(p2 - start_point, true);
  Point3f c = Point3f(p3 - start_point, true);

  float denominator = Point3f::Det(a, b, c);

  float bary_a = Point3f::Det(ray, b, c) / denominator;
  if (bary_a < 0.0f || bary_a > 1.0f) {
    return false;
  }

  float bary_b = Point3f::Det(a, ray, c) / denominator;
  if (bary_b < 0.0f || bary_b > 1.0f) {
    return false;
  }

  float bary_c = Point3f::Det(a, b, ray) / denominator;
  if (0.0f <= bary_c && bary_c <= 1.0f) {
    *intersection = p1 * bary_a + p2 * bary_b + p3 * (1.0f - bary_a - bary_b);
    return true;
  } else {
    return false;
  }
}

void Triangle::GetColor(uint8_t* r, uint8_t* g, uint8_t* b) {
  float rgb[3];
  color_.GetCoords(rgb);
  *r = 255 * rgb[0];
  *g = 255 * rgb[1];
  *b = 255 * rgb[2];
}
