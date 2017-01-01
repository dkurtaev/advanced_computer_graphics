#include "include/triangle.hpp"

#include <stdint.h>

#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

Triangle::Triangle(const Vertex& p1, const Vertex& p2, const Vertex& p3,
                   int lod) {
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
  num_triangles_ = (lod + 1) * (lod + 1);

  float* bary_coords = new float[2 * num_vertices];
  uint16_t* indices = new uint16_t[3 * num_triangles_];

  float u = 0;
  float du = 1.0 / (lod + 1);
  float* bary_offset = bary_coords;
  for (int i = 0; i < lod + 2; ++i, u += du) {
    float v = 0;
    float dv = 1.0 / i;
    for (int j = 0; j <= i; ++j, v += dv) {
      bary_offset[0] = 1 - u;
      bary_offset[1] = u * (1 - v);
      bary_offset += 2;
    }
  }

  uint16_t idx = 1;
  uint16_t* indices_offset = indices;
  for (int i = 0; i <= lod; ++i, idx += 2) {
    for (int j = 0; j < i; ++j, ++idx) {
      indices_offset[0] = idx;
      indices_offset[1] = idx + 1;
      indices_offset[2] = idx - i - 1;

      indices_offset[3] = idx + 1;
      indices_offset[4] = idx - i;
      indices_offset[5] = idx - i - 1;
      indices_offset += 6;
    }
    indices_offset[0] = idx;
    indices_offset[1] = idx + 1;
    indices_offset[2] = idx - i - 1;
    indices_offset += 3;
  }

  glGenBuffers(1, &bary_coords_vbo_);
  if (!bary_coords_vbo_) {
    std::cout << "Failed VBO generation." << std::endl;
  }
  glBindBuffer(GL_ARRAY_BUFFER, bary_coords_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, bary_coords,
               GL_STATIC_DRAW);
  delete[] bary_coords;
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &indices_vbo_);
  if (!indices_vbo_) {
    std::cout << "Failed VBO generation." << std::endl;
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 3 * num_triangles_,
               indices, GL_STATIC_DRAW);
  delete[] indices;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  p1.getCoords(base_coords_);
  p2.getCoords(base_coords_ + 3);
  p3.getCoords(base_coords_ + 6);
  p1.getNormal(base_normals_);
  p2.getNormal(base_normals_ + 3);
  p3.getNormal(base_normals_ + 6);
}

Triangle::~Triangle() {
  glDeleteBuffers(1, &bary_coords_vbo_);
  glDeleteBuffers(1, &indices_vbo_);
}

void Triangle::draw(unsigned program) const {
  unsigned loc_coords = glGetUniformLocation(program, "u_corners_coords");
  unsigned loc_normals = glGetUniformLocation(program, "u_corners_normals");
  unsigned loc_bary = glGetAttribLocation(program, "a_bary_coeffs");

  glUniform3fv(loc_coords, 3, base_coords_);
  glUniform3fv(loc_normals, 3, base_normals_);

  glBindBuffer(GL_ARRAY_BUFFER, bary_coords_vbo_);
  glVertexAttribPointer(loc_bary, 2, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(loc_bary);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_);
  glDrawElements(GL_TRIANGLES, 3 * num_triangles_, GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(loc_bary);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
