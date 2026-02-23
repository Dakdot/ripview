#include "material.h"
#include "cglm/vec3.h"
#include "shader.h"
#include <stdlib.h>

rvMaterial *material_create(vec3 ambient, vec3 diffuse, vec3 specular,
                            float shininess, rvShaderProgram *program) {
  rvMaterial *m = malloc(sizeof(rvMaterial));
  if (!m)
    return NULL;
  if (ambient)
    glm_vec3_copy(ambient, m->ambient);
  if (diffuse)
    glm_vec3_copy(diffuse, m->diffuse);
  if (specular)
    glm_vec3_copy(specular, m->specular);
  m->shininess = shininess;
  m->program = program;
  return m;
}

void material_destroy(rvMaterial *m) { free(m); }
