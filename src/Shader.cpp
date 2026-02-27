#include "Shader.hpp"

#include <csignal>
#include <cstddef>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "glad/glad.h"

namespace Ripview {

int read_file(const char *filepath, char **data) {
  FILE *fptr = NULL;
  fptr = fopen(filepath, "r");
  if (fptr == NULL) {
    perror("Failed to open file.\n");
    return EXIT_FAILURE;
  }

  if (fseek(fptr, 0, SEEK_END)) {
    perror("Failed to seek file end.\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }

  long file_size_long = ftell(fptr);
  if (file_size_long == -1) {
    perror("Failed to determine file size.\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }

  size_t file_size = (size_t)file_size_long;
  fseek(fptr, 0, SEEK_SET);

  char *buffer = new char[file_size + 1];
  if (buffer == NULL) {
    perror("Failed to allocated memory for file read.\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }

  size_t bytes_read = fread(buffer, 1, file_size, fptr);
  if (bytes_read != file_size) {
    perror("Incomplete file read.\n");
    free(buffer);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  buffer[file_size] = '\0';
  fclose(fptr);
  *data = buffer;
  return EXIT_SUCCESS;
}

Shader::Shader(const std::string &filepath, unsigned int type)
    : mFilepath(filepath), mType(type) {
  char *buffer;
  read_file(filepath.c_str(), &buffer);

  unsigned int id = glCreateShader(mType);
  if (!id) {
    spdlog::error("Failed to create GL shader");
    return;
  }

  glShaderSource(id, 1, (char const *const *)&buffer, NULL);

  glCompileShader(id);
  delete[] buffer;

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
