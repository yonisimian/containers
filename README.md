# Containers
In this repository, you will find generic containers written in c / c++.
This repository is made for educational purposes, therefore I have tried to provide diversity of implementations, among them you will find:
- Containers based on arrays (easy implementation, limited size)
- Containers based on nodes (a little complicated, unlimited size)
- Containers with iterators
- And much more...

_So... what's on the menu?!_
|C              |C++                       |
|---------------|--------------------------|
|Ordered Map    |Sorted List (Node-Based)  |
|List    		     |List (Node-Based)         |
|Queue			       |Queue (List-Based)		      |
|Stack    		    |Stack (Array-Based)       |
|Set			         |Set (Array-Based)		       |


## C containers

All of the containers written in C are implemented using nodes, because I think the array-based containers are boring and non educational.
There are currently 5 containers:
- **Ordered Map** - the most detailed and complicated one.
It contains an unlimited amount of key-data pairs, and is sorted by the key.
Both the key and the data can be anything (void*), as long as you copy & free them.
The map also provides an **iterator** and a macro to iterate over the container.
- **Linked List** - good old fashioned lover linked-list. 
Also provides an iterator, but is overall less detailed than the previous container.
However, the list also contains **apply** and **filter** functions which are very useful!
- **Queue** - just a simple queue, no iterator or interesting functions.
- **Stack** - same as above.
- **Set** - also provides an iterator, a macro, and two pleasant functions - **union** and **intersection**.

>NOTE:  All of the C containers use **function pointers** in order to maintain it's generalness, because all of it's data is void* and must be copied, freed and compared using functions given by the user.
 
## C++ Containers

These containers are implemented using various ways, because I think it's more interesting.
There are currently 5 containers:
- **Sorted List** - the most detailed and complicated one.
It is basically just a sorted linked list, but is very detailed and also the only container I have written to provide a **const-iterator** instead of a regular one. 
It also provides **apply** and **filter** functions, that are good examples of using templates.
- **Linked List** - similar to the previous one, but much simpler and unsorted.
- **Queue** - the only container I have written using std as it's base (instead of nodes or array).
It's a good example (in my opinion) for using std for our purposes.
- **Stack** - the first container to be implemented using an array.
It's very basic, so it's a good example for implementing C containers using arrays.
- **Set** - a normal set with all it's find / expand / unite / intersect shenanigans.

>NOTE:  All of the C containers use **templates** in order to maintain it's generalness.
The usage of templates forces us to assume characteristics about the data stored in the container (more details in the Sorted List documentation).

## Summary
All of the above containers are made by me for educational purposes.

You can copy and use it for your purposes, learn from it and even upgrade it.
The downside is that it might have bugs, because I'm not a master programmer, yet ;)

Yehonatan Simian, 2021 Ⓒ yonisimian@gmail.com
