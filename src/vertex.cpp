#include "include/vertex.hpp"

Vertex::Vertex(const Point3f& pos, const Point3f& normal)
  : pos_(pos), normal_(normal, true) {}

Point3f Vertex::operator +(const Vertex& v) const {
  return pos_ + v.pos_;
}

Point3f Vertex::operator -(const Vertex& v) const {
  return pos_ - v.pos_;
}

Point3f Vertex::operator +(const Point3f& p) const {
  return pos_ + p;
}

Point3f Vertex::operator -(const Point3f& p) const {
  return pos_ - p;
}

Point3f Vertex::GetPos() const {
  return pos_;
}
