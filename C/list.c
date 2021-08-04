#include "list.h"

#include <stdlib.h>

typedef struct node_t {
    Element data;
    struct node_t* next;
} Node;

static void removeNodeElement (List list, Node* node);
static Node* createNode(List list, Element element);

struct list_t {
    Node* head;
    Node* iterator;
    int size;
    ElemCopyFunction copyElement;
    ElemFreeFunction freeElement;
};

List listCreate(ElemCopyFunction copyElement, ElemFreeFunction freeElement)
{
    if(copyElement == NULL || freeElement == NULL) {
        return NULL; 
    }
    List list = (List)malloc(sizeof(*list));
    if(list == NULL) {
        return NULL;
    }
    list->size = 0;
    list->head = NULL; 

    list->iterator = NULL;
    list->copyElement = copyElement;
    list->freeElement = freeElement;

    return list;
}

void listDestroy(List list)
{
    if(list == NULL) {
        return;
    }
    listClear(list);
    free(list);
}

List listCopy(List list)
{
    if(list == NULL) {
        return NULL;
    }
    List new_list = listCreate(list->copyElement, list->freeElement);
    if (new_list == NULL) {
        return NULL;
    }
    new_list->size = 0;
    Node* ptr = list->head;
    while (ptr != NULL) {
        if (listInsertLast(new_list, ptr->data) != LIST_SUCCESS) { // TODO: replace with something more efficient
            listDestroy(new_list);
            return NULL;
        }
        ptr = ptr->next;
    }
    new_list->iterator = NULL;
    new_list->copyElement = list->copyElement;
    new_list->freeElement = list->freeElement;

    return new_list;
}

int listGetSize(List list)
{
    if (list == NULL) {
        return 0;
    }

    return list->size;
}

ListResult listInsertFirst(List list, Element element)
{
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARG;
    }

    Node* node = createNode(list, element);
    if (node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }

    Node* next = list->head;
    node->next = next;
    list->head = node;

    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, Element element)
{
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARG;
    }

    if (list->size == 0) {
        return listInsertFirst(list, element);
    }

    Node* node = createNode(list, element);
    if (node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }

    Node* last = list->head;
    for (; last->next != NULL; last = last->next);
    last->next = node;

    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, Element element)
{
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARG;
    }

    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }

    if (list->iterator == list->head) {
        return listInsertFirst(list, element);
    }

    Node* node = createNode(list, element);
    if (node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }

    Node* last = list->head;
    for (; last->next != list->iterator; last = last->next);
    node->next = last->next;
    last->next = node;

    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, Element element)
{
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARG;
    }

    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }

    Node* node = createNode(list, element);
    if (node == NULL) {
        return LIST_OUT_OF_MEMORY;
    }

    node->next = list->iterator->next;
    list->iterator->next = node;

    return LIST_SUCCESS;
}

static Node* createNode(List list, Element element)
{
    Node* new_node = (Node*)malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    
    new_node->data = list->copyElement(element);
    if(new_node->data == NULL) {
        free(new_node);
        return NULL;
    }

    new_node->next = NULL;
    list->iterator = new_node;
    list->size++;

    return new_node;
}

Element listGetFirst(List list)
{
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    list->iterator = list->head;
    return list->iterator->data;
}

Element listGetNext(List list)
{
    if (list == NULL || list->iterator == NULL || list->iterator->next == NULL) {
        return NULL;
    }

    list->iterator = list->iterator->next;
    return list->iterator->data;
}

Element listGetCurrent(List list)
{
    if(list == NULL) {
        return NULL;
    }
    return list->iterator->data;
}

ListResult listRemoveCurrent(List list)
{
    if(list == NULL) {
        return LIST_NULL_ARG;
    }
    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }

    Node* ptr = list->head;
    if(list->iterator = list->head) {
        list->head = list->head->next;
        list->iterator = list->head;
        removeNodeElement(list, ptr);
    }
    else {
        for (; ptr->next != list->iterator; ptr = ptr->next);
        ptr->next = list->iterator->next;

        Node* to_remove = list->iterator;
        removeNodeElement(list, to_remove);
        list->iterator = ptr->next;
    }

    return LIST_SUCCESS;
}

void listClear(List list)
{
    if (list == NULL) {
        return;
    }

    while(list->head != NULL) {
        Node* ptr = list->head;
        list->head = list->head->next;
        removeNodeElement(list, ptr);
    }
}

static void removeNodeElement(List list, Node* node)
{
    list->freeElement(node->data);
    list->size--;
    free(node);
}

bool listIsEmpty(List list)
{
    return list && !list->size;
}

ListResult listSort(List list, ElemCompareFunction compare)
{
    if (list == NULL || compare == NULL)
        return LIST_NULL_ARG;
    
    for (Node* i = list->head; i != NULL; i = i->next) {
        for (Node* j = i->next; j != NULL; j = j->next) {
            if (compare(i->data, j->data)) {
                Element tmp = i->data;
                i->data = j->data;
                j->data = tmp;
            }
        }
    }

    return LIST_SUCCESS;
}

ListResult listApply(List list, ElemApplyFunction function)
{
    if (list == NULL || function == NULL)
        return LIST_NULL_ARG;

    for(Node* ptr = list->head; ptr != NULL; ptr->data = function(ptr->data), ptr = ptr->next);

    return LIST_SUCCESS;
}

List listFilter(List list, ElemConditionFunction condition, void* param)
{
    if (list == NULL || condition == NULL)
        return NULL;

    List result = listCreate(list->copyElement, list->freeElement);
    if (result == NULL)
        return NULL;
    
    LIST_FOREACH(Element, element, list) {
        if (condition(element, param))
            if (listInsertLast(result, element) != LIST_SUCCESS) {
                listDestroy(result);
                return NULL;
            }
    }

    return result;
}
