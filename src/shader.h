#pragma once

#include "glad/glad.h"

#include <stdint.h>

#define SHADER_TYPE_VERTEX GL_VERTEX_SHADER
#define SHADER_TYPE_FRAGMENT GL_FRAGMENT_SHADER

typedef struct {
  int id;
  uint16_t type;
  const char *filepath;
} rvShader;

typedef struct {
  int id;
  rvShader *vertexShader;
  rvShader *fragmentShader;
} rvShaderProgram;

int load_shader_from_file(rvShader *shader);
int link_shaders(rvShaderProgram *program);
