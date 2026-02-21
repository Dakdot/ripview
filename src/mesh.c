#include "mesh.h"
#include "assimp/mesh.h"
#include <stdint.h>
#include <stdlib.h>

#include "glad/glad.h"

rvMesh *mesh_create(void) {
  rvMesh *mesh = malloc(sizeof(rvMesh));
  return mesh;
}

void mesh_destroy(rvMesh *m) {
  if (!m)
    return;
  free(m);
}

void mesh_upload(rvMesh *rm, struct aiMesh *m) {
  if (!rm || !m)
    return;

  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t buffers[3]; // [0] is VBO_pos, [1] is VBO_norm, [2] is IBO
  glGenBuffers(3, buffers);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->mNumVertices * 3,
               m->mVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->mNumVertices * 3,
               m->mNormals, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
  uint32_t *indices = malloc(sizeof(uint32_t) * m->mNumFaces * 3);
  if (!indices)
    return;
  for (int j = 0; j < m->mNumFaces; j++) {
    indices[(j * 3)] = m->mFaces[j].mIndices[0];
    indices[(j * 3) + 1] = m->mFaces[j].mIndices[1];
    indices[(j * 3) + 2] = m->mFaces[j].mIndices[2];
  }
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m->mNumFaces * 3,
               indices, GL_STATIC_DRAW);
  free(indices);

  rm->renderData.vao = vao;
  rm->renderData.vbo = buffers[0];
  rm->renderData.ibo = buffers[1];
  rm->numVertices = m->mNumVertices;
  rm->numIndices = m->mNumFaces * 3;
}
