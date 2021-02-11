#ifndef QUEUE_H
#define QUEUE_H

#include "vector2.h"

struct QueueNode
{
  struct Vector2    value;
  struct QueueNode* next;
  struct QueueNode* prev;
};

struct Queue
{
  struct QueueNode* start;
  struct QueueNode* end;
  int               size;
};

void initQueue(struct Queue* queue);
void queuePush(struct Queue* queue, struct Vector2 value);
struct Vector2 queuePop(struct Queue* queue);
void queueDelete(struct Queue* queue);
int queueSize(struct Queue* queue);

struct Vector2 queueTop(struct Queue* queue);
struct Vector2 queueBottom(struct Queue* queue);

#endif // QUEUE_H
