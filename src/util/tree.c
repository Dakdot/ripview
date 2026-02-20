#include "tree.h"

#include <stdint.h>
#include <stdlib.h>

rvTreeNode *tree_create_node(void *data) {
  rvTreeNode *n = malloc(sizeof(rvTreeNode));
  if (!n)
    return NULL;
  n->data = data;
  n->children = NULL;
  n->num_children = 0;
  n->capacity = 0;
  return n;
}

void tree_destroy(rvTreeNode *node) {}

void tree_add_child(rvTreeNode *parent, rvTreeNode *child) {
  if (parent->num_children + 1 > parent->capacity) {
    parent->capacity = parent->capacity == 0 ? 4 : parent->capacity * 2;
    parent->children =
        realloc(parent->children, parent->capacity * sizeof(rvTreeNode *));
  }
  parent->children[parent->num_children++] = child;
}
