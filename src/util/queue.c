#include "queue.h"
#include <stdbool.h>
#include <stdlib.h>

void queue_init(rvQueue *queue) {
  if (!queue)
    return;
  queue->front = NULL;
  queue->back = NULL;
}

void queue_destroy(rvQueue *queue) {
  if (!queue)
    return;
  while (!queue_is_empty(queue))
    queue_dequeue(queue);
}

bool queue_enqueue(rvQueue *queue, void *element) {
  if (!queue)
    return false;
  rvQueueNode *node = malloc(sizeof(rvQueueNode));
  if (!node)
    return false;
  node->data = element;
  node->next = NULL;

  if (queue_is_empty(queue)) {
    queue->front = node;
    queue->back = node;
  } else {
    queue->back->next = node;
    queue->back = node;
  }

  return true;
}

void *queue_dequeue(rvQueue *queue) {
  if (!queue || queue_is_empty(queue))
    return NULL;
  rvQueueNode *temp = queue->front;
  void *data = temp->data;
  queue->front = queue->front->next;
  free(temp);
  if (queue->front == NULL)
    queue->back = NULL;

  return data;
}

void *queue_peek(rvQueue *queue) {
  if (!queue || queue_is_empty(queue))
    return NULL;
  return queue->front->data;
}

bool queue_is_empty(rvQueue *queue) {
  if (!queue)
    return true;
  return (queue->back == NULL) && (queue->front == NULL);
}
