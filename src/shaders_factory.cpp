#include "include/shaders_factory.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/freeglut.h>

unsigned ShadersFactory::getProgramFromFile(const std::string& vert_shader_path,
                                            const std::string& frag_shader_path) {
  std::ifstream file;

  // Vertex shader.
  file.open(vert_shader_path.c_str());
  if (file.is_open()) {
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    std::string vert_shader_src = ss.str();

    // Fragment shader.
    file.open(frag_shader_path.c_str());
    if (file.is_open()) {
      std::stringstream ss;
      ss << file.rdbuf();
      file.close();
      std::string frag_shader_src = ss.str();

      return getProgramFromSource(vert_shader_src.c_str(),
                                  frag_shader_src.c_str());
    } else {
      std::cout << "Fragment shader " << frag_shader_path << " is not found."
                << std::endl;
      return 0;
    }
  } else {
    std::cout << "Vertex shader " << vert_shader_path << " is not found."
              << std::endl;
    return 0;
  }
}

unsigned
ShadersFactory::getProgramFromSource(const char* vert_shader_src,
                                     const char* frag_shader_src) {
  unsigned program = glCreateProgram();
  if (!program) {
    std::cout << "Program creating failed" << std::endl;
    return 0;
  }

  unsigned vert_shader = createShader(GL_VERTEX_SHADER, vert_shader_src);
  unsigned frag_shader = createShader(GL_FRAGMENT_SHADER, frag_shader_src);

  if (!vert_shader || !frag_shader) {
    return 0;
  }

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  glLinkProgram(program);
  GLint success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    GLint log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    GLchar* log_msg = new GLchar[log_length];
    glGetProgramInfoLog(program, log_length, &log_length,
                        log_msg);
    std::cout << "Cannot compile shader program: " << log_msg << std::endl;
    glDeleteProgram(program);
    program = 0;
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return program;
}

unsigned ShadersFactory::createShader(GLenum type, const char* src) {
  unsigned shader = glCreateShader(type);
  if (!shader) {
    std::cout << "Shader creating failed" << std::endl;
    return 0;
  }

  glShaderSource(shader, 1, &src, 0);

  glCompileShader(shader);
  int success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
      int log_length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      char* log_msg = new char[log_length];
      glGetShaderInfoLog(shader, log_length, &log_length, log_msg);
      std::cout << "Can't compile "
                << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                << " shader: " << log_msg << std::endl;
      return 0;
  }
  return shader;
}
