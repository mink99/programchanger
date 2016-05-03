
#include "numedit.h"

int NumEdit::edit(byte _col, byte _row, byte _len, int _min, int _max, int _value, char *formatBuf)
{
  Field fld;
  Value val;

  fld.col = _col;
  fld.row = _row;
  fld.len = _len;

  val.value = _value;
  val.minValue = _min;
  val.maxValue = _max;
  val.fmt = formatBuf;

  return edit(fld, val);
}

int NumEdit:: edit(Field & fld, Value &val)
{
  bool needsUpdate = false;
  char editBuf[6];
  int editCursor = 0;



  pLCD->blink();
  pLCD->cursor();

  update(fld, val);

  memset(editBuf, 0, 6);
  sprintf(editBuf, val.fmt, val.value);

  while (1)
  {
    if (fPollingHook)
    {
      fPollingHook();
    }
    if (needsUpdate)
    {
      update(fld, val, editCursor);
      needsUpdate = false;
    }
    char key = pKeypad->getKey();

    if (key)
    {

      if (isDigit(key))
      {
        editBuf[editCursor] = key;
        if (!parse(editBuf, val, fld, editCursor)) continue;

        needsUpdate = true;
        editCursor++;
        if (editCursor >= fld.len)
        {
          update(fld, val);
          pLCD->noBlink();
          pLCD->noCursor();
          return val.value;
        }
        continue;
      }
      if ( key == '#' || key == '+' )
      {
        int n = val.value;
        n++;
        if (n > val.maxValue)
        {
          val.value = val.minValue;
        }
        else
        {
          val.value = n;
        }

        needsUpdate = true;
        continue;

      }
      if (  key == '*' || key == '-')
      {
        int n = val.value;
        n--;
        if (n < val.minValue)
        {
          val.value = val.maxValue;

        }
        else
        {
          val.value = n;
        }
        needsUpdate = true;
        continue;
      }
      setLastKey(key);
      out << "unknown KEY: " << key << endl;
      pLCD->noBlink();
      pLCD->noCursor();
      return val.value;

    }//end if

  }// end while

};

bool NumEdit::parse(char *input, Value & v, Field &f, byte nPos)
{

  int n = atoi(input);
  if (range(n, v, f, nPos))return false;
  v.value = n;
  return true;
}

int  NumEdit::range(int n, Value & v, Field &f, byte nPos )
{
  int n0 = pow(10 , (f.len - (nPos + 1)));
  int n1 = n / n0 ;
  int n2 = n0 * n1;
  int n3 = n0 - 1;
  int n4 = n2 + n3;

  if ((n4 < v.minValue) )
  {
    return -1;
  }
  if ( (n2 > v.maxValue))
  {
    return 1;
  }
  v.value = n2;
  return 0;
}
void NumEdit::update(Field & f, Value & v, byte cur)
{
  char ch[6];
  memset(ch, 0, 6);
  sprintf(ch, v.fmt, v.value);
  
  pLCD->setCursor(f.col, f.row);
  pLCD->print(ch);
  
  if (fUpdateHook)
  {
    fUpdateHook(v.value);
  }
  pLCD->setCursor(f.col + cur, f.row);
}

