
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

  private:


   
    int update(char * items[], int itemCount, int startIndex);
    void screen();
    int wrap(int x, int max);

};

