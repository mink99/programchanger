
#include "BasicEdit.h"


class ListEdit : public BasicEdit
{

  public:
    ListEdit() {}
    void begin(LiquidCrystal_I2C *_pLCD, Keypad *_pKeypad, int rows = 4 )
    {
      pLCD = _pLCD;
      pKeypad = _pKeypad;
    }


    int select (char * items[], int itemCount, int selectedItem = 0);
    int select (Vector<String *> items, int selectedItem = 0);

  private:


    int update(Vector<String *> items, int startIndex);

    void screen();
    int wrap(int x, int max);


};

