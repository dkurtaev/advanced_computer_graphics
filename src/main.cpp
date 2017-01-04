#include <float.h>
#include <math.h>
#include <sys/time.h>

#include <iostream>
#include <vector>

#include <GL/freeglut.h>

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/triangle.hpp"
#include "include/cornell_box.hpp"
#include "include/sphere.hpp"
#include "include/fps.hpp"

void display();

const unsigned kMoveDelay = 40;
timeval last_move;
unsigned display_width = 300;
unsigned display_height = 300;
std::vector<Triangle*> cornell_box_tris;
Point3f camera_pos(0, 0, 5);
uint8_t* canvas;
int max_processing_tris = 0;

std::vector<Sphere*> spheres(10);

Triangle* FindIntersection(const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection, float* u, float* v,
                           float max_distance = FLT_MAX);

// Returns color.
const int kMaxIters = 3;
Point3f Ray(const Point3f& from, const Point3f& dir, int iter = 0);

int main(int argc, char** argv) {
  gettimeofday(&last_move, 0);
  canvas = new uint8_t[3 * display_width * display_height];
  CornellBox::GetTriangles(&cornell_box_tris);

  // Setup spheres.
  float radius = 0.2f, speed = 0.05f;
  float center_x = CornellBox::kLeft + 2 * radius;
  float center_y = CornellBox::kBottom + 2 * radius;
  float center_z = CornellBox::kBack + 2 * radius;
  for (int i = 0, n = spheres.size(); i < n; ++i) {
    Point3f dir((float)rand() / RAND_MAX, (float)rand() / RAND_MAX,
                (float)rand() / RAND_MAX, true);
    Point3f color((float)rand() / RAND_MAX,
                  (float)rand() / RAND_MAX,
                  (float)rand() / RAND_MAX);
    spheres[i] = new Sphere(Point3f(center_x, center_y, center_z), dir  * speed,
                            radius, color, 3);
    center_x += 3 * radius;
    if (center_x + radius >= CornellBox::kRight) {
      center_x = CornellBox::kLeft + 2 * radius;
      center_z += 3 * radius;
      if (center_z + radius >= CornellBox::kFront) {
        center_y += 3 * radius;
        center_x = CornellBox::kLeft + 2 * radius;
        center_z = CornellBox::kBack + 2 * radius;
      }
    }
  }

  glutInit(&argc, argv);
  glutInitWindowSize(display_width, display_height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Single view");
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

  glutDisplayFunc(display);

  glutIdleFunc(display);

  glutMainLoop();

  for (int i = 0, n = spheres.size(); i < n; ++i) {
    delete spheres[i];
  }
  for (int i = 0, n = cornell_box_tris.size(); i < n; ++i) {
    delete cornell_box_tris[i];
  }
  std::cout << max_processing_tris << std::endl;

  return 0;
}

void display() {
  if (FPS::TimeFrom(last_move) > kMoveDelay) {
    for (int i = 0, n = spheres.size(); i < n; ++i) {
      spheres[i]->Move(CornellBox::kLeft, CornellBox::kRight,
                       CornellBox::kBottom, CornellBox::kTop,
                       CornellBox::kBack, CornellBox::kFront, spheres);
    }
    gettimeofday(&last_move, 0);
  }
  glClear(GL_COLOR_BUFFER_BIT);

  uint8_t* offset = canvas;
  float rgb[3];
  for (int y = 0; y < display_height; ++y) {
    float v = (float(y) / display_height) * 2.0f - 1.0f;
    for (int x = 0; x < display_width; ++x) {
      float u = (float(x) / display_width) * 2.0f - 1.0f;

      Point3f color = Ray(camera_pos,
                          Point3f(Point3f(u, v, 0) - camera_pos, true));

      color.GetCoords(rgb);
      offset[0] = 255 * std::min(1.0f, rgb[0]);
      offset[1] = 255 * std::min(1.0f, rgb[1]);
      offset[2] = 255 * std::min(1.0f, rgb[2]);
      offset += 3;
    }
  }
  glDrawPixels(display_width, display_height, GL_RGB, GL_UNSIGNED_BYTE, canvas);

  FPS::Stack();
  std::cout << "fps: " << FPS::Get() << std::endl;
  glutSwapBuffers();
}

Triangle* FindIntersection(const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection, float* u, float* v,
                           float max_distance) {
  static const float kMinDistance = 1e-2f;
  Triangle* nearest_tri = 0;
  float nearest_distance = FLT_MAX;

  std::vector<Triangle*> tris(cornell_box_tris);
  for (int i = 0, n = spheres.size(); i < n; ++i) {
    spheres[i]->GetTriangles(ray_point, ray_dir, &tris);
  }
  if (tris.size() > max_processing_tris) {
    max_processing_tris = tris.size();
  }

  Point3f tmp_intersection(0, 0, 0);
  float tmp_u, tmp_v;
  for (int i = 0, n = tris.size(); i < n; ++i) {
    if (tris[i]->IsIntersects(ray_point, ray_dir, &tmp_intersection,
                              &tmp_u, &tmp_v)) {
      float distance = tmp_intersection.SqDistanceTo(ray_point);
      if (distance < nearest_distance &&
          kMinDistance < distance && distance < max_distance) {
        nearest_tri = tris[i];
        nearest_distance = distance;
        *intersection = tmp_intersection;
        *u = tmp_u;
        *v = tmp_v;
      }
    }
  }
  return nearest_tri;
}

Point3f Ray(const Point3f& from, const Point3f& dir, int iter) {
  Point3f light_src = CornellBox::GetLightSrc();
  Point3f intersection(0, 0, 0);
  Point3f result_color(0, 0, 0);

  float u, v;
  Triangle* tri = FindIntersection(from, dir, &intersection, &u, &v);
  if (tri) {
    Point3f normal = tri->GetNormal(u, v);

    // Ambient color.
    Color tri_color = tri->GetColor();
    result_color += tri_color.color * tri_color.ambient;

    // Reflected ray.
    if (tri_color.reflection > 0 && iter + 1 < kMaxIters) {

      Point3f reflected(dir - normal * 2.0f * Point3f::Dot(normal, dir), true);
      result_color += Ray(Point3f(intersection), reflected, iter + 1) *
                      tri_color.reflection;
    }

    // Diffuse component.
    float distance_to_light_src = intersection.SqDistanceTo(light_src);
    Point3f light_vec(light_src - intersection, true);
    Triangle* light_tri = FindIntersection(Point3f(intersection),
                                           light_vec,
                                           &intersection, &u, &v,
                                           distance_to_light_src);
    if (!light_tri) {
      float lum = (Point3f::Dot(normal, light_vec) + 2.0f) * 0.33f;
      lum *= sqrt(std::max(0.0f, 1.0f - distance_to_light_src / 6.0f));
      result_color += tri_color.color * tri_color.diffuse * lum;
    }
  }
  return result_color;
}
