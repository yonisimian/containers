#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdexcept>

#define NULL_ELEMENT nullptr

namespace containers 
{
    /**
    * A Generic Ordered-List Container
    *
    * Each element (T) in the list must meet the following requirements:
    *     - have a copy constructor
    *     - have a destructor
    *     - have an operator<
    * Without these three, the code will not compile (will fail instantiation) (defaults are OK)
    *
    * The list also has provides internal constant iterators.
    * The iterators must return a constant value, or else the user will be able to manually
    * change elements in the list and defect it's being sorted.
    *
    * The container provides the following public methods:
    *   SortedList();
    *   SortedList(const SortedList& other);
    *   SortedList& operator=(const SortedList& other);
    *   ~SortedList();
    *
    *   void insert(T element);
    *   void remove(const_iterator iterator);
    *   int length();
    *
    *   template<typename Predicate>
    *   SortedList filter(Predicate predicate);
    *
    *   template<typename Function>
    *   SortedList apply(Function function);
    *
    *   const_iterator begin() const;
    *   const_iterator end() const;
    *
    * The iterator provides the following public methods:
    *   const_iterator(const const_iterator& other);
    *   const_iterator& operator=(const const_iterator& other);
    *   ~const_iterator();
    *
    *   const_iterator& operator++();
    *   bool operator==(const const_iterator& other)
    *   const T& operator*() const
    */

    template<class T> 
    class SortedList {
        // ====================== Node declaration & implementation ====================== //
        struct Node
        {
            T data;
            struct Node* next;

            Node() = delete;

            Node(T data) :
                data(data),
                next(NULL_ELEMENT)
            {};

            Node(const Node& other) :
                data(other->data),
                next(NULL_ELEMENT)
            {};
        };

        // ====================== SortedList attributes ====================== //
        Node* head;
        int size;

        public:
            // ====================== const_iterator declaration & implementation ====================== //
            class const_iterator 
            {
                const SortedList* list;
                Node* current_node;

                friend class SortedList;

                public:
                    /**
                    * copy constructor of const_iterator based on an existing one.
                    * 
                    * @param other - the iterator whose data we want to copy.
                    * @return - a new const_iterator object with the copied data.
                    */
                    const_iterator(const const_iterator& other) :
                        list(other.list),
                        current_node(other.current_node)
                    {}

                    /**
                    * operator= - copies "other"'s iterator data to the current object.
                    *
                    * @param other - the iterator whose data we want to copy.
                    * @return - a reference to the current const_iterator object.
                    * 
                    * NOTE: the two iterators remain independent after the operation.
                    */
                    const_iterator& operator=(const const_iterator& other)
                    {
                        if (this == &other) {
                            return *this;
                        }

                        setData(other.list, other.current_node);

                        return *this;
                    }

                    /**
                    * const_iterator destructor - deletes the current const_iterator object and release all of its memory.
                    */
                    ~const_iterator() = default;

                    /**
                    * operator++ - moves the iterator to the next object in the list.
                    *
                    * @return - a reference to the current const_iterator object.
                    */
                    const_iterator& operator++()
                    {
                        if(current_node == NULL_ELEMENT) {
                            throw std::out_of_range("");
                        }
                        current_node = current_node->next;
                        return *this;
                    }

                    /**
                    * operator== - checks if the iterators point at the same node in the same list.
                    *
                    * @param other - the iterator we want to compare with the current iterator.
                    * 
                    * @return - a reference to the current const_iterator object.
                    */
                    bool operator==(const const_iterator& other)
                    {
                        return (list == other.list && current_node == other.current_node);
                    }
                    
                    /**
                    * operator* - gets the data of the current node.
                    *
                    * @return - the data of the current node that the iterator points at.
                    */
                    const T& operator*() const
                    {
                        return current_node->data; 
                    }
                
                private:
                    /**
                    * constructor of const_iterator - creates new iterator with given list and node.
                    * 
                    * @param list         - the list that the iterator runs on.
                    * @param current_node - the node that the iterator points at
                    * 
                    * @return - a new const_iterator object.
                    */
                    const_iterator(const SortedList* list , Node* current_node) :
                        list(list),
                        current_node(current_node)
                    {}

                    /**
                    * setData - change the data of the current node.
                    * 
                    * @param list         - the list that the iterator runs on.
                    * @param current_node - the node whose data we want to edit.
                    */
                    void setData(const SortedList* list , Node* node)
                    {
                        this->list = list;
                        current_node = node;
                    }
            };

            // ====================== SortedList methods declaration & implementation ====================== //

            /**
            * Constructor of SortedList - creates new SortedList with given head and size.
            * 
            * @param head - the list that the iterator runs on. default value is nullptr
            * @param size - the node that the iterator points at. default value is 0.
            * 
            * @return - a new SortedList object.
            */
            explicit SortedList() :
                head(NULL_ELEMENT),
                size(0)
            {}

            /**
            * SortedList destructor : deletes the current List object and release all of its memory.
            */
            ~SortedList()
            {
                clear();
            }

            /**
            * copy constructor of SortedList based on an existing one.
            * 
            * @param other - the list whose data we want to copy.
            *  
            * NOTE: default initializing value of head is nullptr, and default value of size is 0;
            * 
            * @return - a new SortedList object with the copied data.
            */
            SortedList(const SortedList& other) :
                head(NULL_ELEMENT),
                size(0)
            {
                copy(other);
            }
            /**
            * operator= - copies "other"'s list data to the current object.
            * 
            * @param other - the list whose data we want to copy.
            *  
            * NOTE: if current list isn't empty, the new data will override previous data.
            * 
            * @return - a reference to the new SortedList object with the copied data.
            */
            SortedList& operator=(const SortedList& other)
            {
                if (this == &other) {
                    return *this;
                }

                clear();
                copy(other);

                return *this;
            }

            /**
            * insert - insert a new node with given data to the list while keeping the list sorted.
            * 
            * @param element - the data we want to insert to the new node.
            */
            void insert(T element)
            {
                if (head == NULL_ELEMENT || element < head->data)
                {
                    Node* next = head;
                    head = new Node(element);
                    head->next = next;
                    ++size;
                    return;
                }

                Node* ptr = head;
                Node* next = head->next;
                while (next != NULL_ELEMENT && next->data < element)
                {
                    ptr = ptr->next;
                    next = next->next;
                }

                ptr->next = new Node(element);
                ptr->next->next = next;
                ++size;
            }

            /**
            * remove - remove given node from the list and earesing his data.
            * 
            * @param iterator - the iterator that points on the element we want to remove.
            */
            void remove(const_iterator iterator)
            {
                if (iterator.list != this || iterator.current_node == NULL_ELEMENT) {
                    return;
                }
                
                --size;
                Node* ptr = head;
                if (iterator.current_node == head) {
                    head = head->next;
                    delete ptr;
                    return;
                }

                for (; ptr->next != iterator.current_node; ptr = ptr->next);

                Node* to_delete = ptr->next;
                ptr->next = ptr->next->next;
                delete to_delete;
            }

            /**
            * length - gets the number of elements in the current list.
            *
            * @return - the size of the current list.
            */
            int length()
            {
                return size;
            }

            /**
            * filter - remove objects that doesn't apply to certain condition from the list.
            * 
            * @param predicate - a boolean function that checks for the wanted condition.
            * 
            * @return new list without the elements that doesn't fit to the condition.
            * NOTE: original lists remains untouched.
            */
            template<typename Predicate>
            SortedList filter(Predicate predicate)
            {
                SortedList<T> list;
                for(const_iterator iterator = begin(); !(iterator == end()); ++iterator) {
                    if (predicate(*iterator)) {
                        list.insert(*iterator);
                    }
                }
                return list;
            }

            /**
            * apply - creates new list with fixed objects. sends all objects from original list to given function 
            *         and puts the output of the function in the new list. 
            * 
            * @param function - the function that changes the objects. 
            * 
            * @return new list with the fixed objects.
            * NOTE: original lists remains untouched.
            */
            template<typename Function>
            SortedList apply(Function function)
            {
                SortedList<T> list;
                for(const_iterator iterator = begin(); !(iterator == end()); ++iterator) {
                    list.insert(function(*iterator));
                }
                return list;
            }

            /**
            * begin - creates and returns a new iterator that points to the head of the list
            *
            * @return - iterator
            */
            const_iterator begin() const
            {
                return const_iterator(this, head);
            }

            /**
            * end - creates and returns a new iterator that points to the end of the list
            *
            * @return - iterator
            */
            const_iterator end() const
            {
                return const_iterator(this, NULL_ELEMENT);
            }
        
        private:

            /**
            * clear - deletes all nodes from the list.
            * NOTE: the list still exists, but empty.
            */
            void clear()
            {
                while (!(begin() == end())) {
                    remove(begin());
                }
            }

            /**
            * copy - copy all of the data from one list to another existing list.
            *
            * @param other - the list whose data we want to copy
            */
            void copy(const SortedList& other)
            {
                if ((size = other.size) == 0) {
                    return;
                }

                head = new Node(other.head->data);
                Node* ptr = head;
                for (Node* other_ptr = other.head->next; other_ptr != NULL_ELEMENT; other_ptr = other_ptr->next)
                {
                    ptr->next = new Node(other_ptr->data);
                    ptr = ptr->next;
                }
            }

    }; // end of SortedList

} // end of namespace containers

#endif
