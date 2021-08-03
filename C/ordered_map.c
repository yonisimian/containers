#include "oredered_map.h"

#include <stdlib.h>
#include <stdbool.h>

#define NULL_MAP_SIZE -1

typedef struct node_t {
    MapDataElement data;
    MapKeyElement key;
    struct node_t* next;
} Node;

static void removeNodeElement(Map map, Node* node);
static bool addNewElement(Map map, MapKeyElement keyElement, MapDataElement dataElement);
static bool updateExistingElement(Map map, MapKeyElement keyElement, MapDataElement dataElement);

struct ordered_map_t {
    Node* head;
    Node* iterator;
    int size;
    copyMapDataElements copyDataElement;
    copyMapKeyElements copyKeyElement;
    freeMapDataElements freeDataElement;
    freeMapKeyElements freeKeyElement;
    compareMapKeyElements compareKeyElements;
};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if(copyDataElement == NULL || copyKeyElement == NULL ||
       freeDataElement == NULL || freeKeyElement == NULL || compareKeyElements == NULL ) {
        return NULL; 
    }
    Map map = (Map)malloc(sizeof(*map));
    if(map == NULL) {
        return NULL;
    }
    map->size = 0;
    map->head = NULL; 
    map->iterator = NULL;
    map->copyDataElement = copyDataElement;
    map->copyKeyElement = copyKeyElement;
    map->freeDataElement = freeDataElement;
    map->freeKeyElement = freeKeyElement;
    map->compareKeyElements = compareKeyElements;

    return map;
}

void mapDestroy(Map map)
{
    if(map == NULL) {
        return;
    }
    mapClear(map);
    free(map);
}

Map mapCopy(Map map)
{
    if(map == NULL) {
        return NULL;
    }
    Map new_map = mapCreate(map->copyDataElement, map->copyKeyElement, map->freeDataElement,
                            map->freeKeyElement, map->compareKeyElements);
    if (new_map == NULL) {
        return NULL;
    }
    new_map->size = 0;
    Node* ptr = map->head;
    while (ptr != NULL) {
        if (mapPut(new_map, ptr->key, ptr->data) != MAP_SUCCESS) {
            mapDestroy(new_map);
            return NULL;
        }
        ptr = ptr->next;
    }
    new_map->iterator = NULL;
    new_map->copyDataElement = map->copyDataElement;
    new_map->copyKeyElement = map->copyKeyElement;
    new_map->freeDataElement = map->freeDataElement;
    new_map->freeKeyElement = map->freeKeyElement;
    new_map->compareKeyElements = map->compareKeyElements;

    return new_map;
}

int mapGetSize(Map map)
{
    if (map == NULL) {
        return NULL_MAP_SIZE;
    }

    return map->size;
}

bool mapContains(Map map, MapKeyElement element)
{
    if(map == NULL || element == NULL) {
        return false;
    }
    Node* ptr = map->head;
    while(ptr != NULL) {
        if(!(map->compareKeyElements(ptr->key , element))) {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if (map == NULL || keyElement == NULL || dataElement == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    if (!mapContains(map, keyElement))  {
        if (!addNewElement(map, keyElement, dataElement)) {
            return MAP_OUT_OF_MEMORY;
        }
    }
    else {
        if (!updateExistingElement(map, keyElement, dataElement)) {
            return MAP_OUT_OF_MEMORY;
        }
    }

    return MAP_SUCCESS;
}

static bool addNewElement(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    Node* new_node = (Node*)malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return false;
    }
    
    new_node->key = map->copyKeyElement(keyElement);
    if(new_node->key == NULL) {
        free(new_node);
        return false;
    }

    new_node->data = map->copyDataElement(dataElement);
    if(new_node->data == NULL) {
        free(new_node->key);
        free(new_node);
        return false;
    }

    Node* ptr = map->head;
    if (ptr == NULL) { // map was empty, adding its head
        new_node->next = NULL;
        map->head = new_node;
        map->size++;
        return true;
    }
    
    Node* next = ptr->next;
    while(next != NULL && map->compareKeyElements(keyElement, next->key) > 0) {
        ptr = next;
        next = ptr->next;
    } // ptr is now the last element whose key's smaller than keyElement
    

    if (ptr == map->head && map->compareKeyElements(keyElement, ptr->key) < 0) {
        new_node->next = ptr;
        map->head = new_node;
    }
    else {
        ptr->next = new_node;
        new_node->next = next;
    }

    map->size++;
    return true;
}

static bool updateExistingElement(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    Node* ptr = map->head;
    while (map->compareKeyElements(keyElement, ptr->key)) {
        ptr = ptr->next;
    }
    MapDataElement data = ptr->data;
    ptr->data = map->copyDataElement(dataElement);
    map->freeDataElement(data);
    if (ptr->data == NULL) {
        return false;
    }
    return true;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL) {
        return NULL;
    }
    Node* ptr = map->head; 
    while(ptr != NULL) {
        if(!(map->compareKeyElements(ptr->key ,keyElement))) {
            return ptr->data;
        }
        ptr = ptr->next;
    }
    return NULL;
}

MapKeyElement mapGetFirst(Map map)
{
    if (map == NULL || map->head == NULL) {
        return NULL;
    }

    map->iterator = map->head;
    return map->copyKeyElement(map->iterator->key);
}

MapKeyElement mapGetNext(Map map)
{
    if (map == NULL || map->iterator == NULL || map->iterator->next == NULL) {
        return NULL;
    }

    map->iterator = map->iterator->next;
    return map->copyKeyElement(map->iterator->key);
}

MapResult mapClear(Map map)
{
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    while(map->head != NULL) {
        Node* ptr = map->head;
        map->head = map->head->next;
        removeNodeElement(map, ptr);
    }

    return MAP_SUCCESS;
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (map->head == NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    Node* ptr = map->head;
    if(!(map->compareKeyElements(ptr->key , keyElement))) {
        map->head = map->head->next;
        removeNodeElement(map , ptr);
        return MAP_SUCCESS;
    }

    while(ptr->next != NULL) {
        if(!(map->compareKeyElements(ptr->next->key , keyElement))) {
            Node* next = ptr->next;
            ptr->next = next->next;
            removeNodeElement(map, next);
            return MAP_SUCCESS;
        }
        ptr = ptr->next;
    }
    
    return MAP_ITEM_DOES_NOT_EXIST;   
}

static void removeNodeElement(Map map, Node* node)
{
    map->freeDataElement(node->data);
    map->freeKeyElement(node->key);
    map->size--;
    free(node);
}
