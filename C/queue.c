#include "queue.h"

#include <stdlib.h>

typedef struct node_t {
    Element data;
    struct node_t* next;
} Node;

static Node* createNode(Queue queue, Element element);

struct queue_t {
    Node* front;
    Node* rear;
    int size;
    ElemCopyFunction copyElement;
    ElemFreeFunction freeElement;
};

Queue queueCreate(ElemCopyFunction copyElement, ElemFreeFunction freeElement)
{
    if (copyElement == NULL || freeElement == NULL) {
        return NULL; 
    }
    Queue queue = (Queue)malloc(sizeof(*queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->size = 0;
    queue->front = NULL;
    queue->rear = NULL; 

    queue->copyElement = copyElement;
    queue->freeElement = freeElement;

    return queue;
}

void queueDestroy(Queue queue)
{
    if (queue == NULL)
        return;

    while (!queueIsEmpty(queue)) 
        queueDequeue(queue);
    
    free(queue);
}

Queue queueCopy(Queue queue)
{
    if (queue == NULL)
        return NULL;
    
    Queue new_queue = queueCreate(queue->copyElement, queue->freeElement);
    if (new_queue == NULL) {
        return NULL;
    }
    new_queue->size = 0;
    Node* ptr = queue->front;
    Node* new_ptr = NULL;
    while (ptr != NULL) {
        if (queueEnqueue(new_queue, ptr->data) != QUEUE_SUCCESS)
        {
            queueDestroy(new_queue);
            return NULL;
        }
        ptr = ptr->next;
    }
    
    new_queue->copyElement = queue->copyElement;
    new_queue->freeElement = queue->freeElement;

    return new_queue;
}

QueueResult queueEnqueue(Queue queue, Element element)
{
    if (queue == NULL || element == NULL) {
        return QUEUE_NULL_ARG;
    }

    Node* node = (Node*)malloc(sizeof(*node));
    if (node == NULL) {
        return QUEUE_OUT_OF_MEMORY;
    }
    
    node->data = queue->copyElement(element);
    if(node->data == NULL) {
        free(node);
        return QUEUE_OUT_OF_MEMORY;
    }
    node->next = NULL;

    if (++queue->size == 1)
    {
        queue->front = node;
    }
    else
        queue->rear->next = node;
    queue->rear = node;

    return QUEUE_SUCCESS;
}

QueueResult queueDequeue(Queue queue)
{
    if (queue == NULL) {
        return QUEUE_NULL_ARG;
    }

    if (queueIsEmpty(queue)) {
        return QUEUE_IS_EMPTY;
    }
  
    Node* to_remove = queue->front;
    queue->front = queue->front->next;

    queue->freeElement(to_remove->data);
    free(to_remove);
    if (--queue->size == 0)
        queue->rear = NULL;

    return QUEUE_SUCCESS;
}

QueueResult queueFront(Queue queue, Element* element)
{
    if (queue == NULL || element == NULL) {
        return QUEUE_NULL_ARG;
    }

    if (queueIsEmpty(queue)) {
        return QUEUE_IS_EMPTY;
    }
  
    *element = queue->front->data;

    return QUEUE_SUCCESS;
}

int queueGetSize(Queue queue)
{
    return (queue == NULL ? 0 : queue->size);
}

bool queueIsEmpty(Queue queue)
{
    return queue && !queue->size;
}
