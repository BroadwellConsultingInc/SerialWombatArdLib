SerialWombatSWUART software18UART0(SW18AB_6B); // 7 9
SerialWombatSWUART software18UART1(SW18AB_6B); // 19 18
SerialWombatSWUART software18UART2(SW18AB_6B);  // 17 16
SerialWombatSWUART software18UART3(SW18AB_6B); // 0 6
SerialWombatSWUART software8UART0(SW8B_68);
SerialWombatSWUART software8UART1(SW8B_68);
SerialWombatSWUART software8UART2(SW8B_68);
SerialWombatSWUART software8UART3(SW8B_68);
SerialWombatUART swUart7Rx(SW4B_6C);
SerialWombatUART swUart17Rx(SW4B_6E);

SerialWombatSWUART* software18Uarts[] = {&software18UART0,&software18UART1,&software18UART2,&software18UART3};
SerialWombatSWUART* software8Uarts[] = {&software8UART0,&software8UART1,&software8UART2,&software8UART3};
#define SWUART_TEST_QUEUE_MAX_LENGTH  32
uint16_t swUartTestQueueLength;
uint8_t uart18Rx[4][SWUART_TEST_QUEUE_MAX_LENGTH];
uint8_t uart18Tx[4][SWUART_TEST_QUEUE_MAX_LENGTH];
uint8_t uart8Rx[4][SWUART_TEST_QUEUE_MAX_LENGTH];
uint8_t uart8Tx[4][SWUART_TEST_QUEUE_MAX_LENGTH];
uint8_t uart8TxLength[4];
uint8_t uart18TxLength[4];
int numberOfSWUarts;


int uartSWTest()
{
  Serial.println("Pin Memory Queue SWUART TEST");
  txSeed = 1;
  countSeed = 1;
   swUartTestQueueLength = 32;
   numberOfSWUarts = 4;
  software18UART0.begin(19200,9,9,7);
  software8UART0.begin(19200,5,5,4);

  software18UART1.begin(300,19,19,18);
  software8UART1.begin(300,6,6,7);

    software18UART2.begin(2400,17,17,16);
  software8UART2.begin(2400,2,2,3);

   software18UART3.begin(9600,0,0,6);
  software8UART3.begin(9600,0,0,1);

  swUart7Rx.begin(19200, 0, 0, 255);
  swUart17Rx.begin(19200, 3, 3, 255); // SW8B Transmit on pin 3 to correspond to this
  
   swUARTLoop();

Serial.println("User Memory Queue SWUART TEST");
    txSeed = 1;
  countSeed = 1;
   swUartTestQueueLength = 12;
   numberOfSWUarts = 2;
  software18UART0.begin(19200,9,9,7,0,swUartTestQueueLength,swUartTestQueueLength);
  software8UART0.begin(19200,5,5,4,0,swUartTestQueueLength,swUartTestQueueLength);

  software18UART1.begin(9600,19,19,18, 50,swUartTestQueueLength,swUartTestQueueLength);
  software8UART1.begin(9600,6,6,7,50,swUartTestQueueLength,swUartTestQueueLength);
   swUARTLoop();
  return (0);
}

void swUARTLoop()
{
   delay(1000);
   for (int uart_i = 0; uart_i < numberOfSWUarts; ++uart_i)
   {
      while(software18Uarts[uart_i]->read() != -1){delay(0);}
      while(software8Uarts[uart_i]->read() != -1){delay(0);}
   }

  
  for (int iteration = 0; iteration < 200; ++ iteration)
  {
    
    uint8_t txCount18[4];
    uint8_t txCount8[4];
    uint32_t rxSeed = txSeed;
    
    for (int uart_i = 0; uart_i < numberOfSWUarts; ++uart_i)
    {
    txCount18[uart_i] = wrandom(&countSeed) % swUartTestQueueLength;

    for (int i = 0; i < txCount18[uart_i]; ++i)
    {
      uart18Tx[uart_i][i] = wrandom(&txSeed);
    }
    txCount8[uart_i] = wrandom(&countSeed) % swUartTestQueueLength;

    for (int i = 0; i < txCount8[uart_i]; ++i)
    {
      uart8Tx[uart_i][i] = wrandom(&txSeed);
    }

    int bytesWritten = software18Uarts[uart_i]->write(uart18Tx[uart_i], txCount18[uart_i]);
    test("SWU 018",bytesWritten,txCount18[uart_i]);

    bytesWritten = software8Uarts[uart_i]->write(uart8Tx[uart_i], txCount8[uart_i]);
    test("SWU 08W",bytesWritten,txCount8[uart_i]);
    }
    bool doneTransmitting = 0;
    while (!doneTransmitting)
    {
      doneTransmitting = true;
      for (int uart_i = 0; uart_i < numberOfSWUarts; ++uart_i)
      {
        doneTransmitting &= (software18Uarts[uart_i]->bytesToTransmit() == 0);
        if (!doneTransmitting) continue;
        doneTransmitting &= (software8Uarts[uart_i]->bytesToTransmit() == 0);
        if (!doneTransmitting) continue;
      }
      delay(0);
    }
    delay(45); // Allow last byte to send
    for (int uart_i = 0; uart_i < numberOfSWUarts; ++uart_i)
    {
          int bytesRead = software8Uarts[uart_i]->readBytes((char*)uart8Rx[uart_i], txCount18[uart_i]);
    test("SWU 1",bytesRead,txCount18[uart_i]);
    for (int i = 0; i < txCount18[uart_i]; ++i)
    {
      test("SWU 1_8",uart8Rx[uart_i][i] ,(uint8_t)wrandom(&rxSeed));
    }
         bytesRead = software18Uarts[uart_i]->readBytes((char*)uart18Rx[uart_i], txCount8[uart_i]);
    test("SWU 2",bytesRead,txCount8[uart_i]);
    for (int i = 0; i < txCount8[uart_i]; ++i)
    {
      test("SWU 2_8",uart18Rx[uart_i][i] ,(uint8_t)wrandom(&rxSeed));
    }
    }
  }
}
