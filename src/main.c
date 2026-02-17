// clang-format off
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "cglm/types.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cglm/cam.h"

#include "mesh.h"
#include "renderer.h"
#include "shader.h"
#include "window.h"

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

  if (load_shader_from_file(&vertex))
    return EXIT_FAILURE;
  if (load_shader_from_file(&fragment))
    return EXIT_FAILURE;

  rvShaderProgram program = {.vertexShader = &vertex,
                             .fragmentShader = &fragment};

  link_shaders(&program);

  const struct aiScene *scene =
      aiImportFile("/Users/thiagoandrade/Projects/experiments/ripview/assets/"
                   "models/Obj/Spider/spider.obj",
                   aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                       aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

  if (!scene) {
    perror("Failed to import model.\n");
    perror(aiGetErrorString());
    return EXIT_FAILURE;
  }

  scene->mMeshes[0]->mVertices[0].z;

  for (int i = 0; i < scene->mNumMeshes; i++) {
    struct aiMesh *mesh = scene->mMeshes[i];

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices,
                 mesh->mVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    uint32_t ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    uint32_t *indices = malloc(sizeof(uint32_t) * mesh->mNumFaces * 3);
    if (!indices)
      return EXIT_FAILURE;
    for (int j = 0; j < mesh->mNumFaces; j++) {
      indices[(j * 3)] = mesh->mFaces[j].mIndices[0];
      indices[(j * 3) + 1] = mesh->mFaces[j].mIndices[1];
      indices[(j * 3) + 2] = mesh->mFaces[j].mIndices[2];
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint32_t) * mesh->mNumFaces * 3, indices,
                 GL_STATIC_DRAW);
    free(indices);

    rvRenderCmd *cmd = malloc(sizeof(rvRenderCmd));
    if (!cmd)
      return EXIT_FAILURE;
    cmd->vao = vao;
    cmd->ibo = ibo;
    cmd->count = mesh->mNumFaces * 3;
    cmd->first = 0;
    cmd->shader = program.id;
    cmd->mode = GL_TRIANGLES;
    renderer_submit(renderer, cmd);
  }

  aiReleaseImport(scene);

  glBindVertexArray(0);

  float vertices[] = {200.0f, 200.0f, 0.0f, 200.0f, 0.0f,   0.0f,
                      0.0f,   0.0f,   0.0f, 0.0f,   200.0f, 0.0f};

  uint32_t indices[] = {0, 1, 3, 1, 2, 3};

  rvVertexList vertexList = {.size = sizeof(vertices), .data = vertices};

  rvIndexList indexList = {.size = sizeof(indices), .data = indices};

  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

  uint32_t ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glBindVertexArray(0);

  rvRenderCmd render_command = {0};
  render_command.vao = vao;
  render_command.ibo = ibo;
  render_command.count = sizeof(indices) / sizeof(uint32_t);
  render_command.first = 0;
  render_command.shader = program.id;
  render_command.mode = GL_TRIANGLES;

  mat4 projection_matrix;
  glm_ortho(0, window.width, 0, window.height, -100.0f, 100.0f,
            projection_matrix);

  int32_t projection_matrix_uniform_loc = 0xfcfcabab;
  projection_matrix_uniform_loc = glGetUniformLocation(program.id, "u_Proj");
  assert(projection_matrix_uniform_loc != -1);

  printf("Render initialization completed.\n");

  // renderer_submit(renderer, &render_command);
  while (!glfwWindowShouldClose(window.glfwHandle)) {
    glUseProgram(program.id);
    glUniformMatrix4fv(projection_matrix_uniform_loc, 1, GL_FALSE,
                       (float *)projection_matrix);

    renderer_draw(renderer);

    glfwSwapBuffers(window.glfwHandle);
    glfwPollEvents();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
