#pragma once

#include <stddef.h>
#include <stdint.h>

#include "glad/glad.h"

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

typedef struct {
  rvVertexList *vertices;
  rvIndexList *indices;
  uint32_t shader;
  rvRenderData renderData;
} rvMesh;

void upload_mesh(rvMesh *mesh);
