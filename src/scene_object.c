#include "scene_object.h"
#include "material.h"
#include "mesh.h"
#include "util/queue.h"
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>
#include <stdlib.h>

rvSceneObject *scene_object_create(void) {
  rvSceneObject *o = malloc(sizeof(rvSceneObject));
  queue_init(&o->meshes);
  o->transform.position[0] = 0;
  o->transform.position[1] = 0;
  o->transform.position[2] = 0;
  o->transform.scale[0] = 1.0f;
  o->transform.scale[1] = 1.0f;
  o->transform.scale[2] = 1.0f;
  o->transform.rotation[0] = 0;
  o->transform.rotation[1] = 0;
  o->transform.rotation[2] = 0;
  return o;
}

void scene_object_destroy(rvSceneObject *o) {
  if (!o)
    return;
  free(o);
}

rvSceneObject *scene_object_load_from_file(const char *filepath) {
  rvSceneObject *o = scene_object_create();

  const struct aiScene *aiScene = aiImportFile(
      filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_GenNormals | aiProcess_JoinIdenticalVertices |
                    aiProcess_SortByPType);

  if (!aiScene) {
    perror("Failed to import model.\n");
    perror(aiGetErrorString());
    return NULL;
  }

  int counter = 0;
  for (int i = 0; i < aiScene->mNumMeshes; i++) {
    struct aiMesh *aiMesh = aiScene->mMeshes[i];
    rvMesh *rvMesh = mesh_create();
    mesh_upload(rvMesh, aiMesh);
    scene_object_attach_mesh(o, rvMesh);

    counter++;
  }
  printf("Added %d meshes.\n", counter);

  aiReleaseImport(aiScene);
  return o;
}

void scene_object_attach_mesh(rvSceneObject *o, rvMesh *m) {
  if (!o || !m)
    return;
  queue_enqueue(&o->meshes, m);
}

void scene_object_attach_material(rvSceneObject *o, rvMaterial *m) {
  if (!o || !m)
    return;
  o->material = m;
}

void scene_object_set_position(rvSceneObject *o, float x, float y, float z) {
  o->transform.position[0] = x;
  o->transform.position[1] = y;
  o->transform.position[2] = z;
}

void scene_object_set_uniform_scale(rvSceneObject *o, float s) {
  o->transform.scale[0] = s;
  o->transform.scale[1] = s;
  o->transform.scale[2] = s;
}
