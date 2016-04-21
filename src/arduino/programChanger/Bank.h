

#ifndef BANK_H
#define BANK_H
using namespace std;
#include <stdlib.h>
#include <printable.h>


#include "vector.h"
#include "patch.h"

#pragma message ">>>>>>>>>>>>>>>>>>> BANK_H"

class Bank : public Printable
{
  private:

    byte CC0;
    byte CC32;




    Vector<Patch *> patches  = Vector<Patch *>(32);

    char name[13];


  public:
    Bank() : CC0(0), CC32(0)
    {

      memset(name, 0, 13);
      strcpy(name, "NO NAME");
    }

    Bank (char* _name, byte _CC0, byte _CC32):
      CC0(_CC0), CC32(_CC32)
    {
      memset(name, 0, 13);
      strncpy(name, _name, 12);
    }
    virtual ~Bank()
    {
      cleanup();
    }
    char *getName()
    {
      return name;
    }
    byte getCC32()
    {
      return CC32;
    }
    byte getCC0()
    {
      return CC0;
    }


    void addPatch(Patch *pPatch)
    {
      patches.add(pPatch);
    }
    Patch *getNextPatch()
    {
      return patches++;
    }
    Patch *getPrevPatch()
    {
      return patches--;
    }
    Patch *getPatch(byte index)
    {
      return patches[index];
    }
    byte getPatchCount()
    {
      return patches.size();
    }
    byte getCurrentPatchIndex() const
    {
      return patches.getCursor();
    }
    size_t printTo(Print& p) const
    {
      p.print(F("B: "));
      p.print(name);
      p.print(F(" CC: "));
      p.print(CC0);
      p.print(F("/"));
      p.print(CC32);
      p.print(F(" P: "));
      p.print(patches.size());
      p.print(F("@"));
      p.println(getCurrentPatchIndex());
      for (int i = 0; i < patches.size(); i++)
      {
        p.print("   +-- ");
        patches[i]->printTo(p);
        p.print("       >");
        p.print(i);
        p.println("<");
      }
      p.println(patches);
      return 0;
    }
  private:
    void cleanup()
    {
      for (int i = 0; i < patches.size(); i++)
      {
        delete patches[i];
        patches[i] = NULL;
      }
    }
};
#endif
