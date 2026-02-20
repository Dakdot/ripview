#pragma once

#include <stddef.h>
#include <stdint.h>

#include "assimp/mesh.h"

typedef struct {
  uint32_t vao;
  uint32_t vbo;
  uint32_t ibo;
} rvRenderData;

typedef struct {
  size_t size;
  float *data;
} rvVertexList;

typedef struct {
  size_t size;
  uint32_t *data;
} rvIndexList;

typedef struct rvMesh {
  rvVertexList *vertices;
  uint32_t numVertices;
  rvIndexList *indices;
  uint32_t numIndices;
  rvRenderData renderData;
} rvMesh;

rvMesh *mesh_create(void);
void mesh_destroy(rvMesh *);
void mesh_upload(rvMesh *, struct aiMesh *);
