#include "renderer.h"
#include "camera.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/simd/sse2/mat4.h"
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
#include <sys/types.h>

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
    rvShaderProgram *prog = o->material->program;

    mat4 modelMatrix;
    glm_mat4_identity(modelMatrix);
    glm_translate(modelMatrix, o->transform.position);
    glm_scale(modelMatrix, o->transform.scale);

    mat4 normalMatrix;
    glm_mat4_inv_sse2(modelMatrix, normalMatrix);
    glm_mat4_transpose(normalMatrix);

    vec3 lightColor = {1.0f, 1.0f, 1.0f};
    vec3 lightPos = {25, 15, 25};

    shader_set_uniform_mat4fv(prog, "u_Proj", (float *)camera->projMatrix);
    shader_set_uniform_mat4fv(prog, "u_View", (float *)camera->viewMatrix);
    shader_set_uniform_mat4fv(prog, "u_Model", (float *)modelMatrix);
    shader_set_uniform_mat4fv(prog, "u_Normal", (float *)normalMatrix);
    shader_set_uniform_3fv(prog, "u_ObjColor", o->material->color);
    shader_set_uniform_1f(prog, "u_Roughness", o->material->roughness);
    shader_set_uniform_3fv(prog, "u_ViewPos", camera->position);
    shader_set_uniform_3fv(prog, "u_LightColor", lightColor);
    shader_set_uniform_3fv(prog, "u_LightPos", lightPos);

    shader_program_use(prog);
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
