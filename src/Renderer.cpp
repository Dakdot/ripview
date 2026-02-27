#include "Renderer.hpp"
#include "Camera.hpp"
#include "SceneObject.hpp"

#include <spdlog/spdlog.h>

namespace Ripview {

Renderer::Renderer() {}

void Renderer::Draw(SceneObject &root, Camera &camera) {
  spdlog::trace("We be drawing........");
}

} // namespace Ripview
