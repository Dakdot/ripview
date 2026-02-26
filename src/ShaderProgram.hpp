#pragma once

#include "Shader.hpp"

#include <string>

#include <glad/glad.h>

namespace Ripview {

class ShaderProgram {
public:
  ShaderProgram(const std::string &debugName);

  void AttachShader(Shader &s);
  int Link();
  void Use();

  void SetUniform1f(const std::string &name, float value);
  void SetUniform3fv(const std::string &name, float *value);
  void SetUniformMat4fv(const std::string &name, float *value);

private:
  GLuint GetUniformLoc(const std::string &name);

  const std::string &mDebugName;
  int mId;
  Shader *mVertexShader;
  Shader *mFragmentShader;
};

} // namespace Ripview
