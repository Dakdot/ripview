#include "SceneObject.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <spdlog/spdlog.h>

namespace Ripview {

SceneObject::SceneObject(const std::string &filepath) {
  const aiScene *scene =
      aiImportFile(filepath.c_str(),
                   aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                       aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
                       aiProcess_SortByPType);

  if (scene == nullptr) {
    spdlog::error("Failed to import model file: {}\n{}", filepath,
                  aiGetErrorString());
    return;
  }
}

} // namespace Ripview
