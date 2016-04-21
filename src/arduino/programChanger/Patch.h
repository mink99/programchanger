

#ifndef PATCH_H
#define PATCH_H

#include <stdlib.h>

using namespace std;

#include "print.h"



class Patch: public Printable
{
private:
    char name[17];
    byte patchNr;
    byte devicePatchNr;

public:
    Patch() : patchNr(0), devicePatchNr(0)
    {
        memset(name, 0, 17);
        strcpy(name, "NO NAME");
    }
    Patch(char *_name, byte _patchNr, byte _devicePatchNr) : patchNr(_patchNr), devicePatchNr(_devicePatchNr)
    {
        memset(name, 0, 17);
        strncpy(name, _name, 16);
    }
    virtual ~Patch() {}
    char *getName()
    {
        return name;
    }
    byte getPatchNr()
    {
        return patchNr;
    }
    byte getDevicePatchNr()
    {
        return devicePatchNr;
    }
    size_t printTo(Print& p) const
    {
        p.print(F("P: "));
        p.print(name);
        p.print(F(" PC: "));
        p.print(patchNr);
        p.print(F("/"));
        p.print(devicePatchNr);
        return 0;
    }

};


#endif
