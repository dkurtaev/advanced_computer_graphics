#include "include/bounding_box.hpp"

#include <float.h>

BoundingBox::BoundingBox(const Point3f& origin, const Point3f& size)
    : origin_(origin), size_(size) {}

BoundingBox::BoundingBox(const Vertex& v1, const Vertex& v2, const Vertex& v3)
    : origin_(0, 0, 0), size_(0, 0, 0) {
  float min[] = {FLT_MAX, FLT_MAX, FLT_MAX};
  float max[] = {FLT_MIN, FLT_MIN, FLT_MIN};
  const Point3f pts[] = { v1.GetPos(), v2.GetPos(), v3.GetPos() };
  for (int i = 0; i < 3; ++i) {
    float coords[3];
    pts[i].GetCoords(coords);
    for (int j = 0; j < 3; ++j) {
      if (coords[j] > max[j]) max[j] = coords[j];
      if (coords[j] < min[j]) min[j] = coords[j];
    }
  }
  origin_ = Point3f(min[0], min[1], min[2]);
  size_ = Point3f(max[0] - min[0], max[1] - min[1], max[2] - min[2]);
}

bool BoundingBox::IsIntersects(const Point3f& ray_point,
                               const Point3f& ray, float* distance) const {
  float point[3], dir[3], origin[3], size[3];
  ray_point.GetCoords(point);
  ray.GetCoords(dir);
  origin_.GetCoords(origin);
  size_.GetCoords(size);

  float min = 0, max = FLT_MAX;
  for (int i = 0; i < 3; ++i) {
    if (dir[i] != 0) {
      float left = (origin[i] - point[i]) / dir[i];
      float right = left + size[i] / dir[i];
      if (dir[i] < 0) {
        std::swap(left, right);
      }
      if (right < min || max < left) {
        return false;
      }
      if (left > min) {
        min = left;
      }
      if (right < max) {
        max = right;
      }
    } else {
      if (point[i] < origin[i] || origin[i] + size[i] < point[i]) {
        return false;
      }
    }
  }
  *distance = min;
  return true;
}

BoundingBox& BoundingBox::operator +=(const Point3f& p) {
  origin_ += p;
  return *this;
}
