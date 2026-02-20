#pragma once

#include "scene_object.h"
#include "util/tree.h"

typedef struct rvScene {
  rvTreeNode objects;
} rvScene;

rvScene *scene_create(void);
void scene_destroy(rvScene *);

void scene_add_object(rvScene *, rvSceneObject *);
