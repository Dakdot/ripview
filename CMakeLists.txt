cmake_minimum_required(VERSION 3.13)

#################################
#                               #
#         CMAKE OPTIONS         #
#                               #
#################################

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

# cglm
add_subdirectory(vendor/cglm)

# GLAD
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Assimp
find_package(assimp REQUIRED)


#################################
#                               #
#        RIPVIEW OPTIONS        #
#                               #
#################################

project(ripview)

# SOURCES:
add_executable(${PROJECT_NAME} src/main.c)

target_sources(${PROJECT_NAME} PRIVATE
    src/main.c

    src/shader.c
    src/shader.h
    src/image.c
    src/image.h
    src/window.c
    src/window.h
    src/mesh.c
    src/mesh.h
    src/renderer.c
    src/renderer.h
    src/camera.c
    src/camera.h
    src/scene.c
    src/scene.h
    src/scene_object.h
    src/scene_object.c
    src/material.h
    src/material.c

    src/util/queue.c
    src/util/queue.h
    src/util/tree.c
    src/util/tree.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    vendor/glfw/include
    vendor/glad/include
    vendor/stb
    vendor/cglm/include
    vendor/assimp/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad assimp::assimp)
