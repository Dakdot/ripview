#pragma once

#include "cglm/types.h"
#include "shader.h"

typedef struct rvMaterial {
  rvShaderProgram *program;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
} rvMaterial;

rvMaterial *material_create(vec3 ambient, vec3 diffuse, vec3 specular,
                            float shininess, rvShaderProgram *program);
void material_destroy(rvMaterial *);
