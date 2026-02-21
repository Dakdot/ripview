// clang-format off
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "camera.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "material.h"
#include "mesh.h"
#include "renderer.h"
#include "scene.h"
#include "scene_object.h"
#include "shader.h"
#include "util/queue.h"
#include "window.h"

#include <math.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// clang-format on

#define GL_CHECK_ERROR() assert(glGetError() == 0)

int main() {
  rvWindow window = {.width = 800, .height = 600, .title = "Ripview"};
  if (create_window(&window))
    return EXIT_FAILURE;

  rvRenderer *renderer = renderer_create();
  if (!renderer) {
    perror("Failed to created renderer.\n");
    return EXIT_FAILURE;
  }

  rvShader vertex = {.type = SHADER_TYPE_VERTEX,
                     .filepath = "/home/thiagoandrade/Projects/experiments/"
                                 "ripview/assets/shaders/main.vert"};

  rvShader fragment = {.type = SHADER_TYPE_FRAGMENT,
                       .filepath = "/home/thiagoandrade/Projects/experiments/"
                                   "ripview/assets/shaders/main.frag"};

  rvShader lampFrag = {.type = SHADER_TYPE_FRAGMENT,
                       .filepath = "/home/thiagoandrade/Projects/experiments/"
                                   "ripview/assets/shaders/lamp.frag"};

  if (shader_load_from_file(&vertex))
    return EXIT_FAILURE;
  if (shader_load_from_file(&fragment))
    return EXIT_FAILURE;
  if (shader_load_from_file(&lampFrag))
    return EXIT_FAILURE;

  rvShaderProgram program = {.vertexShader = &vertex,
                             .fragmentShader = &fragment};

  shader_program_link(&program);

  rvShaderProgram lampProgram = {.vertexShader = &vertex,
                                 .fragmentShader = &lampFrag};
  shader_program_link(&lampProgram);

  vec3 c1 = {1.0f, 0.0f, 1.0f};
  vec3 c2 = {0.992, 0.706, 0.082};
  vec3 c3 = {0.22, 0.757, 0.114};
  rvMaterial *mtr1 = material_create(c1, 0.2, &program);
  rvMaterial *mtr2 = material_create(c2, 0.7, &program);
  rvMaterial *mtr3 = material_create(c3, 0.5, &program);
  rvMaterial *lampMaterial = material_create(c1, 1.0, &lampProgram);

  rvSceneObject *o1 = scene_object_load_from_file(
      "/home/thiagoandrade/Projects/experiments/ripview/assets/"
      "models/glTF2/Lantern.glb");
  rvSceneObject *o2 = scene_object_load_from_file(
      "/home/thiagoandrade/Projects/experiments/ripview/assets/"
      "models/glTF2/Fox.glb");
  rvSceneObject *o3 =
      scene_object_load_from_file("/home/thiagoandrade/Projects/experiments/"
                                  "ripview/assets/models/glTF2/Avocado.glb");

  rvSceneObject *lamp =
      scene_object_load_from_file("/home/thiagoandrade/Projects/experiments/"
                                  "ripview/assets/models/glTF2/Box.glb");

  scene_object_attach_material(o1, mtr1);
  scene_object_attach_material(o2, mtr2);
  scene_object_attach_material(o3, mtr3);
  scene_object_attach_material(lamp, lampMaterial);

  scene_object_set_position(o1, 0, 0, 0);

  scene_object_set_uniform_scale(o2, 0.1);
  scene_object_set_position(o2, 10, -12, 10);

  scene_object_set_position(o3, -10, -12, 10);
  scene_object_set_uniform_scale(o3, 100);

  scene_object_set_uniform_scale(lamp, 5);
  scene_object_set_position(lamp, 25, 15, 25);

  rvCamera *camera =
      camera_create(0.1f, 1000.0f,
                    camera_calculate_aspect(window.width, window.height), 1.07);

  printf("Render initialization completed.\n");

  rvScene *scene = scene_create();
  scene_add_object(scene, o1);
  scene_add_object(scene, o2);
  scene_add_object(scene, o3);
  scene_add_object(scene, lamp);

  for (int i = 0; i < scene->objects.num_children; i++) {
    rvSceneObject *o = (rvSceneObject *)scene->objects.children[i]->data;

    rvQueueNode *n = o->meshes.front;
    while (n) {
      rvRenderCmd *cmd = render_cmd_create();
      rvMesh *m = (rvMesh *)n->data;
      cmd->vao = m->renderData.vao;
      cmd->mode = GL_TRIANGLES;
      cmd->count = m->numIndices;
      cmd->first = 0;
      cmd->shader = program.id;

      renderer_submit(renderer, cmd);

      n = n->next;
    }
  }

  while (!glfwWindowShouldClose(window.glfwHandle)) {
    renderer_draw(scene, camera);

    glfwSwapBuffers(window.glfwHandle);
    glfwPollEvents();

    double now = glfwGetTime();
    vec3 new_pos = {sin(now) * 40, 14.0f, cos(now) * 40};
    glm_vec3_copy(new_pos, camera->position);
    camera_recalculate_view_matrix(camera);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
