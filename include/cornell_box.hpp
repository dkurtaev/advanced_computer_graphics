#ifndef INCLUDE_CORNELL_BOX_HPP_
#define INCLUDE_CORNELL_BOX_HPP_

#include <vector>

#include "include/point3f.hpp"
#include "include/triangle.hpp"

class CornellBox {
 public:
  static void GetTriangles(std::vector<Triangle*>* scene_triangles);

  static Point3f GetLightSrc();

  static const float kBack = -2.0f;
  static const float kFront = 0.0f;
  static const float kLeft = -1.0f;
  static const float kRight = 1.0f;
  static const float kTop = 1.0f;
  static const float kBottom = -1.0f;
};
#endif  // INCLUDE_CORNELL_BOX_HPP_
