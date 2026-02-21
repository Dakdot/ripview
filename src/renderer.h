#pragma once

#include "camera.h"
#include "glad/glad.h"
#include "scene.h"

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
void renderer_flush_queue(rvRenderer *);
void renderer_draw(rvScene *, rvCamera *);

rvRenderCmd *render_cmd_create(void);
void render_cmd_destroy(rvRenderCmd *);
