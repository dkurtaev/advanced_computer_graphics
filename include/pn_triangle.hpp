#ifndef INCLUDE_PN_TRIANGLE_HPP_
#define INCLUDE_PN_TRIANGLE_HPP_

#include <vector>

#include "include/vertex.hpp"
#include "include/color.hpp"
#include "include/triangle.hpp"

class PNTriangle {
 public:
  static void GetTriangles(std::vector<Triangle*>* scene_triangles,
                           const Vertex& v1, const Vertex& v2, const Vertex& v3,
                           const Color& color, int lod = 0);

 private:
};

#endif  // INCLUDE_PN_TRIANGLE_HPP_
