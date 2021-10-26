#ifndef STACK_H_
#define STACK_H_

#include <stdbool.h>

typedef void* Element;
typedef Element (*ElemCopyFunction)(Element);
typedef void (*ElemFreeFunction)(Element);

typedef struct stack_t* Stack;

typedef enum {
    STACK_SUCCESS,
    STACK_OUT_OF_MEMORY,
    STACK_NULL_ARG,
    STACK_IS_EMPTY
} StackResult;

Stack stackCreate(ElemCopyFunction, ElemFreeFunction);
Stack stackCopy(Stack stack);
void stackDestroy(Stack stack);
StackResult stackPush(Stack stack, Element element);
StackResult stackPop(Stack stack);
StackResult stackTop(Stack stack, Element* element);
int stackGetSize(Stack stack);
bool stackIsEmpty(Stack stack);

#endif /* STACK_H_ */
