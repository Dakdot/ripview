#include "Shader.hpp"
#include "shader.h"

#include <cstddef>
#include <fstream>
#include <ios>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace Ripview {

Shader::Shader(const std::string &filepath) : mFilepath(filepath) {
  std::ifstream file(filepath);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size)) {
    spdlog::error("Failed to read shader file: {}", filepath);
    return;
  }

  unsigned int id = glCreateShader(mType);
  if (!id) {
    spdlog::error("Failed to create GL shader");
    return;
  }

  glShaderSource(id, 1, (char const *const *)buffer.data(), NULL);

  glCompileShader(id);

  int compile_status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status != GL_TRUE) {
    spdlog::error("Shader compilation failed for unit: {}", filepath);
    int info_log_length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      std::vector<char> log_buffer(info_log_length);
      glGetShaderInfoLog(id, info_log_length, NULL, log_buffer.data());
      spdlog::error(log_buffer.data());
    } else {
      spdlog::error("No information was provided by OpenGL.");
    }
    return;
  }
  mId = id;
}

} // namespace Ripview
