#include "include/triangle.hpp"

#include <iostream>
#include <math.h>

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
                   const Point3f& color)
    : v1_(v1), v2_(v2), v3_(v3), color_(color) {}

bool Triangle::IsIntersects(const Point3f& start_point, const Point3f& ray,
                            Point3f* intersection) const {
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

  float bary_p2 = Point3f::Det(p1, *intersection, p3) / denominator;
  if (bary_p2 < 0.0f || 1.0f < bary_p2) return false;

  float bary_p3 = Point3f::Det(p1, p2, *intersection) / denominator;
  return 0.0f <= bary_p3 && bary_p3 <= 1.0f;
}

void Triangle::GetColor(uint8_t* r, uint8_t* g, uint8_t* b) const {
  float rgb[3];
  color_.GetCoords(rgb);
  *r = 255 * rgb[0];
  *g = 255 * rgb[1];
  *b = 255 * rgb[2];
}

Point3f Triangle::Project(const Point3f& point, const Point3f& dir) const {
  Point3f point1 = v1_.GetPos();
  Point3f point2 = v2_.GetPos();
  Point3f point3 = v3_.GetPos();
  float p1[3], p2[3], p3[3], line_point[3], line_dir[3];
  point1.GetCoords(p1);
  point2.GetCoords(p2);
  point3.GetCoords(p3);
  point.GetCoords(line_point);
  dir.GetCoords(line_dir);

  float nx = p1[1] * (p2[2] - p3[2]) +
             p2[1] * (p3[2] - p1[2]) +
             p3[1] * (p1[2] - p2[2]);
  float ny = p1[0] * (p3[2] - p2[2]) +
             p2[0] * (p1[2] - p3[2]) +
             p3[0] * (p2[2] - p1[2]);
  float nz = p1[0] * (p2[1] - p3[1]) +
             p2[0] * (p3[1] - p1[1]) +
             p3[0] * (p1[1] - p2[1]);

  if (line_dir[1] != 0) {
    Point3f right_part(p1[1] * (p3[0] * p2[2] - p2[0] * p3[2]) +
                       p2[1] * (p1[0] * p3[2] - p3[0] * p1[2]) +
                       p3[1] * (p2[0] * p1[2] - p1[0] * p2[2]),
                       line_dir[1] * line_point[0] -
                       line_dir[0] * line_point[1],
                       line_dir[2] * line_point[1] -
                       line_dir[1] * line_point[2]);
    Point3f a(nx, line_dir[1], 0);
    Point3f b(ny, -line_dir[0], line_dir[2]);
    Point3f c(nz, 0, -line_dir[1]);

    float denominator = Point3f::Det(a, b, c);
    return Point3f(Point3f::Det(right_part, b, c) / denominator,
                   Point3f::Det(a, right_part, c) / denominator,
                   Point3f::Det(a, b, right_part) / denominator);
  } else {
    Point3f right_part(p1[1] * (p3[0] * p2[2] - p2[0] * p3[2]) +
                       p2[1] * (p1[0] * p3[2] - p3[0] * p1[2]) +
                       p3[1] * (p2[0] * p1[2] - p1[0] * p2[2]),
                       line_dir[2] * line_point[0] -
                       line_dir[0] * line_point[2],
                       0);
    Point3f a(nx, line_dir[2], 0);
    Point3f b(ny, 0, 1);
    Point3f c(nz, -line_dir[0], 0);

    float denominator = Point3f::Det(a, b, c);
    return Point3f(Point3f::Det(right_part, b, c) / denominator,
                   Point3f::Det(a, right_part, c) / denominator,
                   Point3f::Det(a, b, right_part) / denominator);
  }
}
