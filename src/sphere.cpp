#include "include/sphere.hpp"

#include "include/pn_triangle.hpp"

Sphere::Sphere(std::vector<Triangle*>* scene_triangles, const Point3f& center,
               const Point3f& direction, float radius, const Color& color,
               int lod)
    : center_(center), direction_(direction), radius_(radius) {
  Vertex v1(center + Point3f(radius, 0, 0), Point3f(1, 0, 0));
  Vertex v2(center + Point3f(0, 0, -radius), Point3f(0, 0, -1));
  Vertex v3(center + Point3f(-radius, 0, 0), Point3f(-1, 0, 0));
  Vertex v4(center + Point3f(0, 0, radius), Point3f(0, 0, 1));
  Vertex v5(center + Point3f(0, radius, 0), Point3f(0, 1, 0));
  Vertex v6(center + Point3f(0, -radius, 0), Point3f(0, -1, 0));

  PNTriangle::GetTriangles(&triangles_, v1, v2, v5, color, lod);
  PNTriangle::GetTriangles(&triangles_, v2, v3, v5, color, lod);
  PNTriangle::GetTriangles(&triangles_, v3, v4, v5, color, lod);
  PNTriangle::GetTriangles(&triangles_, v4, v1, v5, color, lod);

  PNTriangle::GetTriangles(&triangles_, v1, v6, v2, color, lod);
  PNTriangle::GetTriangles(&triangles_, v2, v6, v3, color, lod);
  PNTriangle::GetTriangles(&triangles_, v3, v6, v4, color, lod);
  PNTriangle::GetTriangles(&triangles_, v4, v6, v1, color, lod);

  scene_triangles->reserve(scene_triangles->size() + triangles_.size());
  scene_triangles->insert(scene_triangles->end(), triangles_.begin(),
                          triangles_.end());
}

void Sphere::Move(float min_x, float max_x, float min_y, float max_y,
                  float min_z, float max_z) {
  float center[3], direction[3];
  center_.GetCoords(center);
  direction_.GetCoords(direction);
  if (center[0] - radius_ + direction[0] < min_x ||
      center[0] + radius_ + direction[0] > max_x) {
    direction_ = Point3f(-direction[0], direction[1], direction[2]);
  }
  if (center[1] - radius_ + direction[1] < min_y ||
      center[1] + radius_ + direction[1] > max_y) {
    direction_ = Point3f(direction[0], -direction[1], direction[2]);
  }
  if (center[2] - radius_ + direction[2] < min_z ||
      center[2] + radius_ + direction[2] > max_z) {
    direction_ = Point3f(direction[0], direction[1], -direction[2]);
  }

  const int n_tris = triangles_.size();
  for (int i = 0; i < n_tris; ++i) {
    triangles_[i]->Move(direction_);
  }
  center_ += direction_;
}
