#include "Mesh.hpp"
#include "assimp/mesh.h"

#include "glad/glad.h"

namespace Ripview {

void Mesh::Upload(aiMesh &mesh) {
  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t buffers[3];
  glGenBuffers(3, buffers);
  // [0] is VBO_pos, [1] is VBO_norm, [2] is IBO

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.mNumVertices * 3,
               mesh.mVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.mNumVertices * 3,
               mesh.mNormals, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  std::vector<uint32_t> indices(mesh.mNumFaces * 3);
  for (int i = 0; i < mesh.mNumFaces; i++) {
    indices.push_back(mesh.mFaces[i].mIndices[0]);
    indices.push_back(mesh.mFaces[i].mIndices[1]);
    indices.push_back(mesh.mFaces[i].mIndices[2]);
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  mRenderData.vao = vao;
  mRenderData.ibo = buffers[2];
}

} // namespace Ripview
