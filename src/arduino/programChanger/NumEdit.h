#include "BasicEdit.h"


class NumEdit : public BasicEdit
{
  public:
    NumEdit()  {};
    int edit(Field & fld, Value &val);
    int edit(byte _col, byte _row, byte _len, int _min, int _max, int _value, char *formatBuf);

  private:

    bool parse(char *input, Value & v, Field &f, byte nPos);
    int range(int n, Value & v, Field &f, byte nPos = 0);
    void update(Field & f, Value & v, byte cur = 0);




};
