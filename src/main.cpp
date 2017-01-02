#include <float.h>
#include <math.h>

#include <iostream>
#include <vector>

#include <GL/freeglut.h>

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/triangle.hpp"
#include "include/cornell_box.hpp"
#include "include/sphere.hpp"

void display();

unsigned display_width = 500;
unsigned display_height = 500;

Triangle* FindIntersection(const std::vector<Triangle*>& tris,
                           const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection,
                           const Triangle* excluded_tri = 0,
                           float max_distance = FLT_MAX);

// Returns color.
const int kMaxIters = 1;
Point3f Ray(const Point3f& from, const Point3f& dir,
            const std::vector<Triangle*>& tris,
            int iter = 0);

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

  Point3f p2(-1, 1, -2);
  Point3f p4(1, -1, -2);
  Point3f p5(-1, -1, 0);
  Vertex v1(p5 * 0.6f + p4 * 0.2f + p2 * 0.2f, Point3f(0, 0, 1));
  Vertex v2(p5 * 0.1f + p4 * 0.8f + p2 * 0.1f, Point3f(0, 0, 1));
  Vertex v3(p5 * 0.2f + p4 * 0.3f + p2 * 0.5f, Point3f(0, 0, 1));

  std::vector<Triangle*> tris;
  CornellBox::GetTriangles(&tris);
  Sphere::GetTriangles(&tris, Point3f(0.4, 0, -0.7), 0.2);
  tris.push_back(new Triangle(v1, v2, v3, Point3f(1, 0.5, 0)));

  Point3f camera_pos(0, 0, 5);

  uint8_t* canvas = new uint8_t[3 * display_width * display_height];
  uint8_t* offset = canvas;

  float rgb[3];
  for (int y = 0; y < display_height; ++y) {
    float v = (float(y) / display_height) * 2.0f - 1.0f;
    for (int x = 0; x < display_width; ++x) {
      float u = (float(x) / display_width) * 2.0f - 1.0f;
      Point3f color = Ray(camera_pos,
                          Point3f(Point3f(u, v, 0) - camera_pos, true),
                          tris);
      color.GetCoords(rgb);
      offset[0] = 255 * std::min(1.0f, rgb[0]);
      offset[1] = 255 * std::min(1.0f, rgb[1]);
      offset[2] = 255 * std::min(1.0f, rgb[2]);
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

Point3f Ray(const Point3f& from, const Point3f& dir,
            const std::vector<Triangle*>& tris,
            int iter) {
  Point3f light_src = CornellBox::GetLightSrc();
  Point3f intersection(0, 0, 0);
  Point3f result_color(0, 0, 0);

  Triangle* tri = FindIntersection(tris, from, dir, &intersection);
  if (tri) {
    Color tri_color = tri->GetColor();
    result_color = result_color + tri_color.color * tri_color.ambient;

    Point3f light_vec(light_src - intersection, true);
    Triangle* light_tri = FindIntersection(tris, Point3f(intersection),
                                           light_vec,
                                           &intersection, tri,
                                           intersection.SqDistanceTo(light_src));
    if (!light_tri) {
      result_color = result_color + tri_color.color * tri_color.diffuse;
    }
  } else {
    return Point3f(0, 0, 0);
  }
  return result_color;
}
