#ifndef INCLUDE_TRIANGLE_HPP_
#define INCLUDE_TRIANGLE_HPP_

#include "include/vertex.hpp"

class Triangle {
 public:
  Triangle(const Vertex& p1, const Vertex& p2, const Vertex& p3, int lod = 0);

  ~Triangle();

  void draw(unsigned program) const;

 private:
  float base_coords_[9];
  float base_normals_[9];
  unsigned bary_coords_vbo_;
  unsigned indices_vbo_;
  unsigned num_triangles_;
};

#endif  // INCLUDE_TRIANGLE_HPP_
