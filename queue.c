#include <limits.h>
#include <stdlib.h>
#include "queue.h"

void initQueue(struct Queue* queue)
{
  queue->start = NULL;
  queue->end = NULL;
  queue->size = 0;
}

void queuePush(struct Queue* queue, struct Vector2 value)
{
  if(queue->start == NULL) {
    queue->start = (struct QueueNode*) malloc(sizeof(struct QueueNode));
    queue->start->value = value;
    queue->start->next = NULL;
    queue->start->prev = NULL;
    queue->end = queue->start;
    ++queue->size;
  } else {
    queue->end->next = (struct QueueNode*) malloc(sizeof(struct QueueNode));
    queue->end->next->value = value;
    queue->end->next->next = NULL;
    queue->end->next->prev = queue->end;
    queue->end = queue->end->next;
    ++queue->size;
  }
}

struct Vector2 queuePop(struct Queue* queue)
{
  if(queue->start == NULL) {
    struct Vector2 ret;
    ret.x = INT_MIN;
    ret.y = INT_MIN;
    return ret;
  }

  struct QueueNode* start = queue->start;

  if(queue->start == queue->end) {
    queue->start = NULL;
    queue->end = NULL;
  } else {
    queue->start = queue->start->next;
  }

  struct Vector2 ret = start->value;
  free(start);

  --queue->size;

  return ret;
}

void queueDelete(struct Queue* queue)
{
  while(queueSize(queue))
    queuePop(queue);
}

int queueSize(struct Queue* queue)
{
  return queue->size;
}

struct Vector2 queueTop(struct Queue* queue)
{
  return queue->start->value;
}

struct Vector2 queueBottom(struct Queue* queue)
{
  return queue->end->value;
}
