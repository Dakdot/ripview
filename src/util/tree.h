#pragma once

#include <stdint.h>

typedef struct rvTreeNode {
  void *data;
  struct rvTreeNode **children;
  uint32_t num_children;
  uint32_t capacity;
} rvTreeNode;

rvTreeNode *tree_create_node(void *);
void tree_destroy(rvTreeNode *);
void tree_add_child(rvTreeNode *, rvTreeNode *);
