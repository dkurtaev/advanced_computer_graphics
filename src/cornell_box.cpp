#include "include/cornell_box.hpp"

#include "include/point3f.hpp"
#include "include/vertex.hpp"

void CornellBox::GetTriangles(std::vector<Triangle*>* scene_triangles) {
  scene_triangles->reserve(scene_triangles->size() + 10);
  Point3f white(1, 1, 1);

  // Back side.
  {
    Point3f normal(0, 0, 1);
    Vertex v1(Point3f(-kSize, -kSize, kBack), normal);
    Vertex v2(Point3f(-kSize, kSize, kBack), normal);
    Vertex v3(Point3f(kSize, kSize, kBack), normal);
    Vertex v4(Point3f(kSize, -kSize, kBack), normal);
    scene_triangles->push_back(new Triangle(v1, v3, v2, white));
    scene_triangles->push_back(new Triangle(v1, v4, v3, white));
  }

  // Left side.
  {
    Point3f red(1, 0, 0);
    Point3f normal(1, 0, 0);
    Vertex v1(Point3f(-kSize, -kSize, kFront), normal);
    Vertex v2(Point3f(-kSize, kSize, kFront), normal);
    Vertex v3(Point3f(-kSize, kSize, kBack), normal);
    Vertex v4(Point3f(-kSize, -kSize, kBack), normal);
    scene_triangles->push_back(new Triangle(v1, v3, v2, red));
    scene_triangles->push_back(new Triangle(v1, v4, v3, red));
  }

  // Right side.
  {
    Point3f green(0, 1, 0);
    Point3f normal(-1, 0, 0);
    Vertex v1(Point3f(kSize, -kSize, kBack), normal);
    Vertex v2(Point3f(kSize, kSize, kBack), normal);
    Vertex v3(Point3f(kSize, kSize, kFront), normal);
    Vertex v4(Point3f(kSize, -kSize, kFront), normal);
    scene_triangles->push_back(new Triangle(v1, v3, v2, green));
    scene_triangles->push_back(new Triangle(v1, v4, v3, green));
  }

  // Top side.
  {
    Point3f normal(0, -1, 0);
    Vertex v1(Point3f(-kSize, kSize, kBack), normal);
    Vertex v2(Point3f(-kSize, kSize, kFront), normal);
    Vertex v3(Point3f(kSize, kSize, kFront), normal);
    Vertex v4(Point3f(kSize, kSize, kBack), normal);
    scene_triangles->push_back(new Triangle(v1, v3, v2, white));
    scene_triangles->push_back(new Triangle(v1, v4, v3, white));
  }

  // Bottom side.
  {
    Point3f normal(0, 1, 0);
    Vertex v1(Point3f(-kSize, -kSize, kFront), normal);
    Vertex v2(Point3f(-kSize, -kSize, kBack), normal);
    Vertex v3(Point3f(kSize, -kSize, kBack), normal);
    Vertex v4(Point3f(kSize, -kSize, kFront), normal);
    scene_triangles->push_back(new Triangle(v1, v3, v2, white));
    scene_triangles->push_back(new Triangle(v1, v4, v3, white));
  }
}

Point3f CornellBox::GetLightSrc() {
  static const float kDelta = 1e-3;
  return Point3f(0, kSize - kDelta, kBack + kSize);
}
