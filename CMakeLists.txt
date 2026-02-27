cmake_minimum_required(VERSION 3.13)

project(ripview LANGUAGES C CXX)

#################################
#                               #
#         CMAKE OPTIONS         #
#                               #
#################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON CACHE BOOL
    "Enable/Disable output of compile commands during generation."
    FORCE)
set(CMAKE_BUILD_TYPE Debug)
set(BUILD_WITH_DEBUG ON)
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

#################################
#                               #
#       SET UP LIBRARIES        #
#                               #
#################################

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# GLAD
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Assimp
find_package(assimp REQUIRED)

# spdlog
find_package(spdlog REQUIRED)

# GLM
add_subdirectory(vendor/glm)


#################################
#                               #
#        RIPVIEW OPTIONS        #
#                               #
#################################

# SOURCES:
add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
    src/Camera.hpp
    src/Material.hpp
    src/Mesh.cpp
    src/Mesh.hpp
    src/Renderer.cpp
    src/Renderer.hpp
    src/SceneObject.cpp
    src/SceneObject.hpp
    src/Shader.cpp
    src/Shader.hpp
    src/ShaderProgram.cpp
    src/ShaderProgram.hpp
    src/Window.cpp
    src/Window.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    vendor/glfw/include
    vendor/glad/include
    vendor/stb
    vendor/cglm/include
    vendor/assimp/include
)

target_include_directories(${PROJECT_NAME} PRIVATE SYSTEM
    vendor/glm
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad assimp::assimp spdlog::spdlog)
set_property(TARGET assimp::assimp PROPERTY SYSTEM TRUE)
set_property(TARGET spdlog::spdlog PROPERTY SYSTEM TRUE)
