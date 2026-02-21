#pragma once

#include "cglm/types.h"
#include "shader.h"

typedef struct rvMaterial {
  rvShaderProgram *program;
  vec3 color;
  float roughness;
} rvMaterial;

rvMaterial *material_create(vec3 color, float roughness,
                            rvShaderProgram *program);
void material_destroy(rvMaterial *);
