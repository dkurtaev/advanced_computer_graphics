#include "include/triangle.hpp"

#include <iostream>
#include <math.h>

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
                   const Color& color)
    : v1_(v1), v2_(v2), v3_(v3), color_(color) {}

bool Triangle::IsIntersects(const Point3f& start_point, const Point3f& ray,
                            Point3f* intersection, float* u, float* v) const {
  *intersection = Project(start_point, ray);
  if (Point3f::Dot(*intersection - start_point, ray) < 0) {
    return false;
  }
  // Compute barycentric coordinates,
  // intersection_point = bary_p1 * p1 + bary_p2 * p2 + bary_p3 * p3
  Point3f p1 = v1_.GetPos();
  Point3f p2 = v2_.GetPos();
  Point3f p3 = v3_.GetPos();
  float denominator = Point3f::Det(p1, p2, p3);

  float bary_p1 = Point3f::Det(*intersection, p2, p3) / denominator;
  if (bary_p1 < 0.0f || 1.0f < bary_p1) return false;
  *u = bary_p1;

  float bary_p2 = Point3f::Det(p1, *intersection, p3) / denominator;
  if (bary_p2 < 0.0f || 1.0f < bary_p2) return false;
  *v = bary_p2;

  float bary_p3 = Point3f::Det(p1, p2, *intersection) / denominator;
  return 0.0f <= bary_p3 && bary_p3 <= 1.0f;
}

Color Triangle::GetColor() const {
  return color_;
}

Point3f Triangle::Project(const Point3f& point, const Point3f& dir) const {
  Point3f p1 = v1_.GetPos();
  Point3f p2 = v2_.GetPos();
  Point3f p3 = v3_.GetPos();
  float line_point[3], line_dir[3], normal[3];
  point.GetCoords(line_point);
  dir.GetCoords(line_dir);

  Point3f::Cross(p2 - p1, p3 - p1).GetCoords(normal);

  if (line_dir[1] != 0) {
    Point3f right_part(Point3f::Det(p1, p2, p3),
                       line_dir[1] * line_point[0] -
                       line_dir[0] * line_point[1],
                       line_dir[2] * line_point[1] -
                       line_dir[1] * line_point[2]);
    Point3f a(normal[0], line_dir[1], 0);
    Point3f b(normal[1], -line_dir[0], line_dir[2]);
    Point3f c(normal[2], 0, -line_dir[1]);

    float denominator = Point3f::Det(a, b, c);
    return Point3f(Point3f::Det(right_part, b, c) / denominator,
                   Point3f::Det(a, right_part, c) / denominator,
                   Point3f::Det(a, b, right_part) / denominator);
  } else {
    Point3f right_part(Point3f::Det(p1, p2, p3),
                       line_dir[2] * line_point[0] -
                       line_dir[0] * line_point[2],
                       0);
    Point3f a(normal[0], line_dir[2], 0);
    Point3f b(normal[1], 0, 1);
    Point3f c(normal[2], -line_dir[0], 0);

    float denominator = Point3f::Det(a, b, c);
    return Point3f(Point3f::Det(right_part, b, c) / denominator,
                   Point3f::Det(a, right_part, c) / denominator,
                   Point3f::Det(a, b, right_part) / denominator);
  }
}

Point3f Triangle::GetNormal(float u, float v) {
  Point3f n1 = v1_.GetNormal();
  Point3f n2 = v2_.GetNormal();
  Point3f n3 = v3_.GetNormal();
  return Point3f(n1 * u + n2 * v + n3 * (1.0f - u - v), true);
}
