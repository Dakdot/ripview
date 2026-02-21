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

int shader_load_from_file(rvShader *shader);
int shader_program_link(rvShaderProgram *program);
void shader_program_use(rvShaderProgram *p);

void shader_set_uniform_1f(rvShaderProgram *, const char *uniformName,
                           float value);
void shader_set_uniform_mat4fv(rvShaderProgram *p, const char *uniformName,
                               float *value);
void shader_set_uniform_3fv(rvShaderProgram *p, const char *uniformName,
                            float *value);
