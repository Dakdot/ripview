#include "window.h"
#include "GLFW/glfw3.h"

#include <stdio.h>

void window_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int create_window(rvWindow *window) {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW.\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  GLFWwindow *glfwWindow;
  glfwWindow = glfwCreateWindow(window->width, window->height, window->title,
                                NULL, NULL);

  if (!glfwWindow) {
    glfwTerminate();
    printf("Failed to create window.\n");
    return -2;
  }
  window->glfwHandle = glfwWindow;

  glfwMakeContextCurrent(glfwWindow);
  glfwSwapInterval(1);

  int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (version == 0) {
    printf("Failed to create OpenGL context.\n");
    return -3;
  }

  glfwSetWindowSizeCallback(glfwWindow, window_size_callback);

  char *GL_version = (char *)glGetString(GL_VERSION);
  char *GL_vendor = (char *)glGetString(GL_VENDOR);
  char *GL_renderer = (char *)glGetString(GL_RENDERER);

  printf("Created OpenGL context.\nVersion: %s\nVendor: %s\nVendor: %s\n",
         GL_version, GL_vendor, GL_renderer);

  glViewport(0, 0, window->width, window->height);

  return 0;
}
