#include "scene.h"
#include "scene_object.h"
#include "util/tree.h"
#include <stdlib.h>

rvScene *scene_create(void) {
  rvScene *s = malloc(sizeof(rvScene));
  return s;
}

void scene_destroy(rvScene *s) { free(s); }

void scene_add_object(rvScene *s, rvSceneObject *o) {
  if (!s || !o)
    return;

  rvTreeNode *node = tree_create_node(o);
  tree_add_child(&s->objects, node);
}
