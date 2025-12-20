SerialWombatUART sw18UART1(SW18AB_6B);
SerialWombatUART sw18UART2(SW18AB_6B);
SerialWombatUART sw8UART1(SW8B_68);
SerialWombatUART sw8UART2(SW8B_68);
SerialWombatUART UART1Match_6C(SW4B_6C);
SerialWombatUART UART2Match_6E(SW4B_6E);

SerialWombatUART* hwUART1;
SerialWombatUART* hwUART2;
SerialWombatUART* UART1Match;
SerialWombatUART* UART2Match;
uint8_t uartRx[200];
uint8_t uartTx[200];
uint32_t txSeed = 1;
uint32_t countSeed = 1;

uint32_t baudArray[] =
{
  300,
  1200,
  2400,
  4800,
  9600,
  19200,
  38400,
  57600,
  115200
};


int uartHWTest(SerialWombatChip &sw , uint8_t rxPin0, uint8_t txPin0, uint8_t rxPin1,uint8_t txPin1)
{
  if (&sw == &SW18AB_6B)
  {
    hwUART1 = &sw18UART1;
    hwUART2 = &sw18UART2;
    UART1Match = &UART1Match_6C;
    UART2Match = &UART2Match_6E;
     SW4B_6C.pinMode(1,OUTPUT);
  SW4B_6C.digitalWrite(1,HIGH);
  SW4B_6E.pinMode(1,OUTPUT);
  SW4B_6E.digitalWrite(1,HIGH);
  }
  else if (&sw == &SW8B_68)
  {

    hwUART1 = &sw8UART1;
    hwUART2 = NULL;
    UART1Match = &sw18UART1;
  }
  else
  {
    test("HW UART Invalid SW Chip",0,1);
  }

  
 


int baudIteration = 0;
if (&sw == &SW8B_68)
{
  baudIteration = 1;
}
for (; baudIteration < 9; ++ baudIteration)  // TODO return to 0
{
  //uint32_t rxSeed = txSeed;
  uint32_t delayMs = 10000/ baudArray[baudIteration];
  int txcount = 0;
  hwUART1->begin(baudArray[baudIteration], rxPin0, rxPin0, txPin0);

    if (&sw == &SW18AB_6B)
  {

  UART1Match->begin(baudArray[baudIteration], 0, 0, 1);
  }
  else if (&sw== &SW8B_68)
  {

  UART1Match->begin(baudArray[baudIteration], 9, 9, 7);    
  }

  if (&sw == &SW18AB_6B)
  {
  hwUART2->begin(baudArray[baudIteration], rxPin1, rxPin1, txPin1, 2);
  UART2Match->begin(baudArray[baudIteration], 3, 3, 1);
  }
  for (int iteration = 0; iteration < 500; ++ iteration)
  {
    txcount = wrandom(&countSeed) % 32;

    for (int i = 0; i < txcount; ++i)
    {
      uartTx[i] = wrandom(&txSeed);
    }
      SW18AB_6B.readPublicData(6); //TODO Remove

    int bytesWritten = hwUART1->write(uartTx, txcount);
    test("HWU 0",bytesWritten,txcount);
    
    delay(delayMs * txcount);
    int bytesRead = UART1Match->readBytes((char*)uartRx, txcount);
    test("HWU 1",bytesRead,txcount);
    for (int i = 0; i < txcount; ++i)
    {
      char c[80];
      sprintf(c,"HWU 2 Baud: %d, iter: %d, i %d",baudIteration, iteration,i);
            test(c,uartRx[i],uartTx[i]);
      /*
      if (uartTx[i] == uartRx[i])
      {
        pass(1);
      }
      else
      {
        Serial.print("F0: b: ");
        Serial.print(baudIteration);
        Serial.print("iter: ");
        Serial.print(iteration);
        Serial.print(" i: ");
        Serial.println(i);
        fail(1);
      }
      */
    }

    txcount = wrandom(&countSeed) % 32;

    for (int i = 0; i < txcount; ++i)
    {
      uartTx[i] = wrandom(&txSeed);
    }
    bytesWritten = UART1Match->write(uartTx, txcount);
    test("HWU 4",bytesWritten,txcount);
    delay(delayMs * txcount);
    bytesRead = hwUART1->readBytes((char*)uartRx, txcount);
    test("HWU 5",bytesRead,txcount);
    for (int i = 0; i < txcount; ++i)
    {
       char c[80];
      sprintf(c,"HWU 6 Baud: %d, iter: %d, i %d",baudIteration, iteration,i);
            test(c,uartRx[i],uartTx[i]);
      /*
      if (uartTx[i] == uartRx[i])
      {
        pass(1);
      }
      else
      {
        Serial.print("F1: b: ");
        Serial.print(baudIteration);
        Serial.print(" iter: ");
        Serial.print(iteration);
        Serial.print(" i: ");
        Serial.print(i);
        Serial.print(" X: ");
        Serial.print(uartTx[i]);
        Serial.print(" G: ");
        Serial.println(uartRx[i]);
        fail(1);
      }
      */
    }
  if (&sw == &SW18AB_6B)
  {

    txcount = wrandom(&countSeed) % 32;

    for (int i = 0; i < txcount; ++i)
    {
      uartTx[i] = wrandom(&txSeed);
    }
    
    bytesWritten = hwUART2->write(uartTx, txcount);
    test("HWU 7",bytesWritten,txcount);
    delay(delayMs * txcount);
    bytesRead = UART2Match->readBytes((char*)uartRx, txcount);
    test("HWU 8",bytesRead,txcount);
    for (int i = 0; i < txcount; ++i)
    {  char c[80];
      sprintf(c,"HWU 9 Baud: %d, iter: %d, i %d",baudIteration, iteration,i);
            test(c,uartRx[i],uartTx[i]);
      /*
      if (uartTx[i] == uartRx[i])
      {
        pass(1);
      }
      else
      {
                Serial.print("F2: b: ");
        Serial.print(baudIteration);
        Serial.print("iter: ");
        Serial.print(iteration);
        Serial.print(" i: ");
        Serial.println(i);
        fail(1);
      }
      */
    }

    txcount = wrandom(&countSeed) % 32;

    for (int i = 0; i < txcount; ++i)
    {
      uartTx[i] = wrandom(&txSeed);
    }
    bytesWritten = UART2Match->write(uartTx, txcount);
      test("HWU 10",bytesWritten,txcount);
    delay(delayMs * txcount);
    bytesRead = hwUART2->readBytes((char*)uartRx, txcount);
      test("HWU 11",bytesRead,txcount);
    for (int i = 0; i < txcount; ++i)
    {
       char c[80];
       sprintf(c,"HWU 12 Baud: %d, iter: %d, i %d",baudIteration, iteration,i);
      test(c,uartRx[i],uartTx[i]);
      /*
      if (uartTx[i] == uartRx[i])
      {
        pass(1);
      }
      else
      {
                Serial.print("F3: b: ");
        Serial.print(baudIteration);
        Serial.print("iter: ");
        Serial.print(iteration);
        Serial.print(" i: ");
        Serial.println(i);
        fail(1);
      }
      */
    }
  }    
    
  }
}
  return (0);
}
