#include "include/pn_triangle.hpp"

#include <vector>

#include "include/point3f.hpp"

PNTriangle::PNTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
                       const Color& color, int lod)
    : bbox_(v1, v2, v3) {
  // 0 +            . p1
  //   |           . .
  //   |          .   .
  //   |         *--p--*
  //   |        .       .
  // 1 +    p2 . . . . . . p3
  //   |       0         1
  // U v       +---------+-----> V
  //
  // p = V * [U*p2 + (1-U)*p1] + (1-V) * [U*p3 + (1-U)*p1] =
  //     (1-u)*p1 + u*v*p2 + u*(1-v)*p3
  //
  const int num_vertices = (lod + 3) * (lod + 2) / 2;
  const int num_triangles = (lod + 1) * (lod + 1);

  tris_.reserve(num_triangles);

  Point3f p1 = v1.GetPos();
  Point3f p2 = v2.GetPos();
  Point3f p3 = v3.GetPos();
  Point3f n1 = v1.GetNormal();
  Point3f n2 = v2.GetNormal();
  Point3f n3 = v3.GetNormal();

  static const float kRatio = 1.0f / 3.0f;
  Point3f b210 = (2.0f * p1 + p2 - Point3f::Dot(p2 - p1, n1) * n1) * kRatio;
  Point3f b120 = (2.0f * p2 + p1 - Point3f::Dot(p1 - p2, n2) * n2) * kRatio;
  Point3f b021 = (2.0f * p2 + p3 - Point3f::Dot(p3 - p2, n2) * n2) * kRatio;
  Point3f b012 = (2.0f * p3 + p2 - Point3f::Dot(p2 - p3, n3) * n3) * kRatio;
  Point3f b102 = (2.0f * p3 + p1 - Point3f::Dot(p1 - p3, n3) * n3) * kRatio;
  Point3f b201 = (2.0f * p1 + p3 - Point3f::Dot(p3 - p1, n1) * n1) * kRatio;
  Point3f E = 0.5f * (b210 + b120 + b021 + b012 + b102 + b201) * kRatio;
  Point3f b111 = E + 0.5f * (E - (p1 + p2 + p3) * kRatio);

  std::vector<Vertex*> vertices(num_vertices);

  int idx = 0;
  float x = 0;
  float dx = 1.0 / (lod + 1);
  for (int i = 0; i < lod + 2; ++i, x += dx) {
    float y = 0;
    float dy = 1.0f / i;
    for (int j = 0; j <= i; ++j, y += dy) {
      float u = 1 - x;
      float v = x * (1 - y);
      float w = 1.0f - u - v;
      Point3f pos(p1 * w * w * w +
                  p2 * u * u * u +
                  p3 * v * v * v +
                  b210 * 3.0f * w * w * u +
                  b120 * 3.0f * w * u * u +
                  b201 * 3.0f * w * w * v +
                  b021 * 3.0f * u * u * v +
                  b102 * 3.0f * w * v * v +
                  b012 * 3.0f * u * v * v +
                  b111 * 6.0f * w * u * v);
      Point3f normal(n1 * w * w + n2 * u * u + n3 * v * v +
                     Point3f(0.5f * (n1 + n2), true) * w * u +
                     Point3f(0.5f * (n2 + n3), true) * u * v +
                     Point3f(0.5f * (n3 + n1), true) * w * v,
                     true);
      vertices[idx] = new Vertex(pos, normal);
      idx += 1;
    }
  }

  idx = 1;
  for (int i = 0; i <= lod; ++i, idx += 2) {
    for (int j = 0; j < i; ++j, ++idx) {
      tris_.push_back(new Triangle(*vertices[idx],
                                   *vertices[idx + 1],
                                   *vertices[idx - i - 1],
                                   color));
      tris_.push_back(new Triangle(*vertices[idx + 1],
                                   *vertices[idx - i],
                                   *vertices[idx - i - 1],
                                   color));
    }
    tris_.push_back(new Triangle(*vertices[idx],
                                 *vertices[idx + 1],
                                 *vertices[idx - i - 1],
                                 color));
  }

  for (int i = 0; i < num_vertices; ++i) {
    delete vertices[i];
  }
}

PNTriangle::~PNTriangle() {
  for (int i = 0, n = tris_.size(); i < n; ++i) {
    delete tris_[i];
  }
}

bool PNTriangle::IsIntersects(const Point3f& point, const Point3f& ray,
                              float* distance) const {
  return bbox_.IsIntersects(point, ray, distance);
}

void PNTriangle::GetTriangles(std::vector<Triangle*>* tris) {
  tris->reserve(tris->size() + tris_.size());
  tris->insert(tris->end(), tris_.begin(), tris_.end());
}

void PNTriangle::Move(const Point3f& delta) {
  for (int i = 0, n = tris_.size(); i < n; ++i) {
    tris_[i]->Move(delta);
  }
  bbox_ += delta;
}
