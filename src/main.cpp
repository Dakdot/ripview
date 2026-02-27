// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <spdlog/spdlog.h>

#include "Material.hpp"
#include "Renderer.hpp"
#include "SceneObject.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Window.hpp"

#include <cstdlib>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <time.h>
// clang-format on

#define GL_CHECK_ERROR() assert(glGetError() == 0)

int main() {
  spdlog::set_level(spdlog::level::trace);

  const char *rootDirCStr = std::getenv("RIPVIEW_ROOT_DIR");
  if (!rootDirCStr) {
    spdlog::critical("Environment variable RIPVIEW_ROOT_DIR is not set.");
    return EXIT_FAILURE;
  }

  const std::string rootDir(rootDirCStr);

  Ripview::Window window(800, 600, "Ripview");

  Ripview::Renderer renderer;

  Ripview::Shader vertex(rootDir + "/assets/shaders/main.vert",
                         GL_VERTEX_SHADER);
  Ripview::Shader fragment(rootDir + "/assets/shaders/main.frag",
                           GL_FRAGMENT_SHADER);
  Ripview::Shader lampFrag(rootDir + "/assets/shaders/lamp.frag",
                           GL_FRAGMENT_SHADER);

  Ripview::ShaderProgram program("Main");
  program.AttachShader(vertex);
  program.AttachShader(fragment);
  program.Link();

  Ripview::ShaderProgram lampProgram("Lamp");
  program.AttachShader(vertex);
  program.AttachShader(lampFrag);
  program.Link();

  glm::vec3 c1 = {1.0f, 0.0f, 1.0f};
  glm::vec3 c2 = {0.992, 0.706, 0.082};
  glm::vec3 c3 = {0.22, 0.757, 0.114};
  glm::vec3 spec = {0.5, 0.5, 0.5};

  Ripview::Material mtr1 = {.program = program,
                            .ambient = c1,
                            .diffuse = c1,
                            .specular = spec,
                            .shininess = 0.7f};

  Ripview::Material lampMaterial = {
      .program = lampProgram,
  };

  Ripview::SceneObject o1(rootDir + "/assets/models/glTF2/Lantern.glb");
  o1.mMaterial = &mtr1;

  // rvSceneObject *o2 = scene_object_load_from_file(
  //     "/Users/thiagoandrade/Projects/experiments/ripview/assets/"
  //     "models/glTF2/Fox.glb");
  // rvSceneObject *o3 =
  //     scene_object_load_from_file("/Users/thiagoandrade/Projects/experiments/"
  //                                 "ripview/assets/models/glTF2/Avocado.glb");

  // rvSceneObject *lamp =
  //     scene_object_load_from_file("/Users/thiagoandrade/Projects/experiments/"
  //                                 "ripview/assets/models/glTF2/Box.glb");

  // scene_object_attach_material(o1, mtr1);
  // scene_object_attach_material(o2, mtr2);
  // scene_object_attach_material(o3, mtr3);
  // scene_object_attach_material(lamp, lampMaterial);

  // scene_object_set_position(o1, 0, 0, 0);

  // scene_object_set_uniform_scale(o2, 0.1);
  // scene_object_set_position(o2, 10, -12, 10);

  // scene_object_set_position(o3, -10, -12, 10);
  // scene_object_set_uniform_scale(o3, 100);

  // scene_object_set_uniform_scale(lamp, 5);
  // scene_object_set_position(lamp, 25, 15, 25);

  Ripview::Camera camera(0.1f, 500.0f,
                         Ripview::Camera::CalculateAspectRatio(800, 600), 1.13);

  printf("Render initialization completed.\n");

  while (!glfwWindowShouldClose(window.glfwHandle)) {
    renderer.Draw(o1, camera);

    glfwSwapBuffers(window.glfwHandle);
    glfwPollEvents();

    double now = glfwGetTime();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
