#include <float.h>
#include <math.h>
#include <sys/time.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include <GL/freeglut.h>

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/triangle.hpp"
#include "include/cornell_box.hpp"
#include "include/sphere.hpp"

void display();

const unsigned kNumSpheres = 10;
const unsigned kDisplayWidth = 256;
const unsigned kDisplayHeight = 256;
const int kMaxRayIters = 3;
unsigned level_of_detalization = 0;
Point3f camera_pos(0, 0, 5);

std::vector<Triangle*> cornell_box_tris;
unsigned display_time = 0;
unsigned num_dispay_calls = 0;
std::vector<Sphere*> spheres(kNumSpheres);

Triangle* FindIntersection(const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection, float* u, float* v,
                           float max_distance = FLT_MAX);

// Returns color.
Point3f Ray(const Point3f& from, const Point3f& dir, int iter = 0);
void SpecialKeyPressed(int key, int x, int y);
int TimeFrom(const timeval& t);
float Rand();

int main(int argc, char** argv) {
  CornellBox::GetTriangles(&cornell_box_tris);

  // Setup spheres.
  float radius = 0.2f, speed = 0.05f;
  float center_x = CornellBox::kLeft + 2 * radius;
  float center_y = CornellBox::kBottom + 2 * radius;
  float center_z = CornellBox::kBack + 2 * radius;
  for (int i = 0, n = spheres.size(); i < n; ++i) {
    Point3f dir((float)rand() / RAND_MAX,
                (float)rand() / RAND_MAX,
                (float)rand() / RAND_MAX, true);
    Point3f color((float)rand() / RAND_MAX,
                  (float)rand() / RAND_MAX,
                  (float)rand() / RAND_MAX);
    spheres[i] = new Sphere(Point3f(center_x, center_y, center_z), dir  * speed,
                            radius, color, level_of_detalization);
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
  glutInitWindowSize(kDisplayWidth, kDisplayHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Single view");
  glutSpecialFunc(SpecialKeyPressed);
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
  std::cout << "Avg. display time: " << (float)display_time / num_dispay_calls
            << "ms." << std::endl;
  return 0;
}

void display() {
  static const unsigned kMoveDelay = 40;

  static timeval last_move;
  static uint8_t* canvas = new uint8_t[3 * kDisplayWidth * kDisplayHeight];

  if (TimeFrom(last_move) > kMoveDelay) {
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
  timeval start;
  gettimeofday(&start, 0);
  for (int y = 0; y < kDisplayHeight; ++y) {
    float v = (float(y) / kDisplayHeight) * 2.0f - 1.0f;
    for (int x = 0; x < kDisplayWidth; ++x) {
      float u = (float(x) / kDisplayWidth) * 2.0f - 1.0f;

      Point3f color = Ray(camera_pos,
                          Point3f(Point3f(u, v, 0) - camera_pos, true));

      color.GetCoords(rgb);
      offset[0] = 255 * std::min(1.0f, rgb[0]);
      offset[1] = 255 * std::min(1.0f, rgb[1]);
      offset[2] = 255 * std::min(1.0f, rgb[2]);
      offset += 3;
    }
  }
  glDrawPixels(kDisplayWidth, kDisplayHeight, GL_RGB, GL_UNSIGNED_BYTE, canvas);
  display_time += TimeFrom(start);
  num_dispay_calls += 1;
  glutSwapBuffers();
}

bool comparator(const std::pair<float, Sphere*>& first,
                const std::pair<float, Sphere*>& second) {
  return first.first < second.first;
}

Triangle* FindIntersection(const Point3f& ray_point, const Point3f& ray_dir,
                           Point3f* intersection, float* u, float* v,
                           float max_distance) {
  Triangle* nearest_tri = 0;

  // Collect spheres which bounding boxes' intersects by ray.
  float distance;
  std::vector<std::pair<float, Sphere*> > spheres_on_ray;
  for (int i = 0, n = spheres.size(); i < n; ++i) {
    if (spheres[i]->IsIntersects(ray_point, ray_dir, &distance)) {
      spheres_on_ray.push_back(std::pair<float, Sphere*>(distance, spheres[i]));
    }
  }
  std::sort(spheres_on_ray.begin(), spheres_on_ray.end(), comparator);

  if (!spheres_on_ray.empty()) {
    // Ray intersects one of spheres.
    for (int i = 0, n = spheres_on_ray.size(); i < n; ++i) {
      nearest_tri = spheres_on_ray[i].second->FindIntersection(
        ray_point, ray_dir, intersection, u, v, max_distance);
      if (nearest_tri) {
        break;
      }
    }
  }

  if (!nearest_tri) {
    // Ray intersects Cornell box.
    for (int i = 0, n = cornell_box_tris.size(); i < n; ++i) {
      Triangle* tri = cornell_box_tris[i];
      if (tri->IsIntersects(ray_point, ray_dir, intersection, u, v,
                            &distance)) {
        if (distance < max_distance) {
          nearest_tri = tri;
          break;
        }
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
    if (tri_color.reflection > 0 && iter + 1 < kMaxRayIters) {

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

void SpecialKeyPressed(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_PAGE_UP:
      if (level_of_detalization < 5) {
        level_of_detalization += 1;
        for (int i = 0, n = spheres.size(); i < n; ++i) {
          Sphere* sphere = spheres[i];
          spheres[i] = new Sphere(*sphere, level_of_detalization);
          delete sphere;
        }
        std::cout << "Level of detalization: " << level_of_detalization
                  << std::endl;
      }
      break;
    case GLUT_KEY_PAGE_DOWN:
      if (level_of_detalization > 0) {
        level_of_detalization -= 1;
        for (int i = 0, n = spheres.size(); i < n; ++i) {
          Sphere* sphere = spheres[i];
          spheres[i] = new Sphere(*sphere, level_of_detalization);
          delete sphere;
        }
        std::cout << "Level of detalization: " << level_of_detalization
                  << std::endl;
      }
      break;
    default: break;
  }
}

int TimeFrom(const timeval& t) {
  timeval now;
  gettimeofday(&now, 0);
  return (now.tv_sec - t.tv_sec) * 1e+3 + (now.tv_usec - t.tv_usec) * 1e-3;
}
