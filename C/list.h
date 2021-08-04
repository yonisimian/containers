#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

typedef void* Element;
typedef Element (*ElemCopyFunction)(Element);
typedef void (*ElemFreeFunction)(Element);
typedef int (*ElemCompareFunction)(Element a, Element b);
typedef bool (*ElemConditionFunction)(Element, void* param);
typedef Element (*ElemApplyFunction)(Element);

typedef struct list_t* List;

typedef enum {
    LIST_SUCCESS,
    LIST_OUT_OF_MEMORY,
    LIST_NULL_ARG,
    LIST_INVALID_CURRENT
} ListResult;

List listCreate(ElemCopyFunction, ElemFreeFunction);
List listCopy(List list);
void listDestroy(List list);
Element listGetFirst(List list); // returns NULL if list is empty
Element listGetNext(List list); // returns NULL if no more elements
Element listGetCurrent(List list);
ListResult listInsertFirst(List list, Element element);
ListResult listInsertLast(List list, Element element);
ListResult listInsertBeforeCurrent(List list, Element element);
ListResult listInsertAfterCurrent(List list, Element element);
ListResult listRemoveCurrent(List list);
void listClear(List list);
int listGetSize(List list);
bool listIsEmpty(List list);
ListResult listSort(List list, ElemCompareFunction compare);
ListResult listApply(List list, ElemApplyFunction function);
List listFilter(List list, ElemConditionFunction condition, void* param);

// Macro to enable simple iteration
#define LIST_FOREACH(Type, element, list) \
    for (Type element = listGetFirst(list); \
        element != NULL; \
        element = listGetNext(list))

#endif /* LIST_H_ */ 
