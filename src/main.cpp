#include <iostream>

#include <GL/freeglut.h>
#include <opencv2/opencv.hpp>

#include "include/point3f.hpp"
#include "include/vertex.hpp"
#include "include/triangle.hpp"

void display();

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitWindowSize(100, 100);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Single view");

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  Vertex v1(Point3f(0, 0, -3), Point3f(0, 0, 1));
  Vertex v2(Point3f(1, 0, -3), Point3f(0, 0, 1));
  Vertex v3(Point3f(0, 1, -3), Point3f(0, 0, 1));
  Triangle tri(v1, v2, v3);

  Point3f camera_pos(0, 0, 5);

  uint8_t* canvas = new uint8_t[3 * 100 * 100];
  uint8_t* offset = canvas;
  for (int y = 0; y < 100; ++y) {
    for (int x = 0; x < 100; ++x) {
      Point3f ray(0.02f * x - 1.0f, 1.0f - 0.02f * y, -5, true);
      if (tri.IsIntersects(camera_pos, ray)) {
        offset[0] = 255;
        offset[1] = 255;
        offset[2] = 255;
      } else {
        offset[0] = 0;
        offset[1] = 0;
        offset[2] = 0;
      }
      offset += 3;
    }
  }
  cv::Mat m(100, 100, CV_8UC3, canvas);
  cv::namedWindow("out", cv::WINDOW_NORMAL);
  cv::imshow("out", m);
  cv::waitKey();

  // unsigned tex_id;
  // glGenTextures(1, &tex_id);
  // glBindTexture(GL_TEXTURE_2D, tex_id);
  // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0,
  //              GL_RGB, GL_UNSIGNED_BYTE, canvas);
  //
  //
  // glBegin(GL_QUADS);
  //   glTexCoord2f(1.0f, 0.0f) glVertex2f(0.0f, 0.5f);
  //   glVertex2f(-0.5f, -0.5f);
  //   glVertex2f(0.5f, -0.5f);
  // glEnd();

  glutSwapBuffers();
}
