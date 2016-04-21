#include <LiquidCrystal_I2C.h>

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

#include <Key.h>
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>


#include "vector.h"

#include "device.h"
#include "bank.h"
#include "patch.h"

#include "NumEdit.h"


void testBanks();
void testPatches();
void makeShortName(String &s, int len = 12);


#define MODE_NONE 0
#define MODE_SELECT_DEVICE 'A'
#define MODE_SELECT_CHANNEL 'C'
#define MODE_SELECT_BANK 'B'
#define MODE_SELECT_PATCH 'D'

//                        12345678901234567890
//const char row_1[21] = "D|############ CH ##"; 
//const char row_2[21] = "B|###   ############";
//const char row_3[21] = "P|### (###)      ### ";
//const char row_4[21] = "################ ###";

const char row_1[] = "D|%12s CH %02d";
const char row_2[] = "B|%03d  %12s";
const char row_3[] = "P|%03d (%03d)    %03d ";
const char row_4[] = "%16s ";

Device currentDevice;
byte currentChannel = 1;
byte currentBank = 1;
byte currentPatch = 1;


const int chipSelect = 4;
bool mustRefreshScreen = false;

byte programState = MODE_SELECT_PATCH;

Vector<String *> files(12);




const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};



byte rowPins[ROWS] = {45, 43, 41, 39}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {37, 35, 33, 31}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 20, 4);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,    MIDI);



void setup()
{
  lcd.begin();



  lcd.setCursor(0, 0);
  lcd.print("+++ Please Wait +++");

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  message("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    message("Card failed, or not present");
    // don't do anything more:
    return;
  }

  message("Initializing Midi...");


  MIDI.begin();
  MIDI.turnThruOn(midi::Full);
  MIDI.setChannelMessageThruFilter(channelMessageThruFilter);
  message("Initializing files...");
  collectFiles();


  doReadReafile("default.rea");

  //doReadReafile("gm.rea");
  //doReadReafile("prot.rea");
  //doReadReafile("ion.rea");

  
  message("Initializing succ.");
  lcd.blink();
  mustRefreshScreen = true;
}

void loop()
{
  MIDI.read();

  if (mustRefreshScreen)
  {
    updateScreen();
  }

  char key = keypad.getKey();
  if (!key) return;
  switch (key)
  {
    case 'A':
      {
        programState = MODE_SELECT_DEVICE;
        mustRefreshScreen = true;
        break;
      }
    case 'C':
      {
        programState = MODE_SELECT_CHANNEL;
        mustRefreshScreen = true;
        break;
      }
    case 'B':
      {
        programState = MODE_SELECT_BANK;
        mustRefreshScreen = true;
        break;
      }
    case 'D':

      {
        programState = MODE_SELECT_PATCH;
        mustRefreshScreen = true;
        break;
      }
  }



}


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
//

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

bool channelMessageThruFilter(midi::MidiType inType,  midi::DataByte &inData1,  midi::DataByte &inData2,  midi::Channel &inChannel)
{
  if (inType == midi::ActiveSensing )  return false;
  if (inType == midi::ProgramChange )
  {
    //Question : take the channel ?
    //    currentPatchID = inData1;
    //updateScreen(VK_CHANNEL);
    return false;
  }
}
void sendProgramChange(byte CC0, byte CC32, byte prg, byte channel)
{
  MIDI.sendControlChange(0, CC0 , channel);
  MIDI.sendControlChange(32, CC32, channel);
  MIDI.sendProgramChange(prg, channel);
}

void message(String s)
{
  lcd.setCursor(0, 3);
  Serial.println();
  Serial.println(s);
  lcd.print(s);

}
void updateScreen()
{
  Bank *pB;
  Patch *pP;
  char ch[21];
  
  lcd.setCursor(0, 0);
  sprintf(ch,row_1,currentDevice.getName(),currentChannel);
  lcd.print(ch);
  
  lcd.setCursor(0, 1);
  sprintf(ch,row_2,currentBank,"BANK 000"); //pB->name D.currentBank;
  lcd.print(ch);
  
  lcd.setCursor(0, 2);
  sprintf(ch,row_3,currentPatch,currentPatch,128);  // pP-> patchNr, pP-> devicePatchNr , pB->getPatchcount
  lcd.print(ch);
  
  lcd.setCursor(0, 3);
  sprintf(ch,row_4,"Patch 000"); //pP->name
  lcd.print(ch);

  
  switch (programState)
  {
    case MODE_SELECT_DEVICE:
      {
        lcd.setCursor(6, 0); // Patch Mode
        break;
      }
    case MODE_SELECT_CHANNEL:
      {
        lcd.setCursor(18, 2); // Patch Mode
        break;
      }
    case MODE_SELECT_BANK:
      {
        lcd.setCursor(4, 1); // Patch Mode
        break;
      }
    case MODE_SELECT_PATCH:
      {
        lcd.setCursor(4, 2); // Patch Mode
        break;
      }
  }
  mustRefreshScreen = false;

}
