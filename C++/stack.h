#ifndef STACK_H
#define STACK_H

namespace containers 
{
    template<class T> 
    class Stack {
      T* data;
      int size;
      int nextIndex;

      public:
          explicit Stack(int size = 100);
          Stack(const Stack& s);
          ~Stack();
          Stack& operator=(const Stack&);
          void push(const T& t);
          void pop();
          T& top();
          const T& top() const;
          int getSize() const;

          class Full {};
          class Empty {};
    }; // end of Stack
} // end of namespace containers

#endif
