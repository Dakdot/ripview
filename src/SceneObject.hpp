#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

namespace Ripview {

struct SceneObject {
  SceneObject(const std::string &filepath);
  void LoadFromFile(const std::string &filepath);

  Transform mTransform;
  Material *mMaterial;
  std::vector<Mesh> mMeshes;

  std::vector<SceneObject> mChildren;
};

} // namespace Ripview
