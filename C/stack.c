#include "stack.h"

#include <stdlib.h>

typedef struct node_t {
    Element data;
    struct node_t* next;
} Node;

static Node* createNode(Stack stack, Element element);

struct stack_t {
    Node* head;
    int size;
    ElemCopyFunction copyElement;
    ElemFreeFunction freeElement;
};

Stack stackCreate(ElemCopyFunction copyElement, ElemFreeFunction freeElement)
{
    if (copyElement == NULL || freeElement == NULL) {
        return NULL; 
    }
    Stack stack = (Stack)malloc(sizeof(*stack));
    if (stack == NULL) {
        return NULL;
    }
    stack->size = 0;
    stack->head = NULL; 

    stack->copyElement = copyElement;
    stack->freeElement = freeElement;

    return stack;
}

void stackDestroy(Stack stack)
{
    if (stack == NULL)
        return;

    while (!stackIsEmpty(stack)) 
        stackPop(stack);
    
    free(stack);
}

Stack stackCopy(Stack stack)
{
    if (stack == NULL)
        return NULL;
    
    Stack new_stack = stackCreate(stack->copyElement, stack->freeElement);
    if (new_stack == NULL) {
        return NULL;
    }
    new_stack->size = 0;
    Node* ptr = stack->head;
    Node* new_ptr = NULL;
    while (ptr != NULL) {
        Node* node = createNode(stack, ptr->data);
        if (node == NULL) {
            stackDestroy(new_stack);
            return NULL;
        }
        if (new_ptr == NULL)
        {
            new_ptr = node;
            stack->head = new_ptr;
        }
        else
        {
            new_ptr->next = node;
            new_ptr = new_ptr->next;
        }
        ptr = ptr->next;
    }
    
    new_stack->copyElement = stack->copyElement;
    new_stack->freeElement = stack->freeElement;

    return new_stack;
}

static Node* createNode(Stack stack, Element element)
{
    Node* new_node = (Node*)malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    
    new_node->data = stack->copyElement(element);
    if(new_node->data == NULL) {
        free(new_node);
        return NULL;
    }

    new_node->next = NULL;
    stack->size++;

    return new_node;
}

StackResult stackPush(Stack stack, Element element)
{
    if (stack == NULL || element == NULL) {
        return STACK_NULL_ARG;
    }

    Node* node = createNode(stack, element);
    if (node == NULL) {
        return STACK_OUT_OF_MEMORY;
    }
  
    node->next = stack->head;
    stack->head = node;

    return STACK_SUCCESS;
}

StackResult stackPop(Stack stack)
{
    if (stack == NULL) {
        return STACK_NULL_ARG;
    }

    if (stackIsEmpty(stack)) {
        return STACK_IS_EMPTY;
    }
  
    Node* to_remove = stack->head;
    stack->head = stack->head->next;

    stack->freeElement(to_remove->data);
    free(to_remove);
    stack->size--;

    return STACK_SUCCESS;
}

StackResult stackTop(Stack stack, Element* element)
{
    if (stack == NULL || element == NULL) {
        return STACK_NULL_ARG;
    }

    if (stackIsEmpty(stack)) {
        return STACK_IS_EMPTY;
    }
  
    *element = stack->head->data;

    return STACK_SUCCESS;
}

int stackGetSize(Stack stack)
{
    return (stack == NULL ? 0 : stack->size);
}

bool stackIsEmpty(Stack stack)
{
    return stack && !stack->size;
}
