#pragma once

#include <glm/vec3.hpp>

namespace Ripview {

struct Transform {
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
};

} // namespace Ripview
