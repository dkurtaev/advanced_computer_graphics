#include <GL/freeglut.h>

#include "include/sphere.hpp"

void display();

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Single view");

  glutDisplayFunc(display);

  glEnable(GL_DEPTH_TEST);

  glutMainLoop();

  return 0;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup camera.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20, 1, 0.1, 1000);
  gluLookAt(5, 5, 5, 0, 0, 0, 0, 0, 1);

  Sphere sphere(1);
  sphere.draw();

  glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);

    glColor3ub(0, 255, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    glColor3ub(0, 0, 255);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
  glEnd();

  glutSwapBuffers();
}
