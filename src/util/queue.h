#pragma once

#include <stdbool.h>

typedef struct rvQueueNode {
  void *data;
  struct rvQueueNode *next;
} rvQueueNode;

typedef struct {
  rvQueueNode *front;
  rvQueueNode *back;
} rvQueue;

void queue_init(rvQueue *queue);
void queue_destroy(rvQueue *queue);
bool queue_enqueue(rvQueue *queue, void *element);
void *queue_dequeue(rvQueue *queue);
void *queue_peek(rvQueue *queue);
bool queue_is_empty(rvQueue *queue);
