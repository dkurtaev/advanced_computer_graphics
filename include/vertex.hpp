#ifndef INCLUDE_VERTEX_HPP_
#define INCLUDE_VERTEX_HPP_

class Vertex {
 public:
  Vertex(float x, float y, float z, float nx, float ny, float nz);

  void getCoords(float* dst) const;

  void getNormal(float* dst) const;

 private:
  float coords_[3];
  float normal_[3];
};

#endif  // INCLUDE_VERTEX_HPP_
