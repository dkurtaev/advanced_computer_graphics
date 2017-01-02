#ifndef INCLUDE_COLOR_HPP_
#define INCLUDE_COLOR_HPP_

#include "include/point3f.hpp"

struct Color {
  Color(const Point3f& color, float ambient = 0.2, float diffuse = 0.8,
        float reflection = 0)
  : color(color), ambient(ambient), diffuse(diffuse), reflection(reflection) {}

  Point3f color;
  float ambient;
  float diffuse;
  float reflection;
};

#endif  // INCLUDE_COLOR_HPP_
