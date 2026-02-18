#include "renderer.h"
#include "util/queue.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define GL_CHECK_ERROR() assert(glGetError() == 0)

struct rvRenderer {
  rvQueue queue;
};

rvRenderer *renderer_create(void) {
  rvRenderer *r = malloc(sizeof(rvRenderer));
  if (!r)
    return NULL;
  queue_init(&r->queue);
  return r;
}

void renderer_destroy(rvRenderer *r) {
  if (!r)
    return;
  queue_destroy(&r->queue);
  free(r);
}

void renderer_submit(rvRenderer *r, rvRenderCmd *cmd) {
  rvQueue *queue = &r->queue;
  queue_enqueue(queue, cmd);
}

void renderer_draw(rvRenderer *r) {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rvQueue *queue = &r->queue;
  int call_idx = 0;

  rvQueueNode *head = queue->front;

  while (head) {
    rvRenderCmd *cmd = (rvRenderCmd *)head->data;
    if (!cmd)
      break;
    call_idx++;

    glUseProgram(cmd->shader);
    glBindVertexArray(cmd->vao);
    glDrawElements(cmd->mode, cmd->count, GL_UNSIGNED_INT,
                   (void *)(uintptr_t)cmd->first);

    head = head->next;
  }

  glBindVertexArray(0);
  glUseProgram(0);
}
