
SerialWombatQueuedPulseOutput queuedPulseOutput(SW18AB_6B);


void queuedPulseOutputTest()
{
      uint8_t  pin = 10;
  { // Test 1 pulse internal queue, uS

    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            0, // uS pulses
                            0xFFFF); // Use internal queue

    initializePulseReaduS(SW18AB_6B,pin);

    queuedPulseOutput.queuePulses(0x8000 | 500, 500); // 500uS high, 500uS low
    delay(5);

    test("QueuedPulseOutput_00A", pulseRead(SW18AB_6B,pin), 500, 100);
    test("QueuedPulseOutput_00B", pulseCounts(SW18AB_6B,pin), 1);
  }

  { // Test 2 pulses internal queue, uS
    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            0, // uS pulses
                            0xFFFF); // Use internal queue

    initializePulseReaduS(SW18AB_6B,pin);

    queuedPulseOutput.queuePulses(0x8000 | 500, 500); // 500uS high, 500uS low
    queuedPulseOutput.queuePulses(0x8000 | 750, 500); // 500uS high, 500uS low
    delay(5);

    test("QueuedPulseOutput_01A", pulseRead(SW18AB_6B,pin), 750, 100);
    test("QueuedPulseOutput_01B", pulseCounts(SW18AB_6B,pin), 2);
  }

  { // Queuing 3 entries to an external queue, uS
    SerialWombatQueue q(SW18AB_6B);
    uint16_t queueIndex = 300;
    q.begin(queueIndex, // Index
            100); // 100 bytes

    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            0, // uS pulses
                            queueIndex); // External queue address

    initializePulseReaduS(SW18AB_6B,pin);
    uint16_t queueData[] = {0x8000 | 500, 500, 0x8000 | 900, 500, 0x8000 | 300, 500};
    q.write(queueData, 6);
    delay(20);

    test("QueuedPulseOutput_02A", pulseRead(SW18AB_6B,pin), 300, 100);
    test("QueuedPulseOutput_02B", pulseCounts(SW18AB_6B,pin), 3);
  }


  { // Queuing 2 continuous high entries to internal queue,  uS

    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            0, // uS pulses
                            0xFFFF); // Use internal queue

    initializePulseReaduS(SW18AB_6B,pin);
    queuedPulseOutput.queuePulses(0x8000 | 750, 0x8000 | 1000); // 1750 high total
    delay(20);

    test("QueuedPulseOutput_03", pulseRead(SW18AB_6B,pin), 1750, 100);
    test("QueuedPulseOutput_03B", pulseCounts(SW18AB_6B,pin), 1);
  }

  { // Queuing 2 continuous high entries to internal queue,  mS
    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            1, // mS pulses
                            0xFFFF); // Use internal queue

    initializePulseReaduS(SW18AB_6B,pin);
    queuedPulseOutput.queuePulses(0x8000 | 10, 0x8000 | 10); // 20000uS (20mS) high total
    delay(50);

    test("QueuedPulseOutput_04", pulseRead(SW18AB_6B,pin), 20000, 1000);
    test("QueuedPulseOutput_04B", pulseCounts(SW18AB_6B,pin), 1);
  }
  { // Queuing 1 high entry to internal queue,  mS
    queuedPulseOutput.begin(pin, 0, // start low
                            0, // Idle state
                            1, // mS pulses
                            0xFFFF); // Use internal queue

    initializePulseReaduS(SW18AB_6B,pin);
    queuedPulseOutput.queuePulses(0x8000 | 10, 10); // 10000uS (20mS) high total
    delay(50);

    test("QueuedPulseOutput_05", pulseRead(SW18AB_6B,pin), 10000, 500);
    test("QueuedPulseOutput_05B", pulseCounts(SW18AB_6B,pin), 1);
  }

  { // 1200 baud UART output, idle high
    resetAll();
        SerialWombatQueue q(SW18AB_6B);
    uint16_t queueIndex = 300;
    q.begin(queueIndex, // Index
            100); // 100 bytes

    queuedPulseOutput.begin(pin, 1, // start low
                            1, // Idle state
                            0, // uS pulses
                            queueIndex); // External queue address
    if (pin != 10)
    {
      //This test is pin specific because of matching the SW4B.  Update the pin or
      // fix the uart config below
      test("QueuedPulseOutput_UARTCONFIG", 0, 1);
    }
    delay(100);
    SerialWombatUART uart(SWChipAndPinTo4BChip(SW18AB_6B,pin));
    uart.begin(1200, SW18ABPinTo4BPin(pin), SW18ABPinTo4BPin(pin), 255);
    uint16_t bitTime = 1000000 / 1200 ;
    uint16_t asciiW[] = { bitTime, //Start Bit
                          (uint16_t)(0x8000|bitTime), // 1  W is 0x57
                          (uint16_t)(0x8000|bitTime), // 
                          (uint16_t)(0x8000|bitTime), // 
                          (uint16_t)(bitTime), // 
                          (uint16_t)(0x8000| bitTime), // 
                          (uint16_t)(bitTime), // 
                          (uint16_t)(0x8000|bitTime), // 
                          (uint16_t)(bitTime), // 
                          (uint16_t)(0x8000|bitTime), //   Stop bit
                          };
    queuedPulseOutput.pause(true);
    q.write(asciiW, 10);
    queuedPulseOutput.pause(false);
    delay(100);
    int data = uart.read();

    test("QueuedPulseOutput_06A", data, 'W');
  }
  

}
