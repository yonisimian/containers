#ifndef SET_H_
#define SET_H_

#include <stdbool.h>

typedef void* Element;
typedef Element (*ElemCopyFunction)(Element);
typedef void (*ElemFreeFunction)(Element);
typedef bool (*ElemEqualFunction)(Element a, Element b);  // return a == b
typedef bool (*ElemConditionFunction)(Element, void* param);

typedef struct set_t* Set;

typedef enum {
    SET_SUCCESS,
    SET_OUT_OF_MEMORY,
    SET_NULL_ARG
} SetResult;

Set setCreate(ElemCopyFunction, ElemFreeFunction, ElemEqualFunction);
Set setCopy(Set set);
void setDestroy(Set set);
SetResult setAdd(Set set, Element element);
SetResult setRemove(Set set, Element element);
Element setContains(Set set, Element element);
Element setFind(Set set, Element element);
int setGetSize(Set set);
bool setIsEmpty(Set set);
Set setUnion(Set set1, Set set2);
Set setIntersection(Set set1, Set set2);
Set setFilter(Set set, ElemConditionFunction condition, void* param);
Element setGetFirst(Set set); // returns NULL if set is empty
Element setGetNext(Set set); // returns NULL if no more elements

// Macro to enable simple iteration
#define SET_FOREACH(Type, element, set) \
    for (Type element = setGetFirst(set); \
        element != NULL; \
        element = setGetNext(set))

#endif /* SET_H_ */ 
