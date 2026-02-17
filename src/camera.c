#include "camera.h"
#include "cglm/cam.h"
#include "cglm/types.h"
#include "cglm/vec3.h"

#include <stdlib.h>

vec3 up = {0, 1, 0};

rvCamera *camera_create(float near, float far, float aspect, float fov) {
  rvCamera *camera = malloc(sizeof(rvCamera));
  if (!camera)
    return NULL;
  camera->near = near;
  camera->far = far;
  camera->aspect = aspect;
  camera->fov = fov;

  camera->position[0] = 0.0f;
  camera->position[1] = 0.0f;
  camera->position[2] = 50.0f;
  glm_perspective(fov, aspect, near, far, camera->projMatrix);
  glm_lookat(camera->position, GLM_VEC3_ZERO, up, camera->viewMatrix);

  return camera;
}

void camera_destroy(rvCamera *c) {
  if (!c)
    return;
  free(c);
}

float camera_calculate_aspect(int width, int height) {
  return (float)width / (float)height;
}

void camera_recalculate_view_matrix(rvCamera *c) {
    glm_lookat(c->position, GLM_VEC3_ZERO, up, c->viewMatrix);
}
