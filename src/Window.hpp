#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
// clang-format on

namespace Ripview {
struct Window {
  Window(int width, int height, std::string title);

  int width;
  int height;
  std::string title;
  GLFWwindow *glfwHandle;
};
} // namespace Ripview
