#ifndef INCLUDE_SPHERE_HPP_
#define INCLUDE_SPHERE_HPP_

#include <vector>

#include "include/triangle.hpp"
#include "include/point3f.hpp"
#include "include/bounding_box.hpp"

class Sphere {
 public:
  Sphere(const Point3f& center, const Point3f& direction, float radius,
         const Color& color, int lod = 0);

  ~Sphere();

  void Move(float min_x, float max_x, float min_y, float max_y,
            float min_z, float max_z);

  bool IsIntersects(const Point3f& ray_point, const Point3f& ray) const;

  void GetTriangles(std::vector<Triangle*>* tris);

 private:
  Point3f center_;
  Point3f direction_;
  float radius_;
  std::vector<Triangle*> triangles_;
  BoundingBox* bbox_;
};

#endif  // INCLUDE_SPHERE_HPP_
