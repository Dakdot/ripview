#pragma once

// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

typedef struct {
  int width;
  int height;
  const char *title;
  GLFWwindow *glfwHandle;
} rvWindow;

int create_window(rvWindow *window);
