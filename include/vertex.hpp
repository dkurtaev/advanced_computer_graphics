#ifndef INCLUDE_VERTEX_HPP_
#define INCLUDE_VERTEX_HPP_

#include "include/point3f.hpp"

class Vertex {
 public:
  Vertex(const Point3f& pos, const Point3f& normal);

  Point3f operator +(const Vertex& v) const;

  Point3f operator -(const Vertex& v) const;

  Point3f operator +(const Point3f& p) const;

  Point3f operator -(const Point3f& p) const;

  Vertex& operator +=(const Point3f& p);

  Point3f GetPos() const;

  Point3f GetNormal() const;

 private:
  Point3f pos_;
  Point3f normal_;
};

#endif  // INCLUDE_VERTEX_HPP_
