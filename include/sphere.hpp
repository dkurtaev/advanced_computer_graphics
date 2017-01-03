#ifndef INCLUDE_SPHERE_HPP_
#define INCLUDE_SPHERE_HPP_

#include <vector>

#include "include/triangle.hpp"
#include "include/point3f.hpp"

class Sphere {
 public:
  static void GetTriangles(std::vector<Triangle*>* scene_triangles,
                           const Point3f& center, float radius, int lod = 0);

 private:
};

#endif  // INCLUDE_SPHERE_HPP_
