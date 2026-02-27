#pragma once

#include "assimp/mesh.h"
#include <vector>
namespace Ripview {

struct RenderData {
  unsigned int vao;
  unsigned int ibo;
};

struct Mesh {
  void Upload(aiMesh &mesh);

  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  RenderData mRenderData;
};

} // namespace Ripview
