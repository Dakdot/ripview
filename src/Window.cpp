#include "Window.hpp"
#include "GLFW/glfw3.h"

#include <cstddef>
#include <spdlog/spdlog.h>
#include <string>

namespace Ripview {
Window::Window(int width, int height, std::string title)
    : width(width), height(height), title(title) {
  if (!glfwInit()) {
    spdlog::critical("Failed to initialize GLFW.");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  glfwHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

  if (!glfwHandle) {
    glfwTerminate();
    spdlog::critical("Failed to create window.");
    return;
  }

  glfwMakeContextCurrent(glfwHandle);
  glfwSwapInterval(1);

  int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (version == 0) {
    spdlog::critical("Failed to create OpenGL context.");
    return;
  }

  std::string GL_version = (const char *)glGetString(GL_VERSION);
  std::string GL_vendor = (const char *)glGetString(GL_VENDOR);
  std::string GL_renderer = (const char *)glGetString(GL_RENDERER);

  glEnable(GL_DEPTH_TEST);

  spdlog::info("Created OpenGL context.\nVersion: {}\nVendor: {}\nRenderer: {}",
               GL_version, GL_vendor, GL_renderer);

  glViewport(0, 0, width, height);
}

} // namespace Ripview
