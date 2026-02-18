#include "shader.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

  char *buffer = malloc(file_size + 1);
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

int shader_load_from_file(rvShader *shader) {
  printf("Compiling shader unit %s...\n", shader->filepath);
  char *data;
  uint32_t file_read_status =
      read_file(shader->filepath, &data); // DO NOT FORGET TO FREE `data`
  if (file_read_status) {
    return EXIT_FAILURE;
  }

  uint32_t id = glCreateShader(shader->type);
  if (!id) {
    perror("Failed to create GL shader.\n");
    return EXIT_FAILURE;
  }

  glShaderSource(id, 1, (char const *const *)&data, NULL);

  glCompileShader(id);

  free(data); // I didn't forget ☺️

  int compile_status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status != GL_TRUE) {
    printf("Shader compilation failed for unit '%s':\n", shader->filepath);
    int info_log_length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
      char *log_buffer = malloc(info_log_length);
      if (!log_buffer) {
        printf("Failed to retrieve info log. Could not allocate memory.\n");
        return EXIT_FAILURE;
      }
      glGetShaderInfoLog(id, info_log_length, NULL, log_buffer);
      puts(log_buffer);
      free(log_buffer);
    } else {
      printf("No information was provided by OpenGL.\n");
    }
    return EXIT_FAILURE;
  }

  shader->id = id;

  return EXIT_SUCCESS;
}

int shader_program_link(rvShaderProgram *program) {
  printf("Linking shader program...\n");
  uint32_t id = glCreateProgram();

  glAttachShader(id, program->vertexShader->id);
  glAttachShader(id, program->fragmentShader->id);

  glLinkProgram(id);

  int link_status;
  glGetProgramiv(id, GL_LINK_STATUS, &link_status);
  if (link_status != GL_TRUE) {
    int info_log_length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length) {
      char *log_buffer = malloc(info_log_length);
      if (!log_buffer) {
        printf("Failed to retrieve info log. Could not allocate memory.\n");
        return EXIT_FAILURE;
      }
      glGetProgramInfoLog(id, info_log_length, NULL, log_buffer);
      puts(log_buffer);
      free(log_buffer);
    } else {
      printf("No information was provided by OpenGL.\n");
    }
    return EXIT_FAILURE;
  }

  glDeleteShader(program->vertexShader->id);
  glDeleteShader(program->fragmentShader->id);

  program->id = id;

  return EXIT_SUCCESS;
}

void shader_program_use(rvShaderProgram *p) {
  if (!p)
    return;
  glUseProgram(p->id);
}

GLuint shader_get_uniform_loc(rvShaderProgram *p, const char *uniformName) {
  if (!p)
    return GL_INVALID_VALUE;
  if (!uniformName)
    return GL_INVALID_VALUE;
  shader_program_use(p);
  return glGetUniformLocation(p->id, uniformName);
}

void shader_set_uniform_3fv(rvShaderProgram *p, const char *uniformName,
                            float *value) {
  GLuint uniformLoc = shader_get_uniform_loc(p, uniformName);
  if (uniformLoc == GL_INVALID_VALUE || uniformLoc == GL_INVALID_OPERATION)
    return;
  glUniform3fv(uniformLoc, 1, value);
}

void shader_set_uniform_mat4fv(rvShaderProgram *p, const char *uniformName,
                               float *value) {
  GLuint uniformLoc = shader_get_uniform_loc(p, uniformName);
  if (uniformLoc == GL_INVALID_VALUE || uniformLoc == GL_INVALID_OPERATION)
    return;
  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value);
}
