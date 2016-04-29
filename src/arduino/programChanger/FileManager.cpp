
#include "FileManager.h"

void doReadReafile(char* name)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  String buf;
  Bank * pBank = NULL;
  Patch *pPatch = NULL;
  byte patchCount = 0;

  File dataFile = SD.open(name);
  if (dataFile)
  {
    while (dataFile.available())
    {
      buf = dataFile.readStringUntil(0x0a);
      String parser(buf);
      parser.trim();
      buf.trim();
      parser.toUpperCase();
      if ((parser[0] == '/') || (parser[0] == ' ') || (parser[0] == ';') || (parser[0] == 0x0b)) continue;
      if (parser.length() == 0 ) continue;
      if (parser.startsWith("BANK"))
      {
        pBank = processBank(parser);
        patchCount = 0;
      }
      else
      {
        if (parser.startsWith("NAME"))
        {
          processName(parser);
        }
        else
        {
          if (pBank)
          {
            processPatch(pBank, parser, patchCount);
            patchCount++;
          }
        }
      }
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else
  {
    message("error opening datalog.txt");
  }
}

void processName(String s)
{
  String s2;
  char ch[13];
  int n1, n2;
  s2 = s.substring(4);
  s2.trim();
  makeShortName(s2);
  s2.toCharArray(ch, 13);
  currentDevice.setName(ch);
}


Bank *processBank(String s)
{
  String s1, s2;
  int i1, i2;
  char ch[13];
  byte _CC0, _CC32;
  s1 = s.substring(4);
  s1.trim();
  i1 = s1.indexOf(' ');
  s2 = s1.substring(0, i1);
  _CC0 = s2.toInt();
  i2 = s1.indexOf(' ', i1 + 1);
  s2 = s1.substring(i1 + 1, i2);
  _CC32 = s2.toInt();
  s2 = s1.substring(i2 + 1);
  makeShortName(s2);
  s2.toCharArray(ch, 13);
  Bank *pB = new Bank(ch, _CC0, _CC32);
  currentDevice.addBank(pB);
  return pB;
}

void processPatch(Bank *pBank, String s, byte patchCount)
{
  String s2;
  int i1;
  char ch[17];
  int _devicePatchNr;
  i1 = s.indexOf(' ');
  s2 = s.substring(0, i1);
  _devicePatchNr = s2.toInt();
  s2 = s.substring(i1 + 1);
  makeShortName(s2, 16);
  memset(ch, NULL, 13);
  s2.toCharArray(ch, 16);
  Patch *pPatch = new Patch(ch, patchCount, _devicePatchNr);
  if (pPatch == NULL)
  {
    Serial.print("Patch Creation Failed");
    Serial.flush();
    return;
  }
  pBank->addPatch(pPatch);
}

void makeShortName(String &s, int len)
{
  String s1;
  int remDr = (len - 3) / 2;
  int n = s.length();
  if (n > len)
  {
    s1 = s.substring(0, remDr);
    s1 += "...";
    s1 += s.substring(n - remDr);
    s = s1;
  }
}

void collectFiles()
{
  File entry ;
  File root = SD.open("/");
  while (true)
  {
    entry =  root.openNextFile();
    if (! entry)
    {
      // no more files
      break;
    }
    if (!entry.isDirectory())
    {
      String *pS = new String(entry.name());
      files.add(pS);
    }
    entry.close();
  }
}


