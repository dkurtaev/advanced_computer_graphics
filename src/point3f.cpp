#include "include/point3f.hpp"

#include <math.h>

Point3f::Point3f(float x, float y, float z, bool normed) {
  Init(x, y, z, normed);
}

Point3f::Point3f(const Point3f& p, bool normed) {
  Init(p.coords_[0], p.coords_[1], p.coords_[2], normed);
}

inline void Point3f::Init(float x, float y, float z, bool normed) {
  if (!normed) {
    coords_[0] = x;
    coords_[1] = y;
    coords_[2] = z;
  } else {
    float norm = sqrt(x * x + y * y + z * z);
    coords_[0] = x / norm;
    coords_[1] = y / norm;
    coords_[2] = z / norm;
  }
}

float Point3f::Dot(const Point3f& p1, const Point3f& p2) {
  return p1.coords_[0] * p2.coords_[0] +
         p1.coords_[1] * p2.coords_[1] +
         p1.coords_[2] * p2.coords_[2];
}

Point3f Point3f::operator -(const Point3f& p) const {
  return Point3f(coords_[0] - p.coords_[0],
                 coords_[1] - p.coords_[1],
                 coords_[2] - p.coords_[2]);
}

Point3f Point3f::operator +(const Point3f& p) const {
  return Point3f(coords_[0] + p.coords_[0],
                 coords_[1] + p.coords_[1],
                 coords_[2] + p.coords_[2]);
}

float Point3f::Det(const Point3f& p1, const Point3f& p2, const Point3f& p3) {
  return p1.coords_[0] * (p2.coords_[1] * p3.coords_[2] -
                          p2.coords_[2] * p3.coords_[1]) -
         p2.coords_[0] * (p1.coords_[1] * p3.coords_[2] -
                          p1.coords_[2] * p3.coords_[1]) +
         p3.coords_[0] * (p1.coords_[1] * p2.coords_[2] -
                          p1.coords_[2] * p2.coords_[1]);
}

void Point3f::GetCoords(float* dst) const {
  dst[0] = coords_[0];
  dst[1] = coords_[1];
  dst[2] = coords_[2];
}

Point3f& Point3f::operator =(const Point3f& p) {
  coords_[0] = p.coords_[0];
  coords_[1] = p.coords_[1];
  coords_[2] = p.coords_[2];
  return *this;
}

Point3f Point3f::operator *(float val) {
  return Point3f(coords_[0] * val, coords_[1] * val, coords_[2] * val);
}

float Point3f::SqDistanceTo(const Point3f& p) const {
  float dx = coords_[0] - p.coords_[0];
  float dy = coords_[1] - p.coords_[1];
  float dz = coords_[2] - p.coords_[2];
  return dx * dx + dy * dy + dz * dz;
}
