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
#include "include/fps.hpp"

void display();

const unsigned kMoveDelay = 40;
const unsigned kNumSpheres = 10;
timeval last_move;
unsigned display_width = 500;
unsigned display_height = 500;
std::vector<Triangle*> cornell_box_tris;
Point3f camera_pos(0, 0, 5);
uint8_t* canvas;

int max_processed_tris = 0;
int max_spheres_on_ray = 0;
int nearest_processed_sphere = kNumSpheres;
int farest_processed_sphere = 0;
int max_tris_from_sphere = 0;

std::vector<Sphere*> spheres(kNumSpheres);

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
                            radius, color, 0);
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
  std::cout << "Max. processed triangles: " << max_processed_tris << std::endl;
  std::cout << "Max. spheres on ray: " << max_spheres_on_ray << std::endl;
  std::cout << "Nearest proc. sphere: " << nearest_processed_sphere << std::endl;
  std::cout << "Farest proc. sphere: " << farest_processed_sphere << std::endl;
  std::cout << "Max. tris from sphere sphere: " << max_tris_from_sphere << std::endl;

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

  int processed_tris = 0;
  if (!spheres_on_ray.empty()) {
    // Ray intersects one of spheres.
    for (int i = 0, n = spheres_on_ray.size(); i < n; ++i) {
      int num_tris_on_sphere = 0;

      nearest_tri = spheres_on_ray[i].second->FindIntersection(
        ray_point, ray_dir, intersection, u, v, max_distance,
        &num_tris_on_sphere
      );

      processed_tris += num_tris_on_sphere;

      if (num_tris_on_sphere > max_tris_from_sphere) {
        max_tris_from_sphere = num_tris_on_sphere;
      }

      if (nearest_tri) {
        if (i + 1 < nearest_processed_sphere) {
          nearest_processed_sphere = i + 1;
        }
        if (i + 1 > farest_processed_sphere) {
          farest_processed_sphere = i + 1;
        }
        break;
      }
    }
    if (spheres_on_ray.size() > max_spheres_on_ray) {
      max_spheres_on_ray = spheres_on_ray.size();
    }
  }

  if (!nearest_tri) {
    // Ray intersects Cornell box.
    for (int i = 0, n = cornell_box_tris.size(); i < n; ++i) {
      Triangle* tri = cornell_box_tris[i];
      processed_tris += 1;
      if (tri->IsIntersects(ray_point, ray_dir, intersection, u, v,
                            &distance)) {
        if (distance < max_distance) {
          nearest_tri = tri;
          break;
        }
      }
    }
  }
  if (processed_tris > max_processed_tris) {
    max_processed_tris = processed_tris;
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
