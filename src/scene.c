#include "scene.h"
#include "scene_object.h"
#include "util/tree.h"
#include <stdlib.h>

rvScene *scene_create(void) {
  rvScene *s = malloc(sizeof(rvScene));
  s->objects.capacity = 0;
  s->objects.children = NULL;
  s->objects.num_children = 0;
  s->objects.data = 0;
  return s;
}

void scene_destroy(rvScene *s) { free(s); }

void scene_add_object(rvScene *s, rvSceneObject *o) {
  if (!s || !o)
    return;

  rvTreeNode *node = tree_create_node(o);
  tree_add_child(&s->objects, node);
}
