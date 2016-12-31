#ifndef INCLUDE_TRIANGLE_HPP_
#define INCLUDE_TRIANGLE_HPP_

#include "include/vertex.hpp"

class Triangle {
 public:
  Triangle(const Vertex& p1, const Vertex& p2, const Vertex& p3);

  void draw() const;

 private:
  float coords_[9];
  float normals_[9];
};

#endif  // INCLUDE_TRIANGLE_HPP_
