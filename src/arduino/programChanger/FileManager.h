#include <arduino.h>
#include <print.h>
#include <SD.h>



#include "vector.h"

#include "device.h"
#include "bank.h"
#include "patch.h"




void doReadReafile(char* name);
Bank *processBank(String s);
void processPatch(Bank *pBank, String s, byte patchCount);
void processName(String s);
void makeShortName(String &s, int len = 12);
void collectFiles();
//

extern void message(String s);
extern Device currentDevice;
extern byte currentChannel;
extern byte currentBank;
extern byte currentPatch;
extern Vector<String *> files;

