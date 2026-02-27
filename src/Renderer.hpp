#pragma once

#include "Camera.hpp"
#include "SceneObject.hpp"
namespace Ripview {

class Renderer {
public:
  Renderer();

  void Draw(SceneObject &root, Camera &camera);

private:
};

} // namespace Ripview
