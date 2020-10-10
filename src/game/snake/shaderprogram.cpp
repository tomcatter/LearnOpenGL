#include "shaderprogram.h"

#include <stdexcept>

ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  {
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      throw std::runtime_error{ "Vertex shader compilation failed" };
    }
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  {
    int success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      throw std::runtime_error{ "Fragment shader compilation failed" };
    }
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  {
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
      throw std::runtime_error{ "Shader linking failed" };
    }
  }

  mid = shaderProgram;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}


ShaderProgram::~ShaderProgram()
{
  glUseProgram(0);
  glDeleteProgram(mid);
}