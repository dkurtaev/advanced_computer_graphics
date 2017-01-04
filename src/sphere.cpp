#include "include/sphere.hpp"

#include <math.h>

#include <algorithm>

#include "include/pn_triangle.hpp"

Sphere::Sphere(const Point3f& center, const Point3f& direction, float radius,
               const Color& color, int lod)
    : center_(center), direction_(direction), radius_(radius), color_(color) {
  Init(lod);
}

Sphere::Sphere(const Sphere& sphere, int lod)
    : center_(sphere.center_), direction_(sphere.direction_),
      radius_(sphere.radius_), color_(sphere.color_) {
  Init(lod);
}

void Sphere::Init(int lod) {
  Vertex v1(center_ + Point3f(radius_, 0, 0), Point3f(1, 0, 0));
  Vertex v2(center_ + Point3f(0, 0, -radius_), Point3f(0, 0, -1));
  Vertex v3(center_ + Point3f(-radius_, 0, 0), Point3f(-1, 0, 0));
  Vertex v4(center_ + Point3f(0, 0, radius_), Point3f(0, 0, 1));
  Vertex v5(center_ + Point3f(0, radius_, 0), Point3f(0, 1, 0));
  Vertex v6(center_ + Point3f(0, -radius_, 0), Point3f(0, -1, 0));

  pn_tris_.resize(8);
  pn_tris_[0] = new PNTriangle(v1, v2, v5, color_, lod);
  pn_tris_[1] = new PNTriangle(v2, v3, v5, color_, lod);
  pn_tris_[2] = new PNTriangle(v3, v4, v5, color_, lod);
  pn_tris_[3] = new PNTriangle(v4, v1, v5, color_, lod);

  pn_tris_[4] = new PNTriangle(v1, v6, v2, color_, lod);
  pn_tris_[5] = new PNTriangle(v2, v6, v3, color_, lod);
  pn_tris_[6] = new PNTriangle(v3, v6, v4, color_, lod);
  pn_tris_[7] = new PNTriangle(v4, v6, v1, color_, lod);

  Point3f bbox_size(2.0f * radius_, 2.0f * radius_, 2.0f * radius_);
  bbox_ = new BoundingBox(center_ - bbox_size * 0.5f, bbox_size);
}


Sphere::~Sphere() {
  for (int i = 0, n = pn_tris_.size(); i < n; ++i) {
    delete pn_tris_[i];
  }
  delete bbox_;
}

void Sphere::Move(float min_x, float max_x, float min_y, float max_y,
                  float min_z, float max_z,
                  const std::vector<Sphere*>& spheres) {
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

  for (int i = 0, n = spheres.size(); i < n; ++i) {
    if (spheres[i] != this && IsIntersects(*spheres[i])) {
      Point3f normal(center_ - spheres[i]->center_, true);
      direction_ = direction_ - normal * 2.0f *
                   Point3f::Dot(normal, direction_);
    }
  }

  const int n_tris = pn_tris_.size();
  for (int i = 0; i < n_tris; ++i) {
    pn_tris_[i]->Move(direction_);
  }
  center_ += direction_;
  *bbox_ += direction_;
}

bool Sphere::IsIntersects(const Sphere& sphere) {
  return center_.SqDistanceTo(sphere.center_) <=
         radius_ * radius_ + sphere.radius_ * sphere.radius_ +
         2.0f * radius_ * sphere.radius_;
}

bool Sphere::IsIntersects(const Point3f& ray_point, const Point3f& ray,
                          float* distance) const {
  float b = Point3f::Dot(ray_point - center_, ray);
  float c = ray_point.SqDistanceTo(center_) - radius_ * radius_;
  float determinant = b * b - c;
  if (determinant >= 0) {
    if (b < 0 || determinant > 4.0f * b * b) {
      *distance = sqrt(determinant) - 2.0f * b;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool comparator(const std::pair<float, PNTriangle*>& first,
                const std::pair<float, PNTriangle*>& second) {
  return first.first < second.first;
}

Triangle* Sphere::FindIntersection(const Point3f& ray_point, const Point3f& ray,
                                   Point3f* intersection, float* u, float* v,
                                   float max_distance) {
  std::vector<std::pair<float, PNTriangle*> > octants_on_ray;

  std::pair<float, PNTriangle*> p;
  for (int i = 0, n = pn_tris_.size(); i < n; ++i) {
    if (pn_tris_[i]->IsIntersects(ray_point, ray, &p.first)) {
      p.second = pn_tris_[i];
      octants_on_ray.push_back(p);
    }
  }
  std::sort(octants_on_ray.begin(), octants_on_ray.end(), comparator);

  Triangle* nearest_tri = 0;
  int n_octants = octants_on_ray.size();
  n_octants = std::min(4, n_octants);
  for (int i = 0; i < n_octants && !nearest_tri; ++i) {
    nearest_tri = octants_on_ray[i].second->FindIntersection(
      ray_point, ray, intersection, u, v, max_distance);
  }
  return nearest_tri;
}

void Sphere::SetColor(const Color& color) {
  for (int i = 0, n = pn_tris_.size(); i < n; ++i) {
    pn_tris_[i]->SetColor(color);
  }
  color_ = color;
}
