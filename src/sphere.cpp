#include "include/sphere.hpp"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "include/vertex.hpp"
#include "include/shaders_factory.hpp"

Sphere::Sphere(float radius, int lod) {
  Vertex p1(0, 0, radius, 0, 0, 1);
  Vertex p2(radius, 0, 0, 1, 0, 0);
  Vertex p3(0, 0, -radius, 0, 0, -1);
  Vertex p4(-radius, 0, 0, -1, 0, 0);
  Vertex p5(0, radius, 0, 0, 1, 0);
  Vertex p6(0, -radius, 0, 0, -1, 0);

  tris_[0] = new Triangle(p4, p1, p5);
  tris_[1] = new Triangle(p1, p2, p5);
  tris_[2] = new Triangle(p2, p3, p5);
  tris_[3] = new Triangle(p3, p4, p5);

  tris_[4] = new Triangle(p1, p4, p6);
  tris_[5] = new Triangle(p2, p1, p6);
  tris_[6] = new Triangle(p3, p2, p6);
  tris_[7] = new Triangle(p4, p3, p6);

  shader_program_ = ShadersFactory::getProgramFromFile(
      "../shaders/default.vshader", "../shaders/default.fshader");
}

Sphere::~Sphere() {
  for (int i = 0; i < 8; ++i) {
    delete tris_[i];
  }
}

void Sphere::draw() const {
  glUseProgram(shader_program_);
  for (int i = 0; i < 8; ++i) {
    tris_[i]->draw(shader_program_);
  }
  glUseProgram(0);
}
