#pragma once

byte state = 0;

#define STATE_DEVICE 3
#define STATE_BANK 2
#define STATE_CHANNEL 1
#define STATE_PROGRAM 0

const char VK_DEVICE = 'A';
const char VK_BANK = 'B';
const char VK_CHANNEL = 'C';
const char VK_PROGRAM ='D';
const char VK_BWD = '*';
const char VK_FWD = '#';

void sendMidiNote(int );
void updateScreen(char );
void advance(int n = 1);
void processKey(byte);

bool channelMessageThruFilter(midi::MidiType inType,  midi::DataByte &inData1,  midi::DataByte &inData2,  midi::Channel &inChannel);

void getPatchName(byte);
void collectFiles();

typedef struct 
{
  byte internalPatchID; 
  int nmIdx;  
} Patch;

typedef struct 
{  
  byte CC0; 
  byte CC32; 
  byte maxPatches; 
  int nmIdx;
  Patch p[128];
} Bank;

typedef struct 
{
  byte maxBanks; 
  Bank b[16];
} 
Device;

bool mustRefreshScreen = false;

char currentDeviceName[13] = "--none--";
char currentBankName[13] =   "BANK";
char currentPatchName[17] =  "PATCH";

byte currentChannel = 1;
byte currentBankID = 0;
byte currentPatchID = 0;
byte currentInternalPatchID = 0;

#define _screen1 0
#define _screen2 1
#define _screen3 2
#define _screen4 3
#define _fmt2 4
#define _fmt3 5
#define _fmt4 6
#define _fmt5 7

const char PROGMEM screen1[] = "CH %2d | %12s";
const char PROGMEM screen2[] = "B  %2d | %12s";
const char PROGMEM screen3[] = "P %03d |        (%3d)";
const char PROGMEM screen4[] = "%16s ###";
const char PROGMEM fmt2[] = "%02d";
const char PROGMEM fmt3[] = "%03d";
const char PROGMEM fmt4[] = "Patch (%03d)";
const char PROGMEM fmt5[] = "XXXXX";

char pbuffer[21];
char rbuffer[81];



#define _s(i) \
 strcpy_P(pbuffer, (char*)pgm_read_word(&(string_table[i]))) // Necessary casts and dereferencing, just copy.

const char* const string_table[] PROGMEM = {screen1, screen2, screen3, screen4, fmt2, fmt3,fmt4, fmt5};

byte charcount = 0;

char input[4] = "000";


File deviceFile;
Device currentDevice;
char pDevices[12][9];
byte deviceCount = 0;


