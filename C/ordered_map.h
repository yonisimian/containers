#ifndef ORDERED_MAP_H_
#define ORDERED_MAP_H_

#include <stdbool.h>

/**
* A Generic Ordered-Map Container (ADT)
*
* Each node in the map contains a pair of key and data elements.
* The keys and the data elements can be anything (primitive types, structs etc).
* The keys are ordered using a compare function given by the user.
* The map also has an internal iterator.
*
* The ADT provides the following methods:
*   mapCreate
*   mapDestroy
*   mapCopy
*   mapGetSize
*   mapContains   - NOTE: Resets the internal iterator.
*   mapPut		    - NOTE: Resets the internal iterator.
*   mapGet  	    - NOTE: Iterator status unchanged.
*   mapRemove		  - NOTE: Resets the internal iterator.
*   mapGetFirst
*   mapGetNext
*   mapClear
*
*   MAP_FOREACH	- A macro for iterating over the map's elements.
*
*   NOTE: the "put" and "copy" methods create copies of the elements,
*         while all of the "get" methods return the elements in the map (and not another copies).
*/

// ============================ TYPEDEFS ============================ //
typedef struct ordered_map_t * Map;

typedef enum MapResult_t {
    MAP_SUCCESS,
    MAP_OUT_OF_MEMORY,
    MAP_NULL_ARGUMENT,
    MAP_ITEM_ALREADY_EXISTS,
    MAP_ITEM_DOES_NOT_EXIST
} MapResult;

typedef void * MapDataElement;
typedef void * MapKeyElement;

typedef MapDataElement(*copyMapDataElements)(MapDataElement);
typedef MapKeyElement(*copyMapKeyElements)(MapKeyElement);

typedef void(*freeMapDataElements)(MapDataElement);
typedef void(*freeMapKeyElements)(MapKeyElement);


/**
* The function type that compare keys and keep the map ordered.
*   - Returns a positive number is the first key element is greater.
*   - Returns 0 if they are equal.
*   - Returns a negative number otherwise.
*/
typedef int(*compareMapKeyElements)(MapKeyElement, MapKeyElement);


// ============================ FUNCTIONS ============================ //
/**
* mapCreate: Allocates and returns a new empty map.
*
* @param copyDataElement    - A Function pointer for copying data elements.
* @param copyKeyElement     - A Function pointer for copying key elements.
* @param freeDataElement    - A Function pointer for removing data elements.
* @param freeKeyElement     - A Function pointer for removing key elements.
* @param compareKeyElements - A Function pointer for comparing key elements.
* @return
* 	NULL - if one of the parameters is NULL or if allocations failed.
* 	A new Map in case of success.
*/
Map mapCreate(copyMapDataElements   copyDataElement,
              copyMapKeyElements    copyKeyElement,
              freeMapDataElements   freeDataElement,
              freeMapKeyElements    freeKeyElement,
              compareMapKeyElements compareKeyElements);

/**
* mapDestroy: Deallocates an existing map and all of it's elements by using the stored free functions.
*
* @param map - Map to be deallocated. If map is NULL nothing will be done.
*/
void mapDestroy(Map map);

/**
* mapCopy: Creates a copy of target map.
* NOTE: Iterator values for both maps is undefined after this operation.
*
* @param map - Target map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	Otherwise, return a Map containing the same elements as the given Map.
*/
Map mapCopy(Map map);

/**
* mapGetSize: Returns the number of elements in a map.
*
* @param map - The map which size is requested.
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise, return the number of elements in the map.
*/
int mapGetSize(Map map);

/**
* mapContains: Checks if a key element exists in the map.
*
* @param map - The map to search in.
* @param element - The element to look for.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true  - if the key element was found in the map.
*/
bool mapContains(Map map, MapKeyElement element);

/**
*	mapPut: Puts in the map a COPY of the given key-data pair. 
* NOTE: Iterator's value is undefined after this operation.
*
* @param map - The map for which to reassign the data element.
* @param keyElement - The key element which need to be reassigned.
* @param dataElement - The new data element to associate with the given key.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent as map.
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying an element failed).
* 	MAP_SUCCESS if the paired elements had been inserted successfully.
*/
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement);

/**
*	mapGet: Returns the data associated with a specific key in the map.
*	NOTE: Iterator status unchanged
*
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and whos data we want to get.
* @return
*   NULL if a NULL pointer was sent or if the map does not contain the requested key.
*   Otherwise, return the data element associated with the key (not a copy!).
*/
MapDataElement mapGet(Map map, MapKeyElement keyElement);

/**
* mapRemove: Removes a pair of key and data elements from the map.
* NOTE: Iterator's value is undefined after this operation.
*
* @param map - The map to remove the elements from.
* @param keyElement - The key element to find and remove from the map.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function.
*   MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the map.
* 	MAP_SUCCESS if the paired elements had been removed successfully.
*/
MapResult mapRemove(Map map, MapKeyElement keyElement);

/**
*	mapGetFirst: Sets the internal iterator (also called current key element) to the first key element in the map.
*	Use this to start iterating over the map.
*
* @param map - The map for which to set the iterator and return the first key element.
* @return
* 	NULL if a NULL pointer was sent or the map is empty.
* 	Otherwise, return the first key element of the map.
*/
MapKeyElement mapGetFirst(Map map);

/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.

* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state or a NULL sent as argument
* 	Otherwise, return the next key element on the map.
*/
MapKeyElement mapGetNext(Map map);


/**
* mapClear: Removes all key and data elements from target map.
* 
* @param map - Target map to remove all element from.
* @return
* 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MAP_SUCCESS - Otherwise.
*/
MapResult mapClear(Map map);

/*!
* Macro for iterating over a map.
* Declares a new iterator for the loop.
*/
#define MAP_FOREACH(type, iterator, map) \
    for(type iterator = (type) mapGetFirst(map) ; \
        iterator ;\
        iterator = mapGetNext(map))

#endif
