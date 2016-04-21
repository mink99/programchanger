#include <avr/pgmspace.h>
#include <SPI.h>
#include <SD.h>

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


#include "remote_sd_defines.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal_I2C lcd(0x27, 20, 4);
MIDI_CREATE_DEFAULT_INSTANCE();


void setup() {

  lcd.begin();
  lcd.blink();

  SD.begin(13);

  MIDI.begin();
  MIDI.turnThruOn(midi::Full);
  MIDI.setChannelMessageThruFilter(channelMessageThruFilter);


}

void loop() {

  MIDI.read();

  char key = keypad.getKey();
  mustRefreshScreen = true;

  switch (key)
  {
    case 0:
      mustRefreshScreen = false;
      break;
    case VK_DEVICE:
      {
        state = STATE_DEVICE;
        sendMidiNote(99);
        break;
      }
    case VK_BANK:
      {
        state = STATE_BANK;
        break;
      }
    case VK_CHANNEL:
      {
        state = STATE_CHANNEL;
        break;
      }
    case VK_PROGRAM:
      {
        state = STATE_PROGRAM;
        break;
      }
    case VK_BWD:
      {
        advance(-1);
        break;
      }
    case VK_FWD:
      {
        advance();
        break;
      }
    default :
      //    case '0':
      //    case '1':
      //    case '2':
      //    case '3':
      //    case '4':
      //    case '5':
      //    case '6':
      //    case '7':
      //    case '8':
      //    case '9':
      {
        processKey(key - 0x30);
        break;
      }

  }
  if (mustRefreshScreen)
  {
    updateScreen(key);
  }
}

void sendMidiNote(int note)
{


  MIDI.sendNoteOn(note, 80, 1); // Send a Note (pitch 42, velo 127 on channel 1)
  delay(1000);            // Wait for a second
  MIDI.sendNoteOff(note, 0, 1); // Stop the note
}

void updateScreen(char key)
{

  char buf[21];

  lcd.setCursor(0, 0);
  sprintf(buf, _s(_screen1), currentChannel, currentDeviceName);
  lcd.print(buf);
  lcd.setCursor(0, 1);
  sprintf(buf,  _s(_screen2), currentBankID, currentBankName);
  lcd.print(buf);
  lcd.setCursor(0, 2);
  sprintf(buf,  _s(_screen3), currentPatchID, currentInternalPatchID);
  lcd.print(buf);
  lcd.setCursor(0, 3);
  sprintf(buf,  _s(_screen4), currentPatchName);
  lcd.print(buf);



  switch (state)
  {
    case STATE_DEVICE:
      {
        lcd.setCursor(8 , 0);
        break;
      }
    case STATE_BANK:
      {
        sprintf(input, _s(_fmt2), currentBankID);
        lcd.setCursor(3 + charcount, 1);
        break;
      }
    case STATE_CHANNEL:
      {
        sprintf(input, _s(_fmt2), currentChannel);
        lcd.setCursor(3 + charcount, 0);
        break;
      }
    case STATE_PROGRAM:
      {
        sprintf(input, _s(_fmt3), currentPatchID);
        lcd.setCursor(2 + charcount, 2);
        break;
      }

  }
}
void advance(int n)
{
  switch (state)
  {
    //    case STATE_DEVICE:
    //    {
    //       lcd.setCursor(8, 0);
    //        break;
    //      }
    case STATE_BANK:
      {
        currentBankID += n;
        if (currentBankID > 15) currentBankID = 0;
        if (currentBankID < 0) currentBankID = 15;

        break;
      }
    case STATE_CHANNEL:
      {
        currentChannel += n;
        if (currentChannel > 16) currentChannel = 1;
        if (currentChannel < 1) currentChannel = 16;

        break;
      }
    case STATE_PROGRAM:
      {
        currentPatchID += n;
        if (currentPatchID > 0xff) currentPatchID = 127;
        if (currentPatchID > 127) currentPatchID = 0;


        break;
      }
  }

}
void processKey(byte num)
{
  char bk = input[charcount];

  input[charcount] = num + 48;

  byte n4 = atoi(input);

  switch (state)
  {
      {
        //    case STATE_DEVICE:
        //      {
        //        lcd.setCursor(8, 0);
        //        break;
      }
    case STATE_BANK:
      {
        if (n4 > 15)
        {
          input[charcount] = bk;
        }
        else
        {
          currentBankID = n4;
          charcount++;
          if (charcount > 1) charcount = 0;
        }
        break;
      }
    case STATE_CHANNEL:
      {
        if ((n4 > 16) || (n4 == 0))
        {
          input[charcount] = bk;
        }
        else
        {
          currentChannel = n4;
          charcount++;
          if (charcount > 1) charcount = 0;
        }
        break;
      }
    case STATE_PROGRAM:
      {
        if (n4 > 127)
        {
          input[charcount] = bk;
        }
        else
        {
          currentPatchID = n4;
          charcount++;
          if (charcount > 2) charcount = 0;
        }
        break;
      }
  }
}
void getPatchName(byte)
{
  //  if (deviceFile)
  {
    sprintf(currentPatchName , _s(_fmt4));
  }
}

bool channelMessageThruFilter(midi::MidiType inType,  midi::DataByte &inData1,  midi::DataByte &inData2,  midi::Channel &inChannel)
{
  if (inType == midi::ActiveSensing )  return false;

  if (inType == midi::ProgramChange )
  {
    //Question : take the channel ?
    currentPatchID = inData1;
    updateScreen(VK_CHANNEL);
    return false;
  }
}

void collectFiles()
{
  File root;
  File entry;
 
  for (byte i = 0; i <12; i ++)
  {
    entry =  root.openNextFile();
    if (!entry) // ende
    {
      deviceCount = i;
      break;
    }
    if (entry.isDirectory())
    {
      continue;
    }
    else
    {
      strcpy(pDevices[i],entry.name());
    }
    entry.close();
  }

}
