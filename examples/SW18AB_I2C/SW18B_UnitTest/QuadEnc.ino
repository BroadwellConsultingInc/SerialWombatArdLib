#include "SerialWombatSimulatedQuadEnc.h"
//#define USEQEA

#ifdef USEQEA
SerialWombatQuadEnc qeA(SW6B);
SerialWombatSimulatedQuadEnc qeSimA(SW6C, SW6C, 2, 1, true, false);
#endif
SerialWombatSimulatedQuadEnc qeSimB(SW6F, SW6F, 3, 2, true, false);
SerialWombatQuadEnc qeB(SW6B);

void QuadEncTest()
{
  resetAll();
  SW6B.SetThroughputPin(10);

  #ifdef USEQUEA
  qeSimA.initialize();
  qeA.begin(5, 6,10,true);
  #endif
  
  qeSimB.initialize();
  qeB.begin(18, 19,10,true,QE_ONBOTH_POLL );
 
  uint16_t target = 30000;

  #ifdef USEQUEA
  qeA.write(target);
  qeSimA.targetPosition = qeSimA.currentPosition = target;
  #endif
  
  qeB.write(target);
  qeSimB.targetPosition = qeSimB.currentPosition = target;

  int testIteration;
  for (testIteration = 0; testIteration < 20; ++ testIteration)
  {
    //Serial.print("Test iteration: ");
    //Serial.println(testIteration);

    #ifdef USEQUEA
    qeSimA.targetPosition += testIteration;
    uint16_t lastASample = qeA.read();
    #endif 
    
    qeSimB.targetPosition += testIteration;
    
    uint16_t lastBSample = qeB.read();
    while(qeSimB.targetPosition != qeSimB.currentPosition 
    #ifdef USEQUEA
    || qeSimA.targetPosition != (uint16_t)qeSimA.currentPosition
    #endif
    )
    {
      /*
      Serial.print("B+: T: ");
      Serial.print(qeSimB.targetPosition);
      Serial.print ("C: ");
      Serial.println(qeSimB.currentPosition);
      */
      #ifdef USEQUEA
      qeSimA.service();
      #endif
      
      qeSimB.service();
      delay(1);
    }
    delay(10);

   #ifdef USEQUEA
    if (qeA.read() == qeSimA.currentPosition)
    {
      pass(0);
    }
    else
    {
      fail(0);
      /*
      Serial.print("0: ");
      Serial.print(qeA.read());
      Serial.print(" ");
      Serial.println(qeSimA.currentPosition);
      */
    }
    qeSimA.targetPosition -= 2*testIteration;
    #endif
    
     if (qeB.read() == (uint16_t)qeSimB.currentPosition)
    {
      pass(1);
    }
    else
    {
      fail(1);
      /*
      Serial.print("1: ");
      Serial.print(qeB.read());
      Serial.print(" ");
      Serial.println(qeSimB.currentPosition);
      */
    }
    qeSimB.targetPosition -= 2*testIteration;
    
   while(qeSimB.targetPosition != (uint16_t)qeSimB.currentPosition  
   #ifdef USEQEA
   || qeSimA.targetPosition != (uint16_t)qeSimA.currentPosition 
   #endif
   )
    {
      /*
      Serial.print("B-: T: ");
      Serial.print(qeSimB.targetPosition);
      Serial.print ("C: ");
      Serial.println(qeSimB.currentPosition);
      char output[100];
      */
      #ifdef USEQUEA
      qeSimA.service();
      #endif
      qeSimB.service();
      delay(1);
      uint16_t newSample;
      // newSample= qeB.read();
     }
     delay(10);
    #ifdef USEQUEA
    if (qeA.read() == (uint16_t)qeSimA.currentPosition)
    {
      pass(2);
    }
    else
    {
      fail(2);
      /*
            Serial.print("2: ");
      Serial.print(qeA.read());
      Serial.print(" ");
      Serial.println(qeSimA.currentPosition);
      */

    }
    #endif
     if (qeB.read() == (uint16_t)qeSimB.currentPosition)
    {
      pass(3);
    }
    else
    {
      fail(3);
      /*
            Serial.print("3: ");
      Serial.print(qeB.read());
      Serial.print(" ");
      Serial.println(qeSimB.currentPosition);
      */

    }
  }
}
