#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

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

class NumEdit
{
  public:
    NumEdit(): fPollingHook(0), fUpdateHook()  {};

    void begin(LiquidCrystal_I2C *_pLCD, Keypad *_pKeypad )
    {
      pLCD = _pLCD;
      pKeypad = _pKeypad;
    }

    void setPollingHook(void (*fptr)(void))
    {
      fPollingHook                 = fptr;
    }
    void removePollingHook()
    {
      fPollingHook                 = 0;
    }
    void setUpdateHook(void (*fptr)(int))
    {
      fUpdateHook                 = fptr;
    }
    void removeUpdateHook()
    {
      fUpdateHook                 = 0;
    }

    int edit(Field & fld, Value &val);
    int edit(byte _col, byte _row, byte _len, int _min, int _max, int _value, char *formatBuf);
    int edit(byte _col, byte _row, byte _len, int _min, int _max, int _value);

  private:


    LiquidCrystal_I2C *pLCD;
    Keypad *pKeypad;

    void (*fPollingHook)(void);
    void (*fUpdateHook)(int);


    bool parse(char *input, Value & v, Field &f, byte nPos);
    int range(int n, Value & v, Field &f, byte nPos = 0);
    void update(Field & f, Value & v, byte cur = 0);




};
