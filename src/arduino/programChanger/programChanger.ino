
#include <LiquidCrystal_I2C.h>

#include <MIDI.h>


#include <Key.h>
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "memory.h"
#include "vector.h"

#include "device.h"
#include "bank.h"
#include "patch.h"

#include "NumEdit.h"
#include "ListEdit.h"

#include "FileManager.h"


void testBanks();
void testPatches();



#define MODE_NONE 0
#define MODE_SELECT_DEVICE 'A'
#define MODE_SELECT_CHANNEL 'C'
#define MODE_SELECT_BANK 'B'
#define MODE_SELECT_PATCH 'D'

//                        12345678901234567890
//const char row_1[21] = "D|############ CH ##";
//const char row_2[21] = "B| ##   ############";
//const char row_3[21] = "P|### (###/###)     ";
//const char row_4[21] = "################ ###";

const char row_1[] = "D|%-12s CH %02d";
const char row_2[] = "B| %02d   %-12s";
const char row_3[] = "A|%03d (%03d/%03d)";
const char row_4[] = "%-16s ";



uint8_t VBAR1[8]  =
{
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
};

uint8_t VBAR2[8]  =
{
  0b00100,
  0b00100,
  0b00100,
  0b11011,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
};


Field fBank = {3, 1, 2};
Field fChannel = {18, 0, 2};
Field fProgram = {2, 2, 3};

Value vBank = {0, 24, 0, "%02d" };
Value vChannel = {1, 16, 1, "%02d" };
Value vProgram = {0, 127, 0, "%03d" };


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
NumEdit numEdit;
ListEdit listEdit;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,    MIDI);




void setup()
{
  lcd.begin();

  lcd.createChar(1, VBAR1);
  lcd.createChar(2, VBAR2);

  lcd.setCursor(0, 0);
  lcd.print("+++ Please Wait +++");

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  numEdit.begin(&lcd, &keypad);
  numEdit.setPollingHook(onUpdateMidi);
  listEdit.begin(&lcd, &keypad);
  listEdit.setPollingHook(onUpdateMidi);
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

  switchedDevice();
  message("Initializing succ.");
  //lcd.blink();
  mustRefreshScreen = true;
}

void loop()
{
  onUpdateMidi();
  if (mustRefreshScreen)
  {
    updateScreen();
  }

  char key = getKey2();
  if (!key) return;
  switch (key)
  {
    case 'D':
      {
        programState = MODE_SELECT_DEVICE;
        out << "\n before load MEM:" << freeMem() << endl;
        int n = listEdit.select(files);
        if (n >= 0)
        {
          currentDevice.clear();
          doReadReafile(*files[n]);
          currentChannel = 0;
          onUpdateBank(0);
          currentPatch = 0;
        }
        out << "\n after load MEM: (" << n << ")"<< freeMem() << endl;

        mustRefreshScreen = true;
        break;
      }
    case 'C':
      {
        programState = MODE_SELECT_CHANNEL;
        vChannel.value = currentChannel;
        int n = numEdit.edit(fChannel, vChannel);
        currentChannel = vChannel.value ;
        mustRefreshScreen = true;
        break;
      }
    case 'B':
      {
        programState = MODE_SELECT_BANK;
        numEdit.setUpdateHook(onUpdateBank);
        vBank.value = currentBank;
        int n = numEdit.edit(fBank, vBank);
        currentBank = vBank.value ;
        numEdit.removeUpdateHook();
        mustRefreshScreen = true;
        break;
      }
    case 'A':

      {
        if (programState == MODE_SELECT_PATCH)
        {
          out << "---------------------- \n Should send Midi now ...\n";
        }
        programState = MODE_SELECT_PATCH;
        numEdit.setUpdateHook(onUpdateProgram);
        vProgram.value = currentPatch;
        int n = numEdit.edit(fProgram, vProgram);
        currentPatch = vProgram.value ;
        numEdit.removeUpdateHook();
        mustRefreshScreen = true;
        break;
      }
  }
}


//

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
  // fix for P600
  if (CC0 >= 0)
  {
    MIDI.sendControlChange(0, CC0 , channel);
    MIDI.sendControlChange(32, CC32, channel);
  }
  MIDI.sendProgramChange(prg, channel);
}

void message(String s)
{
  lcd.setCursor(0, 3);
  out <<s << endl;
  lcd.print(s);
}

void onUpdateBank(int value)
{

  currentBank = value;
  switchedBank();
  updateScreen();
}
void onUpdateProgram(int value)
{
  Bank *pB = currentDevice.getBank(currentBank);
  if (pB->getPatchCount() - 1 < value ) return;
  currentPatch = value;
  updateScreen();

}
void onUpdateMidi()
{
  MIDI.read();
}

void updateScreen()
{
  lcd.clear();
  Bank *pB = currentDevice.getBank(currentBank);
  Patch *pP = pB->getPatch(currentPatch);
  char ch[21];

  lcd.setCursor(0, 0);
  sprintf(ch, row_1, currentDevice.getName(), currentChannel);
  lcd.print(ch);

  lcd.setCursor(0, 1);
  sprintf(ch, row_2, currentBank, pB->getName()); //pB->name D.currentBank;
  lcd.print(ch);

  lcd.setCursor(0, 2);
  sprintf(ch, row_3, pP->getPatchNr(), pP->getDevicePatchNr(),  pB->getPatchCount()); // pP-> patchNr, pP-> devicePatchNr , pB->getPatchcount
  lcd.print(ch);

  lcd.setCursor(0, 3);
  sprintf(ch, row_4, pP->getName()); //pP->name
  lcd.print(ch);

  mustRefreshScreen = false;


}
void switchedBank()
{
  Bank *pB = currentDevice.getBank(currentBank);
  vProgram.maxValue = pB->getPatchCount() - 1;
  if (currentPatch > vProgram.maxValue) currentPatch = vProgram.maxValue;
}
void switchedDevice()
{
  currentBank = 0;
  vBank.maxValue = currentDevice.getBankCount() - 1;
  switchedBank();
}
char getKey2()
{
  char key = keypad.getKey();
  if (key) return key;
  return numEdit.getLastKey();
}


