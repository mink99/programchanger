
#include <stdint.h>
#include <malloc.h>
#include <arduino.h>
#include <print.h>

using namespace std;

#include "vector.h"


#include "device.h"
#include "bank.h"
#include "patch.h"

extern Device currentDevice;


void dumpBank(Bank *pB)
{
  Serial.print(" ");
  Serial.print(pB->getName());
  Serial.print(" ");
  Serial.print(pB->getCC0());
  Serial.print("/");
  Serial.print(pB->getCC32());
  Serial.print("=");
  Serial.println(pB->getPatchCount());
  Serial.flush();
}

void testBanks()
{

  Serial.println();
  Serial.println("----------------- Testing Banks ----------------- ");
  Serial.flush();

  Bank *pB;
  Patch *pP;
  Serial.print("iterating Banks per index... ");
  Serial.print(currentDevice.getBankCount());
  Serial.println("... ");
  Serial.flush();
  for (int i = 0; i < currentDevice.getBankCount();  i++)
  {
    pB = currentDevice.getBank(i);
    dumpBank(pB);
  };
  Serial.println();
  Serial.println("iterating Banks per next... ");
  Serial.flush();
  pB = currentDevice.getBank(0);
  dumpBank(pB);

  for (int i = 0; i < 41;  i++)
  {
    pB = currentDevice.getNextBank();
    dumpBank(pB);
    Serial.print(i);

  };
  Serial.println();

  pB = currentDevice.getBank(5);
  Serial.println("iterating Banks per previous... ");
  for (int i = 0; i < 41;  i++)
  {
    pB = currentDevice.getPrevBank();
    dumpBank(pB);
    Serial.print(i);

  };
  Serial.println();
  Serial.println("----------finish Testing Banks ----------------- ");
  Serial.print("MEM:");
  Serial.println(FreeMem());
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
  Serial.println(*pP);
  for (int i = 0; i < 41;  i++)
  {
    pP = pB->getNextPatch();
    Serial.println(*pP);
  };
  Serial.println();
  Serial.println("iterating Patches 3 on bank 1 per prev ... ");


  for (int i = 0; i < 41;  i++)
  {
    pP = pB->getPrevPatch();
    Serial.println(*pP);
  };
  Serial.println();
  Serial.println("------------finished Testing Patches ----------------- ");
  Serial.print("MEM:");
  Serial.println(FreeMem());


}


