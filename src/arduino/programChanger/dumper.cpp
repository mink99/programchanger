
#include <stdint.h>
#include <malloc.h>
#include <arduino.h>
#include <print.h>

using namespace std;

#include "memory.h"
#include "vector.h"


#include "device.h"
#include "bank.h"
#include "patch.h"

extern Device currentDevice;


void dumpBank(Bank *pB)
{
  out << " " << pB->getName() << " " << pB->getCC0() << "/" << pB->getCC32() << "=" << pB->getPatchCount();
  out.flush();
}

void testBanks()
{

  out << "\n----------------- Testing Banks -----------------\n ";
  out.flush();

  Bank *pB;
  Patch *pP;
  out << "iterating Banks per index... " << currentDevice.getBankCount() << "... ";
  out.flush();
  for (int i = 0; i < currentDevice.getBankCount();  i++)
  {
    pB = currentDevice.getBank(i);
    dumpBank(pB);
  };
  
  out << "\niterating Banks per next... ";
  out.flush();
  pB = currentDevice.getBank(0);
  dumpBank(pB);

  for (int i = 0; i < 41;  i++)
{
  pB = currentDevice.getNextBank();
    dumpBank(pB);
    out << i;

  };
  out << endl;

  pB = currentDevice.getBank(5);
 out << "\nIterating Banks per previous... \n";
  for (int i = 0; i < 41;  i++)
{
  pB = currentDevice.getPrevBank();
    dumpBank(pB);
    out << i;

  };
  
  out << "\n----------finish Testing Banks ----------------- " << "MEM:" << freeMem();
  Serial.flush();
}
void testPatches()
{
  Serial.println();
  Serial.println("----------------- Testing Patches ----------------- ");

  Bank *pB;
  Patch *pP;

  Serial.println();
  Serial.println("iterating Patches on bank 1 per index ... ");

  pB = currentDevice.getBank(1);
  dumpBank(pB);

  for (int i = 0; i < pB->getPatchCount();  i++)
  {
    pP = pB->getPatch(i);
    Serial.println(*pP);
  };
  Serial.println();
  Serial.println("iterating Patches 2 on bank 1 per next on 93... ");

  pP = pB->getPatch(93);
  out << *pP << endl;
  for (int i = 0; i < 41;  i++)
  {
    pP = pB->getNextPatch();
    out << *pP << endl;
  };

  out << "\n iterating Patches 3 on bank 1 per prev ...\n ";


  for (int i = 0; i < 41;  i++)
  {
    pP = pB->getPrevPatch();
    Serial.println(*pP);
  };

  out << "\n ------------finished Testing Patches ----------------- " << "MEM:" << freeMem() << endl;


}


