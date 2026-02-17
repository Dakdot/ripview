#pragma once

#include "glad/glad.h"

typedef struct rvRenderer rvRenderer;

typedef struct rvRenderCmd {
  GLuint vao;
  GLuint ibo;

  GLenum mode;
  GLsizei count;
  GLint first;

  GLuint shader;
  GLuint texture;
} rvRenderCmd;

rvRenderer *renderer_create(void);
void renderer_destroy(rvRenderer *r);
void renderer_submit(rvRenderer *r, rvRenderCmd *cmd);
void renderer_draw(rvRenderer *r);
