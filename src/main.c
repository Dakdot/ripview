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
                     .filepath = "/Users/thiagoandrade/Projects/experiments/"
                                 "ripview/assets/shaders/main.vert"};

  rvShader fragment = {.type = SHADER_TYPE_FRAGMENT,
                       .filepath = "/Users/thiagoandrade/Projects/experiments/"
                                   "ripview/assets/shaders/main.frag"};

  if (shader_load_from_file(&vertex))
    return EXIT_FAILURE;
  if (shader_load_from_file(&fragment))
    return EXIT_FAILURE;

  rvShaderProgram program = {.vertexShader = &vertex,
                             .fragmentShader = &fragment};

  shader_program_link(&program);

  const struct aiScene *aiScene =
      aiImportFile("/Users/thiagoandrade/Projects/experiments/ripview/assets/"
                   "models/glTF2/Lantern.glb",
                   aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                       aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

  if (!aiScene) {
    perror("Failed to import model.\n");
    perror(aiGetErrorString());
    return EXIT_FAILURE;
  }

  rvSceneObject *o = scene_object_create();

  int counter = 0;
  for (int i = 0; i < aiScene->mNumMeshes; i++) {
    // for (int i = 0; i < 1; i++) {
    struct aiMesh *aiMesh = aiScene->mMeshes[i];
    rvMesh *rvMesh = mesh_create();
    mesh_upload(rvMesh, aiMesh);
    scene_object_attach_mesh(o, rvMesh);

    counter++;
  }
  printf("Added %d meshes.\n", counter);

  aiReleaseImport(aiScene);

  rvCamera *camera =
      camera_create(0.1f, 1000.0f,
                    camera_calculate_aspect(window.width, window.height), 1.07);

  printf("Render initialization completed.\n");

  rvScene *scene = scene_create();
  scene_add_object(scene, o);

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
    shader_set_uniform_mat4fv(&program, "u_Proj", (float *)camera->projMatrix);
    shader_set_uniform_mat4fv(&program, "u_View", (float *)camera->viewMatrix);

    shader_program_use(&program);
    renderer_draw(renderer);

    glfwSwapBuffers(window.glfwHandle);
    glfwPollEvents();

    double now = glfwGetTime();
    vec3 new_pos = {sin(now) * 30, 10.0f, cos(now) * 30};
    glm_vec3_copy(new_pos, camera->position);
    camera_recalculate_view_matrix(camera);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
