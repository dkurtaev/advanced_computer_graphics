#ifndef INCLUDE_SPHERE_HPP_
#define INCLUDE_SPHERE_HPP_

#include "include/triangle.hpp"

class Sphere {
 public:
  Sphere(float radius, int lod = 0);

  ~Sphere();

  void draw() const;

 private:
  Triangle* tris_[8];
};

#endif  // INCLUDE_SPHERE_HPP_
