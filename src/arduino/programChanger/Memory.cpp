extern "C" char* sbrk(int incr);
int FreeMem()
{
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
