#pragma once

#include "material.h"
#include "mesh.h"
#include "transform.h"
#include "util/queue.h"

typedef struct rvSceneObject {
  rvTransform transform;
  rvQueue meshes;
  rvMaterial *material;
} rvSceneObject;

rvSceneObject *scene_object_create();
void scene_object_destroy(rvSceneObject *);
rvSceneObject *scene_object_load_from_file(const char *filepath);
void scene_object_attach_mesh(rvSceneObject *, rvMesh *);
void scene_object_attach_material(rvSceneObject *, rvMaterial *);
void scene_object_set_position(rvSceneObject *o, float x, float y, float z);
void scene_object_set_uniform_scale(rvSceneObject *o, float s);
