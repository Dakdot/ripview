#include "camera.h"
#include "cglm/cam.h"

#include <stdlib.h>

rvCamera *camera_create(float near, float far, float aspect, float fov) {
  rvCamera *camera = malloc(sizeof(rvCamera));
  if (!camera)
    return NULL;
  camera->near = near;
  camera->far = far;
  camera->aspect = aspect;
  camera->fov = fov;

  glm_perspective(fov, aspect, near, far, camera->projMatrix);

  return camera;
}

void camera_destroy(rvCamera *c) {
  if (!c)
    return;
  free(c);
}

float camera_calculate_aspect(int width, int height) {}
