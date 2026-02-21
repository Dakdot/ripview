#include "scene_object.h"
#include "cglm/vec3.h"
#include "material.h"
#include "mesh.h"
#include "util/queue.h"
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
