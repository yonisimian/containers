#include "set.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    Element data;
    struct Node* next;
} Node;

static Node* createNodeElement (Set set, Element data);
static void  removeNodeElement (Set set, Node* node);

struct set_t {
    Node* head;
    Node* iterator;
    int size;
    ElemCopyFunction copyElement;
    ElemFreeFunction freeElement;
    ElemEqualFunction equalElements;
};

Set setCreate(ElemCopyFunction copyElement,
              ElemFreeFunction freeElement,
              ElemEqualFunction equalElements)
{
    if(copyElement == NULL || freeElement == NULL || equalElements == NULL ) {
        return NULL; 
    }
    Set set = (Set)malloc(sizeof(*set));
    if(set == NULL) {
        return NULL;
    }
    set->size = 0;
    set->head = NULL; 
    set->iterator = NULL;
    set->copyElement = copyElement;
    set->freeElement = freeElement;
    set->equalElements = equalElements;

    return set;
}

Set setCopy(Set set)
{
    if(set == NULL) {
        return NULL;
    }
    Set new_set = setCreate(set->copyElement, set->freeElement, set->equalElements);
    if (new_set == NULL) {
        return NULL;
    }
    new_set->size = 0;
    Node* ptr = set->head;
    while (ptr != NULL) {
        if (setAdd(new_set, ptr->data) != SET_SUCCESS) {
            setDestroy(new_set);
            return NULL;
        }
        ptr = ptr->next;
    }
    new_set->iterator = NULL;
    new_set->copyElement = set->copyElement;
    new_set->freeElement = set->freeElement;
    new_set->equalElements = set->equalElements;

    return new_set;
}

void setDestroy(Set set)
{
    if(set == NULL) {
        return;
    }
    setClear(set);
    free(set);
}

SetResult setClear(Set set)
{
    if (set == NULL) {
        return SET_NULL_ARG;
    }

    while(set->head != NULL) {
        Node* ptr = set->head;
        set->head = set->head->next;
        removeNodeElement(set, ptr);
    }

    return SET_SUCCESS;
}

SetResult setAdd(Set set, Element element)
{
    if (set == NULL || element == NULL) {
        return SET_NULL_ARG;
    }

    if (setIsEmpty(set))
    {
        set->head = createNodeElement(set, element);
        return (set->head == NULL ? SET_OUT_OF_MEMORY : SET_SUCCESS);
    }

    Node* ptr = set->head;
    Node* next = ptr->next;

    while (next != NULL)
    {
        if (set->equalElements(ptr->data, element))
            return SET_ITEM_ALREADY_EXISTS;
        ptr = next;
        next = ptr->next;
    }

    ptr->next = createNodeElement(set, element);
    return (ptr->next == NULL ? SET_OUT_OF_MEMORY : SET_SUCCESS);

    return SET_SUCCESS;
}

SetResult setRemove(Set set, Element element)
{
    if(set == NULL || element == NULL) {
        return SET_NULL_ARG;
    }
    if (set->head == NULL) {
        return SET_ITEM_DOES_NOT_EXIST;
    }

    Node* ptr = set->head;
    if(set->equalElements(ptr->data , element)) {
        set->head = set->head->next;
        removeNodeElement(set , ptr);
        return SET_SUCCESS;
    }

    while(ptr->next != NULL) {
        if(set->equalElements(ptr->next->data , element)) {
            Node* next = ptr->next;
            ptr->next = next->next;
            removeNodeElement(set, next);
            return SET_SUCCESS;
        }
        ptr = ptr->next;
    }
    
    return SET_ITEM_DOES_NOT_EXIST;   
}

Element setFind(Set set, Element element)
{
    if(set == NULL || element == NULL) {
        return NULL;
    }
    Node* ptr = set->head; 
    while(ptr != NULL) {
        if((set->equalElements(ptr->data ,element))) {
            return set->copyElement(ptr->data);
        }
        ptr = ptr->next;
    }
    return NULL;
}

bool setContains(Set set, Element element)
{
    if(set == NULL || element == NULL) {
        return false;
    }

    Node* ptr = set->head;
    while(ptr != NULL) {
        if(set->equalElements(ptr->data , element)) {
            return true;
        }
        ptr = ptr->next;
    }

    return false;
}

bool setIsEmpty(Set set)
{
    return set && !set->size;
}

int setGetSize(Set set)
{
    if (set == NULL) {
        return 0;
    }

    return set->size;
}

Set setUnion(Set set1, Set set2)
{
    if (set1 == NULL && set2 == NULL) {
        return NULL;
    }
    if (set1 == NULL) {
        return setCopy(set2);
    }
    if (set2 == NULL) {
        return setCopy(set1);
    }
    if (set1->copyElement != set2->copyElement ||
        set1->freeElement != set2->freeElement ||
        set1->equalElements != set2->equalElements) {
            return NULL;
        }

    Set set = setCreate(set1->copyElement, set1->freeElement, set1->equalElements);

    Node* ptr = set1->head;
    while (ptr != NULL) {
        setAdd(set, ptr->data);
        ptr = ptr->next;
    }

    ptr = set2->head;
    while (ptr != NULL) {
        setAdd(set, ptr->data);
        ptr = ptr->next;
    }

    return set;
}

Set setIntersection(Set set1, Set set2)
{
    if (set1 == NULL && set2 == NULL) {
        return NULL;
    }
    if (set1 == NULL) {
        return setCreate(set2->copyElement, set2->freeElement, set2->equalElements);
    }
    if (set2 == NULL) {
        return setCreate(set1->copyElement, set1->freeElement, set1->equalElements);
    }
    if (set1->copyElement != set2->copyElement ||
        set1->freeElement != set2->freeElement ||
        set1->equalElements != set2->equalElements) {
            return NULL;
        }

    Set set = setCreate(set1->copyElement, set1->freeElement, set1->equalElements);

    Node* ptr = set1->head;
    while (ptr != NULL) {
        if (setContains(set2, ptr->data))
            setAdd(set, ptr->data);
        ptr = ptr->next;
    }

    ptr = set2->head;
    while (ptr != NULL) {
        if (setContains(set1, ptr->data))
            setAdd(set, ptr->data);
        ptr = ptr->next;
    }

    return set;
}

Set setFilter(Set set, ElemConditionFunction condition, void* param)
{
    if (set == NULL || condition == NULL) {
        return NULL;
    }

    Set new_set = setCreate(set->copyElement, set->freeElement, set->equalElements);

    Node* ptr = set->head;
    while (ptr != NULL) {
        if (condition(ptr->data, param))
            setAdd(new_set, ptr->data);
        ptr = ptr->next;
    }

    return new_set;
}

Element setGetFirst(Set set)
{
    if (set == NULL || set->head == NULL) {
        return NULL;
    }

    set->iterator = set->head;
    return set->copyElement(set->iterator->data);
}

Element setGetNext(Set set)
{
    if (set == NULL || set->iterator == NULL || set->iterator->next == NULL) {
        return NULL;
    }

    set->iterator = set->iterator->next;
    return set->copyElement(set->iterator->data);
}

static Node* createNodeElement(Set set, Element element)
{
    Node* new_node = (Node*)malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = set->copyElement(element);
    if(new_node->data == NULL) {
        free(new_node);
        return NULL;
    }

    set->size++;

    return new_node;
}

static void removeNodeElement(Set set, Node* node)
{
    set->freeElement(node->data);
    set->size--;
    free(node);
}
