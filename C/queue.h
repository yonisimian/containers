#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>

typedef void* Element;
typedef Element (*ElemCopyFunction)(Element);
typedef void (*ElemFreeFunction)(Element);

typedef struct queue_t* Queue;
typedef enum {
    QUEUE_SUCCESS,
    QUEUE_OUT_OF_MEMORY,
    QUEUE_NULL_ARG,
    QUEUE_IS_EMPTY
} QueueResult;

Queue queueCreate(ElemCopyFunction, ElemFreeFunction);
Queue queueCopy(Queue queue);
void queueDestroy(Queue queue);
QueueResult queueEnqueue(Queue queue, Element element);
QueueResult queueDequeue(Queue queue);
QueueResult queueFront(Queue queue, Element* element);
int queueGetSize(Queue queue);
bool queueIsEmpty(Queue queue);

#endif /* QUEUE_H_ */
