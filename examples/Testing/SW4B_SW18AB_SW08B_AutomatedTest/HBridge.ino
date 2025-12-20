SerialWombatHBridge_18AB HBridge18(SW18AB_6B);
SerialWombatHBridge_18AB HBridge8(SW8B_68);

#define HBRIDGE_TEST_INCREMENTS 100


void hBridgeTest(SerialWombatChip &sw, uint8_t hBridgeFirstPin, uint8_t hBridgeSecondPin)
{
  resetAll();

  SerialWombatHBridge_18AB* HBridge;

  if (&sw == &SW18AB_6B)
  {
    HBridge = &HBridge18;
  }

  if (&sw == &SW8B_68)
  {
    HBridge = &HBridge8;
  }
  

  HBridge->begin(hBridgeFirstPin, hBridgeSecondPin, 4000, HBRIDGE_OFF_BOTH_LOW); // Should initialize to 32768.  250 cycles per second
  initializePulseReaduS(sw,hBridgeFirstPin);
  initializePulseReaduS(sw,hBridgeSecondPin);

  {
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);

    delay(1000);

    test("HBridge_00A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  //There should have been no pulses on either.
    test("HBridge_00B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
  }
  {
    HBridge->writePublicData(0x0000); // One high, one low, no PWM
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_01A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  //There should have been no pulses on either.
    test("HBridge_01B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
  }
  {
    HBridge->writePublicData(0xFFFF); // One high, one low, no PWM
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_02A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  //There should have been no pulses on either.
    test("HBridge_02B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
  }
  {
    HBridge->writePublicData(0xC000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_03A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts + 250, 15); 
    test("HBridge_03B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
    test("HBridge_03C", dutyCycleRead(sw,hBridgeFirstPin),0x8000,0x100);
  }

  {
    HBridge->writePublicData(0xE000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_04A", pulseCounts(sw,hBridgeFirstPin),firstPinCounts + 250, 15); 
    test("HBridge_04B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
    test("HBridge_04C", dutyCycleRead(sw,hBridgeFirstPin),0xC000,0x100);
  }

  {
    HBridge->writePublicData(0xA000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_05A", pulseCounts(sw,hBridgeFirstPin),firstPinCounts +  250, 15); 
    test("HBridge_05B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
    test("HBridge_05C", dutyCycleRead(sw,hBridgeFirstPin),0x4000,0x100);
  }

  {
    HBridge->writePublicData(0x4000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_06A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  
    test("HBridge_06B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts + 250, 15);
     test("HBridge_06C", dutyCycleRead(sw,hBridgeSecondPin),0x8000,0x100);
  }
  {
    HBridge->writePublicData(0x2000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_07A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  
    test("HBridge_07B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts + 250, 15);
     test("HBridge_07C", dutyCycleRead(sw,hBridgeSecondPin),0xC000,0x100);
  }

   {
    HBridge->writePublicData(0x6000); // One PWM, one low
    uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
    uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
    delay(1000);

    test("HBridge_08A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  
    test("HBridge_08B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts + 250, 15);
     test("HBridge_08C", dutyCycleRead(sw,hBridgeSecondPin),0x4000,0x100);
  }
  if (&sw == &SW18AB_6B)
  {  //Frequency sweep
  for (uint16_t period = 2500; period <= 60000; period += 1000)
  {
                uint16_t freq = 1000000ul / period;
    HBridge->begin(hBridgeFirstPin, hBridgeSecondPin, period, HBRIDGE_OFF_BOTH_LOW);
    for (int i = 1000; i < 65535; i += 5000)
    {
          HBridge->writePublicData(i);
           delay(10);
           initializePulseReaduS(sw,hBridgeFirstPin);
            initializePulseReaduS(sw,hBridgeSecondPin);
           uint16_t firstPinCounts = pulseCounts(sw,hBridgeFirstPin);
           uint16_t secondPinCounts = pulseCounts(sw,hBridgeSecondPin);
            delay(1000);

            if (i < 0x8000)
            {
              test("HBridge_09A", pulseCounts(sw,hBridgeFirstPin), firstPinCounts);  
              test("HBridge_09B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts + freq, 15,10);
              test("HBridge_09C", dutyCycleRead(sw,hBridgeSecondPin),2* (0x8000-i),0x100, 10);
            }
            else
            {
    test("HBridge_10A", pulseCounts(sw,hBridgeFirstPin),firstPinCounts +  freq, 15,10); 
    test("HBridge_10B", pulseCounts(sw,hBridgeSecondPin), secondPinCounts);
     test("HBridge_10C", dutyCycleRead(sw,hBridgeFirstPin), 2*(i - 0x8000) ,0x100, 10);

            }
            
            
    }
  }
    delay(100);

  }
}
