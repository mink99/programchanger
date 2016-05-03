#ifndef VECTOR_H
#define VECTOR_H

#pragma message ">>>>>>>>>>>>>>>>>>> VECTOR_H"


#include <stdint.h>
#include <malloc.h>
#include <print.h>

#include "memory.h"




// Minimal class to replace std::vector
// Adds poor man Iterator thru ++ -- operator

template<typename T>
class Vector : public Printable
{
  private:

    size_t d_size; // Stores no. of actually stored objects
    size_t d_capacity; // Stores allocated capacity
    int8_t _cursor ;
    size_t d_growth ; // growth
    T *d_data; // Stores data
    T err;

  public:

    // Default constructor
    Vector() : d_size(0), d_capacity(16), d_data(0), d_growth(16), _cursor(0)
    {
      d_data = (T *)malloc(d_capacity * sizeof(T));
    };

    Vector(size_t initialSize) : d_size(0), d_capacity(initialSize), d_data(0), d_growth(initialSize), _cursor(0)
    {
      d_data = (T *)malloc(d_capacity * sizeof(T));
    };
    // Copy constuctor
    Vector(Vector const &other) : d_size(other.d_size), d_capacity(other.d_capacity), d_data(0)
    {
      d_data = (T *)malloc(d_capacity * sizeof(T));
      memcpy(d_data, other.d_data, d_size * sizeof(T));
    };
    // Destructor
    ~Vector()
    {
      free(d_data);
    };

    // Needed for memory management
    Vector &operator=(Vector const &other)
    {
      free(d_data);
      d_size = other.d_size;
      d_capacity = other.d_capacity;
      d_data = (T *)malloc(d_capacity * sizeof(T));
      memcpy(d_data, other.d_data, d_size * sizeof(T));
      return *this;
    };

    // Adds new value. If needed, allocates more space
    void push_back(T const &x)
    {
      add(x);
    };
    // Adds new value. If needed, allocates more space
    void add(T const &x)
    {
      if (d_capacity == d_size) resize();
      d_data[d_size++] = x;
    };
    //capacity getter
    size_t capacity() const
    {
      return d_capacity;
    };
    // Size getter
    size_t size() const
    {
      return d_size;
    };
     size_t maxIndex() const
    {
      return d_size-1;
    };

    void clear()
    {
      for (int i = 0; i < d_size; i++)
      {
        delete d_data[i];
        d_data[i] = NULL;
      }
      free(d_data);
      d_size = 0;
      d_capacity = 0;
      d_data = NULL;
    }
    // Const getter
    T const &operator[](size_t idx) const
    {
      if (idx > d_size) idx = 0;
      if (idx < 0) idx = d_size - 1;
      return d_data[idx];
    };
    // Changeable getter
    T &operator[](size_t idx)
    {
      //if (idx > d_size) idx = 0;
      if (idx >= d_size) return err;

      if (idx < 0) idx = d_size - 1;
      _cursor = idx;
      return d_data[idx];
    }
    T &current()
    {
     
      return d_data[_cursor];
    }
    T &end()
    {
      _cursor = d_size - 1;
      return d_data[_cursor];
    }
    T &rewind()
    {
      _cursor = 0;
      return d_data[_cursor];
    }
    T &position(size_t idx)
    {
      if (idx >= d_size) idx = 0;
      if (idx < 0) idx = d_size - 1;
      _cursor = idx;
      return d_data[_cursor];
    }
    T &operator ++ (int unused)
    {
      if (_cursor >= d_size) _cursor = 0;
      return (d_data[_cursor++]);
    };
    T &operator -- (int unused)
    {
      if (_cursor < 0) _cursor = d_size - 1;
      return (d_data[_cursor--]);
    };
    T &operator ++ ()
    {
      _cursor++;
      if (_cursor >= d_size) _cursor = 0;
      return d_data[_cursor];
    };
    T &operator -- ()
    {
      _cursor--;
      if (_cursor < 0) _cursor = d_size - 1;
      return d_data[_cursor];
    };
    int8_t getCursor() const
    {
      return _cursor;
    }
    int8_t &cursor() const
    {
      return _cursor;
    }
  private:
    void resize()
    {
      //d_capacity = d_capacity ? d_capacity * 2 : 1;
      d_capacity += d_growth;
      T *newdata = (T *)malloc(d_capacity * sizeof(T));
      memcpy(newdata, d_data, d_size * sizeof(T));
      free(d_data);
      d_data = newdata;
    };
  public:
    size_t printTo(Print& p) const
    {
      p.print("Vector size:");
      p.print(d_size);
      p.print(" capacity:");
      p.print(d_capacity);
      p.print(" growth:");
      p.print(d_growth);
      p.print(" elemSize:");
      p.print(sizeof(T));
      p.print(" freeMem:");
      p.print(freeMem());
      return 0;
    }
};




#endif


