#include <print.h>
#include "vector.h"
#include "ListEdit.h"

int ListEdit::select (Vector<String *> items, int selectedItem)
{
  screen();
  byte startIndex = selectedItem;
  int itemCount = items.size();
  update(items,  startIndex);

  while (1)
  {
    if (fPollingHook)
    {
      fPollingHook();
    }

    char key = pKeypad->getKey();

    if (key)
    {
      switch (key)
      {
        case 'D':
          {
            return startIndex;
          }
        case '+':
        case '#':
          {
            startIndex++;
            startIndex = wrap(startIndex, itemCount);
            update(items, startIndex);
            break;
          }
        case '-':
        case '*':
          {
            startIndex--;
            startIndex = wrap(startIndex, itemCount);
            update(items, startIndex);
            break;
          }
        default:
          {
            setLastKey(key);
            return -1;
          }
      }
    }
  }
}

int ListEdit::select (char * items[], int itemCount, int selectedItem)
{

  Vector<String *> data;

  for (int i = 0; i < itemCount; i++)
  {
    String *s = new String(items[i]);
    data.add(s);
  }
  return select(data, selectedItem);

  /*
    screen();
    byte startIndex = selectedItem;
    update(items, itemCount, startIndex);

    while (1)
    {
    if (fPollingHook)
    {
      fPollingHook();
    }

    char key = pKeypad->getKey();

    if (key)
    {
      switch (key)
      {
        case 'A':
          {
            return startIndex;
          }
        case '+':
        case '#':
          {
            startIndex++;
            startIndex = wrap(startIndex, itemCount);
            update(items, itemCount, startIndex);
            break;
          }
        case '-':
        case '*':
          {
            startIndex--;
            startIndex = wrap(startIndex, itemCount);
            update(items, itemCount, startIndex);
            break;
          }
        default:
          {
            return -1;
          }
      }
    }

    }
  */

}


int ListEdit::update(Vector<String *>items, int startIndex)
{
  int itemCount = items.size();
  int x0 = wrap(startIndex - 1, itemCount);
  int x1 = wrap(startIndex + 1, itemCount);

  pLCD->setCursor(4, 1);
  pLCD->print(x0);
  pLCD->print(" ");
  pLCD->print(*items[x0]);
  pLCD->setCursor(4, 2);
  pLCD->print(startIndex);
  pLCD->print(" ");
  pLCD->print(*items[startIndex]);
  pLCD->setCursor(4, 3);
  pLCD->print(x1);
  pLCD->print(" ");
  pLCD->print(*items[x1]);


  return 0;
}
void ListEdit::screen()
{

  pLCD->clear();
  pLCD->setCursor(2, 0);
  //           12345678901234567890
  pLCD->write(165);
  pLCD->print("--  Select  --");
  pLCD->write(165);

  // links
  pLCD->setCursor(2, 1);
  pLCD->print((char)1);
  pLCD->setCursor(2, 2);
  pLCD->print((char)2);
  pLCD->setCursor(2, 3);
  pLCD->print((char)1);

  // rechts
  pLCD->setCursor(17, 1);
  pLCD->print((char)1);
  pLCD->setCursor(17, 2);
  pLCD->print((char)2);
  pLCD->setCursor(17, 3);
  pLCD->print((char)1);
};

int ListEdit::wrap(int x, int itemCount)
{
  int ubound = itemCount - 1;
  if ((x < 0) || (x >= 255)) return ubound;
  if (x > ubound) return 0;
  return x;
}

