#ifndef SHADER_PROGRAM_H_
#define SHADER_PROGRAM_H_

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
public:
  ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
  ~ShaderProgram();

  inline unsigned int id() const
  {
    return mid;
  };


  inline void setUniform(const char* name, const glm::mat4& value)const
  {
    glUniformMatrix4fv(glGetUniformLocation(mid, name), 1, GL_FALSE, glm::value_ptr(value));
  };

private:
  unsigned int mid;

};

#endif // !SHADER_PROGRAM_H_
