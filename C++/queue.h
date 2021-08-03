#ifndef QUEUE_H
#define QUEUE_H

#include <list>
#include <stdexcept>

namespace containers 
{
    template<class T> 
    class Queue {
        std::list<T> items;

        public:
            Queue() = default;
            Queue(const Queue& other) = default;
            Queue& operator=(const Queue& other) = default;
            ~Queue() = default;

            void enqueue(const T& t) { items.push_back(t); }
            void dequeue() { items.empty() ? throw QueueEmpty() : items.pop_front(); }
            T& front() 			       { return items.front(); }
            const T& front() const { return items.front(); }
            int getSize() const    { return items.size();  }

            class QueueEmpty: public std::exception {};
    }; // end of Queue
} // end of namespace containers

#endif
