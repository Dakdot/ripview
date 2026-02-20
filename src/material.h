#pragma once

#include "cglm/types.h"
#include "shader.h"

typedef struct rvMaterial {
  rvShaderProgram program;
  vec3 color;
  float roughness;
} rvMaterial;
