#include "include/bounding_box.hpp"

#include <float.h>

BoundingBox::BoundingBox(const Point3f& origin, const Point3f& size)
    : origin_(origin), size_(size) {}

int BoundingBox::LongestDim(float* origin, float* size) const {
  int longest_dim = 0;
  float origins[3], sizes[3];
  size_.GetCoords(sizes);
  origin_.GetCoords(origins);
  if (sizes[1] > sizes[longest_dim]) {
    longest_dim = 1;
  }
  if (sizes[2] > sizes[longest_dim]) {
    longest_dim = 2;
  }
  *size = sizes[longest_dim];
  *origin = origins[longest_dim];
  return longest_dim;
}

BoundingBox::BoundingBox(const std::vector<Triangle*>& tris)
    : origin_(0, 0, 0), size_(0, 0, 0) {
  float min[3], max[3], coords[3];
  for (int i = 0, n = tris.size(); i < n; ++i) {
    for (int j = 0; j < 3; ++j) {
      tris[i]->GetCoords(coords, j);
      if (coords[0] > max[j]) max[j] = coords[0];
      if (coords[1] > max[j]) max[j] = coords[1];
      if (coords[2] > max[j]) max[j] = coords[2];
      if (coords[0] < min[j]) min[j] = coords[0];
      if (coords[1] < min[j]) min[j] = coords[1];
      if (coords[2] < min[j]) min[j] = coords[2];
    }
  }
  origin_ = Point3f(min[0], min[1], min[2]);
  size_ = Point3f(max[0] - min[0], max[1] - min[1], max[2] - min[2]);
}

void BoundingBox::Split(int dim, BoundingBox* left, BoundingBox* right) const {
  float origin[3], size[3];
  origin_.GetCoords(origin);
  size_.GetCoords(size);
  size[dim] *= 0.5;
  origin[dim] += size[dim];

  left->origin_ = origin_;
  left->size_ = Point3f(size[0], size[1], size[2]);
  right->origin_ = Point3f(origin[0], origin[1], origin[2]);
  right->size_ = left->size_;
}

bool BoundingBox::IsIntersects(const Point3f& ray_point,
                               const Point3f& ray) const {
  float point[3], dir[3], origin[3], size[3];
  ray_point.GetCoords(point);
  ray.GetCoords(dir);
  origin_.GetCoords(origin);
  size_.GetCoords(size);

  float min = 0, max = FLT_MAX;
  for (int i = 0; i < 3; ++i) {
    float left, right;
    if (dir[i] != 0) {
      left = (origin[i] - point[i]) / dir[i];
      right = left + size[i] / dir[i];
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
  return true;
}

BoundingBox& BoundingBox::operator +=(const Point3f& p) {
  origin_ += p;
  return *this;
}
