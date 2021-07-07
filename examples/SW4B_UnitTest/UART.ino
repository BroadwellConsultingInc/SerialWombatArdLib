
#define UART_BUF_SIZE 128
uint8_t uartTxBuffer[UART_BUF_SIZE];
uint8_t uartRxBuffer[UART_BUF_SIZE];
typedef struct
{
  uint8_t txpinA;
  uint8_t rxpinB;
  uint8_t txpinB;
  uint8_t rxpinA;
  SerialWombatUART* uartA;
  SerialWombatUART* uartB;
}
uartSetup_t;

uartSetup_t uartTests[] =
{
  {3, 1, 3, 1, &uart1, &uart2},
  {1, 3, 1, 3, &uart1, &uart2},
  {1, 3, 2, 2, &uart1, &uart2},
  {2, 0, 2, 0, &uart1, &uart3}
};
void uartTest()
{

  uint32_t randSeed = 1;
//for (int test = 0; test < 4; ++test) 
  int test = 0;  //Versions 2.0.3 and prior do not clear PPS, leading to crossed paths and loopbacks.  Only run one iteration for now.
  {
    resetAll();
    SerialWombatUART *uartA, *uartB;

    uartA = uartTests[test].uartA;
    uartB = uartTests[test].uartB;
    uartA->begin(115200, uartTests[test].txpinA, uartTests[test].rxpinA, uartTests[test].txpinA);
    uartB->begin(115200, uartTests[test].txpinB, uartTests[test].rxpinB, uartTests[test].txpinB);
    Serial.print("UART TEST #"); Serial.println(test);

    for (int iteration = 0; iteration < 100; ++iteration)
    {
      //Serial.print("\r\nUART Iteration #"); Serial.println(iteration);
      updateDisplay();
      int length = wrandom(&randSeed) & 0x7F;
      for (int i = 0; i < length; ++i)
      {
        uartTxBuffer[i] = (uint8_t)wrandom(&randSeed);
      }


      uartA->write(uartTxBuffer, length);
     // Serial.printf("\r\nWrote %d bytes to A",length);
     // delay(1000);
      int bytesReceived = uartB->readBytes((char*)uartRxBuffer, length);
      if (bytesReceived == length)
      {
        pass(((test + 100) << 24) + (iteration << 16) + 0xFFFF);
      }
      else
      {
        #ifdef PRINT_FAILURES
        fail(((test + 100) << 24) + (iteration << 16) + 0xFFFF);
        #endif
        Serial.printf("\r\nRead on B: Expected to receive %d bytes, got %d\r\n", length, bytesReceived);
       }
     for (int i = 0; i < length; ++i)
      {
        if (uartRxBuffer[i] == uartTxBuffer[i])
        {
          pass((test << 24) + (iteration << 16) + i);
        }
        else
        {
          #ifdef PRINT_FAILURES
          Serial.printf("MFB %d %X %X ", i,uartRxBuffer[i],uartTxBuffer[i]);
          #endif
          fail((test << 24) + (iteration << 16) + i);

        }
      }
      length = wrandom(&randSeed) & 0x7F;
      for (int i = 0; i < length; ++i)
      {
        uartTxBuffer[i] = (uint8_t)wrandom(&randSeed);
      }


      uartB->write(uartTxBuffer, length);
      //Serial.printf("\r\n");
      bytesReceived = uartA->readBytes((char*)uartRxBuffer, length);
      if (bytesReceived == length)
      {
        pass(((test + 100) << 24) + (iteration << 16) + 0xFFFF);
      }
      else
      {
        fail(((test + 100) << 24) + (iteration << 16) + 0xFFFF);
        #ifdef PRINT_FAILURES
        Serial.printf("Read on A: Expected to receive %d bytes, got %d\r\n", length, bytesReceived);
        #endif
 
      }
   
     for (int i = 0; i < length; ++i)
      {
        if (uartRxBuffer[i] == uartTxBuffer[i])
        {
          pass(((test + 100) << 24) + (iteration << 16) + i);
        }
        else
        {
          fail(((test + 100) << 24) + (iteration << 16) + i);
          #ifdef PRINT_FAILURES
                 Serial.printf("MFA %d %X %X ", i,uartRxBuffer[i],uartTxBuffer[i]);
          #endif
 
        }
      }
        delay(100);
    }
  

  }

}
