
#ifndef MEMORY_H
#define MEMORY_H
#pragma message ">>>>>>>>>>>>>>>>>>> MEMORY_H"

#include <arduino.h>
extern "C" char* sbrk(int incr);

#define out Serial
#define endl "\n"

template<class T> inline Stream &operator <<(Stream &obj, T arg) { obj.print(arg); return obj; } 

inline int freeMem()
{
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

inline void alert(String s)
{
  out << s;
  out.flush();
}





#endif
