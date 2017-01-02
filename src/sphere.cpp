#include "include/sphere.hpp"

void Sphere::GetTriangles(std::vector<Triangle*>* scene_triangles,
                          const Point3f& center, float radius, int lod) {
  scene_triangles->reserve(scene_triangles->size() + 8);

  Vertex v1(center + Point3f(radius, 0, 0), Point3f(1, 0, 0));
  Vertex v2(center + Point3f(0, 0, -radius), Point3f(0, 0, -1));
  Vertex v3(center + Point3f(-radius, 0, 0), Point3f(-1, 0, 0));
  Vertex v4(center + Point3f(0, 0, radius), Point3f(0, 0, 1));
  Vertex v5(center + Point3f(0, radius, 0), Point3f(0, 1, 0));
  Vertex v6(center + Point3f(0, -radius, 0), Point3f(0, -1, 0));

  Point3f blue(0, 0, 1);
  scene_triangles->push_back(new Triangle(v1, v2, v5, blue));
  scene_triangles->push_back(new Triangle(v2, v3, v5, blue));
  scene_triangles->push_back(new Triangle(v3, v4, v5, blue));
  scene_triangles->push_back(new Triangle(v4, v1, v5, blue));

  scene_triangles->push_back(new Triangle(v1, v6, v2, blue));
  scene_triangles->push_back(new Triangle(v2, v6, v3, blue));
  scene_triangles->push_back(new Triangle(v3, v6, v4, blue));
  scene_triangles->push_back(new Triangle(v4, v6, v1, blue));
}
