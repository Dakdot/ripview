#include "material.h"
#include "cglm/vec3.h"
#include "shader.h"
#include <stdlib.h>

rvMaterial *material_create(vec3 color, float roughness,
                            rvShaderProgram *program) {
  rvMaterial *m = malloc(sizeof(rvMaterial));
  if (!m)
    return NULL;
  if (color)
    glm_vec3_copy(color, m->color);
  m->roughness = roughness;
  m->program = program;
  return m;
}

void material_destroy(rvMaterial *m) { free(m); }
