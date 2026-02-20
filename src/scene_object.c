#include "scene_object.h"
#include "material.h"
#include "mesh.h"
#include "util/queue.h"
#include <stdlib.h>

rvSceneObject *scene_object_create(void) {
  rvSceneObject *o = malloc(sizeof(rvSceneObject));
  queue_init(&o->meshes);
  return o;
}

void scene_object_destroy(rvSceneObject *o) {
  if (!o)
    return;
  free(o);
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
