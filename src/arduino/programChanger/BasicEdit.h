#ifndef BASICEDIT_H
#define BASICEDIT_H

#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

#include "memory.h"

struct Field {
  byte col;
  byte row;
  byte len;
};
struct Value {
  int minValue;
  int maxValue;
  int value;
  char *fmt;
};




class BasicEdit
{
  public:
    BasicEdit(): fPollingHook(0), fUpdateHook(0), lastKey(0) {};
    void setPollingHook(void (*fptr)(void)) {
      fPollingHook = fptr;
    }
    void removePollingHook() {
      fPollingHook = 0;
    }
    void setUpdateHook(void (*fptr)(int)) {
      fUpdateHook  = fptr;
    }
    void removeUpdateHook() {
      fUpdateHook = 0;
    }
    void begin(LiquidCrystal_I2C *_pLCD, Keypad *_pKeypad )
    {
      pLCD = _pLCD;
      pKeypad = _pKeypad;
    }
    void setLastKey(char ch)
    {
      lastKey = ch;
    }
    char getLastKey()
    {
      char ch = lastKey;
      lastKey = 0;
      return ch;
    }

  protected:
    void (*fPollingHook)(void);
    void (*fUpdateHook)(int);


    LiquidCrystal_I2C *pLCD;
    Keypad *pKeypad;
    char lastKey;

};



#endif
