#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>

namespace Ripview {

struct Camera {
  Camera(float near, float far, float aspect, float fov)
      : mNear(near), mFar(far), mAspect(aspect), mFov(fov) {}

  inline static float CalculateAspectRatio(int width, int height) {
    return (float)width / (float)height;
  }

  inline void RecalculateViewProjectionMatrix() {
    mViewProjMatrix =
        glm::lookAt(mPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) *
        glm::perspective(mFov, mAspect, mNear, mFar);
  }

  float mNear;
  float mFar;
  float mAspect;
  float mFov;
  glm::vec3 mPosition;
  glm::mat4 mViewProjMatrix;
};

} // namespace Ripview
