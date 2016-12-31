#include "include/vertex.hpp"

Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz) {
  coords_[0] = x;
  coords_[1] = y;
  coords_[2] = z;
  normal_[0] = nx;
  normal_[1] = ny;
  normal_[2] = nz;
}

void Vertex::getCoords(float* dst) const {
  dst[0] = coords_[0];
  dst[1] = coords_[1];
  dst[2] = coords_[2];
}

void Vertex::getNormal(float* dst) const {
  dst[0] = normal_[0];
  dst[1] = normal_[1];
  dst[2] = normal_[2];
}
