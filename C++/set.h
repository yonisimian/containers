#ifndef SET_H
#define SET_H

#include <ostream>

namespace containers 
{
    template <class T>
    class Set {
    public:
      Set();
      Set(const Set&);
      ~Set();
      Set& operator=(const Set&);
      bool add(const T& element);
      bool remove(const T& element);
      bool contains(const T& element) const;
      int getSize() const;
      Set& uniteWith(const Set&);
      Set& intersectWith(const Set&);
      template<class Condition>
      Set filter(Condition c) const;
      class Iterator;
      Iterator begin() const;
      Iterator end() const;
    private:
      T* data;
      int size;
      int maxSize;
      int find(const T& element) const;
      void expand();
      static const int EXPAND_RATE = 2;
      static const int INITIAL_SIZE = 10;
      static const int ELEMENT_NOT_FOUND = -1;
    };

    template<class T>
    Set<T>& Set<T>::uniteWith(const Set<T>& other) {
      for (int i = 0; i < other.getSize(); ++i)
        this->add(other.data[i]);
      return *this;
    }

    template<class T>
    Set<T>& Set<T>::intersectWith(const Set<T>& other) {
      for (int i = 0; i < this->getSize(); ++i)
        if (!other.contains(data[i]))
          this->remove(data[i]);
      return *this;
    }

    template<class T>
    Set<T> setUnion(const Set<T>& set1, const Set<T>& set2) {
      Set<T> result = set1;
      return result.uniteWith(set2);
    }

    template<class T>
    Set<T> setIntersection(const Set<T>& set1, const Set<T>& set2) {
      Set result = set1;
      return result.intersectWith(set2);
    }

    template<class T>
    std::ostream& operator<<(std::ostream& os,const Set<T>& set);

    template<class T>
    class Set<T>::Iterator {
      const Set<T>* set;
      int index;
      Iterator(const Set<T>* set, int index);
      friend class Set<T>;
    public:
      const T& operator*() const;
      Iterator& operator++();
      Iterator operator++(int);
      const T* operator->() const;
      bool operator==(const Iterator& iterator) const;
      bool operator!=(const Iterator& iterator) const;
      Iterator(const Iterator&) = default;
      Iterator& operator=(const Iterator&) = default;
    };

    template<class T>
    Set<T>::Set() : data(new T[INITIAL_SIZE]), size(0), maxSize(INITIAL_SIZE) {}

    template<class T>
    Set<T>::~Set() { delete[] data; }

    template<class T>
    Set<T>::Set(const Set<T>& set) :
      data(new T[set.getSize()]),
      size(set.getSize()),
      maxSize(set.getSize()) {
      for (int i = 0; i < size; i++)
        data[i] = set.data[i];
    }

    template<class T>
    Set<T>& Set<T>::operator=(const Set<T>& set) {
      if (this == &set)
        return *this;
      delete[] data;
      data = new T[set.size];
      size = set.size;
      maxSize = set.size;
      for (int i = 0; i < size; ++i)
        data[i] = set.data[i];
      return *this;
    }

    template<class T>
    int Set<T>::find(const T& elem) const {
      for (int i = 0; i < size; i++)
        if (data[i] == elem)
          return i;
      return ELEMENT_NOT_FOUND;
    }

    template<class T>
    bool Set<T>::contains(const T& elem) const {
      return find(elem) != ELEMENT_NOT_FOUND;
    }

    template<class T>
    int Set<T>::getSize() const { return size; }

    template<class T>
    void Set<T>::expand() {
      int newSize = maxSize * EXPAND_RATE;
      T* newData = new T[newSize];
      for (int i = 0; i < size; ++i)
        newData[i] = data[i];
      delete[] data;
      data = newData;
      maxSize = newSize;
    }

    template<class T>
    bool Set<T>::add(const T& elem) {
      if (contains(elem)) return false;
      if (size >= maxSize) expand();
      data[size++] = elem;
      return true;
    }

    template<class T>
    bool Set<T>::remove(const T& elem) {
      int index = find(elem);
      if (index == ELEMENT_NOT_FOUND) return false;
      data[index] = data[--size];
      return true;
    }

    template<class T>
    Set<T>& Set<T>::uniteWith(const Set<T>& other) {
      for (int i = 0; i < other.getSize(); ++i)
        this->add(other.data[i]);
      return *this;
    }

    template<class T>
    Set<T>& Set<T>::intersectWith(const Set<T>& other) {
      for (int i = 0; i < this->getSize(); ++i)
        if (!other.contains(data[i]))
          this->remove(data[i]);
      return *this;
    }

    template<class T>
    std::ostream& operator<<(std::ostream& os, const Set<T>& set) {
      os << "{";
      bool first = true;
      for (typename Set<T>::Iterator it = set.begin();
        it != set.end(); ++it) {
        if (!first) os << ",";
        first = false;
        os << " " << (*it);
      }
      os << " }";
      return os;
    }

    template<class T>
    typename Set<T>::Iterator Set<T>::begin() const { return Iterator(this, 0); }

    template<class T>
    typename Set<T>::Iterator Set<T>::end() const { return Iterator(this, size); }

    template<class T>
    Set<T>::Iterator::Iterator(const Set<T>* set, int index) : set(set), index(index) {}

    template<class T>
    const T& Set<T>::Iterator::operator*() const {
      assert(index >= 0 && index < set->getSize());
      return set->data[index];
    }

    template<class T>
    typename Set<T>::Iterator& Set<T>::Iterator::operator++() {
      ++index;
      return *this;
    }

    template<class T>
    typename Set<T>::Iterator Set<T>::Iterator::operator++(int) {
      Iterator result = *this;
      ++* this;
      return result;
    }

    template<class T>
    bool Set<T>::Iterator::operator==(const Iterator& i) const {
      assert(set == i.set);
      return index == i.index;
    }

    template<class T>
    bool Set<T>::Iterator::operator!=(const Iterator& i) const {
      return !(*this == i);
    }

    template<class T>
    const T* Set<T>::Iterator::operator->() const {
      assert(index >= 0 && index < set->getSize());
      return &(set->data[index]);
    }

    template<class T>
    template<class Condition>
    Set<T> Set<T>::filter(Condition c) const {
      Set<T> result;
      for (typename Set<T>::Iterator it = begin(); it != end(); ++it)
        if (c(*it))
          result.add(*it);
      return result;
    }
} // end of namespace containers

#endif
