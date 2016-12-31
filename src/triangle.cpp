#include "include/triangle.hpp"

#include <GL/freeglut.h>

Triangle::Triangle(const Vertex& p1, const Vertex& p2, const Vertex& p3) {
  p1.getCoords(coords_);
  p2.getCoords(coords_ + 3);
  p3.getCoords(coords_ + 6);
  p1.getNormal(normals_);
  p2.getNormal(normals_ + 3);
  p3.getNormal(normals_ + 6);
}

void Triangle::draw() const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, coords_);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, normals_);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}
