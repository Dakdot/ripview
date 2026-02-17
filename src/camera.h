#pragma once

#include "cglm/types.h"

typedef struct rvCamera {
  float near;
  float far;
  float aspect;
  float fov;
  vec3 position;
  mat4 projMatrix;
} rvCamera;

rvCamera *camera_create(float near, float far, float aspect, float fov);
void camera_destroy(rvCamera *c);
float camera_calculate_aspect(int width, int height);
