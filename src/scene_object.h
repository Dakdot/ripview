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
void scene_object_attach_mesh(rvSceneObject *, rvMesh *);
void scene_object_attach_material(rvSceneObject *, rvMaterial *);
