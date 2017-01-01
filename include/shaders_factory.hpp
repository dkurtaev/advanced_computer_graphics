#ifndef INCLUDE_SHADERS_FACTORY_HPP_
#define INCLUDE_SHADERS_FACTORY_HPP_

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

class ShadersFactory {
 public:
  static unsigned getProgramFromFile(const std::string& vert_shader_path,
                                     const std::string& frag_shader_path);

  static unsigned getProgramFromSource(const char* vert_shader,
                                       const char* frag_shader);

 private:
  static unsigned createShader(GLenum type, const char* src);
};

#endif  // INCLUDE_SHADERS_FACTORY_HPP_
