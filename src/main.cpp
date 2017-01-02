#include <float.h>

#include <iostream>
#include <vector>

#include <GL/freeglut.h>

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/triangle.hpp"

void display();

unsigned display_width = 500;
unsigned display_height = 500;

Triangle* FindIntersection(const std::vector<Triangle*>& tris,
                           const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection,
                           const Triangle* excluded_tri = 0,
                           float max_distance = FLT_MAX);

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitWindowSize(display_width, display_height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Single view");

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  static const float kCornellBoxSize = 1.0f;
  static const float kCornellBoxZ = -2.0;
  static const float kCornellBoxFront = kCornellBoxZ + 2 * kCornellBoxSize;

  Vertex v1(Point3f(-kCornellBoxSize, -kCornellBoxSize, kCornellBoxZ),
            Point3f(0, 0, 1));
  Vertex v2(Point3f(-kCornellBoxSize, kCornellBoxSize, kCornellBoxZ),
            Point3f(0, 0, 1));
  Vertex v3(Point3f(kCornellBoxSize, kCornellBoxSize, kCornellBoxZ),
            Point3f(0, 0, 1));
  Vertex v4(Point3f(kCornellBoxSize, -kCornellBoxSize, kCornellBoxZ),
            Point3f(0, 0, 1));
  Vertex v5(Point3f(-kCornellBoxSize, -kCornellBoxSize, kCornellBoxFront),
            Point3f(0, 0, 1));
  Vertex v6(Point3f(-kCornellBoxSize, kCornellBoxSize, kCornellBoxFront),
            Point3f(0, 0, 1));
  Vertex v7(Point3f(kCornellBoxSize, kCornellBoxSize, kCornellBoxFront),
            Point3f(0, 0, 1));
  Vertex v8(Point3f(kCornellBoxSize, -kCornellBoxSize, kCornellBoxFront),
            Point3f(0, 0, 1));

  Vertex v9(v5.GetPos() * 0.6f + v4.GetPos() * 0.2f + v2.GetPos() * 0.2f, Point3f(0, 0, 1));
  Vertex v10(v5.GetPos() * 0.1f + v4.GetPos() * 0.8f + v2.GetPos() * 0.1f, Point3f(0, 0, 1));
  Vertex v11(v5.GetPos() * 0.2f + v4.GetPos() * 0.3f + v2.GetPos() * 0.5f, Point3f(0, 0, 1));

  std::vector<Triangle*> tris(11);
  tris[0] = new Triangle(v1, v3, v2, Point3f(1, 1, 1));
  tris[1] = new Triangle(v1, v4, v3, Point3f(1, 1, 1));
  tris[2] = new Triangle(v5, v2, v6, Point3f(1, 0, 0));
  tris[3] = new Triangle(v5, v1, v2, Point3f(1, 0, 0));
  tris[4] = new Triangle(v4, v7, v3, Point3f(0, 1, 0));
  tris[5] = new Triangle(v4, v8, v7, Point3f(0, 1, 0));
  tris[6] = new Triangle(v6, v2, v3, Point3f(0, 1, 1));
  tris[7] = new Triangle(v6, v3, v7, Point3f(0, 1, 1));
  tris[8] = new Triangle(v5, v4, v1, Point3f(0, 1, 1));
  tris[9] = new Triangle(v5, v8, v4, Point3f(0, 1, 1));
  tris[10] = new Triangle(v9,v10,v11, Point3f(1, 0.5, 0));

  Point3f camera_pos(0, 0, 5);
  Point3f light_src(0, kCornellBoxSize - 0.01, kCornellBoxSize + kCornellBoxZ);
  Point3f intersection(0, 0, 0);

  uint8_t* canvas = new uint8_t[3 * display_width * display_height];
  uint8_t* offset = canvas;

  for (int y = 0; y < display_height; ++y) {
    float v = (float(y) / display_height) * 2.0f - 1.0f;
    for (int x = 0; x < display_width; ++x) {
      float u = (float(x) / display_width) * 2.0f - 1.0f;
      Point3f ray(Point3f(u, v, 0) - camera_pos, true);

      Triangle* tri = FindIntersection(tris, camera_pos, ray, &intersection);
      if (tri) {
        Point3f light_vec(light_src - intersection, true);
        Triangle* light_tri = FindIntersection(tris, Point3f(intersection),
                                               light_vec,
                                               &intersection, tri,
                                               intersection.SqDistanceTo(light_src));
        if (!light_tri) {
          tri->GetColor(offset, offset + 1, offset + 2);
        } else {
          offset[0] = 0;
          offset[1] = 0;
          offset[2] = 0;
        }
      } else {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = 0;
      }
      offset += 3;
    }
  }
  glDrawPixels(display_width, display_height, GL_RGB, GL_UNSIGNED_BYTE, canvas);
  glutSwapBuffers();
}

Triangle* FindIntersection(const std::vector<Triangle*>& tris,
                           const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection,
                           const Triangle* excluded_tri,
                           float max_distance) {
  Triangle* nearest_tri = 0;
  float nearest_distance = FLT_MAX;
  Point3f nearest_tri_intersection(0, 0, 0);
  const unsigned n_tris = tris.size();

  for (int i = 0; i < n_tris; ++i) {
    if (tris[i] == excluded_tri) {
      continue;
    }
    bool is_int = false;
    if (tris[i]->IsIntersects(ray_point, ray_dir, intersection)) {
      float distance = intersection->SqDistanceTo(ray_point);
      if (distance < nearest_distance && distance < max_distance) {
        nearest_tri = tris[i];
        nearest_distance = distance;
        nearest_tri_intersection = *intersection;
      }
    }
  }
  *intersection = nearest_tri_intersection;
  return nearest_tri;
}
