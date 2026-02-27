#pragma once

#include "ShaderProgram.hpp"

#include "glm/vec3.hpp"

namespace Ripview {

struct Material {
  ShaderProgram &program;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

} // namespace Ripview
