

#ifndef DEVICE_H
#define DEVICE_H

#include <stdlib.h>

using namespace std;

#include <print.h>

#include "vector.h"
#include "bank.h"


#pragma message ">>>>>>>>>>>>>>>>>>> DEVICE_H"

class Device: public Printable
{
  private:




    Vector<Bank *> banks = Vector<Bank *>(24);


    char name[13];

  public :
    Device()
    {
      memset(name, 0, 13);
      strcpy(name, "NO NAME");
    }

    char  *getName()
    {
      return name;
    }
    void setName(char* _name)
    {
      memset(name, 0, 13);
      strncpy(name, _name, 12);
    }
    Bank *getBank(byte index)
    {
      return banks[index];
    }
    Bank *getCurrentBank()
    {
      return banks.current();
    }
     Bank *getNextBank()
    {
      return ++banks;
    }
    Bank *getPrevBank()
    {
      return --banks;
    }

    byte getBankCount()
    {
      return banks.size();
    }
    void addBank(Bank *pBank)
    {
      banks.add(pBank);
    }
    byte getCurrentBankIndex() const
    {
      return banks.getCursor();
    }
    size_t printTo(Print& p) const
    {
      p.print(F("D: "));
      p.print(name);
      p.print(F(" B: "));
      p.print(banks.size());
      p.print(F("@"));
      p.println(getCurrentBankIndex());
      for (int i = 0; i < banks.size(); i++)
      {
        p.print("+--+ ");
        banks[i]->printTo(p);


      }
      return 0;
    }
    void clear()
    {
      for (int i = 0; i < banks.size(); i++)
      {
        delete banks[i];
        banks[i] = NULL;
      }
    }
};
#endif
