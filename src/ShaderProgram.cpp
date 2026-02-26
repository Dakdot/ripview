#include "ShaderProgram.hpp"
#include "Shader.hpp"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <string>
#include <vector>

namespace Ripview {

ShaderProgram::ShaderProgram(const std::string &debugName)
    : mDebugName(debugName) {}

void ShaderProgram::AttachShader(Shader &s) {
  if (s.mType != GL_VERTEX_SHADER && s.mType != GL_FRAGMENT_SHADER) {
    return;
  }

  if (s.mType == GL_VERTEX_SHADER) {
    mVertexShader = &s;
  } else if (s.mType == GL_FRAGMENT_SHADER) {
    mFragmentShader = &s;
  }
}

int ShaderProgram::Link() {
  spdlog::trace("Linking shader program...");
  unsigned int id = glCreateProgram();

  glAttachShader(id, mVertexShader->mId);
  glAttachShader(id, mFragmentShader->mId);

  glLinkProgram(id);

  int link_status;
  glGetProgramiv(id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    int info_log_length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length) {
      std::vector<char> log_buffer(info_log_length);
      glGetProgramInfoLog(id, info_log_length, NULL, log_buffer.data());
      spdlog::error(log_buffer.data());
    } else {
      spdlog::error("No information was provided by OpenGL.");
    }
    return 1;
  }

  mId = id;
  return 0;
}

void ShaderProgram::Use() { glUseProgram(mId); }

GLuint ShaderProgram::GetUniformLoc(const std::string &name) {
  Use();
  GLuint uniformLoc = glGetUniformLocation(mId, name.c_str());
  if (uniformLoc == GL_INVALID_VALUE || uniformLoc == GL_INVALID_OPERATION)
    spdlog::warn("Could not find uniform location for reference "
                 "\"{}\"\n\tShader Name: {}\n\tShader ID: {}",
                 name, mDebugName, mId);
  return uniformLoc;
}

void ShaderProgram::SetUniform1f(const std::string &name, float value) {
  GLuint uniformLoc = GetUniformLoc(name);
  glUniform1f(uniformLoc, value);
}

void ShaderProgram::SetUniform3fv(const std::string &name, float *value) {
  GLuint uniformLoc = GetUniformLoc(name);
  glUniform3fv(uniformLoc, 1, value);
}

void ShaderProgram::SetUniformMat4fv(const std::string &name, float *value) {
  GLuint uniformLoc = GetUniformLoc(name);
  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value);
}

} // namespace Ripview
