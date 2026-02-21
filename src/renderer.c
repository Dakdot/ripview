#include "renderer.h"
#include "camera.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "mesh.h"
#include "scene.h"
#include "scene_object.h"
#include "shader.h"
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

void renderer_flush_queue(rvRenderer *r) {
  rvQueueNode *head = r->queue.front;
  while (head) {
    rvQueueNode *temp = head;
    head = head->next;
    r->queue.front = head;
    free(temp);
  }
}

void renderer_draw(rvScene *scene, rvCamera *camera) {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < scene->objects.num_children; i++) {
    rvSceneObject *o = (rvSceneObject *)scene->objects.children[i]->data;

    mat4 transformMatrix;
    glm_mat4_identity(transformMatrix);
    glm_translate(transformMatrix, o->transform.position);
    glm_scale(transformMatrix, o->transform.scale);

    shader_set_uniform_mat4fv(o->material->program, "u_Proj",
                              (float *)camera->projMatrix);
    shader_set_uniform_mat4fv(o->material->program, "u_View",
                              (float *)camera->viewMatrix);
    shader_set_uniform_mat4fv(o->material->program, "u_Model",
                              (float *)transformMatrix);

    shader_program_use(o->material->program);
    rvQueueNode *n = o->meshes.front;
    while (n) {
      rvMesh *m = (rvMesh *)n->data;
      glBindVertexArray(m->renderData.vao);
      glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0);

      n = n->next;
    }
  }

  // rvQueue *queue = &r->queue;
  // int call_idx = 0;

  // rvQueueNode *head = queue->front;

  // while (head) {
  //   rvRenderCmd *cmd = (rvRenderCmd *)head->data;
  //   if (!cmd)
  //     break;
  //   call_idx++;

  //   glUseProgram(cmd->shader);
  //   glBindVertexArray(cmd->vao);
  //   glDrawElements(cmd->mode, cmd->count, GL_UNSIGNED_INT,
  //                  (void *)(uintptr_t)cmd->first);

  //   printf("Draw Call %03d: shader: %d, vao: %d, count: %d\n", call_idx,
  //          cmd->shader, cmd->vao, cmd->count);

  //   head = head->next;
  // }

  // glBindVertexArray(0);
  // glUseProgram(0);
}

rvRenderCmd *render_cmd_create(void) {
  rvRenderCmd *r = malloc(sizeof(rvRenderCmd));
  return r;
}

void render_cmd_destroy(rvRenderCmd *r) {
  if (!r)
    return;
  free(r);
}
